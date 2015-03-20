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
QSqlDatabase LocoPacket::packetDB = QSqlDatabase::addDatabase("QSQLITE");

/*
 * Default Constructor
 */
LocoPacket::LocoPacket()
{
    locobyte_array.clear();
    if (!packetDB.isOpen())
    {
        do_openDB();
    }
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
        locobyte_array.clear();
        return;
    }
    int _length = (_hex.count() / 2);
    locobyte_array.clear();
    for (int _packet = 0; _packet < _length; ++_packet)
    {
        LocoByte _tmp_locohex(_hex.mid((_packet * 2),2));
        locobyte_array.append(_tmp_locohex);
        if (debug) qDebug() << "New packet: " << locobyte_array[_packet].get_hex() << " " << locobyte_array[_packet].get_binary();
    }
    if (hasOP())
    {
        if (debug) qDebug() << "Valid OP code x)";
    }
    if (validChk())
    {
        if (debug) qDebug() << "Valid Checksum x)";
    }
    if (!packetDB.isOpen())
    {
        do_openDB();
    }
}

void LocoPacket::clear()
{
    locobyte_array.clear();
}

bool LocoPacket::do_openDB()
{
    packetDB.setDatabaseName("packets.sqlite");
    if (!packetDB.open())
    {
        qDebug() << packetDB.lastError();
        qDebug() << "Unable to open packets sqlite database.";
        return(false);
    }
    if (debug) qDebug() << "Opened packet database.";
    return(true);
}

void LocoPacket::do_closeDB()
{
    if (debug) qDebug() << "Closed packet database.";
    packetDB.close();
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
        if (debug) qDebug() << "New packet: " << locobyte_array[_packet].get_hex() << " " << locobyte_array[_packet].get_binary();
    }
    if (hasOP())
    {
        if (debug) qDebug() << "Valid OP code x)";
    }
    if (validChk())
    {
        if (debug) qDebug() << "Valid Checksum x)";
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

int LocoPacket::get_finalSize()
{
    int _result = -1;
    if (locobyte_array.count() >= 1)
    {
        _result = (locobyte_array[0].get_packetLength());
    }
    if (_result == 0 && locobyte_array.count() >= 2)
    {
        // N byte packet, check next section for count
        _result = (locobyte_array[1].get_packetLength());
    }
    return(_result);
}

int LocoPacket::get_size ()
{
    return(locobyte_array.count());
}

QString LocoPacket::get_OPcode ()
{
    if (locobyte_array.count() < 1)
    {
        return("00");
    }
    return(locobyte_array.first().get_hex());
}

LocoByte LocoPacket::get_locobyte (int _byte)
{
    if (_byte >= locobyte_array.count())
    {
        return LocoByte("FF");
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
    if (validChk() && hasOP())
    {
        if (debug) qDebug() << "Not generating a checksum for an already valid packet ^-^";
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

/* do_appendByte()
 *
 * Will append a new byte to the end of the packet, replacing the checksum if it exists
 */
void LocoPacket::do_appendByte(QString _byte)
{
    LocoByte _newByte(_byte);
    if (validChk())
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
        locobyte_array.append(_newByte);
    }
}

void LocoPacket::do_appendLocoByte(LocoByte _byte)
{
    locobyte_array.append(_byte);
}

/* get_validChk()
 *
 * Returns whether the packet has a valid OP code
 */
bool LocoPacket::validChk()
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

bool LocoPacket::hasOP()
{
    return(locobyte_array[0].get_isOP());
}

// Check for valid OP code in sqlite
bool LocoPacket::validOP()
{
    if (!packetDB.isOpen())
    {
        bool _status = do_openDB();
        if (!_status)
        {
            return(false); // Can't determine validity
        }
    }
    QString _op = locobyte_array[0].get_hex();
    QSqlQuery _query;
    _query.prepare("SELECT * FROM opcodes WHERE opcode=:_op;");
    _query.bindValue(":_op", _op);
    if (!_query.exec())
    {
        qDebug() << _query.lastError();
        qDebug() << "Query to find valid OP codes failed.";
        return(false);
    }
    if (_query.size() > 0)
    {
        return(true);
    }
    return(false);
}

QVector<QString> LocoPacket::get_DBopcodes ()
{
    QVector<QString> _opcodes;
    if (!packetDB.isOpen())
    {
        bool _status = do_openDB();
        if (!_status)
        {
            return(_opcodes); // Can't determine validity
        }
    }
    QSqlQuery _query;
    _query.prepare("SELECT opcode FROM opcodes;");
    if (!_query.exec())
    {
        qDebug() << _query.lastError();
        qDebug() << "Query to find all OP codes failed.";
        return(_opcodes);
    }
    while (_query.next())
    {
        _opcodes.append(_query.value(0).toString());
    }
    return(_opcodes);
}

QVector<QString> LocoPacket::get_DBnames ()
{
    QVector<QString> _names;
    if (!packetDB.isOpen())
    {
        bool _status = do_openDB();
        if (!_status)
        {
            return(_names); // Can't determine validity
        }
    }
    QSqlQuery _query;
    _query.prepare("SELECT name FROM opcodes;");
    if (!_query.exec())
    {
        qDebug() << _query.lastError();
        qDebug() << "Query to find all OP names failed.";
        return(_names);
    }
    while (_query.next())
    {
        _names.append(_query.value(0).toString());
    }
    return(_names);
}

bool LocoPacket::is_followOnMsg ()
{
    return (locobyte_array[0].get_hasFollowMsg());
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
    short int _bytes = locobyte_array.count();
    for (short int _byteIndex = 0; _byteIndex < _bytes; ++_byteIndex)
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










