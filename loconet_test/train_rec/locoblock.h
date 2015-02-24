#ifndef LOCOBLOCK_H
#define LOCOBLOCK_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QByteArray>
#include "locobyte.h"

namespace std {
class LocoBlock;
}

class LocoBlock
{
public:
    LocoBlock();
    LocoBlock(QByteArray _adr, bool _aux, bool _occupied);
    ~LocoBlock();
    bool operator==(LocoBlock _arg);
    void set_name(QString _name);
    void set_adr(QByteArray _adr);
    void set_aux(bool _aux);
    void set_occupied(bool _occupied);
    QString get_name();
    QByteArray get_adr();
    bool get_aux();
    bool get_occupied();

protected:

private:
    QString name;
    QByteArray adr;
    bool aux;
    bool occupied;
    static bool debug;

};

#endif // LOCOBLOCK_H







