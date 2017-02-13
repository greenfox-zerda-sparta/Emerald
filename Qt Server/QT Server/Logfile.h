#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "MyTime.h"
#include <mutex>

enum LogLevel { UI, Device, Warning, Error };

class Logfile {
private:
  std::string logbuffer;
  std::string logfilename;
  std::ofstream logfile;
  MyTime* LocalTimer;
  bool logging;
  std::mutex logmutex;
  LogLevel loglevel;

public:
  Logfile();
  void log_buffer(LogLevel _loglevel, std::string);
  bool get_logging_status();
};

template<class T>
std::string toString(const T& t)
{
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

#endif 
