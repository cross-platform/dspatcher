/************************************************************************
DSPatcher - Cross-Platform Graphical Test Tool for DSPatch
Copyright (c) 2014-2018 Marcus Tomlinson

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

signals:
    void inputAdded( Component::SPtr component, int index );
    void inputRemoved( Component::SPtr component, int index );
    void outputAdded( Component::SPtr component, int index );
    void outputRemoved( Component::SPtr component, int index );
    void parameterAdded( Component::SPtr component, int index );
    void parameterRemoved( Component::SPtr component, int index );
    void parameterUpdated( Component::SPtr component, int index );

public slots:
    void compInserted( QtpComp* qtpComp );
    void compRemoved( int compId );
    void wireConnected( int fromComp, int fromPin, int toComp, int toPin );
    void wireDisconnected( int toComp, int toPin );

private slots:
    void _loadPlugins();

private:
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
