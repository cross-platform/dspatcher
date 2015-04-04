#include <QtpMain.h>
#include <DSPatch.h>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders);
    ~Controller();

public slots:
    void compInserted(QtpComp::CompInfo const& compInfo, uint compId);
    void compRemoved(uint compId);
    void wireConnected(uint fromComp, int fromPin, uint toComp, int toPin);
    void wireDisconnected(uint fromComp, int fromPin, uint toComp, int toPin);

private:
    QtpMain* _mainWindow;
    std::vector<DspPluginLoader> _pluginLoaders;

    std::map<int, DspComponent*> _components;
    DspCircuit _circuit;
};
