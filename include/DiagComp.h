#ifndef DIAGCOMP_H
#define DIAGCOMP_H

#include <QGraphicsPolygonItem>
#include <QList>

class DiagPin;

class DiagComp : public QGraphicsPolygonItem
{
public:
  enum
  {
    Type = UserType + 1
  };

  enum CompType
  {
    Process, Process2, Process3
  };

  DiagComp( CompType compType, QMenu* contextMenu, QPointF const& position, QGraphicsItem* parent = 0 );
  ~DiagComp();

  int type() const;

  void setColor( const QColor& color );

  void addPin( DiagPin* pin );
  void removePins();

  QPixmap image() const;
  CompType compType() const;

protected:
  void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
  QColor _color;
  CompType _compType;
  QPolygonF _polygon;
  QMenu* _contextMenu;
  QList< DiagPin* > _pins;
  QGraphicsTextItem* _nameText;
};

#endif // DIAGCOMP_H
