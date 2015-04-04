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
