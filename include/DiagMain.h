#ifndef DIAGMAIN_H
#define DIAGMAIN_H

#include <DiagComp.h>
#include <QMainWindow>

class DiagScene;

class QButtonGroup;
class QGraphicsView;
class QToolBox;

class DiagMain : public QMainWindow
{
Q_OBJECT

public:
  DiagMain();

private slots:
  void buttonGroupClicked( int id );
  void compInserted( DiagComp* comp );
  void about();

private:
  void createToolBox();
  void createActions();
  void createMenus();

  QWidget* createCellWidget( const QString& text, DiagComp::CompType type );

  bool eventFilter( QObject* object, QEvent* event );

  DiagScene* _scene;
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

#endif // DIAGMAIN_H
