#ifndef DIAGWIRE_H
#define DIAGWIRE_H

#include <QGraphicsLineItem>

class DiagPin;

class DiagWire : public QGraphicsLineItem
{
public:
  enum
  {
    Type = UserType + 3
  };

  DiagWire( DiagPin* startPin, DiagPin* endPin, QGraphicsItem* parent = 0 );
  ~DiagWire();

  int type() const;

  void setColor( const QColor& color );

  DiagPin* startPin() const;
  DiagPin* endPin() const;

  void updatePosition();

protected:
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );

private:
  DiagPin* _startPin;
  DiagPin* _endPin;
  QColor _color;
  QPolygonF _wireHead;
};

#endif // DIAGWIRE_H
