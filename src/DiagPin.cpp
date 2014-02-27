#include <DiagPin.h>
#include <DiagWire.h>

#include <QGraphicsScene>
#include <QPainter>

DiagPin::DiagPin( PinType pinType, QString pinName, QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
{
  _pinType = pinType;

  _polygon << QPointF( -6, 5 ) << QPointF( 6, 5 ) << QPointF( 6, -5 ) << QPointF( -6, -5 )
      << QPointF( -6, 5 );

  _nameText = new QGraphicsTextItem( this );
  _nameText->setOpacity( 0.08 );

  setName( pinName );
  setPolygon( _polygon );
}

DiagPin::~DiagPin()
{
  // delete wires attached to pin when pin is deleted
  foreach( DiagWire* wire, _wires )
  {
    delete wire;
  }

  removeWires();
}

int DiagPin::type() const
{
  return Type;
}

bool DiagPin::addWire( DiagWire* newWire )
{
  foreach( DiagWire* wire, _wires )
  {
    if( ( _pinType == InPin && wire->startPin() == newWire->startPin() ) ||
        ( _pinType == OutPin && wire->endPin() == newWire->endPin() ) )
    {
      // wire already exists
      return false;
    }
  }

  _wires.append( newWire );
  return true;
}

void DiagPin::removeWire( DiagWire* wire )
{
  int index = _wires.indexOf( wire );

  if( index != -1 )
  {
    _wires.removeAt( index );
  }
}

void DiagPin::removeWires()
{
  _wires.clear();
}

DiagPin::PinType DiagPin::pinType() const
{
  return _pinType;
}

QPolygonF DiagPin::polygon() const
{
  return _polygon;
}

void DiagPin::hover( bool hover )
{
  if( hover )
  {
    _nameText->setOpacity( 1.0 );
  }
  else
  {
    _nameText->setOpacity( 0.08 );
  }
}

QVariant DiagPin::itemChange( GraphicsItemChange change, const QVariant& value )
{
  if( change == QGraphicsItem::ItemPositionChange )
  {
    foreach( DiagWire* wire, _wires )
    {
      wire->updatePosition();
    }
  }

  return value;
}

void DiagPin::setName( QString name )
{
  _nameText->setPlainText( name );

  if( _pinType == InPin )
  {
    _nameText->setPos( -_nameText->boundingRect().width() - 7, -13 );
  }
  else
  {
    _nameText->setPos( 7,-13 );
  }
}
