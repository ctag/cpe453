#include "loconet.h"

bool LocoNet::debug = true;
//QVector<LocoByte> LocoNet::opcodes_hex;
//QVector<QString> LocoNet::opcodes_name;
//QVector<QString> LocoNet::opcodes_desc;

LocoNet::LocoNet ()
{
    //incomingPacket = LocoPacket();
    //usbBuffer = NULL;
    //packetTimer = NULL;
    locoserial.moveToThread(&serialThread);
    //connect(&locoserial, &LocoSerial::receivedPacket, this, &LocoNet::handle_parsePacket);
    serialThread.start();

    locosql.moveToThread(&sqlThread);
    sqlThread.start();

    connect(&locoserial, &LocoSerial::blockUpdated, &locosql, &LocoSQL::do_updateBlock);

    // Setup signal passthrough for SQL
    connect(&locosql, &LocoSQL::DBopened, this, &LocoNet::DBopened);
    connect(&locosql, &LocoSQL::DBclosed, this, &LocoNet::DBclosed);

    // Setup signal passthrough for serial
    connect(&locoserial, &LocoSerial::serialOpened, this, &LocoNet::serialOpened);
    connect(&locoserial, &LocoSerial::serialClosed, this, &LocoNet::serialClosed);

    // Setup signal passthrough for new packet
    connect(&locoserial, &LocoSerial::receivedPacket, this, &LocoNet::receivedPacket);
    connect(&locoserial, &LocoSerial::droppedPacket, this, &LocoNet::droppedPacket);
}

LocoNet::~LocoNet ()
{
    serialThread.wait();
    serialThread.quit();
    sqlThread.wait();
    serialThread.quit();
}

/*
bool LocoNet::get_serialOpen ()
{
    if (!usbBuffer)
    {
        return(false); // pointer is null
    }
    return(usbBuffer->isOpen());
}
*/

/*
QVector<LocoTrain> LocoNet::get_trains ()
{
    return(trains);
}

QVector<LocoBlock> LocoNet::get_blocks ()
{
    return(blocks);
}
*/

/*
 * Timer related functions
 */

/*
bool LocoNet::get_timerActive()
{
    return(packetTimer->isActive());
}

QVector<LocoPacket> LocoNet::get_timerPackets()
{
    return(packetTimerPackets);
}
*/

/*
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
    if (!packetTimer)
    {
        return; // pointer already null
    }
    packetTimer->stop();
    disconnect(packetTimer, 0, 0, 0);
    delete packetTimer;
    packetTimer = NULL;
    packetTimerPackets.clear();
    packetTimerPacketInterval.clear();
    packetTimerPacketState.clear();
}

void LocoNet::handle_packetTimer ()
{
    if (!packetTimer)
    {
        return; // no timer to work with
    }
    for (int _index = 0; _index < packetTimerPackets.count(); ++_index)
    {
        ++packetTimerPacketState[_index];
        if (packetTimerPacketState[_index] >= packetTimerPacketInterval[_index])
        {
            if (debug) qDebug() << "Timer Packet Sent: " << packetTimerPackets[_index].get_packet();
            packetTimerPacketState[_index] = 0;
            do_serialWrite(packetTimerPackets[_index]);
        }
    }
}
*/

/*
 * Serial Port related functions
 */

void LocoNet::do_serialOpen (QSerialPortInfo _port)
{
    locoserial.open(_port);
}

/*
bool LocoNet::do_serialOpen (QSerialPortInfo _port)
{
    usbBuffer = new QSerialPort;
    usbBuffer->setPort(_port);
    usbBuffer->setBaudRate(57600);
    usbBuffer->setFlowControl(QSerialPort::HardwareControl);
    usbBuffer->open(QIODevice::ReadWrite);
    if (usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial port appears to have opened sucessfully.";
        connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(handle_serialRead()));
        return(true);
    }
    if (debug) qDebug() << "Serial port failed to open.";
    delete usbBuffer;
    usbBuffer = NULL;
    return(false);
}
*/

void LocoNet::do_serialClose ()
{
    locoserial.close();
}

void LocoNet::do_sqlOpen(QString hostname, int port, QString database, QString username, QString password)
{
    locosql.do_openDB(hostname, port, database, username, password);
}

void LocoNet::do_sqlClose()
{
    locosql.do_closeDB();
}

/*
void LocoNet::do_serialClose ()
{
    if (!usbBuffer)
    {
        return; // pointer not set
    }
    if (usbBuffer->isOpen())
    {
        disconnect(usbBuffer, 0, 0, 0);
        usbBuffer->close();
        delete usbBuffer;
        usbBuffer = NULL;
    }
}
*/

/*
void LocoNet::handle_serialRead (LocoPacket _packet)
{
    emit newPacket(_packet);
    emit newPacketDescription(handle_parsePacket(_packet));
}

void LocoNet::handle_serialRead ()
{
    if (!usbBuffer)
    {
        return; // null pointer, no port to read
    }
    // If the usb device isn't open, we're not reading anything...
    if (!usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial port suddenly isn't open x.x";
        do_serialClose();
        return;
    }

    // Read immediately if data is available
    while(usbBuffer->bytesAvailable() > 0)
    {
        if (debug) qDebug() << "Reading serial ^_^";
        if (incomingPacket.get_numBytes() > 36)
        {
            incomingPacket = LocoPacket();
        }
        incomingPacket.do_appendByteArray(usbBuffer->read(1)); // Load in a byte from the serial buffer
        // Check to see if the packet has a valid OP code.
        if (!incomingPacket.is_validOP()) {
            incomingPacket.set_allFromHex(""); // Reset the packet, since the OP isn't valid.
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
*/

void LocoNet::do_serialWrite(LocoPacket _packet)
{
    locoserial.write(_packet);
}

void LocoNet::do_serialWrite(QString _hex)
{
    locoserial.write(_hex);
}

/*
void LocoNet::do_serialWrite (LocoPacket _packet)
{
    if (!usbBuffer)
    {
        if (debug) qDebug() << "usbBuffer doesn't exist, not writing to port.";
        return;
    }
    _packet.do_genChecksum();
    if (!usbBuffer->isOpen())
    {
        if (debug) qDebug() << "Serial isn't open...";
        return;
    }
    if (debug) qDebug() << "Writing packet: " << _packet.get_packet();
    usbBuffer->write(_packet.get_QByteArray());
}

void LocoNet::do_serialWrite (QString _hex)
{
    // That was easy enough. Just drop the hex into a new packet and call the 'real' serialWrite().
    do_serialWrite(LocoPacket(_hex));
}
*/




