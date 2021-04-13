/******************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
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
