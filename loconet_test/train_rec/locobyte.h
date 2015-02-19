#ifndef LOCOBYTE_H
#define LOCOBYTE_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <cmath>

namespace std {
class LocoByte;
}

class LocoByte
{
public:
    LocoByte();
    LocoByte(QString _hex);
    ~LocoByte();
    QString get_binary();
    QString get_hex();
    bool get_oneBit(int _bit);
    bool get_isOP();
    short unsigned int get_packetLength();
    bool get_followOnMsg();
    char get_nybleAsChar(int _nyble);
    QBitArray get_qBitArray();
    void set_fromBinary(QString _binary);
    void set_fromHex(QString _hex);
    void set_oneBit(int _bit, bool _value);
    void set_fromByteArray(QByteArray _bytearr);
    void do_genComplement();
    void do_testDriver();
    void do_debugBits();

protected:
    void bitsFromHex(QString _hex, int _nyble);
    void hexFromBits();
    void binaryFromBits();
    void bitsFromBinary();
    void createEmpty();

private:
    //bool ** byte; // Divided into nyble arrays
    QBitArray byte;
    QChar byteCharArray[2];
    QString hex; // Two characters representing hex
    QString binary; // Eight 0/1 characters representing binary
    bool OPcode; // Represents whether the hex is an OPcode or not
    static bool debug; // Set in main cpp
};

#endif // LOCOBYTE_H







