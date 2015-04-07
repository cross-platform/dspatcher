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

#ifndef QTPDIAG_H
#define QTPDIAG_H

#include <QtpComp.h>

#include <QGraphicsScene>

class QtpPin;

class QtpDiag : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode
    {
        InsertComp,
        InsertLine,
        MoveComp
    };

    explicit QtpDiag(QObject* parent = 0);

    void setLineColor(const QColor& color);
    void setCompColor(const QColor& color);

    QColor compColor() const;
    QColor lineColor() const;

signals:
    void compInserted(QtpComp* comp);
    void compRemoved(int compId);
    void wireConnected(int fromComp, int fromPin, int toComp, int toPin);
    void wireDisconnected(int fromComp, int fromPin, int toComp, int toPin);

public slots:
    void setMode(Mode mode);
    void setNextComp(QtpComp::CompInfo const& compInfo);

    void bringToFront();
    void sendToBack();
    void deleteItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:
    bool isItemChange(int type);

    int _compId;
    QtpComp::CompInfo _nextComp;
    Mode _mode;
    bool _leftButtonDown;
    QPointF _startPoint;
    QGraphicsLineItem* _line;
    QColor _compColor;
    QColor _lineColor;
    QtpPin* _pinHovered;
};

#endif  // QTPDIAG_H
