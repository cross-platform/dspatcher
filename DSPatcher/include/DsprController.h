/******************************************************************************
DSPatcher - Cross-Platform Graphical Tool for DSPatch
Copyright (c) 2021, Marcus Tomlinson

BSD 2-Clause License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#pragma once

#include <DSPatch.h>
#include <QtpMain.h>

#include <QFileSystemWatcher>
#include <QObject>

#include <set>

namespace DSPatch
{
namespace DSPatcher
{

class DsprController : public QObject
{
    Q_OBJECT

public:
    DsprController( QtpMain& mainWindow );
    ~DsprController();

public slots:
    void compInserted( QtpComp* qtpComp );
    void compRemoved( int compId );
    void wireConnected( int fromComp, int fromPin, int toComp, int toPin );
    void wireDisconnected( int toComp, int toPin );

private slots:
    void _loadPlugins();

private:
    QString _pluginPath;
    QFileSystemWatcher _fileWatcher;
    QtpMain& _mainWindow;
    std::set<int> _settingParams;
    std::vector<Plugin::SPtr> _pluginLoaders;

    std::map<Component::SPtr, QtpComp*> _qtpComps;
    std::map<int, Component::SPtr> _components;
    Circuit::SPtr _circuit = std::make_shared<Circuit>();
};

}  // namespace DSPatcher
}  // namespace DSPatch
