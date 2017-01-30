#include <QtCore/QCoreApplication>
#include "chatterboxserver.h"

int main(int argc, char **argv) {
  
  QCoreApplication app(argc, argv);

  ChatServer server;
  server.StartServer();

  return app.exec();
}
// ha elkezd irni egy kliens, akkor addig nem fogad