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

  struct CompInfo
  {
    CompInfo() {}
    CompInfo( QString compName, QList< QString > compInPins, QList< QString > compOutPins )
      : name( compName ),
        inPins( compInPins ),
        outPins( compOutPins ) {}

    QString name;
    QList< QString > inPins;
    QList< QString > outPins;
  };

  QtpComp( CompInfo const& compInfo, QMenu* contextMenu, QPointF const& position, QGraphicsItem* parent = 0 );
  ~QtpComp();

  int type() const;

  void setColor( const QColor& color );

  void addInPin( QString pinName );
  void addOutPin( QString pinName );
  void removePins();

  QPixmap image() const;
  CompInfo compInfo() const;

protected:
  void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
  void setName( QString name );
  void updatePolygon();

private:
  QColor _color;
  CompInfo _compInfo;
  QPolygonF _polygon;
  QMenu* _contextMenu;
  QList< QtpPin* > _inPins;
  QList< QtpPin* > _outPins;
  QGraphicsTextItem* _nameText;
};

#endif // QTPCOMP_H
