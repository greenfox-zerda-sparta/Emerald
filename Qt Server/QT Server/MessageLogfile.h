#ifndef MESSAGELOGFILE_H
#define MESSAGELOGFILE_H

#include <fstream>
#include <mutex>
#include "MyTime.h"

typedef unsigned char byte;

enum LogLevel { UILog, DeviceLog, Warning, Error };

class MessageLogfile {
private:
  std::string messageLogBuffer;
  std::string messageLogFilename;
  std::ofstream messageLogfile;
  MyTime* localTimer;
  std::mutex logMutex;
  LogLevel logLevel;

public:
  MessageLogfile();
  void MessageLogging(LogLevel _logLevel, std::string buffer);
};

#endif 
