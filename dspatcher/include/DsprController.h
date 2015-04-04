#include <QtpMain.h>
#include <DSPatch.h>
#include <QObject>

class DsprController : public QObject
{
    Q_OBJECT

public:
    DsprController(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders);
    ~DsprController();

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
