/************************************************************************
DSPatcher - Cross-Platform Graphical Tool for DSPatch
Copyright (c) 2014-2019 Marcus Tomlinson

This file is part of DSPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LICENSE included in the packaging of this file.
Please review the following information to ensure the GNU Lesser
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

#include <DsprController.h>

#include <DSPatch.h>
#include <QtpDiag.h>
#include <UiComponent.h>

#include <QDirIterator>

#include <thread>

using namespace DSPatch;
using namespace DSPatcher;

DsprController::DsprController( QtpMain& mainWindow )
    : _mainWindow( mainWindow )
{
    _fileWatcher.addPath( PLUGIN_DIR );
    connect( &_fileWatcher, SIGNAL( directoryChanged( QString const& ) ), this, SLOT( _loadPlugins() ) );

    _loadPlugins();

    _circuit->SetBufferCount( std::thread::hardware_concurrency() );

    connect( _mainWindow.diagram(), &QtpDiag::compInserted, this, &DsprController::compInserted );
    connect( _mainWindow.diagram(), &QtpDiag::compRemoved, this, &DsprController::compRemoved );
    connect( _mainWindow.diagram(), &QtpDiag::wireConnected, this, &DsprController::wireConnected );
    connect( _mainWindow.diagram(), &QtpDiag::wireDisconnected, this, &DsprController::wireDisconnected );
}

DsprController::~DsprController()
{
    _circuit->StopAutoTick();

    typedef std::pair<int, Component::SPtr> ComponentPair;
    foreach ( ComponentPair comp, _components )
    {
        _circuit->RemoveComponent( comp.second );
    }
    _components.clear();
    _qtpComps.clear();
}

void DsprController::compInserted( QtpComp* qtpComp )
{
    auto loader = _pluginLoaders[qtpComp->compInfo().typeId];

    Component::SPtr component = loader->Create();
    if ( component == nullptr )
    {
        return;
    }

    qtpComp->removeInPins();
    qtpComp->removeOutPins();
    for ( int i = 0; i < component->GetInputCount(); ++i )
    {
        qtpComp->addInPin( component->GetInputName( i ).c_str() );
    }
    for ( int i = 0; i < component->GetOutputCount(); ++i )
    {
        qtpComp->addOutPin( component->GetOutputName( i ).c_str() );
    }

    _circuit->AddComponent( component );

    _components[qtpComp->id()] = component;

    _qtpComps[component] = qtpComp;
}

void DsprController::compRemoved( int compId )
{
    _circuit->RemoveComponent( _components[compId] );

    _qtpComps.erase( _components[compId] );

    _components.erase( compId );
}

void DsprController::wireConnected( int fromComp, int fromPin, int toComp, int toPin )
{
    _circuit->ConnectOutToIn( _components[fromComp], fromPin, _components[toComp], toPin );
    _circuit->StartAutoTick( Component::TickMode::Parallel );
}

void DsprController::wireDisconnected( int toComp, int toPin )
{
    _components[toComp]->DisconnectInput( toPin );
}

void DsprController::_loadPlugins()
{
    _mainWindow.unregisterComponents();

    // Load DSPatch plugins from "DSPatchables" folder
    QFileInfoList files;

    QDirIterator it( PLUGIN_DIR, QDir::Files, QDirIterator::Subdirectories );
    while ( it.hasNext() )
        files += QFileInfo( it.next() );

    foreach ( QFileInfo const& file, files )
    {
#ifdef _WIN32
        if ( file.isFile() && file.fileName().endsWith( ".dll" ) )
#else
        if ( file.isFile() )
#endif
        {
            QString path = file.absoluteFilePath();
            auto loader = std::make_shared<Plugin>( path.toUtf8().constData() );
            if ( loader->IsLoaded() )
            {
                _pluginLoaders.emplace_back( loader );
                Component::SPtr comp = loader->Create();

                QtpComp::CompInfo compInfo;
                compInfo.typeId = _pluginLoaders.size() - 1;
                compInfo.typeName = file.baseName().mid( 0, 3 ) == "lib" ? file.baseName().mid( 3 ) : file.baseName();
                if ( std::dynamic_pointer_cast<UiComponent>( comp ) )
                {
                    compInfo.hasWidget = true;
                }
                else
                {
                    compInfo.hasWidget = false;
                }

                if ( comp )
                {
                    for ( int i = 0; i < comp->GetInputCount(); ++i )
                    {
                        compInfo.inPins.append( comp->GetInputName( i ).c_str() );
                    }
                    for ( int i = 0; i < comp->GetOutputCount(); ++i )
                    {
                        compInfo.outPins.append( comp->GetOutputName( i ).c_str() );
                    }
                }

                _mainWindow.registerComponent( compInfo );
            }
        }
    }
}
