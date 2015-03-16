#include "loconet.h"

bool LocoNet::debug = false;
QVector<LocoByte> LocoNet::opcodes_hex;
QVector<QString> LocoNet::opcodes_name;
QVector<QString> LocoNet::opcodes_desc;

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

QVector<LocoBlock> LocoNet::get_blocks ()
{
    return(blocks);
}

/*
 * Timer related functions
 */

bool LocoNet::get_timerActive()
{
    return(packetTimer->isActive());
}

QVector<LocoPacket> LocoNet::get_timerPackets()
{
    return(packetTimerPackets);
}

int LocoNet::get_timerPacketInterval(LocoPacket _packet)
{
    for (int _index = 0; _index < packetTimerPackets.count(); ++_index)
    {
        if (packetTimerPackets[_index] == _packet) {
            return(packetTimerPacketInterval[_index]);
        }
    }
    return(-1);
}

void LocoNet::set_packetTimer (int _msec) {
    packetTimer = new QTimer(this);
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(handle_packetTimer()));
    packetTimer->start(_msec);
}

void LocoNet::do_addTimerPacket (LocoPacket _packet, int _interval)
{
    do_stopTimerPacket(_packet);
    packetTimerPackets.append(_packet);
    packetTimerPacketInterval.append(_interval);
    packetTimerPacketState.append(0);
}

void LocoNet::do_stopTimerPacket (LocoPacket _packet)
{
    for (int _index = 0; _index < packetTimerPackets.count(); ++_index)
    {
        if (packetTimerPackets[_index] == _packet)
        {
            packetTimerPackets.remove(_index);
            packetTimerPacketInterval.remove(_index);
            packetTimerPacketState.remove(_index);
        }
    }
}

void LocoNet::do_stopPacketTimer ()
{
    packetTimer->stop();
    disconnect(packetTimer, 0, 0, 0);
    delete packetTimer;
    packetTimerPackets.clear();
    packetTimerPacketInterval.clear();
    packetTimerPacketState.clear();
}

void LocoNet::handle_packetTimer ()
{
    for (int _index = 0; _index < packetTimerPackets.count(); ++_index)
    {
        ++packetTimerPacketState[_index];
        if (packetTimerPacketState[_index] >= packetTimerPacketInterval[_index])
        {
            packetTimerPacketState[_index] = 0;
            do_serialWrite(packetTimerPackets[_index]);
        }
    }
}

/*
 * Serial Port related functions
 */

bool LocoNet::do_serialOpen (QSerialPortInfo _port)
{
    usbBuffer = new QSerialPort;
    usbBuffer->setPort(_port);
    usbBuffer->setBaudRate(57600);
    usbBuffer->setFlowControl(QSerialPort::HardwareControl);
    usbBuffer->setReadBufferSize(4); // To force readyread() signal earlier...
    usbBuffer->open(QIODevice::ReadWrite);
    if (usbBuffer->isOpen())
    {
        //locorecv = new LocoRecv(usbBuffer);
        //locorecv->moveToThread(&usbThread);
        //connect(locorecv, SIGNAL(receivedPacket(LocoPacket)), this, SLOT(handle_serialRead(LocoPacket)));
        //usbThread.start();
        connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(handle_serialRead()));
        return(true);
    }
    usbBuffer->close();
    return(false);
}

void LocoNet::do_serialClose ()
{
    //usbThread.quit();
    //usbThread.wait();
    if (usbBuffer->isOpen())
    {
        disconnect(usbBuffer, 0, 0, 0);
        usbBuffer->close();
        delete usbBuffer;
    }
}

void LocoNet::handle_serialRead (LocoPacket _packet)
{
    emit newPacket(_packet);
    emit newPacketDescription(handle_parsePacket(_packet));
}

void LocoNet::handle_serialRead ()
{
    // If the usb device isn't open, we're not reading anything...
    if (!usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial port suddenly isn't open x.x";
        do_serialClose();
        return;
    }

    // Read immediately if data is available
    while(usbBuffer->bytesAvailable() > 0 || usbBuffer->waitForReadyRead(1))
    {
        if (debug) qDebug() << "Reading serial ^_^";
        incomingPacket.do_appendByteArray(usbBuffer->read(1)); // Load in a byte from the serial buffer
        // Check to see if the packet has a valid OP code.
        if (!incomingPacket.is_validOP()) {
            incomingPacket = LocoPacket(); // Reset the packet, since the OP isn't valid.
            continue; // Read in next byte from buffer. I'm sure this section could use some cleaner code.
        }
        // No point having an else-if when we break the while() above.
        if (incomingPacket.is_validChk()) {
            if (debug) qDebug() << "Valid checksum! Need to move this packet out of the way.";
            emit newPacket(incomingPacket);
            emit newPacketDescription(handle_parsePacket(incomingPacket));
            incomingPacket = LocoPacket(); // Reset the packet.
        }
        if (debug) qDebug() << incomingPacket.get_packet();
    }
}

void LocoNet::do_serialWrite (LocoPacket _packet)
{
    _packet.do_genChecksum();
    if (!usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial isn't open...";
        return;
    }
    usbBuffer->write(_packet.get_QByteArray());
}

void LocoNet::do_serialWrite (QString _hex)
{
    // That was easy enough. Just drop the hex into a new packet and call the 'real' serialWrite().
    do_serialWrite(LocoPacket(_hex));
}

/*
 * Packet related functions
 */

QString LocoNet::handle_parsePacket (LocoPacket _packet, bool _enable)
{
    QString _opCode = (_packet.get_OPcode());

    if (!_packet.is_validOP() || !_packet.is_validChk()) {
        return("packet is malformed >.<");
    }

    // This looks so gross >.< ugh
    if (_opCode == "E7") {
        return(parse_E7(_packet, _enable));
    } else if (_opCode == "B2") {
        return(parse_B2(_packet, _enable));
    } else if (_opCode == "85") {
        return(parse_85(_packet, _enable));
    } else if (_opCode == "83") {
        return(parse_83(_packet, _enable));
    } else if (_opCode == "82") {
        return(parse_82(_packet, _enable));
    } else if (_opCode == "81") {
        return(parse_81(_packet, _enable));
    } else if (_opCode == "BF") {
        return(parse_BF(_packet, _enable));
    } else if (_opCode == "BD") {
        return(parse_BD(_packet, _enable));
    } else if (_opCode == "BC") {
        return(parse_BC(_packet, _enable));
    } else if (_opCode == "BB") {
        return(parse_BB(_packet, _enable));
    } else if (_opCode == "BA") {
        return(parse_BA(_packet, _enable));
    } else if (_opCode == "B9") {
        return(parse_B9(_packet, _enable));
    } else if (_opCode == "B8") {
        return(parse_B8(_packet, _enable));
    } else if (_opCode == "B6") {
        return(parse_B6(_packet, _enable));
    } else if (_opCode == "B5") {
        return(parse_B5(_packet, _enable));
    } else if (_opCode == "B4") {
        return(parse_B4(_packet, _enable));
    } else if (_opCode == "B1") {
        return(parse_B1(_packet, _enable));
    } else if (_opCode == "B0") {
        return(parse_B0(_packet, _enable));
    } else if (_opCode == "A2") {
        return(parse_A2(_packet, _enable));
    } else if (_opCode == "A1") {
        return(parse_A1(_packet, _enable));
    } else if (_opCode == "A0") {
        return(parse_A0(_packet, _enable));
    } else if (_opCode == "EF") {
        return(parse_EF(_packet, _enable));
    } else if (_opCode == "E5") {
        return(parse_E5(_packet, _enable));
    } else if (_opCode == "ED") {
        return(parse_ED(_packet, _enable));
    }

    return ("opcode [" + _opCode + "] doesn't match anything in parser :c");
}

QString LocoNet::parse_E7 (LocoPacket _packet, bool _enable)
{
    QString _description = "E7:";
    // Parse packet into usable variables
    bool _busy = _packet.get_locobyte(3).get_oneBit(2);
    //bool _active = _packet.get_locobyte(3).get_oneBit(3);
    LocoByte _slot = _packet.get_locobyte(2);
    LocoByte _adr = _packet.get_locobyte(4);
    LocoByte _speed = _packet.get_locobyte(5);
    bool _dir = _packet.get_locobyte(6).get_oneBit(2);

    if (!_busy) // There is no train in the slot
    {
        _description.append(" Slot " + _slot.get_hex() + " is inactive. Trains associated with it have been deleted.");
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
        _newTrain.set_reverse(_dir);
        _newTrain.set_slot(_slot);
        _newTrain.set_speed(_speed);

        if (_enable)
        {
            bool _existingTrain = false;
            for (int _index = 0; _index < trains.count(); ++_index)
            {
                if (_newTrain == trains[_index])
                {
                    _existingTrain = true;
                    trains[_index] = _newTrain;
                    emit trainUpdated(trains[_index]);
                    _description.append(" Train["+QString::number(_index)+"] adr[" + trains[_index].get_adr().get_hex() + "]");
                    _description.append(" has been updated.");
                }
            }
            if (!_existingTrain)
            {
                trains.append(_newTrain);
                emit trainUpdated(trains.last());
                _description.append(" Train["+QString::number(trains.count()-1)+"] adr[" + trains.last().get_adr().get_hex() + "]");
                _description.append(" has been added.");
            }
        }
        _description.append(" Speed: " + _speed.get_hex() + " Slot: " + _slot.get_hex() + " Direction: " + QString::number(_dir));
    }

    if (_description == "E7:") {
        _description.append(" No action taken?");
    }
    return (_description);
}

QString LocoNet::parse_EF (LocoPacket _packet, bool _enable) {
    QString _description = "EF: Write slot data.";
    return(_description);
}

QString LocoNet::parse_E5 (LocoPacket _packet, bool _enable) {
    QString _description = "E5: Move 8 bytes Peer to Peer.";
    return(_description);
}

QString LocoNet::parse_ED (LocoPacket _packet, bool _enable) {
    QString _description = "ED: Send n-byte packet.";
    return(_description);
}

QString LocoNet::parse_85 (LocoPacket _packet, bool _enable) {
    QString _description = "85: Requesting track state IDLE / EMG STOP.";
    return(_description);
}

QString LocoNet::parse_83 (LocoPacket _packet, bool _enable) {
    QString _description = "83: Requesting track state ON.";
    return(_description);
}

QString LocoNet::parse_82 (LocoPacket _packet, bool _enable) {
    QString _description = "82: Requesting track state OFF.";
    return(_description);
}

QString LocoNet::parse_81 (LocoPacket _packet, bool _enable) {
    QString _description = "81: MASTER sent BUSY code.";
    return(_description);
}

QString LocoNet::parse_BF (LocoPacket _packet, bool _enable) {
    QString _description = "BF: Requesting locomotive address.";
    return(_description);
}

QString LocoNet::parse_BD (LocoPacket _packet, bool _enable) {
    QString _description = "BD: Requesting switch with LACK function."; // LACK - Long ACKnowledge
    return(_description);
}

QString LocoNet::parse_BC (LocoPacket _packet, bool _enable) {
    QString _description = "BC: Requesting state of switch.";
    return(_description);
}

QString LocoNet::parse_BB (LocoPacket _packet, bool _enable) {
    QString _description = "BB: Requesting SLOT data/status block.";
    return(_description);
}

QString LocoNet::parse_BA (LocoPacket _packet, bool _enable) {
    QString _description = "BA: Move slot SRC to DEST.";
    return(_description);
}

QString LocoNet::parse_B9 (LocoPacket _packet, bool _enable) {
    QString _description = "B9: Link slot ARG1 to slot ARG2.";
    return(_description);
}

QString LocoNet::parse_B8 (LocoPacket _packet, bool _enable) {
    QString _description = "B8: Unlink slot ARG1 from slot ARG2.";
    return(_description);
}

QString LocoNet::parse_B6 (LocoPacket _packet, bool _enable) {
    QString _description = "B6: Set FUNC bits in a CONSIST uplink element.";
    return(_description);
}

QString LocoNet::parse_B5 (LocoPacket _packet, bool _enable) {
    QString _description = "B5: Write slot stat1.";
    return(_description);
}

QString LocoNet::parse_B4 (LocoPacket _packet, bool _enable) {
    QString _description = "B4: Long Acknowledge - LACK.";
    return(_description);
}

QString LocoNet::parse_B2 (LocoPacket _packet, bool _enable)
{
    QString _description = "B2:";
    LocoByte _arg1 = _packet.get_locobyte(1);
    LocoByte _arg2 = _packet.get_locobyte(2);
    bool _aux = _arg2.get_qBitArray()[2];
    bool _occupied = _arg2.get_qBitArray()[3];

    QByteArray _adr;
    _adr.append(_arg2.get_hex().mid(1,1)); // Load MS byte of address
    _adr.append(_arg1.get_hex()); // Load LS 2 bytes of address

    LocoBlock _newBlock(_adr, _aux, _occupied);
    if (_enable) // Check if we're supposed to update the state of the track from this packet.
    {
        bool _existing = false;
        for (int _index = 0; _index < blocks.count(); ++_index)
        {
            if (_newBlock == blocks[_index])
            {
                _existing = true;
                blocks[_index] = _newBlock;
                emit blockUpdated(blocks[_index]);
                _description.append(" Block[" + QString::fromLatin1(blocks[_index].get_adr()) + "] updated.");
            }
        }
        if (!_existing)
        {
            blocks.append(_newBlock);
            emit blockUpdated(blocks.last());
            _description.append(" Block[" + QString::fromLatin1(_adr) + "] added.");
        }
    }

    _description.append(" AUX: " + QString::number(_aux) + " OCC: " + QString::number(_occupied) + ".");
    return(_description);
}

QString LocoNet::parse_B1 (LocoPacket _packet, bool _enable) {
    QString _description = "B1: Turnout sensor state report.";
    return(_description);
}

QString LocoNet::parse_B0 (LocoPacket _packet, bool _enable) {
    QString _description = "B0: Request switch function.";
    return(_description);
}

QString LocoNet::parse_A2 (LocoPacket _packet, bool _enable) {
    QString _description = "A2: Setting slot sound functions.";
    return(_description);
}

QString LocoNet::parse_A1 (LocoPacket _packet, bool _enable) {
    QString _description = "A1: Setting slot direction.";
    return(_description);
}

QString LocoNet::parse_A0 (LocoPacket _packet, bool _enable) {
    QString _description = "A0: Setting slot speed.";
    return(_description);
}

/**
 * Static OP list functions
 */

int LocoNet::get_staticOPsize()
{
    return(opcodes_hex.size());
}

QString LocoNet::get_staticOPname(int _index)
{
    return(opcodes_name[_index]);
}

QString LocoNet::get_staticOPhex(int _index)
{
    return(opcodes_hex[_index].get_hex());
}

void LocoNet::do_addStaticOP(QString _hex, QString _name, QString _desc)
{
    for (int _index = 0; _index < opcodes_hex.size(); ++_index)
    {
        if (opcodes_hex[_index].get_hex() == _hex) {
            return;
        }
    }
    opcodes_hex.append(LocoByte(_hex));
    opcodes_name.append(_name);
    opcodes_desc.append(_desc);
}



