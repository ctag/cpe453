#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include "locohex.h"

/*
 * TODO:
 * Determine correct namespace to use
 * work out object goals/workflow
 *
 * locohex: holds one byte of hex
 * locopacket: holds representation of one loconet packet
 */

namespace std {
class LocoPacket;
}

class LocoPacket
{
public:
    LocoPacket();
    LocoPacket(QString _hex);
    ~LocoPacket();
    bool validChecksum();
    bool validOPcode();
    void genChecksum();
    QString getPacket();
    int numArgs();

protected:
    QString doXor(LocoHex _byte1, LocoHex _byte2);

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    LocoHex * locohex_array;
    short unsigned int packet_length;
    static bool debug;
    static LocoHex opcodes_hex[];
    static QString opcodes_name[];
    static int opcodes_args[];


};

#endif // LOCOPACKET_H
