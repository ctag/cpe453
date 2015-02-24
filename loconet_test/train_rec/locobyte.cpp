#include "locobyte.h"

/* Class: LocoHex
 *
 * Store and manipulate one byte of hex
 * For use with LocoPacket class
 * Convention: Most Significant Bit/Byte/Nyble is 0 index
 *
 * Christopher Bero [csb0019@uah.edu]
 * Team 4A
 */

bool LocoByte::debug = false;

/*
 * Default Contructor
 */
LocoByte::LocoByte()
{
    createEmpty();
}

/*
 * Loaded Contstructor
 */
LocoByte::LocoByte(QString _hex)
{
    createEmpty();
    set_fromHex(_hex);
}

LocoByte::LocoByte(QBitArray _bits)
{
    createEmpty();
    if (_bits.count() == 8)
    {
        byte = _bits;
    }
}

/*
 * Default Destructor
 */
LocoByte::~LocoByte()
{
    // Let QT take care of it
}

bool LocoByte::operator ==(LocoByte _arg)
{
    return(byte == _arg.get_qBitArray());
}

/* createEmpty()
 *
 * Initialize all member variables.
 * Do not call except on object creation.
 */
void LocoByte::createEmpty()
{
    if (debug) qDebug() << "Creating new empty locohex object";
    byte = QBitArray(8, 0);
    for (int nyble = 0; nyble < 2; ++nyble)
    {
        for (int bit = 0; bit < 4; ++bit)
        {
            byte[(nyble*4)+bit] = 0;
        }
    }
}

/* bitsFromHex()
 *
 * Take an initial hex character as a QString and generate the array of bits it represents
 * Only works with one nyble (4 bits) at a time
 */
void LocoByte::bitsFromHex(QString _hex, int _nyble)
{
    if (debug) qDebug() << "bitsFromHex()";
    _nyble = (_nyble*4);
    if (_hex == "1" || _hex == "3" || _hex == "5" || _hex == "7" || _hex == "9" || _hex == "B" || _hex == "D" || _hex == "F") {
        byte[_nyble+3] = 1;
    } else {
        byte[_nyble+3] = 0;
    }

    if (_hex == "2" || _hex == "3" || _hex == "6" || _hex == "7" || _hex == "A" || _hex == "B" || _hex == "E" || _hex == "F") {
        byte[_nyble+2] = 1;
    } else {
        byte[_nyble+2] = 0;
    }

    if (_hex == "4" || _hex == "5" || _hex == "6" || _hex == "7" || _hex == "C" || _hex == "D" || _hex == "E" || _hex == "F") {
        byte[_nyble+1] = 1;
    } else {
        byte[_nyble+1] = 0;
    }

    if (_hex == "8" || _hex == "9" || _hex == "A" || _hex == "B" || _hex == "C" || _hex == "D" || _hex == "E" || _hex == "F") {
        byte[_nyble+0] = 1;
    } else {
        byte[_nyble+0] = 0;
    }
    if (debug) qDebug() << "end hexToBits()";
}

/* set_fromHex()
 *
 * Will re-generate an object to match the input string
 */
void LocoByte::set_fromHex (QString _hex)
{
    QString functionName = "defineByHex()";
    if (debug) qDebug() << functionName;

    _hex = _hex.toUpper(); // Only deal with one case

    if (debug) qDebug() << functionName << " nybles: 0-" << _hex.mid(0,1) << " 1-" << _hex.mid(1,1);

    bitsFromHex(_hex.mid(0,1), 0);
    bitsFromHex(_hex.mid(1,1), 1);

    if (debug) qDebug() << functionName << " end";
} /* end set_fromHex */

/* get_binary()
 *
 * Return the binary string
 */
QString LocoByte::get_binary()
{
    QString _binary = "";
    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            if (byte[(_nyble*4)+_bit] == 1)
            {
                _binary.append("1");
            } else {
                _binary.append("0");
            }
        }
    }
    return(_binary);
} /* end get_binary() */

/* get_hex()
 *
 * Return the hex string
 */
QString LocoByte::get_hex()
{
    if (debug) qDebug() << "hexFromBits() " << get_binary();
    int _decimal[2] = {0, 0};
    QChar _hexArray[2] = {'0', '0'};

    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            int power = (3 - _bit);
            _decimal[_nyble] += pow(2, power) * byte[(_nyble*4)+_bit];
        }
        if (debug) qDebug() << "_decimal[" << _nyble << "]: " << _decimal[_nyble];
        if (_decimal[_nyble] <= 9 && _decimal[_nyble] >= 0) {
            _decimal[_nyble] += 48;
        } else {
            _decimal[_nyble] += 55;
        }
        _hexArray[_nyble] = static_cast<char>(_decimal[_nyble]);
    }
    QString _hex = QString(_hexArray, 2);
    if (debug) qDebug() << "hexFromBits:" << _hex;
    if (debug) qDebug() << "Generated hex from bits: " << _hex << " integers: " << _decimal[0] << " " << _decimal[1];
    if (debug) qDebug() << "end hexFromBits()";
    return (_hex);
} /* end get_hex() */

/* get_oneBit()
 *
 * Accepts values 0-7
 * Most Sig. Bit: 0
 */
bool LocoByte::get_oneBit(int _bit)
{
    return(byte[_bit]); // Fire away
} /* end get_oneBit */

/* set_oneBit()
 *
 * Accepts a bit position and value, then regenerates the object to match the new binary value
 */
void LocoByte::set_oneBit(int _bit, bool _value)
{
    if (debug) qDebug() << "setBit() bit: " << _bit << " value: " << _value;
    byte[_bit] = _value;
    if (debug) qDebug() << "end setBit()";
} /* end set_oneBit */

/* get_isOP()
 *
 * Returns a boolean value if the hex is an OPcode or not
 */
bool LocoByte::get_isOP()
{
    return(byte[0]);
} /* end get_isOP */

short unsigned int LocoByte::get_packetLength()
{
    if (!get_isOP()) { // Assume we want the second hex 7-bit packet length
        int _len = 0;
        for (int _nyble = 0; _nyble < 2; ++_nyble)
        {
            for (int _bit = 0; _bit < 4; ++_bit)
            {
                int _pos = ((_nyble*4)+_bit);
                _len += (byte[_pos] * pow(2, (7 - _pos)));
            }
        }
        return(_len);
    }

    bool _bit1 = byte[1];
    bool _bit2 = byte[2];

    if (!_bit1 && !_bit2) {
        return(2);
    } else if (!_bit1 && _bit2) {
        return(4);
    } else if (_bit1 && !_bit2) {
        return(6);
    } else if (_bit1 && _bit2) {
        return(0);
    }
    return(-1);
}

bool LocoByte::get_followOnMsg()
{
    if (get_isOP() && byte[4])
    {
        return (true);
    }
    return(false);
}

/* do_debugBits()
 *
 * Check the status of the bits in the raw nyble arrays
 */
void LocoByte::do_debugBits()
{
    qDebug() << "Debugging bits.";
    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        qDebug() << "Nyble: " << _nyble;
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            qDebug() << " bit: " << _bit << " value: " << byte[(_nyble*4)+_bit];
        }
    }
}

/* do_genComplement()
 *
 * Generate the hex's complement and then save it as the object
 */
void LocoByte::do_genComplement()
{
    if (debug) qDebug() << "genComplement byte: " << get_hex();
    byte = ~byte;
    if (debug) qDebug() << "end genComplement()";
}

/* do_testDriver()
 *
 * Run a test driver. Not yet implemented.
 */
void LocoByte::do_testDriver()
{
    qDebug() << "LocoHex:runDriver is not to be used in normal operation!";
}

/* set_fromBinary()
 *
 */
void LocoByte::set_fromBinary(QString _binary)
{
    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            int _pos = ((_nyble*4)+_bit);
            QString _test = _binary.mid(_pos, 1);
            if (_test == "1")
            {
                byte[_pos] = 1;
            } else {
                byte[_pos] = 0;
            }
        }
    }
}

// http://qt-project.org/wiki/WorkingWithRawData
void LocoByte::set_fromByteArray(QByteArray _bytearr)
{
    byte = QBitArray(_bytearr.count()*8);
    for (int _byte = 0; _byte < _bytearr.count(); ++_byte)
    {
        for (int _bit = 0; _bit < 8; ++_bit)
        {
            short unsigned int _pos = (_byte*8)+_bit;
            bool _val = _bytearr.at(_byte)&(1<<(7-_bit));
            byte.setBit(_pos, _val);
        }
    }
}

QBitArray LocoByte::get_qBitArray()
{
    return(byte);
}

/* Nyble, nable
 * So much babble
 *
 * Bibble bit
 * This code's a hit
 */











