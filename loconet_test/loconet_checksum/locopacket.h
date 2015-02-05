#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>

/*
 * TODO:
 * Determine correct namespace to use
 * work out object goals/workflow
 *
 * locohex: holds one byte of hex
 * locopacket: holds representation of one loconet packet
 *
 * locousb: ?
 * locowireless: ?
 * locoSQL: ?
 */

namespace std {
class LocoPacket;
}

class LocoPacket
{
public:
    LocoPacket();
    ~LocoPacket();

private:
    bool H1[8];
    bool H2[8];
    QString hex;
    void genBinary();
};

#endif // LOCOPACKET_H
