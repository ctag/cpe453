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

QVector<LocoBlock> LocoNet::get_blocks ()
{
    return(blocks);
}

void LocoNet::do_startPacketTimer (int _msec) {
    packetTimer = new QTimer(this);
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(handle_packetTimer()));
    packetTimer->start(_msec);
}

void LocoNet::do_addTimerPacket (LocoPacket _packet, int _interval)
{
    packetTimerPackets.append(_packet);
    packetTimerPacketInterval.append(_interval);
    packetTimerPacketState.append(0);
}

void LocoNet::do_rmTimerPacket (LocoPacket _packet)
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
    while(usbBuffer->waitForReadyRead(20) || usbBuffer->bytesAvailable() > 0)
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
            emit newPacketDescription(do_parsePacket(incomingPacket));
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
    _packet.do_genChecksum(); // in case the packet doesn't have a checksum already
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
        return("packet is malformed >.<");
    }

    if (_opCode == "E7") {
        return(handle_E7(_packet));
    } if (_opCode == "B2") {
        return(handle_B2(_packet));
    }
    return ("opcode [" + _opCode + "] doesn't match anything in parser :c");
}

QString LocoNet::handle_E7 (LocoPacket _packet, bool enable)
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
        _description.append(" Speed: " + _speed.get_hex() + " Slot: " + _slot.get_hex() + " Direction: " + QString::number(_dir));
    }

    if (_description == "E7:") {
        _description.append(" No action taken?");
    }
    return (_description);
}

QString LocoNet::handle_85 (LocoPacket _packet, bool enable) {
    QString _description = "85: Requesting track state IDLE / EMG STOP.";
    return(_description);
}

QString LocoNet::handle_83 (LocoPacket _packet, bool enable) {
    QString _description = "83: Requesting track state ON.";
    return(_description);
}

QString LocoNet::handle_82 (LocoPacket _packet, bool enable) {
    QString _description = "82: Requesting track state OFF.";
    return(_description);
}

QString LocoNet::handle_81 (LocoPacket _packet, bool enable) {
    QString _description = "81: MASTER sent BUSY code.";
    return(_description);
}

QString LocoNet::handle_BF (LocoPacket _packet, bool enable) {
    QString _description = "BF: Requesting locomotive address.";
    return(_description);
}

QString LocoNet::handle_BD (LocoPacket _packet, bool enable) {
    QString _description = "BD: Requesting switch with LACK function."; // LACK - Long ACKnowledge
    return(_description);
}

QString LocoNet::handle_BC (LocoPacket _packet, bool enable) {
    QString _description = "BC: Requesting state of switch.";
    return(_description);
}

QString LocoNet::handle_BB (LocoPacket _packet, bool enable) {
    QString _description = "BB: Requesting SLOT data/status block.";
    return(_description);
}

QString LocoNet::handle_BA (LocoPacket _packet, bool enable) {
    QString _description = "BA: Move slot SRC to DEST.";
    return(_description);
}

QString LocoNet::handle_B9 (LocoPacket _packet, bool enable) {
    QString _description = "B9: Link slot ARG1 to slot ARG2.";
    return(_description);
}

QString LocoNet::handle_B8 (LocoPacket _packet, bool enable) {
    QString _description = "B8: Unlink slot ARG1 from slot ARG2.";
    return(_description);
}

QString LocoNet::handle_B6 (LocoPacket _packet, bool enable) {
    QString _description = "B6: Set FUNC bits in a CONSIST uplink element.";
    return(_description);
}

QString LocoNet::handle_B5 (LocoPacket _packet, bool enable) {
    QString _description = "B5: Write slot stat1.";
    return(_description);
}

QString LocoNet::handle_B4 (LocoPacket _packet, bool enable) {
    QString _description = "B4: Long Acknowledge - LACK.";
    return(_description);
}

QString LocoNet::handle_B2 (LocoPacket _packet, bool enable)
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

    _description.append(" AUX: " + QString::number(_aux) + " OCC: " + QString::number(_occupied) + ".");
    return(_description);
}

QString LocoNet::handle_B1 (LocoPacket _packet, bool enable) {
    QString _description = "B1: Turnout sensor state report.";
    return(_description);
}

QString LocoNet::handle_B0 (LocoPacket _packet, bool enable) {
    QString _description = "B0: Request switch function.";
    return(_description);
}

QString LocoNet::handle_A2 (LocoPacket _packet, bool enable) {
    QString _description = "A2: Setting slot sound functions.";
    return(_description);
}

QString LocoNet::handle_A1 (LocoPacket _packet, bool enable) {
    QString _description = "A1: Setting slot direction.";
    return(_description);
}

QString LocoNet::handle_A0 (LocoPacket _packet, bool enable) {
    QString _description = "A0: Setting slot speed.";
    return(_description);
}


