#include "locoblock.h"

bool LocoBlock::debug = false;

LocoBlock::LocoBlock ()
{
    //name = "No Name";
    //adr = QBitArray(11); //QByteArray(3,'0');
    bdl16x = 0;
    ds = 0;
    aux = false;
    occupied = 0;
}

LocoBlock::LocoBlock (int _bdl16x, int _ds, bool _aux, bool _occupied)
{
    //adr = _adr;
    aux = _aux;
    bdl16x = _bdl16x;
    ds = _ds;
    occupied = _occupied;
}

LocoBlock::LocoBlock (QString _hex, bool _aux, bool _occupied)
{
    set_adr(_hex, _aux);
    occupied = _occupied;
}

LocoBlock::~LocoBlock ()
{
    //
}

QString LocoBlock::timeStamp()
{
    return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

bool LocoBlock::operator== (LocoBlock _arg)
{
    return((get_board()== _arg.get_board()) && (ds == _arg.get_ds()) && (aux == _arg.get_aux()));
}
/*
void LocoBlock::set_name (QString _name)
{
    name = _name;
}
*/
/*
void LocoBlock::set_adr (QByteArray _adr)
{
    adr = _adr;
}
*/

void LocoBlock::set_adr(QString _hex, bool _aux)
{
    aux = _aux;
    int _decimalAdr;
    if (aux)
    {
        _decimalAdr = (_hex.toInt(0, 16)+1)*2;
    } else {
        _decimalAdr = (_hex.toInt(0, 16)*2)+1;
    }
    ds = (_decimalAdr%16);
    if (ds == 0)
    {
        ds = 16;
    }
    bdl16x = ceil(_decimalAdr/16.0);
    if (debug) qDebug() << timeStamp() << aux << ds << bdl16x;
}

void LocoBlock::set_adr(int _bdl16x, int _ds, bool _aux)
{
    aux = _aux;
    ds = _ds;
    bdl16x = _bdl16x;
}

void LocoBlock::set_ds(int _ds)
{
    ds = _ds;
}

void LocoBlock::set_bdl16x(int _bdl16x)
{
    bdl16x = _bdl16x;
}

/*
void LocoBlock::set_adr (QBitArray _adr)
{
    adr = _adr;
}
*/

void LocoBlock::set_aux (bool _aux)
{
    aux = _aux;
}

void LocoBlock::set_occupied (bool _occupied)
{
    occupied = _occupied;
}

/*
QString LocoBlock::get_name ()
{
    return(name);
}
*/
/*
QBitArray LocoBlock::get_adr ()
{
    return(adr);
}
*/

int LocoBlock::get_bdl16x()
{
    return(bdl16x);
}

int LocoBlock::get_board()
{
    if (debug) qDebug() << timeStamp() << "Get board: " << bdl16x%10;
    return(bdl16x%10);
}

int LocoBlock::get_ds()
{
    return(ds);
}

bool LocoBlock::get_aux ()
{
    return(aux);
}

bool LocoBlock::get_occupied ()
{
    return(occupied);
}

int LocoBlock::get_occupied_int ()
{
    return(occupied?1:0);
}
