#ifndef MESSAGELOGFILE_H
#define MESSAGELOGFILE_H

#include <iostream>
#include <fstream>
#include <mutex>
#include "MyTime.h"

typedef unsigned char byte;

enum LogLevel { UILog, DeviceLog, Warning, Error };

class MessageLogfile {
private:
  std::string messagelogbuffer;
  std::string messagelogfilename;
  std::ofstream messagelogfile;
  MyTime* LocalTimer;
  std::mutex logmutex;
  LogLevel loglevel;

public:
  MessageLogfile();
  void MessageLogging(LogLevel _loglevel, std::string buffer);
};

#endif 
