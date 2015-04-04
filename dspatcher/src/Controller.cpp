#include <Controller.h>

#include <QtpDiag.h>

Controller::Controller(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders)
    : _mainWindow(mainWindow)
    , _pluginLoaders(pluginLoaders)
{
    connect(_mainWindow->diagram(), &QtpDiag::compInserted, this, &Controller::compInserted);
    connect(_mainWindow->diagram(), &QtpDiag::compRemoved, this, &Controller::compRemoved);
    connect(_mainWindow->diagram(), &QtpDiag::wireConnected, this, &Controller::wireConnected);
    connect(_mainWindow->diagram(), &QtpDiag::wireDisconnected, this, &Controller::wireDisconnected);
}

void Controller::compInserted(QtpComp::CompInfo const& compInfo, uint compId)
{
    int i = 0;
}

void Controller::compRemoved(uint compId)
{
    int i = 0;
}

void Controller::wireConnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    int i = 0;
}

void Controller::wireDisconnected(uint fromComp, int fromPin, uint toComp, int toPin)
{
    int i = 0;
}
