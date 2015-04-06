#include "locoudp.h"

LocoUDP::LocoUDP()
{
    //
}

LocoUDP::~LocoUDP()
{
    do_socketClose();
    socket->deleteLater();
}

QString LocoUDP::timeStamp()
{
    return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

void LocoUDP::do_run()
{
    socket = new QUdpSocket;
    request = new LocoPacket;
    debug = new bool;
    *debug = false;
    connect(socket, SIGNAL(readyRead()), this, SLOT(do_readPendingDatagram()));
    do_socketOpen(7755);
    qDebug() << timeStamp() << "UDP thread initialized.";
}

void LocoUDP::do_socketOpen(int _port)
{
    if (!socket)
    {
        socket->bind(QHostAddress::LocalHost, _port);
    }
}

void LocoUDP::do_socketClose()
{
    if (socket)
    {
        socket->close();
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

        if (debug) qDebug() << timeStamp() << "New UDP datagram: " << _packet.get_packet();

        if (_packet.validOP())
        {
          if (debug) qDebug() << timeStamp() << "Emitting UDP packet.";
            emit incomingRequest(_packet);
        }
    }
}







