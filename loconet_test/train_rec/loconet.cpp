#include "loconet.h"

bool LocoNet::debug = false;

LocoNet::LocoNet ()
{
    incomingPacket = LocoPacket();
}

LocoNet::~LocoNet ()
{

}

bool LocoNet::get_serialOpen ()
{
    return(usbBuffer->isOpen());
}

QVector<LocoTrain> LocoNet::get_trains ()
{
    return(trains);
}

bool LocoNet::do_serialOpen (QSerialPortInfo _port)
{
    usbBuffer->setPort(_port);
    usbBuffer->setBaudRate(57600);
    usbBuffer->setFlowControl(QSerialPort::HardwareControl);
    usbBuffer->open(QIODevice::ReadWrite);
    if (usbBuffer->isOpen())
    {
        return(true);
    }
    usbBuffer->close();
    return(false);
}

void LocoNet::do_serialClose ()
{
    if (usbBuffer->isOpen())
    {
        usbBuffer->close();
    }
}

void LocoNet::do_findTrains ()
{
    //
}

void LocoNet::do_serialRead ()
{
    if (!usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial port suddenly isn't open x.x";
        do_serialClose();
        return;
    }
    QByteArray _data;
    while(usbBuffer->bytesAvailable() > 0)
    {
        if (debug) qDebug() << "Reading serial ^_^";
        _data = usbBuffer->read(1);

        if (!incomingPacket.is_validOP() && (incomingPacket.get_numBytes() > 1))
        {
            incomingPacket = LocoPacket();
            continue;
        }
         else if (incomingPacket.is_validChk()) {
            if (debug) qDebug() << "Valid checksum! Need to move this packet out of the way.";
            emit newPacket(incomingPacket);
            incomingPacket.set_allFromHex(_data.toHex());
        } else {
            incomingPacket.do_appendByteArray(_data);
        }
        if (debug) qDebug() << incomingPacket.get_packet();
    }
}

void LocoNet::do_serialWrite ()
{
    //
}
