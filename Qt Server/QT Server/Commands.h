#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <QtNetwork>
#include "DeviceLogfile.h"
#include "MessageConverter.h"
#include "MessageLogfile.h"
#include "SubDevice.h"

typedef unsigned char byte;

class Server;
class Commands {
  public:
    Commands(std::vector<std::shared_ptr<Device>>& _addedDevices, std::shared_ptr<MessageLogfile> msgLog, Server* server = nullptr);
    ~Commands();
    void SetMessageMap(std::map<std::string, byte>& _messageMap);
    void SetAddedDevices(std::vector<std::shared_ptr<Device>>& _addedDevices);
    void SetDeviceMap(std::map<QTcpSocket*, std::shared_ptr<Device>>& _deviceMap);
    void SetBytes(std::vector<byte>& _bytes);
    void RunCommand();
  private:
    DeviceLogfile* deviceLog;
    std::shared_ptr<MessageLogfile> msgLogger;
    std::string deviceLogBuffer;
    std::string msgLog;
    MessageConverter* msgConvert;
    std::vector<std::shared_ptr<Device>>& addedDevs;
    std::map<std::string, byte> messageMap;
    std::map<byte, void(Commands::*)()> cmdMap;
    std::map<QTcpSocket*, std::shared_ptr<Device>> deviceMap;
    std::vector<byte> bytes;
    void GenerateNextIDs();
    std::vector<byte> MakeUiFeedback(byte cmd, byte body1, byte body2);
    std::string GetIPString();
    std::string GetDeviceText(std::shared_ptr<Device> dev);
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
    void StatusReport();
    void SetData();
    void ForwardMessage();
    void DevForwardMessageToUi();
    void CommandReplyError();
    void CommandReplySuccess();
    void(Commands::*ptr_resetServer)();
    void(Commands::*ptr_restartServer)();
    void(Commands::*ptr_stopServer)();
    void(Commands::*ptr_addDevice)();
    void(Commands::*ptr_removeDevice)();
    void(Commands::*ptr_getStatusReport)();
    void(Commands::*ptr_statusReport)();
    void(Commands::*ptr_setData)();
    void(Commands::*ptr_forwardMessage)();
    void(Commands::*ptr_devforwardMessageToUi)();
    void(Commands::*ptr_cmdReplyError)();
    void(Commands::*ptr_cmdReplySuccess)();
    Server* server;
};

#endif // COMMANDS_H
