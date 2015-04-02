#include "locoserial.h"

LocoSerial::LocoSerial()
{
    debug = false;
}

LocoSerial::~LocoSerial()
{
    usbBuffer->close();
    readTimerStop();
    delete usbBuffer;
    delete readTimer;
    delete incomingPacket;
    delete outgoingPacket;
}

void LocoSerial::run()
{
    usbBuffer = new QSerialPort;
    readTimer = new QTimer;
    incomingPacket = new LocoPacket;
    outgoingPacket = new LocoPacket;
    incomingPacket->clear();
    outgoingPacket->clear();
}

void LocoSerial::do_write(LocoPacket _packet)
{
    _packet.do_genChecksum();
    do_write(_packet.get_QByteArray()); // just redirect
}

void LocoSerial::do_write(QByteArray _bytes)
{
    if (!usbBuffer->isOpen())
    {
        return;
    }
    if (debug) qDebug() << "Writing to serial: " << _bytes << _bytes.toInt(0, 16);
    usbBuffer->write(_bytes);
    outgoingPacket->clear();
    outgoingPacket->do_appendByteArray(_bytes);
    parse(*outgoingPacket);
}

void LocoSerial::do_querySlot(LocoByte _slot)
{
    QString _queryText = "BB" + _slot.get_hex() + "00";
    LocoPacket _querySlot(_queryText);
    do_write(_querySlot); // the write() method will generate a checksum for us
}

void LocoSerial::readTimerStart(int _msec)
{
    connect(readTimer, SIGNAL(timeout()), this, SLOT(do_read()));
    readTimer->start(_msec);
}

void LocoSerial::readTimerStop()
{
    disconnect(readTimer, 0, 0, 0);
    readTimer->stop();
}

void LocoSerial::do_close()
{
    readTimerStop();
    disconnect(usbBuffer, 0, 0, 0);
    usbBuffer->close();
    emit serialClosed();
}

bool LocoSerial::do_open(QSerialPortInfo _port)
{
    usbBuffer->close(); // Make sure to close any previous connection

    usbBuffer->setPort(_port);
    usbBuffer->setBaudRate(57600);
    usbBuffer->setFlowControl(QSerialPort::HardwareControl);
    usbBuffer->open(QIODevice::ReadWrite);

    if (usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial port appears to have opened sucessfully.";
        connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(do_read()));
        readTimerStart(15);
        emit serialOpened();
        return(true);
    }

    if (debug) qDebug() << "Serial port failed to open.";
    usbBuffer->close();
    emit serialClosed();
    return(false);
}

void LocoSerial::do_read()
{
    if (!usbBuffer->isOpen())
    {
        return; // not open
    }

    // Read immediately if data is available
    while(usbBuffer->bytesAvailable() > 0)
    {
        if (debug) qDebug() << "Reading byte from serial.";
        LocoByte _byte;
        _byte.set_fromByteArray(usbBuffer->read(1));
        if (!_byte.get_isOP() && incomingPacket->get_size() == 0)
        {
            if (debug) qDebug() << "Skipping non opcode with empty incoming packet.";
            continue; // no need to assign anything but an OP to position 0
        }
        if (_byte.get_isOP())
        {
            if (debug) qDebug() << "OP code: " << _byte.get_hex();
        }
        if (_byte.get_isOP() && (incomingPacket->get_size() > 0))
        {
            if (debug) qDebug() << "Dropping packet to catch incoming op code.";
            emit droppedPacket();
            incomingPacket->clear();
        }
        incomingPacket->do_appendLocoByte(_byte);
        //incomingPacket->do_appendByteArray(usbBuffer->read(1)); // Load in a byte from the serial buffer
        if (debug) qDebug() << "packet:" << incomingPacket->get_size() << ":" << incomingPacket->get_finalSize();
        if (incomingPacket->get_size() == incomingPacket->get_finalSize() && incomingPacket->get_size() >= 2) // packet is the right size
        {
            if (incomingPacket->validChk()) // packet has a valid checksum, bingo!
            {
                if (debug) qDebug() << "Received a full packet! " << incomingPacket->get_packet();
                emit receivedPacket(*incomingPacket);
                parse(*incomingPacket);
            } else {
                emit droppedPacket();
            }
            incomingPacket->clear(); // doesn't matter if packet has valid checksum here, time to move to the next packet
            continue;
        }
        if (incomingPacket->get_size() > incomingPacket->get_finalSize() && incomingPacket->get_size() >= 2)
        {
            if (debug) qDebug() << "Dropping packet for exceeding length stated by op code.";
            emit droppedPacket();
            incomingPacket->clear();
            continue;
        }
    }
}

/*
 * Packet related functions
 */

QString LocoSerial::parse (LocoPacket _packet)
{
    QString _opCode = (_packet.get_OPcode());

    if (!_packet.hasOP() || !_packet.validChk()) {
        return("packet is malformed >.<");
    }

    // This looks so gross >.<
    if (_opCode == "E7") {
        return(parse_E7(_packet));
    } else if (_opCode == "B2") {
        return(parse_B2(_packet));
    } else if (_opCode == "85") {
        return(parse_85(_packet));
    } else if (_opCode == "83") {
        return(parse_83(_packet));
    } else if (_opCode == "82") {
        return(parse_82(_packet));
    } else if (_opCode == "81") {
        return(parse_81(_packet));
    } else if (_opCode == "BF") {
        return(parse_BF(_packet));
    } else if (_opCode == "BD") {
        return(parse_BD(_packet));
    } else if (_opCode == "BC") {
        return(parse_BC(_packet));
    } else if (_opCode == "BB") {
        return(parse_BB(_packet));
    } else if (_opCode == "BA") {
        return(parse_BA(_packet));
    } else if (_opCode == "B9") {
        return(parse_B9(_packet));
    } else if (_opCode == "B8") {
        return(parse_B8(_packet));
    } else if (_opCode == "B6") {
        return(parse_B6(_packet));
    } else if (_opCode == "B5") {
        return(parse_B5(_packet));
    } else if (_opCode == "B4") {
        return(parse_B4(_packet));
    } else if (_opCode == "B1") {
        return(parse_B1(_packet));
    } else if (_opCode == "B0") {
        return(parse_B0(_packet));
    } else if (_opCode == "A2") {
        return(parse_A2(_packet));
    } else if (_opCode == "A1") {
        return(parse_A1(_packet));
    } else if (_opCode == "A0") {
        return(parse_A0(_packet));
    } else if (_opCode == "EF") {
        return(parse_EF(_packet));
    } else if (_opCode == "E5") {
        return(parse_E5(_packet));
    } else if (_opCode == "ED") {
        return(parse_ED(_packet));
    }

    return ("opcode [" + _opCode + "] doesn't match anything in parser :c");
}

QString LocoSerial::parse_E7 (LocoPacket _packet)
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
        _description.append(" Slot " + _slot.get_hex() + " is inactive.");
        // Find trains assigned to the slot and delete them
        /*
        for (int _index = 0; _index < trains.count(LocoPacket _packet); ++_index)
        {
            if (trains[_index].get_slot(LocoPacket _packet) == _slot) {
                trains.remove(_index);
            }
        }
        */
    } else { // Past here we know a 'train' is in the slot
        LocoTrain _newTrain;
        _newTrain.set_adr(_adr);
        _newTrain.set_reverse(_dir);
        _newTrain.set_slot(_slot);
        _newTrain.set_speed(_speed);

        /*
        if (_enable)
        {
            bool _existingTrain = false;
            for (int _index = 0; _index < trains.count(LocoPacket _packet); ++_index)
            {
                if (_newTrain == trains[_index])
                {
                    _existingTrain = true;
                    trains[_index] = _newTrain;
                    emit trainUpdated(trains[_index]);
                    _description.append(" Train["+QString::number(_index)+"] adr[" + trains[_index].get_adr(LocoPacket _packet).get_hex(LocoPacket _packet) + "]");
                    _description.append(" has been updated.");
                }
            }
            if (!_existingTrain)
            {
                trains.append(_newTrain);
                emit trainUpdated(trains.last(LocoPacket _packet));
                _description.append(" Train["+QString::number(trains.count(LocoPacket _packet)-1)+"] adr[" + trains.last(LocoPacket _packet).get_adr(LocoPacket _packet).get_hex(LocoPacket _packet) + "]");
                _description.append(" has been added.");
            }
        }
        */
        emit trainUpdated(_newTrain);
        _description.append(" Speed: " + _speed.get_hex() + " Slot: " + _slot.get_hex() + " Direction: " + QString::number(_dir));
    }

    if (_description == "E7:") {
        _description.append(" No action taken?");
    }
    return (_description);
}

QString LocoSerial::parse_EF (LocoPacket _packet) {
    QString _description = "EF: Write slot data.";
    return(_description);
}

QString LocoSerial::parse_E5 (LocoPacket _packet) {
    QString _description = "E5: Move 8 bytes Peer to Peer.";
    return(_description);
}

QString LocoSerial::parse_ED (LocoPacket _packet) {
    QString _description = "ED: Send n-byte packet.";
    return(_description);
}

QString LocoSerial::parse_85 (LocoPacket _packet) {
    QString _description = "85: Requesting track state IDLE / EMG STOP.";
    return(_description);
}

QString LocoSerial::parse_83 (LocoPacket _packet) {
    QString _description = "83: Requesting track state ON.";
    return(_description);
}

QString LocoSerial::parse_82 (LocoPacket _packet) {
    QString _description = "82: Requesting track state OFF.";
    return(_description);
}

QString LocoSerial::parse_81 (LocoPacket _packet) {
    QString _description = "81: MASTER sent BUSY code.";
    return(_description);
}

QString LocoSerial::parse_BF (LocoPacket _packet) {
    QString _description = "BF: Requesting locomotive address.";
    return(_description);
}

QString LocoSerial::parse_BD (LocoPacket _packet) {
    QString _description = "BD: Requesting switch with LACK function."; // LACK - Long ACKnowledge
    return(_description);
}

QString LocoSerial::parse_BC (LocoPacket _packet) {
    QString _description = "BC: Requesting state of switch.";
    return(_description);
}

QString LocoSerial::parse_BB (LocoPacket _packet) {
    QString _description = "BB: Requesting SLOT data/status block.";
    return(_description);
}

QString LocoSerial::parse_BA (LocoPacket _packet) {
    QString _description = "BA: Move slot SRC to DEST.";
    return(_description);
}

QString LocoSerial::parse_B9 (LocoPacket _packet) {
    QString _description = "B9: Link slot ARG1 to slot ARG2.";
    return(_description);
}

QString LocoSerial::parse_B8 (LocoPacket _packet) {
    QString _description = "B8: Unlink slot ARG1 from slot ARG2.";
    return(_description);
}

QString LocoSerial::parse_B6 (LocoPacket _packet) {
    QString _description = "B6: Set FUNC bits in a CONSIST uplink element.";
    return(_description);
}

QString LocoSerial::parse_B5 (LocoPacket _packet) {
    QString _description = "B5: Write slot stat1.";
    return(_description);
}

QString LocoSerial::parse_B4 (LocoPacket _packet) {
    QString _description = "B4: Long Acknowledge - LACK.";
    return(_description);
}

QString LocoSerial::parse_B2 (LocoPacket _packet)
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

    emit blockUpdated(_newBlock);

    _description.append(" AUX: " + QString::number(_aux) + " OCC: " + QString::number(_occupied) + ".");
    return(_description);
}

QString LocoSerial::parse_B1 (LocoPacket _packet) {
    QString _description = "B1: Turnout sensor state report.";
    return(_description);
}

QString LocoSerial::parse_B0 (LocoPacket _packet) {
    QString _description = "B0: Request switch function.";
    return(_description);
}

QString LocoSerial::parse_A2 (LocoPacket _packet) {
    QString _description = "A2: Setting slot sound functions.";
    return(_description);
}

QString LocoSerial::parse_A1 (LocoPacket _packet) {
    QString _description = "A1: Setting slot direction.";
    return(_description);
}

QString LocoSerial::parse_A0 (LocoPacket _packet) {
    QString _description = "A0: Setting slot speed.";
    LocoByte _arg1 = _packet.get_locobyte(1);
    LocoByte _arg2 = _packet.get_locobyte(2);
    do_querySlot(_arg1); // Ask for E7 slot data
    return(_description);
}


