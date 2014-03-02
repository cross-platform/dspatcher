#include <QtpMain.h>

#include <QApplication>

int main( int argv, char* args[] )
{
  Q_INIT_RESOURCE( QtpRes );

  QApplication app( argv, args );
  QtpMain mainWindow;
  mainWindow.setGeometry( 100, 100, 800, 500 );
  mainWindow.show();

  QtpComp::CompInfo comp;
  comp.inPins.append( "in1" );
  comp.inPins.append( "in2" );
  comp.outPins.append( "out1" );
  comp.outPins.append( "out2" );

  comp.name = "comp1";
  mainWindow.registerComp( comp );
  comp.name = "comp2";
  mainWindow.registerComp( comp );
  comp.name = "comp3";
  mainWindow.registerComp( comp );
  comp.name = "comp4";
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );
  mainWindow.registerComp( comp );

  return app.exec();
}
