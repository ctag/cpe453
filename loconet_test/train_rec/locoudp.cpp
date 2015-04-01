#include "locoudp.h"

bool LocoUDP::debug = false;

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
    connect(&socket, SIGNAL(readyRead()), this, SLOT(do_readPendingDatagram()));
}

void LocoUDP::do_closeSocket()
{
    socket.close();
}

void LocoUDP::do_readPendingDatagram()
{
    while (socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString _hex(datagram);
        LocoPacket _packet(_hex);

        qDebug() << "New UDP datagram: " << _packet.get_packet();

        if (_packet.validOP())
        {
            qDebug() << "Emitting UDP packet.";
            emit incomingRequest(_packet);
        }
    }
}

void LocoUDP::do_writeDatagram(LocoPacket _packet)
{
    QByteArray datagram;
    datagram.resize(_packet.get_size());
    QHostAddress address("127.0.0.1");
    quint16 port = 7756;
    socket.writeDatagram(datagram.data(), datagram.size(), address, port);
}







