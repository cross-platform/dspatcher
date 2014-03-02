#ifndef QTPMAIN_H
#define QTPMAIN_H

#include <QtpComp.h>
#include <QMainWindow>

class QtpScene;

class QButtonGroup;
class QGraphicsView;
class QToolBox;

class QtpMain : public QMainWindow
{
Q_OBJECT

public:
  QtpMain();

private slots:
  void buttonGroupClicked( int id );
  void compInserted( QtpComp* comp );
  void about();

private:
  void createToolBox();
  void createActions();
  void createMenus();

  QWidget* createCellWidget( const QString& text, QtpComp::CompType type );

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
};

#endif // QTPMAIN_H
