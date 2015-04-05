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
    foreach(QFileInfo const& file, files)
    {
        if (file.isFile())
        {
            QString path = file.absoluteFilePath();
            DspPluginLoader loader(path.toStdString());
            if (loader.IsLoaded())
            {
                pluginLoaders.push_back(loader);
                std::map<std::string, DspParameter> params = loader.GetCreateParams();
                DspComponent* comp = loader.Create(params);

                QtpComp::CompInfo compInfo;
                compInfo.typeId = pluginLoaders.size() - 1;
                compInfo.typeName = file.baseName().mid(0, 3) == "lib" ? file.baseName().mid(3) : file.baseName();
                for (int i = 0; i < comp->GetInputCount(); ++i)
                {
                    compInfo.inPins.append(comp->GetInputName(i).c_str());
                }
                for (int i = 0; i < comp->GetOutputCount(); ++i)
                {
                    compInfo.outPins.append(comp->GetOutputName(i).c_str());
                }
                mainWindow.registerComponent(compInfo);

                delete comp;
            }
        }
    }

    DsprController controller(mainWindow.diagram(), pluginLoaders);

    return app.exec();
}
