#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <QTcpSocket>
#include "DeviceLogfile.h"
#include "MessageConverter.h"
#include "SubDevice.h"

typedef unsigned char byte;

class Commands {
  public:
    Commands(std::vector<Device*>& _addedDevices);
    ~Commands();
    void SetMessageMap(std::map<std::string, byte>& _messageMap);
    void SetAddedDevices(std::vector<Device*>& _addedDevices);
    void SetDeviceMap(std::map<QTcpSocket*, Device*>& _deviceMap);
    void SetBytes(std::vector<byte>& _bytes);
    void RunCommand();
  private:
    DeviceLogfile* deviceLog;
    std::string deviceLogBuffer;
    MessageConverter* msgConvert;
    std::vector<Device*>& addedDevs;
    std::map<std::string, byte> messageMap;
    std::map<byte, void(Commands::*)()> cmdMap;
    std::map<QTcpSocket*, Device*> deviceMap;
    std::vector<byte> bytes;
    void GenerateNextIDs();
    std::string GetIPString();
    std::string GetDeviceText(Device* dev);
    bool IsRoomForDevice();
    void LogDeviceList();
    int IDLow;
    int IDHigh;
    bool IsSenderUi();
    bool IsServerCommand();
    void ResetServer();
    void RestartServer();
    void StopServer();
    void AddDevice();
    void RemoveDevice();
    void GetStatusReport();
    void SetData();
    void ForwardMessage();
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
