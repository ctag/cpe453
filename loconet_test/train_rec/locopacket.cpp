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
QVector<LocoByte> LocoPacket::opcodes_hex;
QVector<QString> LocoPacket::opcodes_name;
QVector<QString> LocoPacket::opcodes_desc;

/*
 * Default Constructor
 */
LocoPacket::LocoPacket()
{
    locobyte_array.clear();
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
    int _length = (_hex.count() / 2);
    locobyte_array.clear();
    for (int _packet = 0; _packet < _length; ++_packet)
    {
        LocoByte _tmp_locohex(_hex.mid((_packet * 2),2));
        locobyte_array.append(_tmp_locohex);
        qDebug() << "New packet: " << locobyte_array[_packet].get_hex() << " " << locobyte_array[_packet].get_binary();
    }
    if (is_validOP())
    {
        qDebug() << "Valid OP code x)";
    }
    if (is_validChk())
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
    int _length = _hex.count() / 2;
    locobyte_array.clear();
    for (int _packet = 0; _packet < _length; ++_packet)
    {
        LocoByte _tmp_locohex(_hex.mid((_packet * 2),2));
        locobyte_array.append(_tmp_locohex);
        qDebug() << "New packet: " << locobyte_array[_packet].get_hex() << " " << locobyte_array[_packet].get_binary();
    }
    if (is_validOP())
    {
        qDebug() << "Valid OP code x)";
    }
    if (is_validChk())
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

bool LocoPacket::operator ==(LocoPacket _arg)
{
    return(this->get_QByteArray() == _arg.get_QByteArray());
}

QString LocoPacket::get_packet()
{
    QString _result = "";
    for (int _index = 0; _index < locobyte_array.size(); ++_index)
    {
        _result.append(locobyte_array[_index].get_hex());
    }
    return (_result);
}

int LocoPacket::get_packetLen()
{
    int _result = -1;
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locobyte_array[0].get_hex() && locobyte_array[0].get_isOP()) {
            _result = (locobyte_array[0].get_packetLength());
            return(_result);
        }
    }
    if (_result < 0) {
        _result = (locobyte_array[1].get_packetLength());
    }
    return(_result);
}

int LocoPacket::get_numBytes ()
{
    return(locobyte_array.count());
}

int LocoPacket::get_staticOPsize()
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

QString LocoPacket::get_OPcode ()
{
    if (locobyte_array.count() < 1)
    {
        return("00");
    }
    LocoByte _opcode = locobyte_array[0];
    return(_opcode.get_hex());
}

LocoByte LocoPacket::get_locobyte (int _byte)
{
    if (_byte >= locobyte_array.count())
    {
        return LocoByte("00");
    }
    return locobyte_array[_byte];
}

QString LocoPacket::do_xor(LocoByte _byte1, LocoByte _byte2)
{
    if (debug) qDebug() << "doXor() ";
    LocoByte _result;
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

QString LocoPacket::do_genChecksum()
{
    if (is_validChk() && is_validOP())
    {
        qDebug() << "Not generating a checksum for an already valid packet ^-^";
        QString _chk = "";
        _chk.append(locobyte_array[locobyte_array.size()-1].get_hex());
        return(_chk);
    }
    LocoByte _checksum;
    for (int _index = 0; _index < locobyte_array.size(); ++_index)
    {
        if (debug) qDebug() << "hexIndex: " << _index;
        if (debug) qDebug() << "genChecksum: " << locobyte_array[_index].get_hex() << " : " << locobyte_array[_index].get_binary() << " and: " << _checksum.get_hex() << " : " << _checksum.get_binary();
        _checksum.set_fromHex(do_xor(_checksum, locobyte_array[_index]));
    }
    _checksum.do_genComplement();
    locobyte_array.append(_checksum);
    return(_checksum.get_hex());
}

void LocoPacket::do_addStaticOP(QString _hex, QString _name, QString _desc)
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == _hex) {
            return;
        }
    }
    opcodes_hex.append(LocoByte(_hex));
    opcodes_name.append(_name);
    opcodes_desc.append(_desc);
}

/* do_appendByte()
 *
 * Will append a new byte to the end of the packet, replacing the checksum if it exists
 */
void LocoPacket::do_appendByte(QString _byte)
{
    LocoByte _newByte(_byte);
    if (is_validChk())
    {
        locobyte_array.removeLast();
        locobyte_array.append(_newByte);
    } else {
        locobyte_array.append(_newByte);
    }
}

void LocoPacket::do_appendByteArray(QByteArray _byteArray)
{
    for (int _index = 0; _index < _byteArray.count(); ++_index)
    {
        LocoByte _newByte;
        _newByte.set_fromByteArray(_byteArray);
        if (is_validChk())
        {
            locobyte_array.removeLast();
        }
        locobyte_array.append(_newByte);
    }
}

/* get_validChk()
 *
 * Returns whether the packet has a valid OP code
 */
bool LocoPacket::is_validChk()
{
    LocoByte _hexHolder;
    for (int _index = 0; _index < locobyte_array.size(); ++_index)
    {
        if (debug) qDebug() << "hexIndex: " << _index;
        if (debug) qDebug() << "validPacket: working: " << locobyte_array[_index].get_hex() << " : " << locobyte_array[_index].get_binary() << " and: " << _hexHolder.get_hex() << " : " << _hexHolder.get_binary();
        LocoByte tmp = _hexHolder;
        _hexHolder.set_fromHex(do_xor(tmp, locobyte_array[_index]));
    }
    //qDebug() << "checksum: " << _hexHolder.get_hex();
    if (_hexHolder.get_hex() == "FF")
    {
        return(true);
    }
    return(false);
}

bool LocoPacket::is_validOP()
{
    int _len = locobyte_array.count();
    if (_len == 0)
    {
        qDebug() << "No bytes in packet, skipping OP check.";
        return true;
    }
    // Begin at byte after OP and search for incorrectly placed OPs
    for (int _index = 1; _index < locobyte_array.count(); ++_index)
    {
        if (locobyte_array[_index].get_isOP())
        {
            return(false); // There's an OP where there shouldn't be, packet is bad.
        }
    }
    // This NEEDS to be fixed, what an awful idea...
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == locobyte_array[0].get_hex()) {
            return(true); // So we're only reporting true if the OP is a KNOWN code.
        }
    }
    return(false); // First byte isn't a known op code.
}

bool LocoPacket::is_followOnMsg ()
{
    return (locobyte_array[0].get_followOnMsg());
}

QBitArray LocoPacket::get_QBitArray()
{
    int _bytes = locobyte_array.count();
    QBitArray _bitArray = QBitArray(_bytes*8);
    for (int _byteIndex = 0; _byteIndex < _bytes; ++_byteIndex)
    {
        for (int _bitIndex = 0; _bitIndex < 8; ++_bitIndex)
        {
            int _pos = (_byteIndex*8)+_bitIndex;
            _bitArray[_pos] = locobyte_array[_byteIndex].get_oneBit(_bitIndex);
        }
    }
    return _bitArray;
}

QByteArray LocoPacket::get_QByteArray()
{
    QByteArray _byteArray;
    QBitArray _bitArray = get_QBitArray();
    short unsigned int _bytes = locobyte_array.count();
    for (int _byteIndex = 0; _byteIndex < _bytes; ++_byteIndex)
    {
        for (int _bitIndex = 0; _bitIndex < 8; ++_bitIndex)
        {
            int _pos = (_byteIndex*8)+(7-_bitIndex); // This is disgusting :|
            _byteArray[_byteIndex] = (_byteArray[_byteIndex] | (_bitArray[_pos]?1:0)<<(_bitIndex));
        }
    }
    return (_byteArray);
}

/* Sometimes I wonder /
 * Whether my code should wander /
 * Those tretcherous glades /
 */










