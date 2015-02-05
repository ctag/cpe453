#ifndef LOCOHEX_H
#define LOCOHEX_H

#include <QString>

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

private:
    bool nyble_1[8];
    bool nyble_2[8];
    QString hex;
    QString binary;
    bool isOP;
    bool numArgs;

    void hexToBits();
    void binaryToBits();
    void bitsToHex();
    void bitsToBinary();
    QString getBinary();
    QString getHex();

};

#endif // LOCOHEX_H
