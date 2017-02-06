#include <QtCore/QCoreApplication>
#include "server.h"

// #define TEST             // when this line is in comment, main() runs.
#ifdef TEST
#include "test.cpp"
#endif

#ifndef TEST

int main(int argc, char **argv) {

  QCoreApplication app(argc, argv);

  Server server;
  server.StartServer();

  return app.exec();
}
#endif