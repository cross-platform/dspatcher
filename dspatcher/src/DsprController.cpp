/************************************************************************
DSPatcher - Cross-Platform Graphical Patching Tool For DSPatch
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of DSPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LGPLv3.txt included in the packaging of this
file. Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

#include <DsprController.h>

#include <QtpDiag.h>

DsprController::DsprController(QtpDiag* diagram, std::vector<DspPluginLoader> const& pluginLoaders)
    : _settingParam(false)
    , _pluginLoaders(pluginLoaders)
{
    _circuit.SetThreadCount(2);
    _circuit.StartAutoTick();

    connect(diagram, &QtpDiag::compInserted, this, &DsprController::compInserted);
    connect(diagram, &QtpDiag::compRemoved, this, &DsprController::compRemoved);
    connect(diagram, &QtpDiag::wireConnected, this, &DsprController::wireConnected);
    connect(diagram, &QtpDiag::wireDisconnected, this, &DsprController::wireDisconnected);

    connect(this, &DsprController::inputAdded, this, &DsprController::_inputAdded);
    connect(this, &DsprController::inputRemoved, this, &DsprController::_inputRemoved);
    connect(this, &DsprController::outputAdded, this, &DsprController::_outputAdded);
    connect(this, &DsprController::outputRemoved, this, &DsprController::_outputRemoved);
    connect(this, &DsprController::parameterAdded, this, &DsprController::_parameterAdded);
    connect(this, &DsprController::parameterRemoved, this, &DsprController::_parameterRemoved);
    connect(this, &DsprController::parameterUpdated, this, &DsprController::_parameterUpdated);
}

DsprController::~DsprController()
{
    _circuit.StopAutoTick();

    typedef std::pair<int, DspComponent*> ComponentPair;
    foreach(ComponentPair comp, _components)
    {
        _circuit.RemoveComponent(comp.second);
        delete comp.second;
    }
    _components.clear();

    typedef std::pair< int, std::vector<DsprParam*> > ParamPair;
    foreach(ParamPair paramList, _params)
    {
        foreach (DsprParam* param, paramList.second)
        {
            delete param;
        }
    }
    _params.clear();
    
    _qtpComps.clear();

    DSPatch::Finalize();
}

void callback(DspComponent* component, DspComponent::CallbackType const& callbackType,
              int index, void* userData)
{
    DsprController* controller = static_cast<DsprController*>(userData);
    switch (callbackType)
    {
        case DspComponent::InputAdded:
            emit controller->inputAdded(component, index);
            break;
        case DspComponent::InputRemoved:
            emit controller->inputRemoved(component, index);
            break;
        case DspComponent::OutputAdded:
            emit controller->outputAdded(component, index);
            break;
        case DspComponent::OutputRemoved:
            emit controller->outputRemoved(component, index);
            break;
        case DspComponent::ParameterAdded:
            emit controller->parameterAdded(component, index);
            break;
        case DspComponent::ParameterRemoved:
            emit controller->parameterRemoved(component, index);
            break;
        case DspComponent::ParameterUpdated:
            emit controller->parameterUpdated(component, index);
            break;
    }
}

void DsprController::compInserted(QtpComp* qtpComp)
{
    DspPluginLoader loader = _pluginLoaders[qtpComp->compInfo().typeId];
    std::map<std::string, DspParameter> params = loader.GetCreateParams();
    DspComponent* component = loader.Create(params);

    component->SetCallback(callback, this);
    _circuit.AddComponent(component);

    _components[qtpComp->id()] = component;

    _params[qtpComp->id()] = std::vector<DsprParam*>();
    for (int i = 0; i < component->GetParameterCount(); ++i)
    {
        DsprParam* param = new DsprParam(qtpComp->id(), i, component->GetParameterName(i),
                                         *component->GetParameter(i), qtpComp->contextMenu());
        qtpComp->contextMenu()->addAction(param->action());

        connect(param, &DsprParam::boolUpdated, this, &DsprController::boolUpdated);
        connect(param, &DsprParam::intUpdated, this, &DsprController::intUpdated);
        connect(param, &DsprParam::floatUpdated, this, &DsprController::floatUpdated);
        connect(param, &DsprParam::stringUpdated, this, &DsprController::stringUpdated);
        connect(param, &DsprParam::triggerUpdated, this, &DsprController::triggerUpdated);

        _params[qtpComp->id()].push_back(param);
    }

    _qtpComps[component] = qtpComp;
}

void DsprController::compRemoved(int compId)
{
    _circuit.RemoveComponent(_components[compId]);

    delete _components[compId];
    _components.erase(compId);

    foreach (DsprParam* param, _params[compId])
    {
        delete param;
    }
    _params.erase(compId);

    _qtpComps.erase(_components[compId]);
}

void DsprController::wireConnected(int fromComp, int fromPin, int toComp, int toPin)
{
    _circuit.ConnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}

void DsprController::wireDisconnected(int fromComp, int fromPin, int toComp, int toPin)
{
    _circuit.DisconnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}

void DsprController::boolUpdated(bool value)
{
    _settingParam = true;
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Bool, value));
    _settingParam = false;
}

void DsprController::intUpdated(int value)
{
    _settingParam = true;
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Int, value));
    _settingParam = false;
}

void DsprController::floatUpdated(float value)
{
    _settingParam = true;
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Float, value));
    _settingParam = false;
}

void DsprController::stringUpdated(std::string const& value)
{
    _settingParam = true;
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::String, value));
    _settingParam = false;
}

void DsprController::triggerUpdated()
{
    _settingParam = true;
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Trigger));
    _settingParam = false;
}

void DsprController::_inputAdded(DspComponent* component, int index)
{
    _qtpComps[component]->addInPin(component->GetInputName(index).c_str());
}

void DsprController::_inputRemoved(DspComponent* component, int index)
{
    if (index == -1)
    {
        _qtpComps[component]->removeInPins();
    }
    else
    {
        _qtpComps[component]->removeInPin();
    }
}

void DsprController::_outputAdded(DspComponent* component, int index)
{
    _qtpComps[component]->addOutPin(component->GetOutputName(index).c_str());
}

void DsprController::_outputRemoved(DspComponent* component, int index)
{
    if (index == -1)
    {
        _qtpComps[component]->removeOutPins();
    }
    else
    {
        _qtpComps[component]->removeOutPin();
    }
}

void DsprController::_parameterAdded(DspComponent* component, int index)
{
    QtpComp* comp = _qtpComps[component];

    DsprParam* param = new DsprParam(comp->id(), index, component->GetParameterName(index),
                                     *component->GetParameter(index), comp->contextMenu());
    comp->contextMenu()->addAction(param->action());

    connect(param, &DsprParam::boolUpdated, this, &DsprController::boolUpdated);
    connect(param, &DsprParam::intUpdated, this, &DsprController::intUpdated);
    connect(param, &DsprParam::floatUpdated, this, &DsprController::floatUpdated);
    connect(param, &DsprParam::stringUpdated, this, &DsprController::stringUpdated);
    connect(param, &DsprParam::triggerUpdated, this, &DsprController::triggerUpdated);

    _params[comp->id()].push_back(param);
}

void DsprController::_parameterRemoved(DspComponent* component, int index)
{
    if (index == -1)
    {
        typedef std::pair< int, std::vector<DsprParam*> > ParamPair;
        foreach(ParamPair paramList, _params)
        {
            foreach (DsprParam* param, paramList.second)
            {
                delete param;
            }
        }
        _params.clear();
    }
    else
    {
        QtpComp* comp = _qtpComps[component];
        delete _params[comp->id()].back();
        _params[comp->id()].pop_back();
    }
}

void DsprController::_parameterUpdated(DspComponent* component, int index)
{
    if (_settingParam)
    {
        return;
    }
    QtpComp* qtpComp = _qtpComps[component];
    switch (_params[qtpComp->id()][index]->param().Type())
    {
        case DspParameter::Null:
            break;
        case DspParameter::Bool:
            _params[qtpComp->id()][index]->SetBool(*component->GetParameter(index)->GetBool());
            break;
        case DspParameter::Int:
            if (component->GetParameter(index)->GetIntRange())
            {
                _params[qtpComp->id()][index]->SetIntRange(*component->GetParameter(index)->GetIntRange());
            }
            _params[qtpComp->id()][index]->SetInt(*component->GetParameter(index)->GetInt());
            break;
        case DspParameter::Float:
            if (component->GetParameter(index)->GetFloatRange())
            {
                _params[qtpComp->id()][index]->SetFloatRange(*component->GetParameter(index)->GetFloatRange());
            }
            _params[qtpComp->id()][index]->SetFloat(*component->GetParameter(index)->GetFloat());
            break;
        case DspParameter::String:
            _params[qtpComp->id()][index]->SetString(*component->GetParameter(index)->GetString());
            break;
        case DspParameter::FilePath:
            _params[qtpComp->id()][index]->SetString(*component->GetParameter(index)->GetString());
            break;
        case DspParameter::List:
            _params[qtpComp->id()][index]->SetList(*component->GetParameter(index)->GetList());
            break;
        case DspParameter::Trigger:
            break;
    }
}
