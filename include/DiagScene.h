#ifndef DIAGSCENE_H
#define DIAGSCENE_H

#include <DiagComp.h>
#include <QGraphicsScene>

class DiagPin;

class DiagScene : public QGraphicsScene
{
Q_OBJECT

public:
  enum Mode
  {
    InsertComp, InsertLine, MoveComp
  };

  explicit DiagScene( QMenu* compMenu, QObject* parent = 0 );

  void setLineColor( const QColor& color );
  void setCompColor( const QColor& color );

  QColor compColor() const;
  QColor lineColor() const;

public slots:
  void setMode( Mode mode );
  void setCompType( DiagComp::CompType type );

  void bringToFront();
  void sendToBack();
  void deleteItem();

signals:
  void compInserted( DiagComp* comp );

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent );
  void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent );

private:
  bool isItemChange( int type );

  DiagComp::CompType _compType;
  QMenu* _compMenu;
  Mode _mode;
  bool _leftButtonDown;
  QPointF _startPoint;
  QGraphicsLineItem* _line;
  QColor _compColor;
  QColor _lineColor;
  DiagPin* _pinHovered;
};

#endif // DIAGSCENE_H
