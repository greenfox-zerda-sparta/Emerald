#include "MessageLogfile.h"

MessageLogfile::MessageLogfile() {
  messagelogbuffer = "";
  messagelogfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
}


void MessageLogfile::MessageLogging(LogLevel _loglevel, std::string buffer) {
  const static char* LogLevelStr[] = { "UI", "Device", "Warning", "Error" };
  logmutex.lock();
  messagelogfile.open(messagelogfilename.c_str(), std::ios::app);
  std::cout << "[" << LogLevelStr[_loglevel] << "] " << LocalTimer->GetTimeFileFormat() << " " << buffer << std::endl;
  messagelogfile << "[" << LogLevelStr[_loglevel] << "] " << LocalTimer->GetTimeFileFormat() << " " << buffer << std::endl;
  messagelogfile.close();
  logmutex.unlock();
}


