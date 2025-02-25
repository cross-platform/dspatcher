/******************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
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

#include <QtpPin.h>
#include <QtpWire.h>

#include <QGraphicsScene>
#include <QPainter>

QtpPin::QtpPin( PinType pinType, QString pinName, int pinIndex, QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
    , _pinIndex( pinIndex )
{
    _pinType = pinType;

    _polygon << QPointF( -6, 5 ) << QPointF( 6, 5 ) << QPointF( 6, -5 ) << QPointF( -6, -5 ) << QPointF( -6, 5 );

    _nameText = new QGraphicsTextItem( this );
    _nameText->setOpacity( 0.08 );

    setName( pinName );
    setPolygon( _polygon );
}

QtpPin::~QtpPin()
{
    // delete wires attached to pin when pin is deleted
    foreach ( QtpWire* wire, _wires )
    {
        delete wire;
    }

    removeWires();
}

std::string QtpPin::name()
{
    return _nameText->toPlainText().toUtf8().constData();
}

int QtpPin::index()
{
    return _pinIndex;
}

int QtpPin::type() const
{
    return Type;
}

bool QtpPin::addWire( QtpWire* newWire )
{
    foreach ( QtpWire* wire, _wires )
    {
        if ( ( _pinType == InPin && wire->startPin() == newWire->startPin() ) ||
             ( _pinType == OutPin && wire->endPin() == newWire->endPin() ) )
        {
            // wire already exists
            return false;
        }
    }

    _wires.append( newWire );
    return true;
}

void QtpPin::removeWire( QtpWire* wire )
{
    int index = _wires.indexOf( wire );

    if ( index != -1 )
    {
        _wires.removeAt( index );
    }
}

void QtpPin::removeWires()
{
    _wires.clear();
}

QList<QtpWire*> QtpPin::wires()
{
    return _wires;
}

QtpPin::PinType QtpPin::pinType() const
{
    return _pinType;
}

QPolygonF QtpPin::polygon() const
{
    return _polygon;
}

void QtpPin::hover( bool hover )
{
    if ( !_nameText )
        return;

    if ( hover )
    {
        _nameText->setOpacity( 1.0 );
    }
    else
    {
        _nameText->setOpacity( 0.08 );
    }
}

QVariant QtpPin::itemChange( GraphicsItemChange change, const QVariant& value )
{
    if ( change == QGraphicsItem::ItemPositionChange )
    {
        foreach ( QtpWire* wire, _wires )
        {
            wire->updatePosition();
        }
    }

    return value;
}

void QtpPin::setName( QString name )
{
    _nameText->setPlainText( name );

    if ( _pinType == InPin )
    {
        _nameText->setPos( -_nameText->boundingRect().width() - 7, -13 );
    }
    else
    {
        _nameText->setPos( 7, -13 );
    }
}
