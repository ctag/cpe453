#include "locopacket.h"

/* Class: LocoPacket
 *
 * For creating, manipulating, and extracting LocoNet Packets
 *
 * Christopher Bero [csb0019@uah.edu]
 * Team 4A
 */


/* Static Members */
bool LocoPacket::debug = false;
QVector<LocoHex> LocoPacket::opcodes_hex;
QVector<QString> LocoPacket::opcodes_name;
QVector<QString> LocoPacket::opcodes_desc;
QVector<int> LocoPacket::opcodes_args;

/*
 * Default Contructor
 */
LocoPacket::LocoPacket()
{
    locohex_array.clear();
}

/*
 * Overloaded Constructor
 */
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
    if (get_validOP())
    {
        qDebug() << "Valid OP code x)";
    }
    if (get_validChk())
    {
        qDebug() << "Valid Checksum x)";
    }
}

/* set_allFromHex()
 *
 * Define binary, hex, and bits from a string
 */
void LocoPacket::set_allFromHex(QString _hex)
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
    if (get_validOP())
    {
        qDebug() << "Valid OP code x)";
    }
    if (get_validChk())
    {
        qDebug() << "Valid Checksum x)";
    }
}

/*
 * Default Destructor
 */
LocoPacket::~LocoPacket()
{
    //delete [] locohex_array;
}

/* get_validChk()
 *
 * Returns whether the packet has a valid OP code
 */
bool LocoPacket::get_validChk()
{
    LocoHex _hexHolder;
    for (int _hexIndex = 0; _hexIndex < packet_length; ++_hexIndex)
    {
        if (debug) qDebug() << "hexIndex: " << _hexIndex;
        if (debug) qDebug() << "validPacket: working: " << locohex_array[_hexIndex].get_hex() << " : " << locohex_array[_hexIndex].get_binary() << " and: " << _hexHolder.get_hex() << " : " << _hexHolder.get_binary();
        LocoHex tmp = _hexHolder;
        _hexHolder.set_fromHex(do_xor(tmp, locohex_array[_hexIndex]));
    }
    if (_hexHolder.get_hex() == "FF")
    {
        return(true);
    }
    return(false);
}

bool LocoPacket::get_validOP()
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locohex_array[0].get_hex()) {
            return(true);
        }
    }
    return(false);
}

QString LocoPacket::do_xor(LocoHex _byte1, LocoHex _byte2)
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

void LocoPacket::do_genChecksum()
{
    if (get_validChk() && get_validOP())
    {
        qDebug() << "Not generating a checksum for an already valid packet :C";
        return;
    }
    LocoHex _checksum;
    for (int _index = 0; _index < packet_length; ++_index)
    {
        if (debug) qDebug() << "hexIndex: " << _index;
        if (debug) qDebug() << "genChecksum: " << locohex_array[_index].get_hex() << " : " << locohex_array[_index].get_binary() << " and: " << _checksum.get_hex() << " : " << _checksum.get_binary();
        _checksum.set_fromHex(do_xor(_checksum, locohex_array[_index]));
    }
    _checksum.do_genComplement();
    locohex_array.append(_checksum);
    packet_length++;
}

QString LocoPacket::get_packet()
{
    QString _result = "";
    for (int _index = 0; _index < packet_length; ++_index)
    {
        _result.append(locohex_array[_index].get_hex());
    }
    return (_result);
}

int LocoPacket::get_numArgs()
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locohex_array[0].get_hex()) {
            return(opcodes_args[_index]);
        }
    }
    return(0);
}

void LocoPacket::do_addStaticOP(QString _hex, QString _name, QString _desc, int _numArgs)
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

int LocoPacket::get_numStaticOP()
{
    return(opcodes_hex.size());
}

QString LocoPacket::get_staticOPname(int _index)
{
    return(opcodes_name[_index]);
}

QString LocoPacket::get_staticOPhex(int _index)
{
    return(opcodes_hex[_index].get_hex());
}

/* Sometimes I wonder /
 * Whether my code should wander /
 * Those tretcherous glades /
 */










