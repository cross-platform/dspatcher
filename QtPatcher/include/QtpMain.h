/******************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2025, Marcus Tomlinson

BSD 2-Clause License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

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

    void registerComponent( QtpComp::CompInfo const& compInfo );
    void unregisterComponents();

private slots:
    void buttonGroupClicked( int id );
    void compInserted( QtpComp* comp );
    void about();

private:
    void createToolBox();
    void createActions();
    void createMenus();

    QWidget* createCellWidget( QtpComp::CompInfo compInfo );

    bool eventFilter( QObject* object, QEvent* event );

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
    QWidget* _uiCompWidget;
    QList<QtpComp::CompInfo> _comps;
};

#endif  // QTPMAIN_H
