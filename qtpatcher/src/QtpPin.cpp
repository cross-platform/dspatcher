#include <QtpPin.h>
#include <QtpWire.h>

#include <QGraphicsScene>
#include <QPainter>

QtpPin::QtpPin(PinType pinType, QString pinName, int pinIndex, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent)
    , _pinIndex(pinIndex)
{
    _pinType = pinType;

    _polygon << QPointF(-6, 5) << QPointF(6, 5) << QPointF(6, -5) << QPointF(-6, -5) << QPointF(-6, 5);

    _nameText = new QGraphicsTextItem(this);
    _nameText->setOpacity(0.08);

    setName(pinName);
    setPolygon(_polygon);
}

QtpPin::~QtpPin()
{
    // delete wires attached to pin when pin is deleted
    foreach (QtpWire* wire, _wires)
    {
        delete wire;
    }

    removeWires();
}

std::string QtpPin::name()
{
    return _nameText->toPlainText().toStdString();
}

int QtpPin::index()
{
    return _pinIndex;
}

int QtpPin::type() const
{
    return Type;
}

bool QtpPin::addWire(QtpWire* newWire)
{
    foreach (QtpWire* wire, _wires)
    {
        if ((_pinType == InPin && wire->startPin() == newWire->startPin()) ||
            (_pinType == OutPin && wire->endPin() == newWire->endPin()))
        {
            // wire already exists
            return false;
        }
    }

    _wires.append(newWire);
    return true;
}

void QtpPin::removeWire(QtpWire* wire)
{
    int index = _wires.indexOf(wire);

    if (index != -1)
    {
        _wires.removeAt(index);
    }
}

void QtpPin::removeWires()
{
    _wires.clear();
}

QList<QtpWire*> QtpPin::wires()
{
    return _wires;
}

QtpPin::PinType QtpPin::pinType() const
{
    return _pinType;
}

QPolygonF QtpPin::polygon() const
{
    return _polygon;
}

void QtpPin::hover(bool hover)
{
    if (hover)
    {
        _nameText->setOpacity(1.0);
    }
    else
    {
        _nameText->setOpacity(0.08);
    }
}

QVariant QtpPin::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (QtpWire* wire, _wires)
        {
            wire->updatePosition();
        }
    }

    return value;
}

void QtpPin::setName(QString name)
{
    _nameText->setPlainText(name);

    if (_pinType == InPin)
    {
        _nameText->setPos(-_nameText->boundingRect().width() - 7, -13);
    }
    else
    {
        _nameText->setPos(7, -13);
    }
}
