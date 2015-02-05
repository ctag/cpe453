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

};

#endif // LOCOPACKET_H
