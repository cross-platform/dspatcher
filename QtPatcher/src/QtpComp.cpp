/******************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2020, Marcus Tomlinson

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

#include <QtpComp.h>
#include <QtpPin.h>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtWidgets>

QtpComp::QtpComp( CompInfo const& compInfo, int id, QPointF const& position, QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
{
    setPos( position );

    _id = id;
    _color = Qt::white;
    _compInfo = compInfo;
    _contextMenu = new QMenu();

    _hw = 15;
    _hh = 15;

    _polygon << QPointF( -_hw, -_hh ) << QPointF( _hw, -_hh ) << QPointF( _hw, _hh ) << QPointF( -_hw, _hh ) << QPointF( -_hw, -_hh );

    _nameText = new QGraphicsTextItem( this );

    setName( compInfo.typeName );
    foreach ( QString const& pin, compInfo.inPins )
    {
        addInPin( pin );
    }
    foreach ( QString const& pin, compInfo.outPins )
    {
        addOutPin( pin );
    }

    setPolygon( _polygon );
}

QtpComp::~QtpComp()
{
    delete _contextMenu;
    removeInPins();
    removeOutPins();
}

int QtpComp::id()
{
    return _id;
}

int QtpComp::type() const
{
    return Type;
}

void QtpComp::setColor( const QColor& color )
{
    _color = color;
    setBrush( _color );

    foreach ( QtpPin* pin, _inPins )
    {
        pin->setBrush( _color );
    }

    foreach ( QtpPin* pin, _outPins )
    {
        pin->setBrush( _color );
    }
}

void QtpComp::addInPin( QString pinName )
{
    QtpPin* pin = new QtpPin( QtpPin::InPin, pinName, _inPins.size(), this );
    pin->setPos( -_hw - 6, -_hh + 15 + 20 * _inPins.size() );
    pin->setBrush( _color );
    _inPins.push_back( pin );

    updatePolygon();
}

void QtpComp::addOutPin( QString pinName )
{
    QtpPin* pin = new QtpPin( QtpPin::OutPin, pinName, _outPins.size(), this );
    pin->setPos( _hw + 6, -_hh + 15 + 20 * _outPins.size() );
    pin->setBrush( _color );
    _outPins.push_back( pin );

    updatePolygon();
}

void QtpComp::removeInPin()
{
    delete _inPins.back();
    _inPins.pop_back();

    updatePolygon();
}

void QtpComp::removeOutPin()
{
    delete _outPins.back();
    _outPins.pop_back();

    updatePolygon();
}

void QtpComp::removeInPins()
{
    foreach ( QtpPin* pin, _inPins )
    {
        delete pin;
    }

    _inPins.clear();

    updatePolygon();
}

void QtpComp::removeOutPins()
{
    foreach ( QtpPin* pin, _outPins )
    {
        delete pin;
    }

    _outPins.clear();

    updatePolygon();
}

QMenu* QtpComp::contextMenu()
{
    return _contextMenu;
}

void QtpComp::setWidget( QWidget* widget )
{
    _widget = widget;

    _hw = _widget->width() / 2;
    _hh = _widget->height() / 2;

    updatePolygon();
}

QWidget* QtpComp::getWidget() const
{
    return _widget;
}

QPixmap QtpComp::image() const
{
    QPixmap pixmap( 250, 250 );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen( QPen( Qt::black, 1 ) );

    int pinsAfterFirst = std::max( _inPins.size(), _outPins.size() );
    pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;

    painter.translate( 125, 125 - ( 10 * pinsAfterFirst ) );
    painter.drawPolyline( _polygon );

    painter.translate( -_hw - 5, 0 );
    foreach ( QtpPin* pin, _inPins )
    {
        painter.drawPolyline( pin->polygon() );
        painter.translate( 0, _hh + 5 );
    }

    painter.resetTransform();
    painter.translate( 125 + _hw + 10, 125 - ( 10 * pinsAfterFirst ) );
    foreach ( QtpPin* pin, _outPins )
    {
        painter.drawPolyline( pin->polygon() );
        painter.translate( 0, _hh + 5 );
    }

    return pixmap;
}

QtpComp::CompInfo QtpComp::compInfo() const
{
    return _compInfo;
}

void QtpComp::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    if ( _contextMenu->actions().size() != 0 )
    {
        scene()->clearSelection();
        setSelected( true );
        _contextMenu->exec( event->screenPos() );
    }
}

void QtpComp::setName( QString name )
{
    _nameText->setPlainText( name );
    _nameText->setPos( -_nameText->boundingRect().width() / 2, -_hh - 24 );
}

void QtpComp::updatePolygon()
{
    int pinsAfterFirst = std::max( _inPins.size(), _outPins.size() );
    pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;

    auto height = -_hh + 30 + 20 * pinsAfterFirst;
    height = _hh > height ? _hh : height;

    _polygon.replace( 0, QPointF( -_hw, -_hh ) );
    _polygon.replace( 1, QPointF( _hw, -_hh ) );
    _polygon.replace( 2, QPointF( _hw, height ) );
    _polygon.replace( 3, QPointF( -_hw, height ) );
    _polygon.replace( 4, QPointF( -_hw, -_hh ) );

    setPolygon( _polygon );

    _nameText->setPos( -_nameText->boundingRect().width() / 2, -_hh - 24 );
}
