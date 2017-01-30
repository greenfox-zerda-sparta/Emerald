#ifndef MYTIME_H
#define MYTIME_H
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>

class MyTime {
public:
  MyTime();
//  std::string GetCurrentTime();
  std::string GetTimeFileFormat();
};

#endif //MYTIME_H
