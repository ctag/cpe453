#ifndef LOCOHEX_H
#define LOCOHEX_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <cmath>

namespace std {
class LocoHex;
}

class LocoHex
{
public:
    LocoHex();
    LocoHex(QString _hex);
    ~LocoHex();
    QString get_binary();
    QString get_hex();
    bool get_oneBit(int _bit);
    bool get_isOP();
    short unsigned int get_packetLength();
    bool get_followOnMsg();
    void set_fromBinary(QString _binary);
    void set_fromHex(QString _hex);
    void set_oneBit(int _bit, bool _value);
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
    bool ** byte; // Divided into nyble arrays
    QString hex; // Two characters representing hex
    QString binary; // Eight 0/1 characters representing binary
    bool OPcode; // Represents whether the hex is an OPcode or not
    static bool debug; // Set in main cpp
};

#endif // LOCOHEX_H







