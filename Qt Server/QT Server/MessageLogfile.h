#ifndef messageLogfile_H
#define messageLogfile_H

#include <iostream>
#include <fstream>
#include <mutex>
#include "MyTime.h"

typedef unsigned char byte;

enum LogLevel { UILog, DeviceLog, Log, Warning, Error };

class messageLogfile {
private:
  std::string logBuffer;
  std::string messageLogfilename;
  std::ofstream messageLog;
  MyTime* localTimer;
  std::mutex logMutex;
  LogLevel logLevel;

public:
  messageLogfile();
  void MessageLogging(LogLevel _logLevel, std::string buffer);
};

#endif 
