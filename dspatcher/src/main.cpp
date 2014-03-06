#include <QtpMain.h>

#include <QApplication>

int main( int argv, char* args[] )
{
  Q_INIT_RESOURCE( QtpRes );

  QApplication app( argv, args );
  QtpMain mainWindow;
  mainWindow.show();

  QtpComp::CompInfo comp;
  comp.outPins.append( "L Channel" );
  comp.outPins.append( "R Channel" );
  comp.name = "Wave Decoder";
  mainWindow.registerComp( comp );

  comp.inPins.clear();
  comp.outPins.clear();
  comp.outPins.append( "L Channel" );
  comp.outPins.append( "R Channel" );
  comp.name = "Mp3 Decoder";
  mainWindow.registerComp( comp );

  comp.inPins.clear();
  comp.outPins.clear();
  comp.inPins.append( "Input" );
  comp.outPins.append( "Output" );
  comp.name = "Gain";
  mainWindow.registerComp( comp );

  comp.inPins.clear();
  comp.outPins.clear();
  comp.inPins.append( "Mono Input" );
  comp.outPins.append( "FL Output" );
  comp.outPins.append( "FR Output" );
  comp.outPins.append( "RL Output" );
  comp.outPins.append( "RR Output" );
  comp.name = "Ambisonix";
  mainWindow.registerComp( comp );

  comp.inPins.clear();
  comp.outPins.clear();
  comp.inPins.append( "L Input" );
  comp.inPins.append( "R Input" );
  comp.outPins.append( "L Output" );
  comp.outPins.append( "R Output" );
  comp.name = "Audio Device";
  mainWindow.registerComp( comp );

  return app.exec();
}
