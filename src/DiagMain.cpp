#include <DiagScene.h>
#include <DiagMain.h>

#include <QtWidgets>

DiagMain::DiagMain()
{
  createActions();
  createToolBox();
  createMenus();

  _scene = new DiagScene( _compMenu, this );
  _scene->setSceneRect( QRectF( 0, 0, 5000, 5000 ) );
  connect( _scene, SIGNAL( compInserted( DiagComp* ) ), this, SLOT( compInserted( DiagComp* ) ) );
  connect( _toFrontAction, SIGNAL( triggered() ), _scene, SLOT( bringToFront() ) );
  connect( _sendBackAction, SIGNAL( triggered() ), _scene, SLOT( sendToBack() ) );
  connect( _deleteAction, SIGNAL( triggered() ), _scene, SLOT( deleteItem() ) );

  _view = new QGraphicsView( _scene );
  _view->viewport()->installEventFilter( this );
  _view->setMouseTracking( true );

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget( _toolBox );
  layout->addWidget( _view );

  QWidget* widget = new QWidget;
  widget->setLayout( layout );

  setCentralWidget( widget );
  setWindowTitle( tr( "DSPatcher" ) );
  setUnifiedTitleAndToolBarOnMac( true );
}

void DiagMain::buttonGroupClicked( int id )
{
  QList< QAbstractButton* > buttons = _buttonGroup->buttons();
  foreach( QAbstractButton* button, buttons )
  {
    if( _buttonGroup->button(id) != button )
      button->setChecked(false);
  }

  _scene->setCompType( DiagComp::CompType( id ) );
  _scene->setMode( DiagScene::InsertComp );
}

void DiagMain::compInserted( DiagComp* comp )
{
  _scene->setMode( DiagScene::MoveComp );
  _buttonGroup->button( int( comp->compType() ) )->setChecked( false );
}

void DiagMain::about()
{
  QMessageBox::about( this, tr( "About DSPatcher" ), tr( "<b>DSPatcher</b> is a graphical patching "
      "tool for creating DSPatch circuits." ) );
}

void DiagMain::createToolBox()
{
  _buttonGroup = new QButtonGroup( this );
  _buttonGroup->setExclusive( false );
  connect( _buttonGroup, SIGNAL( buttonClicked(int) ), this, SLOT( buttonGroupClicked(int) ) );
  QGridLayout* layout = new QGridLayout;
  layout->addWidget( createCellWidget( tr( "1i-1o" ), DiagComp::Process ), 0, 0 );
  layout->addWidget( createCellWidget( tr( "2i-2o" ), DiagComp::Process2 ), 0, 1 );
  layout->addWidget( createCellWidget( tr( "3i-3o" ), DiagComp::Process3 ), 1, 0 );

  layout->setRowStretch( 3, 10 );
  layout->setColumnStretch( 2, 10 );

  QWidget* compWidget = new QWidget;
  compWidget->setLayout( layout );

  _toolBox = new QToolBox;
  _toolBox->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Ignored ) );
  _toolBox->setMinimumWidth( compWidget->sizeHint().width() );
  _toolBox->addItem( compWidget, tr( "Components" ) );
}

void DiagMain::createActions()
{
  _toFrontAction = new QAction( QIcon( ":/bringtofront.png" ), tr( "Bring to &Front" ), this );
  _toFrontAction->setShortcut( tr( "Ctrl+F" ) );
  _toFrontAction->setStatusTip( tr( "Bring component to front" ) );

  _sendBackAction = new QAction( QIcon( ":/sendtoback.png" ), tr( "Send to &Back" ), this );
  _sendBackAction->setShortcut( tr( "Ctrl+B" ) );
  _sendBackAction->setStatusTip( tr( "Send component to back" ) );

  _deleteAction = new QAction( QIcon( ":/delete.png" ), tr( "&Delete" ), this );
  _deleteAction->setShortcut( tr( "Delete" ) );
  _deleteAction->setStatusTip( tr( "Delete component from diagram" ) );

  _exitAction = new QAction( tr( "E&xit" ), this );
  _exitAction->setShortcuts( QKeySequence::Quit );
  _exitAction->setStatusTip( tr( "Quit DSPatcher" ) );
  connect( _exitAction, SIGNAL( triggered() ), this, SLOT( close() ) );

  _aboutAction = new QAction( tr( "Ab&out" ), this );
  _aboutAction->setShortcut( tr( "Ctrl+O" ) );
  connect( _aboutAction, SIGNAL( triggered() ), this, SLOT( about() ) );
}

void DiagMain::createMenus()
{
  _fileMenu = menuBar()->addMenu( tr( "&File" ) );
  _fileMenu->addAction( _exitAction );

  _compMenu = menuBar()->addMenu( tr( "&Component" ) );
  _compMenu->addAction( _deleteAction );
  _compMenu->addSeparator();
  _compMenu->addAction( _toFrontAction );
  _compMenu->addAction( _sendBackAction );

  _aboutMenu = menuBar()->addMenu( tr( "&Help" ) );
  _aboutMenu->addAction( _aboutAction );
}

QWidget* DiagMain::createCellWidget( const QString& text, DiagComp::CompType type )
{
  DiagComp comp( type, _compMenu, QPointF() );
  QIcon icon( comp.image() );

  QToolButton* button = new QToolButton;
  button->setIcon( icon );
  button->setIconSize( QSize( 50, 50 ) );
  button->setCheckable( true );
  _buttonGroup->addButton( button, int( type ) );

  QGridLayout* layout = new QGridLayout;
  layout->addWidget( button, 0, 0, Qt::AlignHCenter );
  layout->addWidget( new QLabel( text ), 1, 0, Qt::AlignCenter );

  QWidget* widget = new QWidget;
  widget->setLayout( layout );

  return widget;
}

bool DiagMain::eventFilter( QObject*, QEvent *event )
{
  if( event->type() == QEvent::Wheel && QApplication::keyboardModifiers() == Qt::ControlModifier )
  {
    QWheelEvent* wheelEvent = static_cast< QWheelEvent* >( event );
    if( wheelEvent->orientation() == Qt::Vertical )
    {
      double angle = wheelEvent->angleDelta().y();
      double factor = qPow( 1.0015, angle );

      _view->scale( factor, factor );
      _view->centerOn( _zoomScenePos );

      QPointF deltaViewportPos = _zoomViewportPos
          - QPointF( _view->viewport()->width() / 2.0, _view->viewport()->height() / 2.0 );
      QPointF viewportCenter = _view->mapFromScene( _zoomScenePos ) - deltaViewportPos;

      _view->centerOn( _view->mapToScene( viewportCenter.toPoint() ) );
    }

    return true;
  }
  else if( event->type() == QEvent::MouseMove || event->type() == QEvent::Wheel )
  {
    QPoint mousePos;
    if( event->type() == QEvent::MouseMove )
    {
      mousePos = static_cast< QMouseEvent* >( event )->pos();
    }
    else if( event->type() == QEvent::Wheel )
    {
      mousePos = static_cast< QWheelEvent* >( event )->pos();
    }

    _zoomViewportPos = mousePos;
    _zoomScenePos = _view->mapToScene( mousePos );
  }

  return false;
}
