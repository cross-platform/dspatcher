/************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of QtPatcher.

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

#ifndef QTPPIN_H
#define QTPPIN_H

#include <QGraphicsPolygonItem>
#include <QList>

class QtpWire;

class QtpPin : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 2
    };

    enum PinType
    {
        InPin,
        OutPin
    };

    QtpPin(PinType pinType, QString pinName, int pinIndex, QGraphicsItem* parent = 0);
    ~QtpPin();

    std::string name();
    int index();
    int type() const;

    bool addWire(QtpWire* newWire);
    void removeWire(QtpWire* wire);
    void removeWires();

    QList<QtpWire*> wires();

    PinType pinType() const;
    QPolygonF polygon() const;

    void hover(bool hover);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    void setName(QString name);

private:
    PinType _pinType;
    QPolygonF _polygon;
    QList<QtpWire*> _wires;
    QGraphicsTextItem* _nameText;
    int _pinIndex;
};

#endif  // QTPPIN_H
