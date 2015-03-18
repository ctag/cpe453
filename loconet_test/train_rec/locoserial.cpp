#include "locoserial.h"

bool LocoSerial::debug = true;

LocoSerial::LocoSerial()
{
    incomingPacket.set_allFromHex("");
}

LocoSerial::LocoSerial(QSerialPortInfo _port)
{
    incomingPacket.set_allFromHex("");
}

LocoSerial::~LocoSerial()
{
    usbBuffer.close();
    readTimerStop();
}

void LocoSerial::writePacket(LocoPacket _packet)
{
    writeBytes(_packet.get_QByteArray()); // just redirect
}

void LocoSerial::writeBytes(QByteArray _bytes)
{
    if (!usbBuffer.isOpen())
    {
        return;
    }
    usbBuffer.write(_bytes);
}

void LocoSerial::readTimerStart(int _msec)
{
    connect(&readTimer, SIGNAL(timeout()), this, SLOT(do_read()));
    readTimer.start(_msec);
}

void LocoSerial::readTimerStop()
{
    disconnect(&readTimer, 0, 0, 0);
    readTimer.stop();
}

void LocoSerial::close()
{
    readTimerStop();
    disconnect(&usbBuffer, 0, 0, 0);
    usbBuffer.close();
}

bool LocoSerial::open(QSerialPortInfo _port)
{
    usbBuffer.close(); // Make sure to close any previous connection

    usbBuffer.setPort(_port);
    usbBuffer.setBaudRate(57600);
    usbBuffer.setFlowControl(QSerialPort::HardwareControl);
    usbBuffer.open(QIODevice::ReadWrite);

    if (usbBuffer.isOpen())
    {
        if (debug) qDebug() << "Serial port appears to have opened sucessfully.";
        connect(&usbBuffer, SIGNAL(readyRead()), this, SLOT(do_read()));
        readTimerStart(10);
        return(true);
    }

    if (debug) qDebug() << "Serial port failed to open.";
    usbBuffer.close();
    return(false);
}

void LocoSerial::do_read()
{
    if (!usbBuffer.isOpen())
    {
        return; // not open
    }

    // Read immediately if data is available
    while(usbBuffer.bytesAvailable() > 0)
    {
        if (debug) qDebug() << "Reading serial ^_^";
        LocoByte _byte;
        _byte.set_fromByteArray(usbBuffer.read(1));
        if (_byte.get_isOP() && (incomingPacket.get_numBytes() > 0))
        {
            emit droppedPacket();
            incomingPacket.set_allFromHex("");
        }
        incomingPacket.do_appendByteArray(_byte.get_qByteArray());
        //incomingPacket.do_appendByteArray(usbBuffer.read(1)); // Load in a byte from the serial buffer
        if (incomingPacket.get_numBytes() == incomingPacket.get_packetLen()) // packet is the right size
        {
            if (incomingPacket.is_validChk()) // packet has a valid checksum, bingo!
            {
                emit receivedPacket(incomingPacket);
            }
            incomingPacket.set_allFromHex(""); // doesn't matter if packet has valid checksum here, time to move to the next packet
            continue;
        }
        if (incomingPacket.get_numBytes() > incomingPacket.get_packetLen())
        {
            emit droppedPacket();
            incomingPacket.set_allFromHex("");
            continue;
        }
    }
}



