#include <QtpMain.h>

#include <QApplication>
#include <QDir>

#include <DSPatch.h>
#include <DsprController.h>

int main(int argv, char* args[])
{
    QApplication app(argv, args);
    QtpMain mainWindow;
    mainWindow.show();

    std::vector<DspPluginLoader> pluginLoaders;

    // Load DSPatch plugins from "dspatchables" folder
    QDir dir(PLUGIN_DIR);
    QFileInfoList files = dir.entryInfoList();
    foreach(QFileInfo const& fi, files)
    {
        if (fi.isFile())
        {
            QString path = fi.absoluteFilePath();
            DspPluginLoader loader(path.toStdString());
            if (loader.IsLoaded())
            {
                std::map<std::string, DspParameter> params = loader.GetCreateParams();
                DspComponent* x = loader.Create(params);

                QtpComp::CompInfo comp;
                pluginLoaders.push_back(loader);
                comp.typeId = pluginLoaders.size() - 1;
                comp.typeName = fi.baseName().mid(0, 3) == "lib" ? fi.baseName().mid(3) : fi.baseName();
                for (int i = 0; i < x->GetInputCount(); ++i)
                {
                    comp.inPins.append(x->GetInputName(i).c_str());
                }
                for (int i = 0; i < x->GetOutputCount(); ++i)
                {
                    comp.outPins.append(x->GetOutputName(i).c_str());
                }
                mainWindow.registerComp(comp);

                delete x;
            }
        }
    }

    DsprController controller(&mainWindow, pluginLoaders);

    return app.exec();
}
