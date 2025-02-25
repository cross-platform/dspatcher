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

#ifndef QTPPIN_H
#define QTPPIN_H

#include <QGraphicsPolygonItem>
#include <QList>

class QtpWire;

class QtpPin : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 2
    };

    enum PinType
    {
        InPin,
        OutPin
    };

    QtpPin( PinType pinType, QString pinName, int pinIndex, QGraphicsItem* parent = 0 );
    ~QtpPin();

    std::string name();
    int index();
    int type() const;

    bool addWire( QtpWire* newWire );
    void removeWire( QtpWire* wire );
    void removeWires();

    QList<QtpWire*> wires();

    PinType pinType() const;
    QPolygonF polygon() const;

    void hover( bool hover );

protected:
    QVariant itemChange( GraphicsItemChange change, const QVariant& value );

private:
    void setName( QString name );

private:
    PinType _pinType;
    QPolygonF _polygon;
    QList<QtpWire*> _wires;
    QGraphicsTextItem* _nameText;
    int _pinIndex;
};

#endif  // QTPPIN_H
