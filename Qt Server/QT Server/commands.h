#ifndef COMMANDS_H
#define COMMANDS_H
#include "Device.h"
#include <vector>
#include <memory>
#include <map>
#include <iostream>

typedef unsigned char byte;

class Commands {
public:
    Commands(std::map<std::string, byte>& messageMap, std::shared_ptr<std::vector<Device>> Devices);
    void resetServer();
    void restartServer();
    void stopServer();
    void addDevice();
    void removeDevice();
    void getStatusReport();
    void setData();
    void forwardMessage();
    std::map<std::string, byte> messageMap;
private:
    std::weak_ptr<std::vector<Device>> Devices;
    bool isSenderUi();
    bool isServerCommand();

};

#endif // COMMANDS_H
