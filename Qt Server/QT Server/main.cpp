#include <QtCore/QCoreApplication>
#include "Server.h"

// #define TEST             // when this line is in comment, main() runs.
#ifdef TEST
#include "test.cpp"
#endif

#ifndef TEST

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);
  Server server;
  server.RunServer();
  QObject::connect(&server, SIGNAL(Quit()), &app, SLOT(quit()));
  return app.exec();
}
#endif
