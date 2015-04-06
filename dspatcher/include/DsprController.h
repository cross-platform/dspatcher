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

#ifndef DSPRCONTROLLER_H
#define DSPRCONTROLLER_H

#include <DSPatch.h>
#include <DsprParam.h>
#include <QtpMain.h>

#include <QObject>

class DsprController : public QObject
{
    Q_OBJECT

public:
    DsprController(QtpDiag* diagram, std::vector<DspPluginLoader> const& pluginLoaders);
    ~DsprController();

signals:
    void inputAdded(DspComponent* component, int index);
    void inputRemoved(DspComponent* component, int index);
    void outputAdded(DspComponent* component, int index);
    void outputRemoved(DspComponent* component, int index);
    void parameterAdded(DspComponent* component, int index);
    void parameterRemoved(DspComponent* component, int index);
    void parameterUpdated(DspComponent* component, int index);

public slots:
    void compInserted(QtpComp* qtpComp);
    void compRemoved(int compId);
    void wireConnected(int fromComp, int fromPin, int toComp, int toPin);
    void wireDisconnected(int fromComp, int fromPin, int toComp, int toPin);

    void boolUpdated(bool value);
    void intUpdated(int value);
    void floatUpdated(float value);
    void stringUpdated(std::string const& value);
    void triggerUpdated();

private slots:
    void _inputAdded(DspComponent* component, int index);
    void _inputRemoved(DspComponent* component, int index);
    void _outputAdded(DspComponent* component, int index);
    void _outputRemoved(DspComponent* component, int index);
    void _parameterAdded(DspComponent* component, int index);
    void _parameterRemoved(DspComponent* component, int index);
    void _parameterUpdated(DspComponent* component, int index);

private:
    bool _settingParam;
    std::vector<DspPluginLoader> _pluginLoaders;

    std::map<DspComponent const*, QtpComp*> _qtpComps;
    std::map<int, DspComponent*> _components;
    std::map< int, std::vector<DsprParam*> > _params;
    DspCircuit _circuit;
};

#endif  // DSPRCONTROLLER_H
