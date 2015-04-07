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
    };

    QtpComp(CompInfo const& compInfo, int id, QPointF const& position, QGraphicsItem* parent = 0);
    ~QtpComp();

    int id();
    int type() const;

    void setColor(const QColor& color);

    void addInPin(QString pinName);
    void addOutPin(QString pinName);
    void removeInPin();
    void removeOutPin();
    void removeInPins();
    void removeOutPins();

    QMenu* contextMenu();

    QPixmap image() const;
    CompInfo compInfo() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
    void setName(QString name);
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
};

#endif  // QTPCOMP_H
