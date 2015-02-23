#include "locotrain.h"

bool LocoTrain::debug = true;

LocoTrain::LocoTrain ()
{
    name = "";
    slot = LocoByte("00");
    adr = LocoByte("00");
    speed = LocoByte("00");
    direction = 1;
}

LocoTrain::~LocoTrain ()
{

}

void LocoTrain::set_name (QString _name)
{
    name = _name;
}

void LocoTrain::set_adr (LocoByte _adr)
{
    adr = _adr;
}

void LocoTrain::set_slot (LocoByte _slot)
{
    slot = _slot;
}

void LocoTrain::set_direction (bool _forward)
{
    direction = _forward;
}

void LocoTrain::set_speed (LocoByte _speed)
{
    speed = _speed;
}

LocoByte LocoTrain::get_adr ()
{
    return (adr);
}

LocoByte LocoTrain::get_slot ()
{
    return(slot);
}

LocoByte LocoTrain::get_speed ()
{
    return(slot);
}

bool LocoTrain::get_direction ()
{
    return(direction);
}
