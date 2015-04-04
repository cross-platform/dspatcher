#include <QtpMain.h>
#include <QObject>

#include <DSPatch.h>
#include <DsprParam.h>

class DsprController : public QObject
{
    Q_OBJECT

public:
    DsprController(QtpMain* mainWindow, std::vector<DspPluginLoader> const& pluginLoaders);
    ~DsprController();

public slots:
    void compInserted(QtpComp* comp);
    void compRemoved(uint compId);
    void wireConnected(uint fromComp, int fromPin, uint toComp, int toPin);
    void wireDisconnected(uint fromComp, int fromPin, uint toComp, int toPin);

    void boolUpdated(bool value);
    void intUpdated(int value);
    void floatUpdated(float value);
    void stringUpdated(std::string const& value);
    void triggerUpdated();

private:
    QtpMain* _mainWindow;
    std::vector<DspPluginLoader> _pluginLoaders;

    std::map<int, DspComponent*> _components;
    std::map<int, std::vector<DsprParam*> > _params;
    DspCircuit _circuit;
};
