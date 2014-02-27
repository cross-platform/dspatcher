#include <DiagComp.h>
#include <DiagPin.h>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagComp::DiagComp( CompType compType, QMenu* contextMenu, QPointF const& position,
                    QGraphicsItem* parent )
  : QGraphicsPolygonItem( parent )
{
  setPos( position );

  _color = Qt::black;
  _compType = compType;
  _contextMenu = contextMenu;

  _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 15 ) << QPointF( -15, 15 )
           << QPointF( -15, -15 );

  _nameText = new QGraphicsTextItem( this );

  switch( _compType )
  {
    case Process:
    {
      setName("Gain");
      addInPin("Audio In");
      addOutPin("Audio Out");
    }
    break;
    case Process2:
    {
      setName("Audio Device");
      addInPin("Left Spk");
      addInPin("Right Spk");
      addOutPin("Left Mic");
      addOutPin("Right Mic");
    }
    break;
    case Process3:
    {
      setName("Ambisonix");
      addInPin("Audio In");
      addOutPin("Spk LF");
      addOutPin("Spk LR");
      addOutPin("Spk RF");
      addOutPin("Spk RR");
    }
    break;
  }

  setPolygon( _polygon );

  setFlag( QGraphicsItem::ItemIsMovable, true );
  setFlag( QGraphicsItem::ItemIsSelectable, true );
}

DiagComp::~DiagComp()
{
  removePins();
}

int DiagComp::type() const
{
  return Type;
}

void DiagComp::setColor( const QColor& color )
{
  _color = color;
  setBrush( _color );

  foreach( DiagPin* pin, _inPins )
  {
    pin->setBrush( _color );
  }

  foreach( DiagPin* pin, _outPins )
  {
    pin->setBrush( _color );
  }
}

void DiagComp::addInPin( QString pinName )
{
  DiagPin* pin = new DiagPin( DiagPin::InPin, pinName, this );
  pin->setPos( -21, 20 * _inPins.size() );
  pin->setBrush( _color );
  _inPins.append( pin );

  updatePolygon();
}

void DiagComp::addOutPin( QString pinName )
{
  DiagPin* pin = new DiagPin( DiagPin::OutPin, pinName, this );
  pin->setPos( 21, 20 * _outPins.size() );
  pin->setBrush( _color );
  _outPins.append( pin );

  updatePolygon();
}

void DiagComp::removePins()
{
  foreach( DiagPin* pin, _inPins )
  {
    delete pin;
  }

  _inPins.clear();

  foreach( DiagPin* pin, _outPins )
  {
    delete pin;
  }

  _outPins.clear();

  updatePolygon();
}

QPixmap DiagComp::image() const
{
  QPixmap pixmap( 250, 250 );
  pixmap.fill( Qt::transparent );
  QPainter painter( &pixmap );
  painter.setPen( QPen( Qt::black, 1 ) );

  int pinsAfterFirst = std::max( _inPins.size(), _outPins.size() );
  pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;

  painter.translate( 125, 125 - ( 10 * pinsAfterFirst ) );
  painter.drawPolyline( _polygon );

  painter.translate( -20, 0 );
  foreach( DiagPin* pin, _inPins )
  {
    painter.drawPolyline( pin->polygon() );
    painter.translate( 0, 20 );
  }

  painter.resetTransform();
  painter.translate( 150, 125 - ( 10 * pinsAfterFirst ) );
  foreach( DiagPin* pin, _outPins )
  {
    painter.drawPolyline( pin->polygon() );
    painter.translate( 0, 20 );
  }

  return pixmap;
}

DiagComp::CompType DiagComp::compType() const
{
  return _compType;
}

void DiagComp::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
  scene()->clearSelection();
  setSelected( true );
  _contextMenu->exec( event->screenPos() );
}

void DiagComp::setName( QString name )
{
  _nameText->setPlainText( name );
  _nameText->setPos( -_nameText->boundingRect().width() / 2, -39 );
}

void DiagComp::updatePolygon()
{
  int pinsAfterFirst = std::max( _inPins.size(), _outPins.size() );
  pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;

  _polygon.clear();
  _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 15 + ( 20 * pinsAfterFirst ) )
           << QPointF( -15, 15 + ( 20 * pinsAfterFirst )) << QPointF( -15, -15 );
}
