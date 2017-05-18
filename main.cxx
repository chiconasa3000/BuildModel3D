#include <QApplication>
#include "Create3DBones.h"
 
int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );
 
  Create3DBones create3dBones;
  create3dBones.show();
 
  return app.exec();
}
