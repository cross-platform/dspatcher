/************************************************************************
DSPatcher - Cross-Platform Graphical Patching Tool For DSPatch
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of DSPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LGPLv3.txt included in the packaging of this
file. Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

#include <DSPatch.h>
#include <DsprController.h>
#include <QtpMain.h>

#include <QApplication>
#include <QDir>

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
        #ifdef _WIN32
        if (file.isFile() && file.fileName().endsWith(".dll"))
        #else
        if (file.isFile())
        #endif
        {
            QString path = file.absoluteFilePath();
            DspPluginLoader loader(path.toUtf8().constData());
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
