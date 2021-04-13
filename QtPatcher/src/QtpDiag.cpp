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

#include <QtpDiag.h>
#include <QtpPin.h>
#include <QtpWire.h>

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QWidget>

QtpDiag::QtpDiag( QObject* parent )
    : QGraphicsScene( parent )
{
    _compId = 0;
    _mode = MoveComp;
    _nextComp = QtpComp::CompInfo();
    _line = 0;
    _compColor = Qt::white;

    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    _mt = std::mt19937( seed );

    std::uniform_int_distribution<unsigned int> dist;
    _lineColor = QColor::fromRgb( dist( _mt ) );

    _pinHovered = 0;
}

void QtpDiag::setLineColor( const QColor& color )
{
    _lineColor = color;
    if ( isItemChange( QtpWire::Type ) )
    {
        QtpWire* wire = qgraphicsitem_cast<QtpWire*>( selectedItems().first() );
        wire->setColor( _lineColor );
        update();
    }
}

void QtpDiag::setCompColor( const QColor& color )
{
    _compColor = color;
    if ( isItemChange( QtpComp::Type ) )
    {
        QtpComp* comp = qgraphicsitem_cast<QtpComp*>( selectedItems().first() );
        comp->setBrush( _compColor );
    }
}

QColor QtpDiag::compColor() const
{
    return _compColor;
}

QColor QtpDiag::lineColor() const
{
    return _lineColor;
}

void QtpDiag::setMode( Mode mode )
{
    _mode = mode;
}

void QtpDiag::setNextComp( QtpComp::CompInfo const& compInfo )
{
    _nextComp = compInfo;
}

void QtpDiag::bringToFront()
{
    if ( selectedItems().isEmpty() )
    {
        return;
    }

    QGraphicsItem* selectedItem = selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach ( QGraphicsItem* item, overlapItems )
    {
        if ( item->zValue() >= zValue )
        {
            zValue = item->zValue() + 0.1;
        }
    }

    selectedItem->setZValue( zValue );
}

void QtpDiag::sendToBack()
{
    if ( selectedItems().isEmpty() )
    {
        return;
    }

    QGraphicsItem* selectedItem = selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach ( QGraphicsItem* item, overlapItems )
    {
        if ( item->zValue() <= zValue )
        {
            zValue = item->zValue() - 0.1;
        }
    }

    selectedItem->setZValue( zValue );
}

void QtpDiag::deleteItem()
{
    // remove wires first as they can be deleted with pins,
    // hence leaving invalid wire pointers behind
    foreach ( QGraphicsItem* item, selectedItems() )
    {
        if ( item->type() == QtpWire::Type )
        {
            QtpWire* wire = qgraphicsitem_cast<QtpWire*>( item );
            emit wireDisconnected( qgraphicsitem_cast<QtpComp*>( wire->endPin()->parentItem() )->id(), wire->endPin()->index() );
            delete item;
        }
    }

    foreach ( QGraphicsItem* item, selectedItems() )
    {
        auto comp = qgraphicsitem_cast<QtpComp*>( item );
        if ( comp )
        {
            emit compRemoved( comp->id() );
        }
        else
        {
            foreach ( QGraphicsItem* child, item->childItems() )
            {
                comp = qgraphicsitem_cast<QtpComp*>( child );
                if ( comp )
                {
                    emit compRemoved( comp->id() );
                    break;
                }
            }
        }

        delete item;
    }
}

void QtpDiag::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( mouseEvent->button() != Qt::LeftButton )
    {
        return;
    }

    QList<QGraphicsItem*> startItems = items( mouseEvent->scenePos() );

    if ( _mode == InsertComp )
    {
        QtpComp* comp = new QtpComp( _nextComp, _compId++, mouseEvent->scenePos() );
        emit compInserted( comp );

        auto compWidget = comp->getWidget();
        if ( compWidget )
        {
            auto initPos = mouseEvent->scenePos();

            auto hw = compWidget->width() / 2;
            auto hh = compWidget->height() / 2;

            auto proxyControl =
                addRect( initPos.x() - hw, initPos.y() - hh, comp->boundingRect().width(), comp->boundingRect().height(), Qt::NoPen );
            proxyControl->setFlag( QGraphicsItem::ItemIsMovable, true );
            proxyControl->setFlag( QGraphicsItem::ItemIsSelectable, true );

            auto proxy = addWidget( compWidget );
            proxy->setPos( initPos.x() - hw, initPos.y() - hh );
            proxy->setParentItem( proxyControl );

            addItem( comp );
            comp->setParentItem( proxyControl );
        }
        else
        {
            comp->setColor( _compColor );
            addItem( comp );

            comp->setFlag( QGraphicsItem::ItemIsMovable, true );
            comp->setFlag( QGraphicsItem::ItemIsSelectable, true );
        }
    }
    else if ( startItems.count() && startItems.first()->type() == QtpPin::Type )
    {
        _mode = InsertLine;
        _line = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(), mouseEvent->scenePos() ) );
        _line->setPen( QPen( _lineColor, 2 ) );
        addItem( _line );
    }
    else if ( startItems.count() == 0 )
    {
        views()[0]->setDragMode( QGraphicsView::ScrollHandDrag );
    }
    else
    {
        _mode = MoveComp;
    }

    QGraphicsScene::mousePressEvent( mouseEvent );
}

void QtpDiag::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( _mode == InsertLine && _line != 0 )
    {
        QLineF newLine( _line->line().p1(), mouseEvent->scenePos() );
        _line->setLine( newLine );
    }

    // clear _pinHovered
    if ( _pinHovered )
    {
        _pinHovered->hover( false );
        _pinHovered = 0;
    }

    // update _pinHovered
    QList<QGraphicsItem*> hoverItems = items( mouseEvent->scenePos() );
    if ( hoverItems.count() && hoverItems.first() == _line )
    {
        hoverItems.removeFirst();
    }
    if ( hoverItems.count() && hoverItems.first()->type() == QtpPin::Type )
    {
        QtpPin* pin = qgraphicsitem_cast<QtpPin*>( hoverItems.first() );
        pin->hover( true );
        _pinHovered = pin;
    }

    QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void QtpDiag::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    views()[0]->setDragMode( QGraphicsView::NoDrag );

    if ( _line != 0 && _mode == InsertLine )
    {
        QList<QGraphicsItem*> startItems = items( _line->line().p1() );
        if ( startItems.count() && startItems.first() == _line )
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem*> endItems = items( _line->line().p2() );
        if ( endItems.count() && endItems.first() == _line )
        {
            endItems.removeFirst();
        }

        removeItem( _line );
        delete _line;

        if ( startItems.count() && endItems.count() && startItems.first()->type() == QtpPin::Type &&
             endItems.first()->type() == QtpPin::Type && startItems.first() != endItems.first() )
        {
            QtpPin* startPin = qgraphicsitem_cast<QtpPin*>( startItems.first() );
            QtpPin* endPin = qgraphicsitem_cast<QtpPin*>( endItems.first() );

            if ( startPin->pinType() != endPin->pinType() )
            {
                if ( startPin->pinType() == QtpPin::InPin && endPin->pinType() == QtpPin::OutPin )
                {
                    std::swap( startPin, endPin );
                }

                QtpWire* newWire = new QtpWire( startPin, endPin );
                if ( startPin->addWire( newWire ) )
                {
                    foreach ( QtpWire* wire, endPin->wires() )
                    {
                        delete wire;
                    }
                    endPin->removeWires();

                    endPin->addWire( newWire );

                    newWire->setColor( _lineColor );
                    newWire->setZValue( 1000.0 );
                    newWire->updatePosition();

                    std::uniform_int_distribution<unsigned int> dist;
                    _lineColor = QColor::fromRgb( dist( _mt ) );

                    addItem( newWire );

                    emit wireConnected( qgraphicsitem_cast<QtpComp*>( startPin->parentItem() )->id(), startPin->index(),
                                        qgraphicsitem_cast<QtpComp*>( endPin->parentItem() )->id(), endPin->index() );
                }
                else
                {
                    delete newWire;
                }
            }
        }
    }
    _line = 0;
    _mode = MoveComp;
    QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

bool QtpDiag::isItemChange( int type )
{
    foreach ( QGraphicsItem* item, selectedItems() )
    {
        if ( item->type() == type )
        {
            return true;
        }
    }

    return false;
}
