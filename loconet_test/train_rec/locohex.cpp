#include "locohex.h"

/*
 * Class: LocoHex
 * Store and manipulate one byte of hex
 * For use with LocoPacket
 * Convention: Most Significant Bit/Byte/Nyble is 0 index
 *
 * By: Christopher Bero [csb0019@uah.edu]
 */

bool LocoHex::debug = false;

/*
 * Default Contructor
 */
LocoHex::LocoHex()
{
    do_createEmpty();
}

/*
 * Loaded Contstructor
 */
LocoHex::LocoHex(QString _hex)
{
    do_createEmpty();
    set_allFromHex(_hex);
}

/*
 * Default Destructor
 */
LocoHex::~LocoHex()
{
    // Let QT take care of it, but learning how to dealloc the pointers is a good idea
}

/*
 * Initialize all member variables.
 * Do not call except on object creation.
 */
void LocoHex::do_createEmpty()
{
    if (debug) qDebug() << "Creating new empty locohex object";
    byte = new bool * [2]; // MSByte is 0
    byte[0] = new bool [4]; // MSBit is 0
    byte[1] = new bool [4]; // MSBit is 0
    for (int nyble = 0; nyble < 2; ++nyble)
    {
        for (int bit = 0; bit < 4; ++bit)
        {
            byte[nyble][bit] = 0;
        }
    }
    binary = "00000000";
    hex = "00";
    isOPcode = false;
}

/*
 * defineByHex
 * Will re-generate an object to match the input string
 */
void LocoHex::set_allFromHex (QString _hex)
{
    QString functionName = "defineByHex()";
    if (debug) qDebug() << functionName;

    hex = _hex.toUpper(); // Only deal with one case

    if (debug) qDebug() << functionName << " nybles: 0-" << hex.mid(0,1) << " 1-" << hex.mid(1,1);

    set_bitsFromHex(hex.mid(0,1), 0);
    set_bitsFromHex(hex.mid(1,1), 1);

    set_binaryFromBits();

    if (debug) qDebug() << functionName << " end";
}

/* hexToBits()
 *
 * Take an initial hex character as a QString and generate the array of bits it represents
 * Only works with one nyble (4 bits) at a time
 */
void LocoHex::set_bitsFromHex(QString _hex, int _index)
{
    if (debug) qDebug() << "hexToBits()";
    if (_hex == "1" || _hex == "3" || _hex == "5" || _hex == "7" || _hex == "9" || _hex == "B" || _hex == "D" || _hex == "F") {
        byte[_index][3] = 1;
    } else {
        byte[_index][3] = 0;
    }

    if (_hex == "2" || _hex == "3" || _hex == "6" || _hex == "7" || _hex == "A" || _hex == "B" || _hex == "E" || _hex == "F") {
        byte[_index][2] = 1;
    } else {
        byte[_index][2] = 0;
    }

    if (_hex == "4" || _hex == "5" || _hex == "6" || _hex == "7" || _hex == "C" || _hex == "D" || _hex == "E" || _hex == "F") {
        byte[_index][1] = 1;
    } else {
        byte[_index][1] = 0;
    }

    if (_hex == "8" || _hex == "9" || _hex == "A" || _hex == "B" || _hex == "C" || _hex == "D" || _hex == "E" || _hex == "F") {
        byte[_index][0] = 1;
    } else {
        byte[_index][0] = 0;
    }

    if (_index == 0)
    {
        isOPcode = byte[_index][0];
    }
    if (debug) qDebug() << "end hexToBits()";
}

/* bitsToBinary()
 *
 * Convert bits in dynamic array to a binary representation in a QString
 * Works with an entire byte (8 bits) at once
 */
void LocoHex::set_binaryFromBits()
{
    if (debug) qDebug() << "bitsToBinary()";
    binary = "";
    for (int nyble = 0; nyble < 2; ++nyble)
    {
        for (int bit = 0; bit < 4; ++bit)
        {
            if (byte[nyble][bit])
            {
                binary.append("1");
            } else {
                binary.append("0");
            }
        }
    }
    if (debug) qDebug() << "Created binary string: " << binary;
    if (debug) qDebug() << "end bitsToBinary()";
}

/* bitsToHex()
 *
 * Converts the bits member array to the hex string
 */
void LocoHex::set_hexFromBits()
{
    if (debug) qDebug() << "bitsToHex() " << get_binary();
    int _decimal[2] = {0, 0};
    QChar _hexArray[2] = {'0', '0'};

    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            int power = (3 - _bit);
            _decimal[_nyble] += pow(2, power) * byte[_nyble][_bit];
        }
        if (debug) qDebug() << "_decimal[" << _nyble << "]: " << _decimal[_nyble];
        if (_decimal[_nyble] <= 9 && _decimal[_nyble] >= 0) {
            _decimal[_nyble] += 48;
        } else {
            _decimal[_nyble] += 55;
        }
        _hexArray[_nyble] = static_cast<char>(_decimal[_nyble]);
    }
    QString tmp = QString(_hexArray);
    hex = tmp;
    if (debug) qDebug() << "bitsToHex: From: " << tmp << " to: " << hex;
    if (debug) qDebug() << "Generated hex from bits: " << hex << " integers: " << _decimal[0] << " " << _decimal[1];
    if (debug) qDebug() << "end bitsToHex()";
}

/* getBinary()
 *
 * Return the binary string
 */
QString LocoHex::get_binary()
{
    return(binary);
}

/* getHex()
 *
 * Return the hex string
 */
QString LocoHex::get_hex()
{
    if (debug) qDebug() << "getHex(): " << " : " << binary;
    return(hex);
}

/* getBit()
 *
 * Accepts values 0-7
 * Most Sig. Bit: 0
 */
bool LocoHex::get_oneBit(int _bit)
{
    int _nyble = (_bit / 4); // Which nyble are we looking at?
    _bit = (_bit - (_nyble*4)); // Which bit in the nyble?
    return(byte[_nyble][_bit]); // Fire away
}

/* setBit()
 *
 * Accepts a bit position and value, then regenerates the object to match the new binary value
 */
void LocoHex::set_oneBit(int _bit, bool _value)
{
    if (debug) qDebug() << "setBit() bit: " << _bit << " value: " << _value;
    int _nyble = (_bit / 4); // Which nyble are we looking at?
    _bit = (_bit - (_nyble*4)); // Which bit in the nyble?
    byte[_nyble][_bit] = _value; // Fire away
    set_hexFromBits();
    set_binaryFromBits();
    if (debug) qDebug() << "end setBit()";
}

/* isOP()
 *
 * Returns a boolean value if the hex is an OPcode or not
 */
bool LocoHex::get_isOP()
{
    return(isOPcode);
}

/* debugBits()
 *
 * Check the status of the bits in the raw nyble arrays
 */
void LocoHex::do_debugBits()
{
    qDebug() << "Debugging bits.";
    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        qDebug() << "Nyble: " << _nyble;
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            qDebug() << " bit: " << _bit << " value: " << byte[_nyble][_bit];
        }
    }
}

/* genComplement()
 *
 * Generate the hex's complement and then save it as the object
 */
void LocoHex::do_genComplement()
{
    if (debug) qDebug() << "genComplement byte: " << get_hex();
    for (int _nyble = 0; _nyble < 2; ++_nyble)
    {
        for (int _bit = 0; _bit < 4; ++_bit)
        {
            byte[_nyble][_bit] = !byte[_nyble][_bit]; // Flip each bit on the array
        }
    }
    set_binaryFromBits();
    set_hexFromBits();
    if (debug) qDebug() << "end genComplement()";
}

/* runDriver()
 *
 * Run a test driver. Not yet implemented.
 */
void LocoHex::do_testDriver()
{
    qDebug() << "LocoHex:runDriver is not to be used in normal operation!";
}

/* Nyble, nable
 * So much babble
 *
 * Bibble bit
 * This code's a hit
 */











