#include "locoudp.h"

bool LocoUDP::debug = false;

LocoUDP::LocoUDP()
{
    //
}

LocoUDP::~LocoUDP()
{
    do_closeSocket();
}

void LocoUDP::do_run(int _port)
{
    socket = new QUdpSocket;
    request = new LocoPacket;
    socket->bind(QHostAddress::LocalHost, _port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(do_readPendingDatagram()));
    qDebug() << "UDP thread initialized.";
}

void LocoUDP::do_closeSocket()
{
    if (socket)
    {
        socket->close();
        socket->deleteLater();
    }
}

void LocoUDP::do_readPendingDatagram()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString _hex(datagram);
        LocoPacket _packet(_hex);

        if (debug) qDebug() << "New UDP datagram: " << _packet.get_packet();

        if (_packet.validOP())
        {
          if (debug) qDebug() << "Emitting UDP packet.";
            emit incomingRequest(_packet);
        }
    }
}







