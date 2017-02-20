#ifndef DEVICE_H
#define DEVICE_H

#include <map>
#include <string>


typedef unsigned char byte;

struct IDs {
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  byte homeID;
  byte floorID;
  byte roomID;
};

class Device {
  protected:
    byte deviceIDHigh;
    byte deviceIDLow;
    byte groupID;
    byte homeID;
    byte floorID;
    byte roomID;
    std::string IP;
    bool isDeviceWorking;
    bool isOnline = false;
  public:
    Device();
    Device(IDs ids, std::string _IP, bool _isDeviceWorking = true);
    Device(std::map<std::string, byte> messageMap, std::string _IP, bool _isDeviceWorking = true);
    byte GetDeviceIDHigh();
    byte GetDeviceIDLow();
    byte GetGroupID();
    byte GetHomeID();
    byte GetFloorID();
    byte GetRoomID();
    std::string GetIP();
    byte IsWorking();
    void SetIsOnline(bool);
    bool GetIsOnline();

};


#endif
