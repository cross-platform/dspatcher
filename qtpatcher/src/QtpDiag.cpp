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

#include <QtpDiag.h>
#include <QtpPin.h>
#include <QtpWire.h>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

QtpDiag::QtpDiag(QObject* parent)
    : QGraphicsScene(parent)
{
    _compId = 0;
    _mode = MoveComp;
    _nextComp = QtpComp::CompInfo();
    _line = 0;
    _compColor = Qt::white;
    _lineColor = Qt::black;
    _pinHovered = 0;
}

void QtpDiag::setLineColor(const QColor& color)
{
    _lineColor = color;
    if (isItemChange(QtpWire::Type))
    {
        QtpWire* wire = qgraphicsitem_cast<QtpWire*>(selectedItems().first());
        wire->setColor(_lineColor);
        update();
    }
}

void QtpDiag::setCompColor(const QColor& color)
{
    _compColor = color;
    if (isItemChange(QtpComp::Type))
    {
        QtpComp* comp = qgraphicsitem_cast<QtpComp*>(selectedItems().first());
        comp->setBrush(_compColor);
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

void QtpDiag::setMode(Mode mode)
{
    _mode = mode;
}

void QtpDiag::setNextComp(QtpComp::CompInfo const& compInfo)
{
    _nextComp = compInfo;
}

void QtpDiag::bringToFront()
{
    if (selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem* selectedItem = selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems)
    {
        if (item->zValue() >= zValue)
        {
            zValue = item->zValue() + 0.1;
        }
    }

    selectedItem->setZValue(zValue);
}

void QtpDiag::sendToBack()
{
    if (selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem* selectedItem = selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems)
    {
        if (item->zValue() <= zValue)
        {
            zValue = item->zValue() - 0.1;
        }
    }

    selectedItem->setZValue(zValue);
}

void QtpDiag::deleteItem()
{
    // remove wires first as they can be deleted with pins,
    // hence leaving invalid wire pointers behind
    foreach (QGraphicsItem* item, selectedItems())
    {
        if (item->type() == QtpWire::Type)
        {
            QtpWire* wire = qgraphicsitem_cast<QtpWire*>(item);
            emit wireDisconnected(qgraphicsitem_cast<QtpComp*>(wire->startPin()->parentItem())->id(),
                                  wire->startPin()->index(),
                                  qgraphicsitem_cast<QtpComp*>(wire->endPin()->parentItem())->id(),
                                  wire->endPin()->index());
            delete item;
        }
    }

    foreach (QGraphicsItem* item, selectedItems())
    {
        emit compRemoved(qgraphicsitem_cast<QtpComp*>(item)->id());
        delete item;
    }
}

void QtpDiag::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }

    QList<QGraphicsItem*> startItems = items(mouseEvent->scenePos());

    if (_mode == InsertComp)
    {
        QtpComp* comp = new QtpComp(_nextComp, _compId++, mouseEvent->scenePos());

        comp->setColor(_compColor);
        addItem(comp);

        emit compInserted(comp);
    }
    else if (startItems.count() && startItems.first()->type() == QtpPin::Type)
    {
        _mode = InsertLine;
        _line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        _line->setPen(QPen(_lineColor, 2));
        addItem(_line);
    }
    else if (startItems.count() == 0)
    {
        views()[0]->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        _mode = MoveComp;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void QtpDiag::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (_mode == InsertLine && _line != 0)
    {
        QLineF newLine(_line->line().p1(), mouseEvent->scenePos());
        _line->setLine(newLine);
    }

    // clear _pinHovered
    if (_pinHovered)
    {
        _pinHovered->hover(false);
        _pinHovered = 0;
    }

    // update _pinHovered
    QList<QGraphicsItem*> hoverItems = items(mouseEvent->scenePos());
    if (hoverItems.count() && hoverItems.first() == _line)
    {
        hoverItems.removeFirst();
    }
    if (hoverItems.count() && hoverItems.first()->type() == QtpPin::Type)
    {
        QtpPin* pin = qgraphicsitem_cast<QtpPin*>(hoverItems.first());
        pin->hover(true);
        _pinHovered = pin;
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void QtpDiag::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    views()[0]->setDragMode(QGraphicsView::NoDrag);

    if (_line != 0 && _mode == InsertLine)
    {
        QList<QGraphicsItem*> startItems = items(_line->line().p1());
        if (startItems.count() && startItems.first() == _line)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem*> endItems = items(_line->line().p2());
        if (endItems.count() && endItems.first() == _line)
        {
            endItems.removeFirst();
        }

        removeItem(_line);
        delete _line;

        if (startItems.count() && endItems.count() && startItems.first()->type() == QtpPin::Type &&
            endItems.first()->type() == QtpPin::Type && startItems.first() != endItems.first())
        {
            QtpPin* startPin = qgraphicsitem_cast<QtpPin*>(startItems.first());
            QtpPin* endPin = qgraphicsitem_cast<QtpPin*>(endItems.first());

            if (startPin->pinType() != endPin->pinType())
            {
                if (startPin->pinType() == QtpPin::InPin && endPin->pinType() == QtpPin::OutPin)
                {
                    std::swap(startPin, endPin);
                }

                QtpWire* newWire = new QtpWire(startPin, endPin);
                if (startPin->addWire(newWire))
                {
                    foreach (QtpWire* wire, endPin->wires())
                    {
                        emit wireDisconnected(qgraphicsitem_cast<QtpComp*>(wire->startPin()->parentItem())->id(),
                                              wire->startPin()->index(),
                                              qgraphicsitem_cast<QtpComp*>(wire->endPin()->parentItem())->id(),
                                              wire->endPin()->index());
                        delete wire;
                    }
                    endPin->removeWires();

                    endPin->addWire(newWire);

                    newWire->setColor(_lineColor);
                    newWire->setZValue(1000.0);
                    newWire->updatePosition();

                    addItem(newWire);

                    emit wireConnected(qgraphicsitem_cast<QtpComp*>(startPin->parentItem())->id(),
                                       startPin->index(),
                                       qgraphicsitem_cast<QtpComp*>(endPin->parentItem())->id(),
                                       endPin->index());
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
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool QtpDiag::isItemChange(int type)
{
    foreach (QGraphicsItem* item, selectedItems())
    {
        if (item->type() == type)
        {
            return true;
        }
    }

    return false;
}
