#ifndef DIAGPIN_H
#define DIAGPIN_H

#include <QGraphicsPolygonItem>
#include <QList>

class DiagWire;

class DiagPin : public QGraphicsPolygonItem
{
public:
  enum
  {
    Type = UserType + 2
  };

  enum PinType
  {
    InPin, OutPin
  };

  DiagPin( PinType pinType, QGraphicsItem* parent = 0 );
  ~DiagPin();

  int type() const;

  bool addWire( DiagWire* newWire );
  void removeWire( DiagWire* wire );
  void removeWires();

  PinType pinType() const;
  QPolygonF polygon() const;

  void hover( bool hover );

protected:
  QVariant itemChange( GraphicsItemChange change, const QVariant& value );

private:
  PinType _pinType;
  QPolygonF _polygon;
  QList< DiagWire* > _wires;
  QGraphicsTextItem* _nameText;
};

#endif // DIAGPIN_H
