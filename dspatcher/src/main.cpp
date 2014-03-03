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

  comp.name = "Wave Decoder";
  mainWindow.registerComp( comp );
  comp.name = "Mp3 Decoder";
  mainWindow.registerComp( comp );
  comp.name = "Gain";
  mainWindow.registerComp( comp );
  comp.name = "Ambisonix";
  mainWindow.registerComp( comp );
  comp.name = "Audio Device";
  mainWindow.registerComp( comp );

  return app.exec();
}
