#include <DiagComp.h>
#include <DiagPin.h>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagComp::DiagComp( CompType diagramType, QMenu* contextMenu, QPointF const& position,
    QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
{
  setPos( position );

  _color = Qt::black;
  _compType = diagramType;
  _contextMenu = contextMenu;

  switch( _compType )
  {
      case Process:
      {
        _nameText = new QGraphicsTextItem( this );
        _nameText->setPos(-34,-39);
        _nameText->setPlainText("Process 1");
        DiagPin* pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 0 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 0 );
        addPin( pin );
      }
      _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 15 ) << QPointF( -15, 15 )
          << QPointF( -15, -15 );
      break;
      case Process2:
      {
        _nameText = new QGraphicsTextItem( this );
        _nameText->setPos(-34,-39);
        _nameText->setPlainText("Process 2");
        DiagPin* pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 0 );
        addPin( pin );
        pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 20 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 0 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 20 );
        addPin( pin );
      }
      _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 35 ) << QPointF( -15, 35 )
          << QPointF( -15, -15 );
      break;
      case Process3:
      {
        _nameText = new QGraphicsTextItem( this );
        _nameText->setPos(-34,-39);
        _nameText->setPlainText("Process 3");
        DiagPin* pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 0 );
        addPin( pin );
        pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 20 );
        addPin( pin );
        pin = new DiagPin( DiagPin::OutPin, this );
        pin->setPos( 21, 40 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 0 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 20 );
        addPin( pin );
        pin = new DiagPin( DiagPin::InPin, this );
        pin->setPos( -21, 40 );
        addPin( pin );
      }
      _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 55 ) << QPointF( -15, 55 )
          << QPointF( -15, -15 );
      break;
    default:
      _polygon << QPointF( -120, -80 ) << QPointF( -70, 80 ) << QPointF( 120, 80 ) << QPointF( 70, -80 )
          << QPointF( -120, -80 );
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

  foreach( DiagPin* pin, _pins )
  {
    pin->setBrush( _color );
  }
}

void DiagComp::addPin( DiagPin* pin )
{
  pin->setBrush( _color );
  _pins.append( pin );
}

void DiagComp::removePins()
{
  foreach( DiagPin* pin, _pins )
  {
    delete pin;
  }

  _pins.clear();
}

QPixmap DiagComp::image() const
{
  QPixmap pixmap( 250, 250 );
  pixmap.fill( Qt::transparent );
  QPainter painter( &pixmap );
  painter.setPen( QPen( Qt::black, 8 ) );
  painter.translate( 125, 125 );
  painter.drawPolyline( _polygon );

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
