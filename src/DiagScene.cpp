#include <DiagScene.h>
#include <DiagPin.h>
#include <DiagWire.h>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

DiagScene::DiagScene( QMenu* compMenu, QObject* parent )
    : QGraphicsScene( parent )
{
  _compMenu = compMenu;
  _mode = MoveComp;
  _compType = DiagComp::Process;
  _line = 0;
  _compColor = Qt::white;
  _lineColor = Qt::black;
}

void DiagScene::setLineColor( const QColor& color )
{
  _lineColor = color;
  if( isItemChange( DiagWire::Type ) )
  {
    DiagWire* wire = qgraphicsitem_cast< DiagWire* >( selectedItems().first() );
    wire->setColor( _lineColor );
    update();
  }
}

void DiagScene::setCompColor( const QColor& color )
{
  _compColor = color;
  if( isItemChange( DiagComp::Type ) )
  {
    DiagComp* comp = qgraphicsitem_cast< DiagComp* >( selectedItems().first() );
    comp->setBrush( _compColor );
  }
}

QColor DiagScene::compColor() const
{
  return _compColor;
}

QColor DiagScene::lineColor() const
{
  return _lineColor;
}

void DiagScene::setMode( Mode mode )
{
  _mode = mode;
}

void DiagScene::setCompType( DiagComp::CompType type )
{
  _compType = type;
}

void DiagScene::bringToFront()
{
  if( selectedItems().isEmpty() )
    return;

  QGraphicsItem* selectedItem = selectedItems().first();
  QList< QGraphicsItem* > overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach( QGraphicsItem* item, overlapItems )
  {
    if( item->zValue() >= zValue )
      zValue = item->zValue() + 0.1;
  }

  selectedItem->setZValue( zValue );
}

void DiagScene::sendToBack()
{
  if( selectedItems().isEmpty() )
    return;

  QGraphicsItem* selectedItem = selectedItems().first();
  QList< QGraphicsItem* > overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach( QGraphicsItem* item, overlapItems)
  {
    if( item->zValue() <= zValue )
      zValue = item->zValue() - 0.1;
  }

  selectedItem->setZValue( zValue );
}

void DiagScene::deleteItem()
{
  // remove wires first as they can be deleted with pins,
  // hence leaving invalid wire pointers behind
  foreach( QGraphicsItem* item, selectedItems() )
  {
    if( item->type() == DiagWire::Type )
    {
      delete item;
    }
  }

  foreach( QGraphicsItem* item, selectedItems() )
  {
    delete item;
  }
}

void DiagScene::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
  if( mouseEvent->button() != Qt::LeftButton )
    return;

  QList< QGraphicsItem* > startItems = items( mouseEvent->scenePos() );

  if( _mode == InsertComp )
  {
    DiagComp* comp = new DiagComp( _compType, _compMenu, mouseEvent->scenePos() );

    comp->setColor( _compColor );
    addItem( comp );

    emit compInserted( comp );
  }
  else if( startItems.count() && startItems.first()->type() == DiagPin::Type )
  {
    _mode = InsertLine;
    _line = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(), mouseEvent->scenePos() ) );
    _line->setPen( QPen( _lineColor, 2 ) );
    addItem( _line );
  }
  else if( startItems.count() == 0 )
  {
    views()[0]->setDragMode( QGraphicsView::ScrollHandDrag );
  }
  else
  {
    _mode = MoveComp;
  }

  QGraphicsScene::mousePressEvent( mouseEvent );
}

void DiagScene::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
  if( _mode == InsertLine && _line != 0 )
  {
    QLineF newLine( _line->line().p1(), mouseEvent->scenePos() );
    _line->setLine( newLine );
  }

  // update pin hover info
  foreach( QGraphicsItem* item, items() )
  {
    if( item->type() == DiagPin::Type )
    {
      DiagPin* pin = qgraphicsitem_cast< DiagPin* >( item );
      pin->hover( false );
    }
  }

  QList< QGraphicsItem* > hoverItems = items( mouseEvent->scenePos() );
  if( hoverItems.count() && hoverItems.first() == _line )
    hoverItems.removeFirst();

  if( hoverItems.count() && hoverItems.first()->type() == DiagPin::Type )
  {
    DiagPin* pin = qgraphicsitem_cast< DiagPin* >( hoverItems.first() );
    pin->hover( true );
  }

  QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void DiagScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
  views()[0]->setDragMode( QGraphicsView::NoDrag );

  if( _line != 0 && _mode == InsertLine )
  {
    QList< QGraphicsItem* > startItems = items( _line->line().p1() );
    if( startItems.count() && startItems.first() == _line )
      startItems.removeFirst();
    QList< QGraphicsItem* > endItems = items( _line->line().p2() );
    if( endItems.count() && endItems.first() == _line )
      endItems.removeFirst();

    removeItem( _line );
    delete _line;

    if( startItems.count() && endItems.count() &&
        startItems.first()->type() == DiagPin::Type && endItems.first()->type() == DiagPin::Type &&
        startItems.first() != endItems.first() )
    {
      DiagPin* startPin = qgraphicsitem_cast< DiagPin* >( startItems.first() );
      DiagPin* endPin = qgraphicsitem_cast< DiagPin* >( endItems.first() );

      if( startPin->pinType() != endPin->pinType() )
      {
        if( startPin->pinType() == DiagPin::InPin && endPin->pinType() == DiagPin::OutPin )
        {
          std::swap( startPin, endPin );
        }

        DiagWire* wire = new DiagWire( startPin, endPin );
        if( startPin->addWire( wire ) )
        {
          endPin->addWire( wire );

          wire->setColor( _lineColor );
          wire->setZValue( 1000.0 );
          wire->updatePosition();

          addItem( wire );
        }
        else
        {
          delete wire;
        }
      }
    }
  }
  _line = 0;
  _mode = MoveComp;
  QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

bool DiagScene::isItemChange( int type )
{
  foreach( QGraphicsItem* item, selectedItems() )
  {
    if( item->type() == type )
      return true;
  }

  return false;
}
