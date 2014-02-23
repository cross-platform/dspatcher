#include <DiagMain.h>

#include <QApplication>

int main( int argv, char* args[] )
{
  Q_INIT_RESOURCE( DiagRes );

  QApplication app( argv, args );
  DiagMain mainWindow;
  mainWindow.setGeometry( 100, 100, 800, 500 );
  mainWindow.show();

  return app.exec();
}
