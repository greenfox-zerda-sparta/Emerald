#include <ctime>
#include <string>

#include "MyTime.h"
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS

MyTime::MyTime() {}

//std::string Time::GetCurrentTime() {
//  time_t     now = time(0);
//  struct tm  tstruct;
//  char       buf[80];
//  tstruct = *localtime(&now);
//  strftime(buf, sizeof(buf), "%Y-%m-%d %X ", &tstruct);
//  return buf;
//}

std::string MyTime::GetTimeFileFormat() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H-%M-%S", &tstruct);
  return buf;
}