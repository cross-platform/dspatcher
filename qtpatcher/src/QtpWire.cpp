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

#include <QtpWire.h>
#include <QtpPin.h>

#include <QPainter>
#include <QPen>

#include <math.h>

const qreal Pi = 3.14;

QtpWire::QtpWire(QtpPin* startPin, QtpPin* endPin, QGraphicsItem* parent)
    : QGraphicsLineItem(parent)
{
    _startPin = startPin;
    _endPin = endPin;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    _color = Qt::black;
    setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QtpWire::~QtpWire()
{
    _startPin->removeWire(this);
    _endPin->removeWire(this);
}

int QtpWire::type() const
{
    return Type;
}

void QtpWire::setColor(const QColor& color)
{
    _color = color;
}

QtpPin* QtpWire::startPin() const
{
    return _startPin;
}

QtpPin* QtpWire::endPin() const
{
    return _endPin;
}

void QtpWire::updatePosition()
{
    QLineF line(mapFromItem(_startPin, 0, 0), mapFromItem(_endPin, 0, 0));
    setLine(line);
}

void QtpWire::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (_startPin->collidesWithItem(_endPin))
    {
        return;
    }

    QPen wirePen = pen();
    wirePen.setColor(_color);
    qreal wireHeadSize = 7;
    painter->setPen(wirePen);
    painter->setBrush(_color);

    QLineF centerLine(mapFromItem(_startPin, 0, 0), mapFromItem(_endPin, 0, 0));

    QLineF polyLine;
    QPointF p2;

    QPointF startIntersect;
    QPolygonF startPolygon = _startPin->polygon();
    QPointF p1 = startPolygon.first() + mapFromItem(_startPin, 0, 0);
    for (int i = 1; i < startPolygon.count(); ++i)
    {
        p2 = startPolygon.at(i) + mapFromItem(_startPin, 0, 0);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &startIntersect);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }

    QPointF endIntersect;
    QPolygonF endPolygon = _endPin->polygon();
    p1 = endPolygon.first() + mapFromItem(_endPin, 0, 0);
    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon.at(i) + mapFromItem(_endPin, 0, 0);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &endIntersect);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }

    setLine(QLineF(endIntersect, startIntersect));

    double angle = acos(line().dx() / line().length());
    if (line().dy() >= 0)
    {
        angle = (Pi * 2) - angle;
    }

    QPointF wireP1 = line().p1() + QPointF(sin(angle + Pi / 3) * wireHeadSize, cos(angle + Pi / 3) * wireHeadSize);
    QPointF wireP2 =
        line().p1() + QPointF(sin(angle + Pi - Pi / 3) * wireHeadSize, cos(angle + Pi - Pi / 3) * wireHeadSize);

    _wireHead.clear();
    _wireHead << line().p1() << wireP1 << wireP2;
    painter->drawLine(line());
    painter->drawPolygon(_wireHead);
    if (isSelected())
    {
        painter->setPen(QPen(_color, 1, Qt::DashLine));
        QLineF wireLine = line();
        wireLine.translate(0, 4.0);
        painter->drawLine(wireLine);
        wireLine.translate(0, -8.0);
        painter->drawLine(wireLine);
    }
}
