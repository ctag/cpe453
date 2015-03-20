#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QByteArray>
#include <QBitArray>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "locobyte.h"
//#include "locotrain.h"
//#include "locoblock.h"

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
    int get_finalSize();
    int get_size();
    QString get_OPcode();
    LocoByte get_locobyte(int _byte);
    QString get_description(); // Pull packet description from sqlite
    QString get_name(); // Pull packet name from sqlite
    QVector<QString> get_DBopcodes();
    QVector<QString> get_DBnames();
    void set_allFromHex(QString _hex);
    QString do_genChecksum();
    void do_appendByte(QString _byte);
    void do_appendByteArray(QByteArray _byteArray);
    void do_appendLocoByte(LocoByte _byte);
    bool validChk();
    bool hasOP(); // Returns that first byte is OP formatted
    bool validOP(); // Returns that first byte is OP and in our database
    bool is_followOnMsg();
    void clear();
    static bool debug;

public slots:

signals:

protected:
    QString do_xor(LocoByte _byte1, LocoByte _byte2);
    bool do_openDB();
    void do_closeDB();

private:
    QString packet_text_hex;
    QString packet_text_binary;
    QString packet_text_desc;
    QVector<LocoByte> locobyte_array;
    static QSqlDatabase packetDB;

};

#endif // LOCOPACKET_H
