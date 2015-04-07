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

#ifndef QTPWIRE_H
#define QTPWIRE_H

#include <QGraphicsLineItem>

class QtpPin;

class QtpWire : public QGraphicsLineItem
{
public:
    enum
    {
        Type = UserType + 3
    };

    QtpWire(QtpPin* startPin, QtpPin* endPin, QGraphicsItem* parent = 0);
    ~QtpWire();

    int type() const;

    void setColor(const QColor& color);

    QtpPin* startPin() const;
    QtpPin* endPin() const;

    void updatePosition();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

private:
    QtpPin* _startPin;
    QtpPin* _endPin;
    QColor _color;
    QPolygonF _wireHead;
};

#endif  // QTPWIRE_H
