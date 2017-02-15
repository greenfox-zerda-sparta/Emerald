#include "MessageLogfile.h"

MessageLogfile::MessageLogfile() {
  messagelogbuffer = "";
  messagelogfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
//  logging = true;
}


void MessageLogfile::message_log_buffer(LogLevel _loglevel, std::string messagelogbuffer) {

  const static char* LogLevelStr[] = { "UI", "Device", "Warning", "Error" };

  logmutex.lock();
//  if (logging) {
    messagelogfile.open(messagelogfilename.c_str(), std::ios::app);
    messagelogfile << "[" << LogLevelStr[_loglevel] << "] " << messagelogbuffer << std::endl;
    messagelogfile.close();
//  }
  logmutex.unlock();
}

//bool MessageLogfile::get_logging_status() {
//  return logging;
//}


