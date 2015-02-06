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
    bool validPacket();
    void genChecksum();
    void stringToArray();
    QString doXor(LocoHex _byte1, LocoHex _byte2);

protected:

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    LocoHex * locohex_array;
    int numArgs;
    short unsigned int packet_length;


};

#endif // LOCOPACKET_H
