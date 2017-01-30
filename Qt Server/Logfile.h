#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "MyTime.h"

class Logfile {
private:
  std::string logbuffer;
  std::string logfilename;
  std::ofstream logfile;
  MyTime* LocalTimer;
  bool logging;

public:
  Logfile();
  ~Logfile();
  void log_buffer(std::string);
  // void print_and_log();
};

template<class T>
std::string toString(const T& t)
{
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

#endif //LOGFILE_H
