#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QByteArray>
#include <QBitArray>
#include "locobyte.h"

/* LocoPacket()
 *
 * locohex: holds one byte of hex
 * locopacket: holds representation of one loconet packet
 * Checksum = Chk
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
    bool operator==(LocoPacket _arg);
    QString get_packet();
    QByteArray get_QByteArray();
    QBitArray get_QBitArray();
    int get_packetLen();
    int get_numBytes();
    QString get_OPcode();
    LocoByte get_locobyte(int _byte);
    void set_allFromHex(QString _hex);
    QString do_genChecksum();
    void do_appendByte(QString _byte);
    void do_appendByteArray(QByteArray _byteArray);
    bool is_validChk();
    bool is_validOP();
    bool is_followOnMsg();

public slots:
    void setDebug(bool);

protected:
    QString do_xor(LocoByte _byte1, LocoByte _byte2);

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    QVector<LocoByte> locobyte_array;
    static bool debug;
};

#endif // LOCOPACKET_H
