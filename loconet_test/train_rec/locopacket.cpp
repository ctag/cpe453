#include "locopacket.h"

/*
 * Class: LocoPacket
 * For creating, manipulating, and extracting LocoNet Packets
 *
 * By: Christopher Bero [csb0019@uah.edu]
 */

/* Static Members */
bool LocoPacket::debug = false;
QVector<LocoHex> LocoPacket::opcodes_hex;
QVector<QString> LocoPacket::opcodes_name;
QVector<QString> LocoPacket::opcodes_desc;
QVector<int> LocoPacket::opcodes_args;

LocoPacket::LocoPacket()
{
    locohex_array.clear();
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
    locohex_array.clear();
    for (int _packet = 0; _packet < packet_length; ++_packet)
    {
        LocoHex _tmp_locohex(_hex.mid((_packet * 2),2));
        locohex_array.append(_tmp_locohex);
        qDebug() << "New packet: " << locohex_array[_packet].get_hex() << " " << locohex_array[_packet].get_binary();
    }
    if (validOPcode())
    {
        qDebug() << "Valid OP code x)";
    }
    if (validChecksum())
    {
        qDebug() << "Valid Checksum x)";
    }
}

void LocoPacket::define(QString _hex)
{
    _hex = _hex.remove(" ");
    _hex = _hex.remove(":");

    if ((_hex.count()%2) != 0)
    {
        if (debug) qDebug() << "Hex packet is malformed! D:";
        return;
    }
    packet_length = (_hex.count() / 2);
    locohex_array.clear();
    for (int _packet = 0; _packet < packet_length; ++_packet)
    {
        LocoHex _tmp_locohex(_hex.mid((_packet * 2),2));
        locohex_array.append(_tmp_locohex);
        qDebug() << "New packet: " << locohex_array[_packet].get_hex() << " " << locohex_array[_packet].get_binary();
    }
    if (validOPcode())
    {
        qDebug() << "Valid OP code x)";
    }
    if (validChecksum())
    {
        qDebug() << "Valid Checksum x)";
    }
}

LocoPacket::~LocoPacket()
{
    //delete [] locohex_array;
}

bool LocoPacket::validChecksum()
{
    LocoHex _hexHolder;
    for (int _hexIndex = 0; _hexIndex < packet_length; ++_hexIndex)
    {
        if (debug) qDebug() << "hexIndex: " << _hexIndex;
        if (debug) qDebug() << "validPacket: working: " << locohex_array[_hexIndex].get_hex() << " : " << locohex_array[_hexIndex].get_binary() << " and: " << _hexHolder.get_hex() << " : " << _hexHolder.get_binary();
        LocoHex tmp = _hexHolder;
        _hexHolder.set_allFromHex(doXor(tmp, locohex_array[_hexIndex]));
    }
    if (_hexHolder.get_hex() == "FF")
    {
        return(true);
    }
    return(false);
}

bool LocoPacket::validOPcode()
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locohex_array[0].get_hex()) {
            return(true);
        }
    }
    return(false);
}

QString LocoPacket::doXor(LocoHex _byte1, LocoHex _byte2)
{
    if (debug) qDebug() << "doXor() ";
    LocoHex _result;
    for (short unsigned int _bit = 0; _bit < 8; ++_bit)
    {
        if (debug) qDebug() << "doxor: " << _bit << " - " << _byte1.get_oneBit(_bit) << " : " << _byte2.get_oneBit(_bit);

        if (_byte1.get_oneBit(_bit) == 0 && _byte2.get_oneBit(_bit) == 1) {
            _result.set_oneBit(_bit, 1);
        } else if (_byte1.get_oneBit(_bit) == 1 && _byte2.get_oneBit(_bit) == 0) {
            _result.set_oneBit(_bit, 1);
        } else if (_byte1.get_oneBit(_bit) == 1 && _byte2.get_oneBit(_bit) == 1) {
            _result.set_oneBit(_bit, 0);
        } else if (_byte1.get_oneBit(_bit) == 0 && _byte2.get_oneBit(_bit) == 0) {
            _result.set_oneBit(_bit, 0);
        }
    }
    if (debug) qDebug() << "end doXor: hex: " << _result.get_hex();
    return (_result.get_hex());
}

void LocoPacket::genChecksum()
{
    if (validChecksum() && validOPcode())
    {
        qDebug() << "Not generating a checksum for an already valid packet :C";
        return;
    }
    LocoHex _checksum;
    for (int _index = 0; _index < packet_length; ++_index)
    {
        if (debug) qDebug() << "hexIndex: " << _index;
        if (debug) qDebug() << "genChecksum: " << locohex_array[_index].get_hex() << " : " << locohex_array[_index].get_binary() << " and: " << _checksum.get_hex() << " : " << _checksum.get_binary();
        _checksum.set_allFromHex(doXor(_checksum, locohex_array[_index]));
    }
    _checksum.do_genComplement();
    locohex_array.append(_checksum);
    packet_length++;
}

QString LocoPacket::getPacket()
{
    QString _result = "";
    for (int _index = 0; _index < packet_length; ++_index)
    {
        _result.append(locohex_array[_index].get_hex());
    }
    return (_result);
}

int LocoPacket::numArgs()
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locohex_array[0].get_hex()) {
            return(opcodes_args[_index]);
        }
    }
    return(0);
}

void LocoPacket::addOPcode(QString _hex, QString _name, QString _desc, int _numArgs)
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == _hex) {
            return;
        }
    }
    opcodes_hex.append(LocoHex(_hex));
    opcodes_name.append(_name);
    opcodes_desc.append(_desc);
    opcodes_args.append(_numArgs);
}

int LocoPacket::numOPcode()
{
    return(opcodes_hex.size());
}

QString LocoPacket::getOPcodeName(int _index)
{
    return(opcodes_name[_index]);
}

QString LocoPacket::getOPcodeHex(int _index)
{
    return(opcodes_hex[_index].get_hex());
}












