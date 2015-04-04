#include <Controller.h>

#include <QtpDiag.h>

Controller::Controller(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders)
    : _mainWindow(mainWindow)
    , _pluginLoaders(pluginLoaders)
{
    _circuit.StartAutoTick();
    connect(_mainWindow->diagram(), &QtpDiag::compInserted, this, &Controller::compInserted);
    connect(_mainWindow->diagram(), &QtpDiag::compRemoved, this, &Controller::compRemoved);
    connect(_mainWindow->diagram(), &QtpDiag::wireConnected, this, &Controller::wireConnected);
    connect(_mainWindow->diagram(), &QtpDiag::wireDisconnected, this, &Controller::wireDisconnected);
}

Controller::~Controller()
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

void Controller::compInserted(QtpComp::CompInfo const& compInfo, uint compId)
{
    DspPluginLoader loader = _pluginLoaders[compInfo.typeId];
    std::map<std::string, DspParameter> params = loader.GetCreateParams();
    DspComponent* x = loader.Create(params);

    _components[compId] = x;

    _circuit.AddComponent(x);
}

void Controller::compRemoved(uint compId)
{
    _circuit.RemoveComponent(_components[compId]);
    _components.erase(compId);
}

void Controller::wireConnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    _circuit.ConnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}

void Controller::wireDisconnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    _circuit.DisconnectOutToIn(_components[fromComp], fromPin, _components[toComp], toPin);
}
