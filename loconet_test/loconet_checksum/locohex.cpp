#include "locohex.h"

LocoHex::LocoHex()
{
    for (int index = 0; index < 8; ++index)
    {
        nyble_1[index] = 0;
        nyble_2[index] = 0;
    }
    isOP = false;
    numArgs = false;
}

LocoHex::LocoHex(QString _hex)
{
    _hex = _hex.toUpper();
    int _hexint = _hex.toInt(0, 2);
}

LocoHex::~LocoHex()
{

}

bool hexToBits(QString _hex)
{
    if (_hex == "0") {
        bool _bits[4] = {0,0,0,0};
        return(_bits);
    } else if (_hex == "1") {
        bool _bits[4] = {1,0,0,0};
        return(_bits);
    } else if (_hex == "2") {
        bool _bits[4] = {0,1,0,0};
        return(_bits);
    } else if (_hex == "3") {
        bool _bits[4] = {1,1,0,0};
        return(_bits);
    } else if (_hex == "4") {
        bool _bits[4] = {0,0,1,0};
        return(_bits);
    } else if (_hex == "5") {
        bool _bits[4] = {1,0,1,0};
        return(_bits);
    } else if (_hex == "6") {
        bool _bits[4] = {0,1,1,0};
        return(_bits);
    } else if (_hex == "7") {
        bool _bits[4] = {1,1,1,0};
        return(_bits);
    } else if (_hex == "8") {
        bool _bits[4] = {0,0,0,1};
        return(_bits);
    } else if (_hex == "9") {
        bool _bits[4] = {1,0,0,1};
        return(_bits);
    } else if (_hex == "A") {
        bool _bits[4] = {0,1,0,1};
        return(_bits);
    } else if (_hex == "B") {
        bool _bits[4] = {1,1,0,1};
        return(_bits);
    } else if (_hex == "C") {
        bool _bits[4] = {0,0,1,1};
        return(_bits);
    } else if (_hex == "D") {
        bool _bits[4] = {1,0,1,1};
        return(_bits);
    } else if (_hex == "E") {
        bool _bits[4] = {0,1,1,1};
        return(_bits);
    } else if (_hex == "F") {
        bool _bits[4] = {1,1,1,1};
        return(_bits);
    }
    bool _bits[4] = {0};
    return (_bits);
}

void binaryToBits()
{

}

void bitsToHex()
{

}

void bitsToBinary()
{

}

QString getBinary()
{

    return("");
}

QString getHex()
{

    return("");
}




