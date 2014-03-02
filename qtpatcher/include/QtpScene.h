#ifndef QTPSCENE_H
#define QTPSCENE_H

#include <QtpComp.h>
#include <QGraphicsScene>

class QtpPin;

class QtpScene : public QGraphicsScene
{
Q_OBJECT

public:
  enum Mode
  {
    InsertComp, InsertLine, MoveComp
  };

  explicit QtpScene( QMenu* compMenu, QObject* parent = 0 );

  void setLineColor( const QColor& color );
  void setCompColor( const QColor& color );

  QColor compColor() const;
  QColor lineColor() const;

public slots:
  void setMode( Mode mode );
  void setNextComp( QtpComp::CompInfo const& compInfo );

  void bringToFront();
  void sendToBack();
  void deleteItem();

signals:
  void compInserted();

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent );
  void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent );

private:
  bool isItemChange( int type );

  QtpComp::CompInfo _nextComp;
  QMenu* _compMenu;
  Mode _mode;
  bool _leftButtonDown;
  QPointF _startPoint;
  QGraphicsLineItem* _line;
  QColor _compColor;
  QColor _lineColor;
  QtpPin* _pinHovered;
};

#endif // QTPSCENE_H
