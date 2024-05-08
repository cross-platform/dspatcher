/******************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2024, Marcus Tomlinson

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

#ifndef QTPDIAG_H
#define QTPDIAG_H

#include <QtpComp.h>

#include <QGraphicsScene>

#include <random>

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

    explicit QtpDiag( QObject* parent = 0 );

    void setLineColor( const QColor& color );
    void setCompColor( const QColor& color );

    QColor compColor() const;
    QColor lineColor() const;

signals:
    void compInserted( QtpComp* comp );
    void compRemoved( int compId );
    void wireConnected( int fromComp, int fromPin, int toComp, int toPin );
    void wireDisconnected( int toComp, int toPin );
    void playPauseTriggered();

public slots:
    void setMode( Mode mode );
    void setNextComp( QtpComp::CompInfo const& compInfo );

    void bringToFront();
    void sendToBack();
    void deleteItem();
    void playPause();

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent );

private:
    bool isItemChange( int type );

    int _compId;
    QtpComp::CompInfo _nextComp;
    Mode _mode;
    bool _leftButtonDown;
    QPointF _startPoint;
    QGraphicsLineItem* _line;
    QColor _compColor;
    QColor _lineColor;
    QtpPin* _pinHovered;
    std::mt19937 _mt;
};

#endif  // QTPDIAG_H
