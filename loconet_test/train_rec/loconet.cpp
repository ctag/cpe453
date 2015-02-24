#include "loconet.h"

bool LocoNet::debug = true;

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
    usbBuffer = new QSerialPort;
    usbBuffer->setPort(_port);
    usbBuffer->setBaudRate(57600);
    usbBuffer->setFlowControl(QSerialPort::HardwareControl);
    usbBuffer->open(QIODevice::ReadWrite);
    if (usbBuffer->isOpen())
    {
        connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(do_serialRead()));
        return(true);
    }
    usbBuffer->close();
    return(false);
}

void LocoNet::do_serialClose ()
{
    if (usbBuffer->isOpen())
    {
        disconnect(usbBuffer, 0, 0, 0);
        usbBuffer->close();
        delete usbBuffer;
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

void LocoNet::do_serialWrite (LocoPacket _packet)
{
    if (!_packet.is_validChk())
    {
        qDebug() << "Packet isn't right `_`";
        return;
    }
    if (!usbBuffer->isOpen())
    {
        qDebug() << "Serial isn't open...";
        return;
    }

    usbBuffer->write(_packet.get_QByteArray());
}

void LocoNet::do_serialWrite (QString _hex)
{
    LocoPacket _packet(_hex);
    if (!_packet.is_validChk())
    {
        qDebug() << "Packet isn't right `_`";
        return;
    }
    if (!usbBuffer->isOpen())
    {
        qDebug() << "Serial isn't open...";
        return;
    }

    usbBuffer->write(_packet.get_QByteArray());
}

QString LocoNet::do_parsePacket (LocoPacket _packet)
{
    QString _opCode = (_packet.get_OPcode());

    if (!_packet.is_validOP() || !_packet.is_validChk()) {
        return("packet is malformed :c");
    }

    if (_opCode == "E7") {
        return(handle_E7(_packet));
    }
    return ("opcode doesn't match anything in parser :c");
}

QString LocoNet::handle_E7 (LocoPacket _packet)
{
    QString _description = "E7: ";
    // Parse packet into usable variables
    bool _busy = _packet.get_locobyte(3).get_oneBit(2);
    bool _active = _packet.get_locobyte(3).get_oneBit(3);
    LocoByte _slot = _packet.get_locobyte(2);
    LocoByte _adr = _packet.get_locobyte(4);
    LocoByte _speed = _packet.get_locobyte(5);
    bool _dir = _packet.get_locobyte(6).get_oneBit(2);

    if (!_busy) // There is no train in the slot
    {
        _description.append("Slot " + _slot.get_hex() + " is inactive. Trains associated with it have been deleted.");
        // Find trains assigned to the slot and delete them
        for (int _index = 0; _index < trains.count(); ++_index)
        {
            if (trains[_index].get_slot() == _slot) {
                trains.remove(_index);
            }
        }
    } else { // Past here we know a train is in the slot
        LocoTrain _newTrain;
        _newTrain.set_adr(_adr);
        _newTrain.set_direction(_dir);
        _newTrain.set_slot(_slot);
        _newTrain.set_speed(_speed);
        trains.append(_newTrain);

        for (int _index = 0; _index < trains.count(); ++_index)
        {
            if (_newTrain == trains[_index])
            {
                trains[_index] = _newTrain;
                _description.append("Train adr[" + trains[_index].get_adr().get_hex() + "] has been updated.");
            }
        }
    }

    if (_description == "E7: ") {
        _description.append("No action taken?");
    }
    return (_description);
}

