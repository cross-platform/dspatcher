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
    pin->setPos( -_hw - 6, 20 * _inPins.size() );
    pin->setBrush( _color );
    _inPins.push_back( pin );

    updatePolygon();
}

void QtpComp::addOutPin( QString pinName )
{
    QtpPin* pin = new QtpPin( QtpPin::OutPin, pinName, _outPins.size(), this );
    pin->setPos( _hw + 6, 20 * _outPins.size() );
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

    _polygon.replace( 0, QPointF( -_hw, -_hh ) );
    _polygon.replace( 1, QPointF( _hw, -_hh ) );
    _polygon.replace( 2, QPointF( _hw, _hh + ( 20 * pinsAfterFirst ) ) );
    _polygon.replace( 3, QPointF( -_hw, _hh + ( 20 * pinsAfterFirst ) ) );
    _polygon.replace( 4, QPointF( -_hw, -_hh ) );

    setPolygon( _polygon );

    _nameText->setPos( -_nameText->boundingRect().width() / 2, -_hh - 24 );
}
