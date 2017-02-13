#ifndef COMMANDS_H
#define COMMANDS_H
#include <map>
#include <iostream>

typedef unsigned char byte;

class Commands {
public:
    Commands(std::map<std::string, byte>& messageMap);
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
 
    bool isSenderUi();
    bool isServerCommand();

};

#endif // COMMANDS_H
