#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <QTcpSocket>
#include <string>
#include "DeviceLogfile.h"
#include "MessageConverter.h"
#include "SubDevice.h"

typedef unsigned char byte;

class Commands {
  public:
    Commands(std::vector<Device*>& _addedDevices);
    ~Commands();
    void setMessageMap(std::map<std::string, byte>& _messageMap);
    void setAddedDevices(std::vector<Device*>& _addedDevices);
    void setDeviceMap(std::map<QTcpSocket*, Device*>& _deviceMap);
    void setBytes(std::vector<byte>& _bytes);
    void runCommand();
  private:
    DeviceLogfile* devicelog;
    std::string deviceLogBuffer;
    MessageConverter* msgConvert;
    std::vector<Device*>& addedDevs;
    std::map<std::string, byte> messageMap;
    std::map<byte, void(Commands::*)()> cmdMap;
    std::map<QTcpSocket*, Device*> deviceMap;
    std::vector<byte> bytes;
    void generateNextIDs();
    std::string getIPString();
    std::string getDeviceText(Device* dev);
    bool IsRoomForDevice();
    void logDeviceList();
    int IDLow;
    int IDHigh;
    bool isSenderUi();
    bool isServerCommand();
    void resetServer();
    void restartServer();
    void stopServer();
    void addDevice();
    void removeDevice();
    void getStatusReport();
    void setData();
    void forwardMessage();
    void(Commands::*ptr_resetServer)();
    void(Commands::*ptr_restartServer)();
    void(Commands::*ptr_stopServer)();
    void(Commands::*ptr_addDevice)();
    void(Commands::*ptr_removeDevice)();
    void(Commands::*ptr_getStatusReport)();
    void(Commands::*ptr_setData)();
    void(Commands::*ptr_forwardMessage)();
};

#endif // COMMANDS_H
