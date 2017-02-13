#include "Logfile.h"
using namespace std;

Logfile::Logfile() {
  logbuffer = "";
  std::ofstream logfile;
  logfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
  logging = true;
}


void Logfile::log_buffer(std::string logbuffer) {
  // add mutex and enum of log levels
  if (logging) {
    logfile.open(logfilename.c_str(), std::ios::app);
    logfile << logbuffer << endl;
    logfile.close();
  }
}

bool Logfile::get_logging_status() {
  return logging;
}