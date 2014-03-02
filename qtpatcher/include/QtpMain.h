#ifndef QTPMAIN_H
#define QTPMAIN_H

#include <QtpComp.h>
#include <QMainWindow>

class QtpScene;

class QButtonGroup;
class QGraphicsView;
class QGridLayout;
class QToolBox;

class QtpMain : public QMainWindow
{
Q_OBJECT

public:
  QtpMain();

public slots:
  void registerComp( QtpComp::CompType comp );

private slots:
  void buttonGroupClicked( int id );
  void compInserted();
  void about();

private:
  void createToolBox();
  void createActions();
  void createMenus();

  bool eventFilter( QObject* object, QEvent* event );

  QtpScene* _scene;
  QGraphicsView* _view;

  QAction* _exitAction;
  QAction* _addAction;
  QAction* _deleteAction;

  QAction* _toFrontAction;
  QAction* _sendBackAction;
  QAction* _aboutAction;

  QMenu* _fileMenu;
  QMenu* _compMenu;
  QMenu* _aboutMenu;

  QToolBox* _toolBox;
  QButtonGroup* _buttonGroup;

  QPointF _zoomScenePos;
  QPointF _zoomViewportPos;

  QWidget* _compWidget;
  QList< QtpComp::CompType > _compTypes;
};

#endif // QTPMAIN_H
