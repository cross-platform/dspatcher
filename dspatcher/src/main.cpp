#include <QtpMain.h>

#include <QApplication>

int main( int argv, char* args[] )
{
  Q_INIT_RESOURCE( QtpRes );

  QApplication app( argv, args );
  QtpMain mainWindow;
  mainWindow.setGeometry( 100, 100, 800, 500 );
  mainWindow.show();

  return app.exec();
}
