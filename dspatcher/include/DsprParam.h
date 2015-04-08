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

#ifndef DSPRPARAM_H
#define DSPRPARAM_H

#include <DSPatch.h>

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QWidgetAction>

class QtpWire;

class DsprParam : public QObject
{
    Q_OBJECT

public:
    DsprParam(int compId, int paramId, std::string const& name, DspParameter const& param, QMenu* contextMenu);
    QWidgetAction* action();
    DspParameter const& param();

    int compId() const;
    int paramId() const;
    std::string name() const;

    bool SetBool(bool const& value);
    bool SetInt(int const& value);
    bool SetIntRange(std::pair<int, int> const& intRange);
    bool SetFloat(float const& value);
    bool SetFloatRange(std::pair<float, float> const& floatRange);
    bool SetString(std::string const& value);
    bool SetList(std::vector<std::string> const& value);

signals:
    void boolUpdated(bool value);
    void intUpdated(int value);
    void floatUpdated(float value);
    void stringUpdated(std::string const& value);
    void triggerUpdated();

private slots:
    void paramChanged(bool value);
    void paramChanged(int value);
    void paramChanged(QString const&);
    void paramChanged();

    void updateSlider(int value);
    void browseForFile();

private:
    bool _settingParam;
    int _compId;
    int _paramId;
    std::string _name;
    DspParameter _param;
    QMenu* _contextMenu;
    QWidgetAction* _action;

    QCheckBox* _checkbox;
    QSlider* _slider;
    QLineEdit* _textBox;
    QComboBox* _listBox;
    QPushButton* _button;
};

#endif  // DSPRPARAM_H
