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

#include <QtpDiag.h>
#include <QtpMain.h>

#include <QtWidgets>

QtpMain::QtpMain()
{
    Q_INIT_RESOURCE(QtpRes);

    createActions();
    createToolBox();
    createMenus();

    _diagram = new QtpDiag(this);
    _diagram->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(_diagram, &QtpDiag::compInserted, this, &QtpMain::compInserted);

    connect(_toFrontAction, SIGNAL(triggered()), _diagram, SLOT(bringToFront()));
    connect(_sendBackAction, SIGNAL(triggered()), _diagram, SLOT(sendToBack()));
    connect(_deleteAction, SIGNAL(triggered()), _diagram, SLOT(deleteItem()));

    _view = new QGraphicsView(_diagram);
    _view->viewport()->installEventFilter(this);
    _view->setMouseTracking(true);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_toolBox);
    layout->addWidget(_view);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    setGeometry(100, 100, 800, 500);
    setCentralWidget(widget);
    setWindowTitle(tr("DSPatcher"));
    setUnifiedTitleAndToolBarOnMac(true);
}

QtpDiag* QtpMain::diagram()
{
    return _diagram;
}

void QtpMain::registerComponent(QtpComp::CompInfo const& compInfo)
{
    _compWidget->layout()->addWidget(createCellWidget(compInfo));

    _toolBox->deleteLater();
    _toolBox = new QToolBox;
    _toolBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    _toolBox->addItem(_compWidget, tr("Components"));

    centralWidget()->layout()->removeWidget(_view);
    centralWidget()->layout()->addWidget(_toolBox);
    centralWidget()->layout()->addWidget(_view);
}

void QtpMain::unregisterComponents()
{
    _buttonGroup->deleteLater();
    _compWidget->deleteLater();
    _toolBox->deleteLater();

    createToolBox();

    centralWidget()->layout()->removeWidget(_view);
    centralWidget()->layout()->addWidget(_toolBox);
    centralWidget()->layout()->addWidget(_view);
}

void QtpMain::buttonGroupClicked(int id)
{
    QList<QAbstractButton*> buttons = _buttonGroup->buttons();
    foreach (QAbstractButton* button, buttons)
    {
        if (_buttonGroup->button(id) != button)
        {
            button->setChecked(false);
        }
    }

    _diagram->setNextComp(_comps[id]);
    _diagram->setMode(QtpDiag::InsertComp);
}

void QtpMain::compInserted(QtpComp*)
{
    _diagram->setMode(QtpDiag::MoveComp);

    QList<QAbstractButton*> buttons = _buttonGroup->buttons();
    foreach (QAbstractButton* button, buttons)
    {
        button->setChecked(false);
    }
}

void QtpMain::about()
{
    QMessageBox::about(this,
                       tr("About DSPatcher"),
                       tr("<b>DSPatcher</b> is a graphical patching "
                          "tool for creating DSPatch circuits."));
}

void QtpMain::createToolBox()
{
    _buttonGroup = new QButtonGroup(this);
    _buttonGroup->setExclusive(false);
    connect(_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));

    QGridLayout* compLayout = new QGridLayout;
    compLayout->setColumnStretch(1, 1);
    compLayout->setRowStretch(1000, 1);

    _compWidget = new QWidget;
    _compWidget->setLayout(compLayout);

    _toolBox = new QToolBox;
    _toolBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    _toolBox->addItem(_compWidget, tr("Components"));
}

void QtpMain::createActions()
{
    _toFrontAction = new QAction(QIcon(":/bringtofront.png"), tr("Bring to &Front"), this);
    _toFrontAction->setShortcut(tr("Ctrl+F"));
    _toFrontAction->setStatusTip(tr("Bring component to front"));
    addAction(_toFrontAction);

    _sendBackAction = new QAction(QIcon(":/sendtoback.png"), tr("Send to &Back"), this);
    _sendBackAction->setShortcut(tr("Ctrl+B"));
    _sendBackAction->setStatusTip(tr("Send component to back"));
    addAction(_sendBackAction);

    _deleteAction = new QAction(QIcon(":/delete.png"), tr("&Delete"), this);
    QList<QKeySequence> deleteKeys;
    deleteKeys.append(tr("Delete"));
    deleteKeys.append(tr("Backspace"));
    _deleteAction->setShortcuts(deleteKeys);
    _deleteAction->setStatusTip(tr("Delete component from diagram"));
    addAction(_deleteAction);

    _exitAction = new QAction(tr("E&xit"), this);
    _exitAction->setShortcuts(QKeySequence::Quit);
    _exitAction->setStatusTip(tr("Quit DSPatcher"));
    addAction(_exitAction);
    connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    _aboutAction = new QAction(tr("Ab&out"), this);
    _aboutAction->setShortcut(tr("Ctrl+O"));
    addAction(_aboutAction);
    connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void QtpMain::createMenus()
{
    _fileMenu = menuBar()->addMenu(tr("&File"));
    _fileMenu->addAction(_exitAction);

    _compMenu = menuBar()->addMenu(tr("&Component"));
    _compMenu->addAction(_deleteAction);
    _compMenu->addSeparator();
    _compMenu->addAction(_toFrontAction);
    _compMenu->addAction(_sendBackAction);

    _aboutMenu = menuBar()->addMenu(tr("&Help"));
    _aboutMenu->addAction(_aboutAction);
}

QWidget* QtpMain::createCellWidget(QtpComp::CompInfo compInfo)
{
    QtpComp comp(compInfo, 0, QPointF());
    QIcon icon(comp.image());

    QToolButton* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    _comps.append(compInfo);
    _buttonGroup->addButton(button, _comps.size() - 1);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(compInfo.typeName), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

bool QtpMain::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::Wheel && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->orientation() == Qt::Vertical)
        {
            double angle = wheelEvent->angleDelta().y();
            double factor = qPow(1.0015, angle);

            _view->scale(factor, factor);
            _view->centerOn(_zoomDiagPos);

            QPointF deltaViewportPos =
                _zoomViewportPos - QPointF(_view->viewport()->width() / 2.0, _view->viewport()->height() / 2.0);
            QPointF viewportCenter = _view->mapFromScene(_zoomDiagPos) - deltaViewportPos;

            _view->centerOn(_view->mapToScene(viewportCenter.toPoint()));
        }

        return true;
    }
    else if (event->type() == QEvent::MouseMove || event->type() == QEvent::Wheel)
    {
        QPoint mousePos;
        if (event->type() == QEvent::MouseMove)
        {
            mousePos = static_cast<QMouseEvent*>(event)->pos();
        }
        else if (event->type() == QEvent::Wheel)
        {
            mousePos = static_cast<QWheelEvent*>(event)->pos();
        }

        _zoomViewportPos = mousePos;
        _zoomDiagPos = _view->mapToScene(mousePos);
    }

    return false;
}
