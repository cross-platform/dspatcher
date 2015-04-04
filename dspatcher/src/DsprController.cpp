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

void DsprController::compInserted(QtpComp* comp)
{
    DspPluginLoader loader = _pluginLoaders[comp->compInfo().typeId];
    std::map<std::string, DspParameter> params = loader.GetCreateParams();
    DspComponent* x = loader.Create(params);

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
