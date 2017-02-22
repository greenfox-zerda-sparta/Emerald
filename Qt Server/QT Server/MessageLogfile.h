#ifndef MESSAGELOGFILE_H
#define MESSAGELOGFILE_H

#include <iostream>
#include <fstream>
#include <mutex>
#include "MyTime.h"

typedef unsigned char byte;

enum LogLevel { UILog, DeviceLog, Log, Warning, Error };

class MessageLogfile {
private:
  std::string logBuffer;
  std::string messageLogfilename;
  std::ofstream messageLog;
  MyTime* localTimer;
  std::mutex logMutex;
  LogLevel logLevel;
public:
  MessageLogfile();
  void MessageLogging(LogLevel _logLevel, std::string buffer);
};

#endif 
