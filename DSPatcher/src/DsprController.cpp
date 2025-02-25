/******************************************************************************
DSPatcher - Cross-Platform Graphical Tool for DSPatch
Copyright (c) 2025, Marcus Tomlinson

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
    _pluginPath = qgetenv( "DSPATCH_USER_PLUGINS" );
    if ( !_pluginPath.isEmpty() )
    {
        auto userPlugins = QDir( _pluginPath );
        auto dspatchPlugins = QDir( qgetenv( "DSPATCH_PLUGINS" ) );
        if ( userPlugins.entryInfoList( QDir::NoDotAndDotDot | QDir::AllEntries ).count() == 0 )
        {
            foreach ( QString fileName, dspatchPlugins.entryList( QDir::Files ) )
            {
                QFile::copy( dspatchPlugins.path() + "/" + fileName, userPlugins.path() + "/" + fileName );
            }
        }
    }
    else
    {
        _pluginPath = DEFAULT_PLUGIN_DIR;
    }

    _fileWatcher.addPath( _pluginPath );
    connect( &_fileWatcher, SIGNAL( directoryChanged( QString const& ) ), this, SLOT( _loadPlugins() ) );

    _loadPlugins();

    connect( _mainWindow.diagram(), &QtpDiag::compInserted, this, &DsprController::compInserted );
    connect( _mainWindow.diagram(), &QtpDiag::compRemoved, this, &DsprController::compRemoved );
    connect( _mainWindow.diagram(), &QtpDiag::wireConnected, this, &DsprController::wireConnected );
    connect( _mainWindow.diagram(), &QtpDiag::wireDisconnected, this, &DsprController::wireDisconnected );
}

DsprController::~DsprController()
{
    _circuit.StopAutoTick();

    typedef std::pair<int, Component::SPtr> ComponentPair;
    foreach ( ComponentPair comp, _components )
    {
        _circuit.RemoveComponent( comp.second );
    }
    _components.clear();
    _qtpComps.clear();
}

void DsprController::compInserted( QtpComp* qtpComp )
{
    _circuit.StartAutoTick();
    auto loader = _pluginLoaders[qtpComp->compInfo().typeId];

    Component::SPtr component = loader->Create();
    if ( component == nullptr )
    {
        return;
    }

    auto ui_component = std::dynamic_pointer_cast<UiComponent>( component );
    if ( ui_component )
    {
        qtpComp->setWidget( ui_component->widget() );
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

    _circuit.AddComponent( component );

    _components[qtpComp->id()] = component;

    _qtpComps[component] = qtpComp;
}

void DsprController::compRemoved( int compId )
{
    _circuit.RemoveComponent( _components[compId] );

    _qtpComps.erase( _components[compId] );

    _components.erase( compId );
}

void DsprController::wireConnected( int fromComp, int fromPin, int toComp, int toPin )
{
    _circuit.ConnectOutToIn( _components[fromComp], fromPin, _components[toComp], toPin );
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

    QDirIterator it( _pluginPath, QDir::Files, QDirIterator::Subdirectories );
    while ( it.hasNext() )
        files += QFileInfo( it.next() );

    std::sort( files.begin(), files.end(), []( const QFileInfo& one, const QFileInfo& two ) { return one.fileName() < two.fileName(); } );

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

                compInfo.isWidget = false;
                if ( std::dynamic_pointer_cast<UiComponent>( comp ) )
                {
                    compInfo.isWidget = true;
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
