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

#ifndef QTPCOMP_H
#define QTPCOMP_H

#include <QGraphicsPolygonItem>
#include <QList>

class QtpPin;

class QtpComp : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 1
    };

    struct CompInfo
    {
        int typeId;
        QString typeName;
        QList<QString> inPins;
        QList<QString> outPins;
        bool isWidget;
    };

    QtpComp( CompInfo const& compInfo, int id, QPointF const& position, QGraphicsItem* parent = 0 );
    ~QtpComp();

    int id();
    int type() const;

    void setColor( const QColor& color );

    void addInPin( QString pinName );
    void addOutPin( QString pinName );
    void removeInPin();
    void removeOutPin();
    void removeInPins();
    void removeOutPins();

    QMenu* contextMenu();

    void setWidget( QWidget* widget );
    QWidget* getWidget() const;

    QPixmap image() const;
    CompInfo compInfo() const;

protected:
    void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
    void setName( QString name );
    void updatePolygon();

private:
    int _id;
    QColor _color;
    CompInfo _compInfo;
    QPolygonF _polygon;
    QMenu* _contextMenu;
    QList<QtpPin*> _inPins;
    QList<QtpPin*> _outPins;
    QGraphicsTextItem* _nameText;
    QWidget* _widget = nullptr;
    int _hw;
    int _hh;
};

#endif  // QTPCOMP_H
