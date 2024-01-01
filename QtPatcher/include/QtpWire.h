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

#ifndef QTPWIRE_H
#define QTPWIRE_H

#include <QGraphicsLineItem>

class QtpPin;

class QtpWire : public QGraphicsPathItem
{
public:
    enum
    {
        Type = UserType + 3
    };

    QtpWire( QtpPin* startPin, QtpPin* endPin, QGraphicsItem* parent = 0 );
    ~QtpWire();

    int type() const;

    void setColor( const QColor& color );

    QtpPin* startPin() const;
    QtpPin* endPin() const;

    void updatePosition();

protected:
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );

private:
    QtpPin* _startPin;
    QtpPin* _endPin;
    QColor _color;
    QPainterPath _path;
};

#endif  // QTPWIRE_H
