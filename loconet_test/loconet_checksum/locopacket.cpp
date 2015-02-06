#include "locopacket.h"

/*
 * Class: LocoPacket
 * For creating, manipulating, and extracting LocoNet Packets
 *
 * By: Christopher Bero [csb0019@uah.edu]
 */

static bool debug = true;

LocoPacket::LocoPacket()
{
    QString _hex = "83"; // Global Power On
    locohex_array = new LocoHex();
    locohex_array->defineByHex(_hex);
}

LocoPacket::LocoPacket(QString _hex)
{
    _hex = _hex.remove(" ");
    _hex = _hex.remove(":");

    if ((_hex.count()%2) != 0)
    {
        if (debug) qDebug() << "Hex packet is malformed! D:";
        return;
    }
    packet_length = (_hex.count() / 2);
    locohex_array = new LocoHex[packet_length];
    for (int _packet = 0; _packet < packet_length; ++_packet)
    {
        locohex_array[_packet].defineByHex(_hex.mid((_packet * 2),2));
        qDebug() << "New packet: " << locohex_array[_packet].getHex() << " " << locohex_array[_packet].getBinary();
    }

    //qDebug() << "Test: " << doXor(locohex_array[0], locohex_array[1]).getBinary();
    qDebug() << "Test: " << validPacket();
}

LocoPacket::~LocoPacket()
{
    delete [] locohex_array;
}

bool LocoPacket::validPacket()
{
    LocoHex _hexHolder;
    for (int _hexIndex = 0; _hexIndex < packet_length; ++_hexIndex)
    {
        if (debug) qDebug() << "hexIndex: " << _hexIndex;
        if (debug) qDebug() << "validPacket: working: " << locohex_array[_hexIndex].getHex() << " : " << locohex_array[_hexIndex].getBinary() << " and: " << _hexHolder.getHex() << " : " << _hexHolder.getBinary();
        LocoHex tmp = _hexHolder;
        _hexHolder.defineByHex(doXor(tmp, locohex_array[_hexIndex]));
    }
    if (_hexHolder.getHex() == "FF")
    {
        return(true);
    }
    return(false);
}

QString LocoPacket::doXor(LocoHex _byte1, LocoHex _byte2)
{
    if (debug) qDebug() << "doXor() ";
    LocoHex _result;
    for (short unsigned int _bit = 0; _bit < 8; ++_bit)
    {
        if (debug) qDebug() << "doxor: " << _bit << " - " << _byte1.getBit(_bit) << " : " << _byte2.getBit(_bit);

        if (_byte1.getBit(_bit) == 0 && _byte2.getBit(_bit) == 1) {
            _result.setBit(_bit, 1);
        } else if (_byte1.getBit(_bit) == 1 && _byte2.getBit(_bit) == 0) {
            _result.setBit(_bit, 1);
        } else if (_byte1.getBit(_bit) == 1 && _byte2.getBit(_bit) == 1) {
            _result.setBit(_bit, 0);
        } else if (_byte1.getBit(_bit) == 0 && _byte2.getBit(_bit) == 0) {
            _result.setBit(_bit, 0);
        }
    }
    if (debug) qDebug() << "end doXor: hex: " << _result.getHex();
    return (_result.getHex());
}

void LocoPacket::genChecksum()
{

}

void LocoPacket::stringToArray()
{

}












