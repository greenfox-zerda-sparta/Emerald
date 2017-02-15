#ifndef MESSAGELOGFILE_H
#define MESSAGELOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include "MyTime.h"
#include "Device.h"

typedef unsigned char byte;

enum LogLevel { UILog, DeviceLog, Warning, Error };

class MessageLogfile {
private:
  std::string messagelogbuffer;
  std::string messagelogfilename;
  std::ofstream messagelogfile;
  std::string devicelogbuffer;
  std::string devicelogfilename;
  std::ofstream devicelogfile;
  MyTime* LocalTimer;
//  bool logging;
  std::mutex logmutex;
  LogLevel loglevel;
  int numberofids = 7;

public:
  MessageLogfile();
  void message_log_buffer(LogLevel _loglevel, std::string);
//  bool get_logging_status();
};

#endif 
