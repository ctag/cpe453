#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
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
    void define(QString _hex);
    bool validChecksum();
    bool validOPcode();
    void genChecksum();
    QString getPacket();
    int numArgs();
    void addOPcode(QString _hex, QString _name, QString _desc, int _numArgs);
    int numOPcode();
    QString getOPcodeName(int);
    QString getOPcodeHex(int);

protected:
    QString doXor(LocoHex _byte1, LocoHex _byte2);

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
    static QVector<int> opcodes_args;


};

#endif // LOCOPACKET_H
