#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char **argv) {
  
  QCoreApplication app(argc, argv);

  Server server;
  server.StartServer();

  return app.exec();
}
// ha elkezd irni egy kliens, akkor addig nem fogad