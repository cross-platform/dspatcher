#ifndef QTPMAIN_H
#define QTPMAIN_H

#include <QtpComp.h>

#include <QMainWindow>

class QtpDiag;

class QButtonGroup;
class QGraphicsView;
class QGridLayout;
class QToolBox;

class QtpMain : public QMainWindow
{
    Q_OBJECT

public:
    QtpMain();

    QtpDiag* diagram();

    void registerComponent(QtpComp::CompInfo const& compInfo);

private slots:
    void buttonGroupClicked(int id);
    void compInserted(QtpComp* comp);
    void about();

private:
    void createToolBox();
    void createActions();
    void createMenus();

    QWidget* createCellWidget(QtpComp::CompInfo compInfo);

    bool eventFilter(QObject* object, QEvent* event);

    QtpDiag* _diagram;
    QGraphicsView* _view;

    QAction* _exitAction;
    QAction* _deleteAction;

    QAction* _toFrontAction;
    QAction* _sendBackAction;
    QAction* _aboutAction;

    QMenu* _fileMenu;
    QMenu* _compMenu;
    QMenu* _aboutMenu;

    QToolBox* _toolBox;
    QButtonGroup* _buttonGroup;

    QPointF _zoomDiagPos;
    QPointF _zoomViewportPos;

    QWidget* _compWidget;
    QList<QtpComp::CompInfo> _comps;
};

#endif  // QTPMAIN_H
