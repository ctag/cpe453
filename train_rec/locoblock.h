#ifndef LOCOBLOCK_H
#define LOCOBLOCK_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QByteArray>
#include <QBitArray>

#include "locobyte.h"

namespace std {
class LocoBlock;
}

class LocoBlock
{
public:
    LocoBlock();
    LocoBlock(int _bdl16x, int _ds, bool _aux, bool _occupied);
    LocoBlock(QString _hex, bool _aux, bool _occupied);
    ~LocoBlock();
    bool operator==(LocoBlock _arg);
    //void set_name(QString _name);
    void set_adr(QString _hex, bool _aux);
    void set_adr(int _bdl16x, int _ds, bool _aux);
    //void set_adr(QBitArray _adr);
    //void set_adr(int _board, int _ds, int _aux);
    void set_aux(bool _aux);
    void set_bdl16x(int _board);
    void set_ds(int _ds);
    void set_occupied(bool _occupied);
    //QString get_name();
    //QBitArray get_adr();
    int get_bdl16x();
    int get_board(); // Helper function, returns last digit of bdl16x
    int get_ds();
    bool get_aux();
    bool get_occupied();
    int get_occupied_int();

protected:

private:
    //QString name;
    int bdl16x; // The main board which hosts 16 DS
    int ds;
    bool aux;
    bool occupied;
    static bool debug;

};

#endif // LOCOBLOCK_H







