#include <QtpScene.h>
#include <QtpPin.h>
#include <QtpWire.h>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

QtpScene::QtpScene( QMenu* compMenu, QObject* parent )
    : QGraphicsScene( parent )
{
  _compMenu = compMenu;
  _mode = MoveComp;
  _compType = QtpComp::CompType();
  _line = 0;
  _compColor = Qt::white;
  _lineColor = Qt::black;
  _pinHovered = 0;
}

void QtpScene::setLineColor( const QColor& color )
{
  _lineColor = color;
  if( isItemChange( QtpWire::Type ) )
  {
    QtpWire* wire = qgraphicsitem_cast< QtpWire* >( selectedItems().first() );
    wire->setColor( _lineColor );
    update();
  }
}

void QtpScene::setCompColor( const QColor& color )
{
  _compColor = color;
  if( isItemChange( QtpComp::Type ) )
  {
    QtpComp* comp = qgraphicsitem_cast< QtpComp* >( selectedItems().first() );
    comp->setBrush( _compColor );
  }
}

QColor QtpScene::compColor() const
{
  return _compColor;
}

QColor QtpScene::lineColor() const
{
  return _lineColor;
}

void QtpScene::setMode( Mode mode )
{
  _mode = mode;
}

void QtpScene::setCompType( QtpComp::CompType type )
{
  _compType = type;
}

void QtpScene::bringToFront()
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

void QtpScene::sendToBack()
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

void QtpScene::deleteItem()
{
  // remove wires first as they can be deleted with pins,
  // hence leaving invalid wire pointers behind
  foreach( QGraphicsItem* item, selectedItems() )
  {
    if( item->type() == QtpWire::Type )
    {
      delete item;
    }
  }

  foreach( QGraphicsItem* item, selectedItems() )
  {
    delete item;
  }
}

void QtpScene::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
  if( mouseEvent->button() != Qt::LeftButton )
    return;

  QList< QGraphicsItem* > startItems = items( mouseEvent->scenePos() );

  if( _mode == InsertComp )
  {
    QtpComp* comp = new QtpComp( _compType, _compMenu, mouseEvent->scenePos() );

    comp->setColor( _compColor );
    addItem( comp );

    emit compInserted();
  }
  else if( startItems.count() && startItems.first()->type() == QtpPin::Type )
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

void QtpScene::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
  if( _mode == InsertLine && _line != 0 )
  {
    QLineF newLine( _line->line().p1(), mouseEvent->scenePos() );
    _line->setLine( newLine );
  }

  // clear _pinHovered
  if( _pinHovered )
  {
    _pinHovered->hover( false );
    _pinHovered = 0;
  }

  // update _pinHovered
  QList< QGraphicsItem* > hoverItems = items( mouseEvent->scenePos() );
  if( hoverItems.count() && hoverItems.first() == _line )
  {
    hoverItems.removeFirst();
  }
  if( hoverItems.count() && hoverItems.first()->type() == QtpPin::Type )
  {
    QtpPin* pin = qgraphicsitem_cast< QtpPin* >( hoverItems.first() );
    pin->hover( true );
    _pinHovered = pin;
  }

  QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void QtpScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
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
        startItems.first()->type() == QtpPin::Type && endItems.first()->type() == QtpPin::Type &&
        startItems.first() != endItems.first() )
    {
      QtpPin* startPin = qgraphicsitem_cast< QtpPin* >( startItems.first() );
      QtpPin* endPin = qgraphicsitem_cast< QtpPin* >( endItems.first() );

      if( startPin->pinType() != endPin->pinType() )
      {
        if( startPin->pinType() == QtpPin::InPin && endPin->pinType() == QtpPin::OutPin )
        {
          std::swap( startPin, endPin );
        }

        QtpWire* wire = new QtpWire( startPin, endPin );
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

bool QtpScene::isItemChange( int type )
{
  foreach( QGraphicsItem* item, selectedItems() )
  {
    if( item->type() == type )
      return true;
  }

  return false;
}
