#include "commands.h"
#include <algorithm>

Commands::Commands() {
    ptr_resetServer = &Commands::resetServer;
    ptr_restartServer = &Commands::restartServer;
    ptr_stopServer = &Commands::stopServer;
    ptr_addDevice = &Commands::addDevice;
    ptr_removeDevice = &Commands::removeDevice;
    ptr_getStatusReport = &Commands::getStatusReport;
    ptr_setData = &Commands::setData;
    ptr_forwardMessage = &Commands::forwardMessage;

    cmdMap[239] = ptr_setData;
    cmdMap[246] = ptr_getStatusReport;
    cmdMap[247] = ptr_addDevice;
    cmdMap[248] = ptr_removeDevice;
    cmdMap[253] = ptr_resetServer;
    cmdMap[254] = ptr_restartServer;
    cmdMap[255] = ptr_stopServer;
    cmdMap[0] = ptr_forwardMessage;
    cmdMap[1] = ptr_forwardMessage;
    cmdMap[2] = ptr_forwardMessage;
    cmdMap[3] = ptr_forwardMessage;
    cmdMap[4] = ptr_forwardMessage;
    cmdMap[5] = ptr_forwardMessage;
}

void Commands::setMessageMap(std::map<std::string, byte>& messageMap) {
  this->messageMap = messageMap;
}

void Commands::setAddedDevices(std::vector<Device*>& _addedDevices) {
  this->addedDevices = _addedDevices;
}

void Commands::runCommand() {
  if (cmdMap.count(messageMap["cmdID"]) < 1) {
    std::cout << "INVALID COMMAND" << std::endl;
  } else {
    (this->*cmdMap[messageMap["cmdID"]])();
  }
}

bool Commands::isSenderUi() {
    return (messageMap["senderIDHigh"] == 255 && messageMap["senderIDLow"] == 253);
}

bool Commands::isServerCommand() {
    return (messageMap["targetIDHigh"] == 255 && messageMap["targetIDLow"] == 254 && isSenderUi());
}

void Commands::resetServer() {
    if(isServerCommand()) {
        std::cout << "RESETTING SERVER" << std::endl; // reset server;  delete all devices
    } else {
      std::cerr << "Invalid command: target must be the server." << std::endl;
    }
}

void Commands::restartServer() {
    if(isServerCommand()) {
        std::cout << "RESTARTING SERVER" << std::endl; // restart server;
    } else {
      std::cerr << "Invalid command: target must be the server." << std::endl;
    }
}

void Commands::stopServer() {
    if(isServerCommand()) {
        std::cout << "STOPPING SERVER" << std::endl; //stop server;
    } else {
      std::cerr << "Invalid command: target must be the server." << std::endl;
    }
}

void Commands::addDevice() {
    if(isServerCommand()) {
        std::cout << "ADDING DEVICE" << std::endl; //add device;
        // IP-t hozzaadni, message bodyban kell erkeznie
        int devicesNum2 = addedDevices.empty() ? addedDevices.size() + 1 : 0;
        if (devicesNum2 > 252) { devicesNum2 = 252; };
        int devicesNum1 = devicesNum2 <= 252 ? 0 : devicesNum1 + 1;
        if (devicesNum1 >= 252) {
          std::cerr << "Warning: no more devices can be added." << std::endl;
        } else {
          messageMap["deviceIDHigh"] = (byte)devicesNum1;
          messageMap["deviceIDLow"] = (byte)devicesNum2;
          Device* newDevice = new Device(messageMap);
          addedDevices.push_back(newDevice);
          // log 2x: device log es normal log
        }
    } else {
      std::cerr << "Invalid command: target must be the server." << std::endl;
    }
}

void Commands::removeDevice() {
    if(isServerCommand()) {
        std::cout << "REMOVING DEVICE" << std::endl; //remove device;
        // Remove device using target device ID High, Low from MessageMap NEM, mert az a szerver ID-ja!!!!
        //for (Device* d : addedDevices) {

        //  }
        //}
        // message bodyban kell erkeznie
        // Remove AddedDevices vector (save)
    } else {
      std::cerr << "Invalid command: target must be the server." << std::endl;
    }
}

void Commands::getStatusReport() {
    if(isSenderUi()) {
        std::cout << "TO DEVICES/GETTING INFOS FROM DEVICES" << std::endl; //getting reports from devices;
    } else {
      std::cerr << "Invalid command: sender must be the User Interface." << std::endl;
    }
}

void Commands::setData() {
    if(isSenderUi()) {
        std::cout << "TO DEVICES/TO SET ID" << std::endl; //devices to set id;
    } else {
      std::cerr << "Invalid command: sender must be the User Interface." << std::endl;
    }
}

void Commands::forwardMessage() {
    std::cout << "FORWARDING MESSAGE TO TARGET DEVICE" << std::endl; //forward message;
    // 1. to target device ID High, Low -> get TCPsocket from devices map in server class, forward msg.
    // 2. to several devices: group, room or floor ID: get TCPsockets from devices map in server 
    // class, to vector, forward msg to them.
}
