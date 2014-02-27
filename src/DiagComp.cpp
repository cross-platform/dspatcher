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

  _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 15 ) << QPointF( -15, 15 )
           << QPointF( -15, -15 );

  _nameText = new QGraphicsTextItem( this );
  _nameText->setPos(-34,-39);

  switch( _compType )
  {
    case Process:
    {
      _nameText->setPlainText("Process 1");
      addInPin();
      addOutPin();
    }
    break;
    case Process2:
    {
      _nameText->setPlainText("Process 2");
      addInPin();
      addInPin();
      addOutPin();
      addOutPin();
    }
    break;
    case Process3:
    {
      _nameText->setPlainText("Process 3");
      addInPin();
      addInPin();
      addInPin();
      addOutPin();
      addOutPin();
      addOutPin();
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

void DiagComp::addInPin()
{
  DiagPin* pin = new DiagPin( DiagPin::InPin, this );
  pin->setPos( -21, 20 * _inPins.size() );
  pin->setBrush( _color );
  _inPins.append( pin );

  updatePolygon();
}

void DiagComp::addOutPin()
{
  DiagPin* pin = new DiagPin( DiagPin::OutPin, this );
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

void DiagComp::updatePolygon()
{
  int pinsAfterFirst = std::max( _inPins.size(), _outPins.size() ) - 1;
  _polygon.clear();
  _polygon << QPointF( -15, -15 ) << QPointF( 15, -15 ) << QPointF( 15, 15 + ( 20 * pinsAfterFirst ) )
           << QPointF( -15, 15 + ( 20 * pinsAfterFirst )) << QPointF( -15, -15 );
}
