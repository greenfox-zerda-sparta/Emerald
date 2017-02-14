#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "MyTime.h"
#include <mutex>

enum LogLevel { UILog, DeviceLog, Warning, Error };

class Logfile {
private:
  std::string messagelogbuffer;
  std::string messagelogfilename;
  std::ofstream messagelogfile;
  std::string devicelogbuffer;
  std::string devicelogfilename;
  std::ofstream devicelogfile;
  MyTime* LocalTimer;
  bool logging;
  std::mutex logmutex;
  LogLevel loglevel;

public:
  Logfile();
  void message_log_buffer(LogLevel _loglevel, std::string);
  void Logfile::device_log_buffer(std::string devicelogbuffer);
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
