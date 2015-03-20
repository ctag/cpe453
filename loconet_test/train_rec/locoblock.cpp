#include "locoblock.h"

bool LocoBlock::debug = false;

LocoBlock::LocoBlock ()
{
    name = "No Name";
    adr = QByteArray(3,'0');
    aux = 0;
    occupied = 0;
}

LocoBlock::LocoBlock (QByteArray _adr, bool _aux, bool _occupied)
{
    adr = _adr;
    aux = _aux;
    occupied = _occupied;
}

LocoBlock::~LocoBlock ()
{

}

bool LocoBlock::operator== (LocoBlock _arg)
{
    return((adr == _arg.get_adr()) && (aux == _arg.get_aux()));
}

void LocoBlock::set_name (QString _name)
{
    name = _name;
}

void LocoBlock::set_adr (QByteArray _adr)
{
    adr = _adr;
}

void LocoBlock::set_aux (bool _aux)
{
    aux = _aux;
}

void LocoBlock::set_occupied (bool _occupied)
{
    occupied = _occupied;
}

QString LocoBlock::get_name ()
{
    return(name);
}

QByteArray LocoBlock::get_adr ()
{
    return(adr);
}

bool LocoBlock::get_aux ()
{
    return(aux);
}

bool LocoBlock::get_occupied ()
{
    return(occupied);
}
