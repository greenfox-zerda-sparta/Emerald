#include "MessageLogfile.h"

MessageLogfile::MessageLogfile() {
  logBuffer = "";
  messageLogfilename = "Smart_Home_" + localTimer->GetTimeFileFormat() + ".txt";
}


void MessageLogfile::MessageLogging(LogLevel _logLevel, std::string buffer) {
  const static char* LogLevelStr[] = { "UI", "Device", "Log", "Warning", "Error" };
  logMutex.lock();
  messageLog.open(messageLogfilename.c_str(), std::ios::app);
  std::cout << localTimer->GetTimeFileFormat() << " " << buffer << std::endl;
  messageLog << localTimer->GetTimeFileFormat() << "[" << LogLevelStr[_logLevel] << "] " << buffer << std::endl;
  messageLog.close();
  logMutex.unlock();
}


