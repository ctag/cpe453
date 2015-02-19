#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QByteArray>
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
    int get_staticOPsize();
    QString get_staticOPname(int);
    QString get_staticOPhex(int);
    QString get_packet();
    QByteArray get_QByteArray();
    QBitArray get_QBitArray();
    int get_packetLen();
    void set_allFromHex(QString _hex);
    void do_addStaticOP(QString _hex, QString _name, QString _desc);
    QString do_genChecksum();
    void do_appendByte(QString _byte);
    void do_appendByteArray(QByteArray _byteArray);
    bool is_validChk();
    bool is_validOP();
    bool is_followOnMsg();

protected:
    QString do_xor(LocoByte _byte1, LocoByte _byte2);

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    QVector<LocoByte> locobyte_array;
    static bool debug;
    static QVector<LocoByte> opcodes_hex;
    static QVector<QString> opcodes_name;
    static QVector<QString> opcodes_desc;
};

#endif // LOCOPACKET_H
