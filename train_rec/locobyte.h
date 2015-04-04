#ifndef LOCOBYTE_H
#define LOCOBYTE_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <cmath>

namespace std {
class LocoByte;
}

class LocoByte
{
public:
    LocoByte();
    LocoByte(QString _hex);
    LocoByte(QBitArray _bits);
    ~LocoByte();
    bool operator==(LocoByte _arg);
    void operator=(QBitArray _arg);
    QString get_binary();
    QString get_hex();
    bool get_oneBit(int _bit);
    bool get_isOP();
    short unsigned int get_packetLength();
    bool get_hasFollowMsg();
    QBitArray get_qBitArray();
    QByteArray get_qByteArray();
    int get_decimal();
    void set_fromBinary(QString _binary);
    void set_fromHex(QString _hex);
    void set_oneBit(int _bit, bool _value);
    void set_fromByteArray(QByteArray _bytearr);
    void do_genComplement();
    void do_debugBits();

public slots:
    void setDebug(bool);

protected:
    void bitsFromHex(QString _hex, int _nyble);
    void createEmpty();

private:
    QBitArray byte;
    static bool debug; // Set in main cpp
};

#endif // LOCOBYTE_H







