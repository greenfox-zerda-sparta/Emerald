#include "Logfile.h"

using namespace std;

Logfile::Logfile() {
  logbuffer = "";
  std::ofstream logfile;
  logfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
  logging = true;
}

Logfile::~Logfile() {

}

void Logfile::log_buffer(std::string logbuffer) {
  if (logging) {
    logfile.open(logfilename.c_str(), std::ios::app);
    logfile << logbuffer << endl;
    logfile.close();
  }
}
/*
void Logfile::print_and_log() {
cout << logbuffer << endl;
log_buffer();
}
*/
// logbuffer = LocalTimer->GetCurrentTime() + "Client connected. There are now " + toString(clientCount) + " client(s) connected.";
