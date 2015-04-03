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

    QtpPin(PinType pinType, QString pinName, QGraphicsItem* parent = 0);
    ~QtpPin();

    std::string name();
    int type() const;

    bool addWire(QtpWire* newWire);
    void removeWire(QtpWire* wire);
    void removeWires();

    PinType pinType() const;
    QPolygonF polygon() const;

    void hover(bool hover);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    void setName(QString name);

private:
    PinType _pinType;
    QPolygonF _polygon;
    QList<QtpWire*> _wires;
    QGraphicsTextItem* _nameText;
};

#endif  // QTPPIN_H
