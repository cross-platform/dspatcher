#ifndef QTPCOMP_H
#define QTPCOMP_H

#include <QGraphicsPolygonItem>
#include <QList>

class QtpPin;

class QtpComp : public QGraphicsPolygonItem
{
public:
  enum
  {
    Type = UserType + 1
  };

  struct CompType
  {
    CompType() {}
    CompType( QString compName, QList< QString > compInPins, QList< QString > compOutPins )
      : name( compName ),
        inPins( compInPins ),
        outPins( compOutPins ) {}
    QString name;
    QList< QString > inPins;
    QList< QString > outPins;
  };

  QtpComp( CompType compType, QMenu* contextMenu, QPointF const& position, QGraphicsItem* parent = 0 );
  ~QtpComp();

  int type() const;

  void setColor( const QColor& color );

  void addInPin( QString pinName );
  void addOutPin( QString pinName );
  void removePins();

  QPixmap image() const;
  CompType compType() const;

protected:
  void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
  void setName( QString name );
  void updatePolygon();

private:
  QColor _color;
  CompType _compType;
  QPolygonF _polygon;
  QMenu* _contextMenu;
  QList< QtpPin* > _inPins;
  QList< QtpPin* > _outPins;
  QGraphicsTextItem* _nameText;
};

#endif // QTPCOMP_H
