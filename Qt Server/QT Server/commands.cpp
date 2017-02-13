#include "commands.h"

Commands::Commands(std::map<std::string, byte>& messageMap)
{
    this->messageMap = messageMap;
}

bool Commands::isSenderUi() {
    return (messageMap["senderIDHigh"] == 255 && messageMap["senderIDLow"] == 253);
}


bool Commands::isServerCommand() {
    return (messageMap["targetIDHigh"] == 255 && messageMap["targetIDLow"] == 254 && isSenderUi());
}

void Commands::resetServer() {
  //  if(isServerCommand() && messageMap["cmdID"] == 253) {
        std::cout << "RESETTING SERVER\n" << std::endl; // reset server;  delete all devices
  //  }
}

void Commands::restartServer() {
    if(isServerCommand() && messageMap["cmdID"] == 254) {
        std::cout << "RESTARTING SERVER\n" << std::endl; // restart server;
    }
}

void Commands::stopServer() {
  //  if(isServerCommand() && messageMap["cmdID"] == 255) {
        std::cout << "STOPPING SERVER\n" << std::endl; //stop server;
  //  }
}

void Commands::addDevice() {
    if(isServerCommand() && messageMap["cmdID"] == 247) {
        std::cout << "ADDING DEVICE\n" << std::endl; //add device;
    }
}

void Commands::removeDevice() {
    if(isServerCommand() && messageMap["cmdID"] == 248) {
        std::cout << "REMOVING DEVICE\n" << std::endl; //remove device;
    }
}

void Commands::getStatusReport() {
    if(isSenderUi() && messageMap["cmdID"] == 246) {
        std::cout << "TO DEVICES/GETTING INFOS FROM DEVICES\n" << std::endl; //getting reports from devices;
    }
}

void Commands::setData() {
    if(isSenderUi() && messageMap["cmdID"] == 239) {
        std::cout << "TO DEVICES/TO SET ID\n" << std::endl; //devices to set id;
    }
}

void Commands::forwardMessage() {
        std::cout << "FORWARDING MESSAGE TO TARGET DEVICE\n" << std::endl; //forward message;
}
