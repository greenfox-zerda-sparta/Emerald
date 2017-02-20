#include "MessageLogfile.h"

MessageLogfile::MessageLogfile() {
  messageLogBuffer = "";
  messageLogFilename = "Smart_Home_" + localTimer->GetTimeFileFormat() + ".txt";
}


void MessageLogfile::MessageLogging(LogLevel _logLevel, std::string buffer) {
  const static char* LogLevelStr[] = { "UI", "Device", "Log", "Warning", "Error" };
  logMutex.lock();
  messageLogfile.open(messageLogFilename.c_str(), std::ios::app);
  std::cout << "[" << LogLevelStr[_logLevel] << "] " << localTimer->GetTimeFileFormat() << " " << buffer << std::endl;
  messageLogfile << "[" << LogLevelStr[_logLevel] << "] " << buffer << std::endl;
  messageLogfile.close();
  logMutex.unlock();
}


