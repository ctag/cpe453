#include "locotrain.h"

bool LocoTrain::debug = false;

LocoTrain::LocoTrain ()
{
    name = "";
    slot = LocoByte("00");
    adr = LocoByte("00");
    speed = LocoByte("00");
    reverse = 1;
}

LocoTrain::~LocoTrain ()
{

}

QString LocoTrain::timeStamp()
{
    return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

bool LocoTrain::operator ==(LocoTrain _arg)
{
    return((this->get_adr() == _arg.get_adr()) && (this->get_slot() == _arg.get_slot()));
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

void LocoTrain::set_reverse (bool _forward)
{
    reverse = _forward;
}

void LocoTrain::set_speed (LocoByte _speed)
{
    speed = _speed;
}

void LocoTrain::set_state(QString _state)
{
    state = _state;
}

QString LocoTrain::get_state()
{
    return(state);
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
    return(speed);
}

bool LocoTrain::get_direction ()
{
    return(reverse);
}

QString LocoTrain::get_descrtiption ()
{
    QString _desc = "Train[" + adr.get_hex() + "]";
    _desc.append(" Slot[" + slot.get_hex() + "]");
    _desc.append(" Speed[" + speed.get_hex() + "]");
    _desc.append(" Reverse[" + QString::number(reverse) + "]");
    return(_desc);
}
