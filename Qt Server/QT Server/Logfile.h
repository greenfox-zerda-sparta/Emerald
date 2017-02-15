#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include "MyTime.h"
#include "Device.h"

typedef unsigned char byte;

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
  int numberofids = 7;

public:
  Logfile();
  void message_log_buffer(LogLevel _loglevel, std::string);
  void device_log_buffer(std::string devicelogbuffer);
  bool get_logging_status();
  std::vector<Device> get_devices_vector();
  Device get_device(std::string);
  std::string get_devicelogfilename();
};

template<class T>
std::string toString(const T& t)
{
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

#endif 
