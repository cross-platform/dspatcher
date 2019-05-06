/************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2014-2019 Marcus Tomlinson

This file is part of QtPatcher.

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

#include <QtpPin.h>
#include <QtpWire.h>

#include <QPainter>
#include <QPen>

#include <math.h>

QtpWire::QtpWire( QtpPin* startPin, QtpPin* endPin, QGraphicsItem* parent )
    : QGraphicsPathItem( parent )
{
    _startPin = startPin;
    _endPin = endPin;
    _color = Qt::black;

    setFlag( QGraphicsItem::ItemIsSelectable, true );
}

QtpWire::~QtpWire()
{
    _startPin->removeWire( this );
    _endPin->removeWire( this );
}

int QtpWire::type() const
{
    return Type;
}

void QtpWire::setColor( const QColor& color )
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
    QPointF start = mapFromItem( _startPin, 6, 0 );
    QPointF end = mapFromItem( _endPin, -6, 0 );

    QPointF c1;
    c1.setX( start.x() + ( ( end.x() - start.x() ) / 2 ) );
    c1.setY( start.y() );

    QPointF c2;
    c2.setX( start.x() + ( ( end.x() - start.x() ) / 2 ) );
    c2.setY( start.y() + ( ( end.y() - start.y() ) ) );

    _path = QPainterPath();

    _path.moveTo( start );
    _path.cubicTo( c1, c2, end );
    _path.cubicTo( c2, c1, start );

    setPath( _path );
}

void QtpWire::paint( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* )
{
    if ( _startPin->collidesWithItem( _endPin ) )
    {
        return;
    }

    updatePosition();

    if ( isSelected() )
    {
        painter->setPen( QPen( _color, 1, Qt::DashLine ) );
    }
    else
    {
        painter->setPen( QPen( _color, 3, Qt::SolidLine ) );
    }

    painter->drawPath( _path );
}
