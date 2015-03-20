#include "locoudp.h"

bool LocoUDP::debug = true;

LocoUDP::LocoUDP()
{
    //
}

LocoUDP::~LocoUDP()
{
    socket.close();
}

void LocoUDP::do_openSocket(int _port)
{
    socket.bind(QHostAddress::LocalHost, _port);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(do_readPendingDatagrams()));
}

void LocoUDP::do_closeSocket()
{
    socket.close();
}

void LocoUDP::do_readPendingDatagrams()
{
    while (socket.hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket.pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket.readDatagram(datagram.data(), datagram.size(),
                                    &sender, &senderPort);

            qDebug() << "New UDP datagram: " << datagram;
        }
}
