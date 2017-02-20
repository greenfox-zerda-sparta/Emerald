#include "MessageLogfile.h"

messageLogfile::messageLogfile() {
  logBuffer = "";
  messageLogfilename = "Smart_Home_" + localTimer->GetTimeFileFormat() + ".txt";
}


void messageLogfile::MessageLogging(LogLevel _logLevel, std::string buffer) {
  const static char* LogLevelStr[] = { "UI", "Device", "Log", "Warning", "Error" };
  logMutex.lock();
  messageLog.open(messageLogfilename.c_str(), std::ios::app);
  std::cout << "[" << LogLevelStr[_logLevel] << "] " << localTimer->GetTimeFileFormat() << " " << buffer << std::endl;
  messageLog << "[" << LogLevelStr[_logLevel] << "] " << buffer << std::endl;
  messageLog.close();
  logMutex.unlock();
}


