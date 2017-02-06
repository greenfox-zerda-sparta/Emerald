#include "Lamp.h"

Lamp::Lamp()
{

}

Lamp::~Lamp()
{

}

Lamp::Lamp(unsigned char deviceIDHigh, unsigned char devideIDLow)
{

}

void Lamp::set_groupID()
{
  groupID = deviceCommandMap.at("GroupID");
}


unsigned char Lamp::get_groupID()
{
  return groupID;
}


