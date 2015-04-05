#ifndef QTPWIRE_H
#define QTPWIRE_H

#include <QGraphicsLineItem>

class QtpPin;

class QtpWire : public QGraphicsLineItem
{
public:
    enum
    {
        Type = UserType + 3
    };

    QtpWire(QtpPin* startPin, QtpPin* endPin, QGraphicsItem* parent = 0);
    ~QtpWire();

    int type() const;

    void setColor(const QColor& color);

    QtpPin* startPin() const;
    QtpPin* endPin() const;

    void updatePosition();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

private:
    QtpPin* _startPin;
    QtpPin* _endPin;
    QColor _color;
    QPolygonF _wireHead;
};

#endif  // QTPWIRE_H
