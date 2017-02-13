#include "Logfile.h"
using namespace std;

Logfile::Logfile() {
  logbuffer = "";
  std::ofstream logfile;
  logfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
  logging = true;
}


void Logfile::log_buffer(LogLevel _loglevel, std::string logbuffer) {

  const static char* LogLevelStr[] = { "UI", "Device", "Warning", "Error" };

  logmutex.lock();
  if (logging) {
    logfile.open(logfilename.c_str(), std::ios::app);
    logfile << "[" << LogLevelStr[_loglevel] << "]" << logbuffer << endl;
    logfile.close();
  }
  logmutex.unlock();
}

bool Logfile::get_logging_status() {
  return logging;
}