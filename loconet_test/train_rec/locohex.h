#ifndef LOCOHEX_H
#define LOCOHEX_H

#include <QString>
#include <QDebug>
#include <cmath>
#include <QChar>

/*
 * TODO:
 * Determine correct namespace to use
 * work out object goals/workflow
 */

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
    bool get_isOP();
    bool get_oneBit(int _bit);
    void set_allFromHex(QString _hex);
    void set_oneBit(int _bit, bool _value);
    void do_genComplement();
    void do_testDriver();
    void do_debugBits();

protected:
    void set_bitsFromHex(QString _hex, int index);
    void set_hexFromBits();
    void set_binaryFromBits();
    void do_createEmpty();

private:
    bool ** byte; // Divided into nyble arrays
    QString hex; // Two characters representing hex
    QString binary; // Eight 0/1 characters representing binary
    bool OPcode; // Represents whether the hex is an OPcode or not
    static bool debug; // Set in main cpp

};

#endif // LOCOHEX_H







