#include "Logfile.h"
using namespace std;

Logfile::Logfile() {
  messagelogbuffer = "";
 // std::ofstream logfile;
  messagelogfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
  logging = true;
//  std::ofstream devicelog;
  devicelogfilename = "Smart_Home_Device_Log_" + LocalTimer->GetTimeFileFormat() + ".txt";
}


void Logfile::message_log_buffer(LogLevel _loglevel, std::string messagelogbuffer) {

  const static char* LogLevelStr[] = { "UI", "Device", "Warning", "Error" };

  logmutex.lock();
  if (logging) {
    messagelogfile.open(messagelogfilename.c_str(), std::ios::app);
    messagelogfile << "[" << LogLevelStr[_loglevel] << "]" << messagelogbuffer << endl;
    messagelogfile.close();
  }
  logmutex.unlock();
}

void Logfile::device_log_buffer(std::string devicelogbuffer) {

  logmutex.lock();
  if (logging) {
    devicelogfile.open(devicelogfilename.c_str(), std::ios::app);
    devicelogfile << devicelogbuffer << endl;
    devicelogfile.close();
  }
  logmutex.unlock();
}

bool Logfile::get_logging_status() {
  return logging;
}