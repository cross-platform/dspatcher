#include <QtpMain.h>

#include <QApplication>
#include <QDir>

#include <DSPatch.h>

int main(int argv, char* args[])
{
    QApplication app(argv, args);
    QtpMain mainWindow;
    mainWindow.show();

    std::vector<DspPluginLoader> componentLoaders;

    // Read in DSPatch plugins
    QDir dir(PLUGIN_DIR);
    QFileInfoList files = dir.entryInfoList();
    foreach(const QFileInfo &fi, files)
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
                componentLoaders.push_back(loader);
                comp.typeId = componentLoaders.size() - 1;
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
            }
        }
    }

    return app.exec();
}
