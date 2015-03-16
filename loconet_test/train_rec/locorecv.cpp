#include "locorecv.h"

bool LocoRecv::debug = true;

// Don't use this
LocoRecv::LocoRecv ()
{

}

LocoRecv::LocoRecv (QSerialPort *buffer)
{
    usbBuffer = buffer;

    connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(handleRecv()));

}

LocoRecv::~LocoRecv()
{

}

void LocoRecv::handleRecv ()
{
    while(usbBuffer->bytesAvailable() > 0 || usbBuffer->waitForReadyRead(20))
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
            emit receivedPacket(incomingPacket);
            //emit newPacketDescription(handle_parsePacket(incomingPacket));
            incomingPacket = LocoPacket(); // Reset the packet.
        }
        if (debug) qDebug() << incomingPacket.get_packet();
    }
}


