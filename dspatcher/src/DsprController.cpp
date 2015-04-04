#include <DsprController.h>

#include <QtpDiag.h>

DsprController::DsprController(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders)
    : _mainWindow(mainWindow)
    , _pluginLoaders(pluginLoaders)
{
    _circuit.StartAutoTick();

    connect(_mainWindow->diagram(), &QtpDiag::compInserted, this, &DsprController::compInserted);
    connect(_mainWindow->diagram(), &QtpDiag::compRemoved, this, &DsprController::compRemoved);
    connect(_mainWindow->diagram(), &QtpDiag::wireConnected, this, &DsprController::wireConnected);
    connect(_mainWindow->diagram(), &QtpDiag::wireDisconnected, this, &DsprController::wireDisconnected);

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
    typedef std::pair< int, std::vector<DsprParam*> > ParamPair;
    foreach(ParamPair paramList, _params)
    {
        foreach (DsprParam* param, paramList.second)
        {
            delete param;
        }
    }
    _params.clear();

    typedef std::pair<int, DspComponent*> ComponentPair;
    foreach(ComponentPair comp, _components)
    {
        _circuit.RemoveComponent(comp.second);
        _components.erase(comp.first);
        delete comp.second;
    }
    _circuit.StopAutoTick();

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

void DsprController::compInserted(QtpComp* comp)
{
    DspPluginLoader loader = _pluginLoaders[comp->compInfo().typeId];
    std::map<std::string, DspParameter> params = loader.GetCreateParams();
    DspComponent* x = loader.Create(params);
    _qtpComps[x] = comp;
    x->SetCallback(callback, this);

    _params[comp->id()] = std::vector<DsprParam*>();
    for (int i = 0; i < x->GetParameterCount(); ++i)
    {
        DsprParam* y = new DsprParam(comp->id(), i, x->GetParameterName(i), *x->GetParameter(i), comp->contextMenu());
        comp->contextMenu()->addAction(y->action());

        connect(y, &DsprParam::boolUpdated, this, &DsprController::boolUpdated);
        connect(y, &DsprParam::intUpdated, this, &DsprController::intUpdated);
        connect(y, &DsprParam::floatUpdated, this, &DsprController::floatUpdated);
        connect(y, &DsprParam::stringUpdated, this, &DsprController::stringUpdated);
        connect(y, &DsprParam::triggerUpdated, this, &DsprController::triggerUpdated);

        _params[comp->id()].push_back(y);
    }

    _components[comp->id()] = x;

    _circuit.AddComponent(x);
}

void DsprController::compRemoved(uint compId)
{
    _circuit.RemoveComponent(_components[compId]);
    delete _components[compId];
    _components.erase(compId);
}

void DsprController::wireConnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    _circuit.ConnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}

void DsprController::wireDisconnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    _circuit.DisconnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}

void DsprController::boolUpdated(bool value)
{
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Bool, value));
}

void DsprController::intUpdated(int value)
{
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Int, value));
}

void DsprController::floatUpdated(float value)
{
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Float, value));
}

void DsprController::stringUpdated(std::string const& value)
{
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::String, value));
}

void DsprController::triggerUpdated()
{
    DsprParam* param = dynamic_cast<DsprParam*>(sender());
    _components[param->compId()]->SetParameter(param->paramId(), DspParameter(DspParameter::Trigger));
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
    DsprParam* y = new DsprParam(comp->id(), index, component->GetParameterName(index), *component->GetParameter(index), comp->contextMenu());
    comp->contextMenu()->addAction(y->action());

    connect(y, &DsprParam::boolUpdated, this, &DsprController::boolUpdated);
    connect(y, &DsprParam::intUpdated, this, &DsprController::intUpdated);
    connect(y, &DsprParam::floatUpdated, this, &DsprController::floatUpdated);
    connect(y, &DsprParam::stringUpdated, this, &DsprController::stringUpdated);
    connect(y, &DsprParam::triggerUpdated, this, &DsprController::triggerUpdated);

    _params[comp->id()].push_back(y);
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
//    QtpComp* comp = _qtpComps[component];
//    switch (_params[comp->id()][index]->param().Type())
//    {
//        case DspParameter::Null:
//            break;
//        case DspParameter::Bool:
//            _params[comp->id()][index]->SetBool(*component->GetParameter(index)->GetBool());
//            break;
//        case DspParameter::Int:
//            if (component->GetParameter(index)->GetIntRange())
//            {
//                _params[comp->id()][index]->SetIntRange(*component->GetParameter(index)->GetIntRange());
//            }
//            _params[comp->id()][index]->SetInt(*component->GetParameter(index)->GetInt());
//            break;
//        case DspParameter::Float:
//            if (component->GetParameter(index)->GetFloatRange())
//            {
//                _params[comp->id()][index]->SetFloatRange(*component->GetParameter(index)->GetFloatRange());
//            }
//            _params[comp->id()][index]->SetFloat(*component->GetParameter(index)->GetFloat());
//            break;
//        case DspParameter::String:
//            _params[comp->id()][index]->SetString(*component->GetParameter(index)->GetString());
//            break;
//        case DspParameter::FilePath:
//            _params[comp->id()][index]->SetString(*component->GetParameter(index)->GetString());
//            break;
//        case DspParameter::List:
//            //_params[comp->id()][index]->SetList(*component->GetParameter(index)->GetList());
//            break;
//        case DspParameter::Trigger:
//            break;
//    }
}
