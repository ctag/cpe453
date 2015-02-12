#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
#include "locohex.h"

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
    int get_packetLen();
    void set_allFromHex(QString _hex);
    void do_addStaticOP(QString _hex, QString _name, QString _desc);
    QString do_genChecksum();
    void do_appendByte(QString _byte);
    bool is_validChk();
    bool is_validOP();
    bool is_followOnMsg();

protected:
    QString do_xor(LocoHex _byte1, LocoHex _byte2);

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    QVector<LocoHex> locohex_array;
    short unsigned int packet_length;
    static bool debug;
    static QVector<LocoHex> opcodes_hex;
    static QVector<QString> opcodes_name;
    static QVector<QString> opcodes_desc;
};

#endif // LOCOPACKET_H
