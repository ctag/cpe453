#ifndef LOCOHEX_H
#define LOCOHEX_H

#include <QString>
#include <QDebug>
#include <cmath>
#include <QChar>

/*
 * TODO:
 * Determine correct namespace to use
 * work out object goals/workflow
 */

namespace std {
class LocoHex;
}

class LocoHex
{
public:
    LocoHex();
    LocoHex(QString _hex);
    ~LocoHex();
    QString getBinary();
    QString getHex();
    bool isOP();
    short unsigned int getNumArgs();
    void debugBits();
    bool getBit(int _bit);
    void setBit(int _bit, bool _value);
    void genComplement();
    void defineByHex(QString _hex);
    void runDriver();

protected:
    void hexToBits(QString _hex, int index);
    void bitsToHex();
    void bitsToBinary();

private:
    //static const char hexConversion[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    //char * hexConversion;
    bool ** byte; // Divided into nybles
    QString hex;
    QString binary;
    bool isOPcode;
    short unsigned int numArgs;

};

#endif // LOCOHEX_H







