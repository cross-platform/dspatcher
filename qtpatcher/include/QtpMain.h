/************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of QtPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LGPLv3.txt included in the packaging of this
file. Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

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
