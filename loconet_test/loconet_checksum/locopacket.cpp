#include "locopacket.h"

/*
 * Class: LocoPacket
 * For creating, manipulating, and extracting LocoNet Packets
 *
 * By: Christopher Bero [csb0019@uah.edu]
 */

bool LocoPacket::debug = false;

LocoHex LocoPacket::opcodes_hex[16] = {LocoHex("85"), LocoHex("83"), LocoHex("82"), LocoHex("BF"), LocoHex("BD"), LocoHex("BC"), LocoHex("BB"), LocoHex("BA"), LocoHex("B9"), LocoHex("B8"), LocoHex("B6"), LocoHex("B5"), LocoHex("B4"), LocoHex("B2"), LocoHex("B1"), LocoHex("B0")};
QString LocoPacket::opcodes_name[16] = {"Global IDLE", "Global ON", "Global OFF", "OPC_LOCO_ADR", "OPC_SW_ACK", "OPC_SW_STATE", "OPC_RQ_SL_DATA", "OPC_MOVE_SLOTS", "OPC_LINK_SLOTS", "OPC_UNLINK_SLOTS", "OPC_CONSIST_FUNCT", "OPC_SLOT_STAT1", "OPC_LONG_ACK", "OPC_INPUT_REP", "OPC_SQ_REP", "OPC_SW_REQ"};
int LocoPacket::opcodes_args[16] = {0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

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

bool LocoPacket::validOPcode()
{
    if (debug) qDebug() << "validOPcode()";
    short unsigned int _arr_size = (sizeof(opcodes_args)/sizeof(*opcodes_args));
    QString _hex = locohex_array[0].getHex();

    for (int _index = 0; _index < _arr_size; ++_index)
    {
        if (_hex == opcodes_hex[_index].getHex())
        {
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
    if (validChecksum() && validOPcode())
    {
        qDebug() << "Not generating a checksum for an already valid packet :C";
        return;
    }
    LocoHex _checksum;
    for (int _index = 0; _index < packet_length; ++_index)
    {
        if (debug) qDebug() << "hexIndex: " << _index;
        if (debug) qDebug() << "genChecksum: " << locohex_array[_index].getHex() << " : " << locohex_array[_index].getBinary() << " and: " << _checksum.getHex() << " : " << _checksum.getBinary();
        _checksum.defineByHex(doXor(_checksum, locohex_array[_index]));
    }
    _checksum.genComplement();
    LocoHex * tmp_array;
    tmp_array = new LocoHex[(packet_length + 1)];
    for (int _index = 0; _index < packet_length; ++_index)
    {
        tmp_array[_index] = locohex_array[_index];
    }
    tmp_array[packet_length].defineByHex(_checksum.getHex());
    delete [] locohex_array;
    locohex_array = tmp_array;
    packet_length++;
}

QString LocoPacket::getPacket()
{
    QString _result = "";
    for (int _index = 0; _index < packet_length; ++_index)
    {
        _result.append(locohex_array[_index].getHex());
    }
    return (_result);
}

int LocoPacket::numArgs()
{
    short unsigned int _arr_size = (sizeof(opcodes_args)/sizeof(*opcodes_args));
    for (int _index = 0; _index < _arr_size; ++_index)
    {
        if (locohex_array[0].getHex() == opcodes_hex[_index].getHex())
        {
            return(opcodes_args[_index]);
        }
    }
}













