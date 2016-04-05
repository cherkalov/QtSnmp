#include "Get.h"

#include "Types/DataTypeFactory.h"

#include "Types/ObjectIdentifier.h"
#include "Types/Null.h"

#include "Types/Sequence.h"
#include "Types/SnmpMessage.h"

#include "Types/SnmpVersion.h"
#include "Types/OctetString.h"

#include "Types/Varbind.h"
#include "Types/VarbindList.h"

#include "Types/GetRequestPDU.h"
#include "Types/GetResponsePDU.h"

#include <QUdpSocket>

#ifdef QT_SNMP_DEBUG
#include <QDebug>
#endif

Get::Get(QObject *parent) :
    QObject(parent),
    socket(new QUdpSocket(this))
{
    socket->bind(10500);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagram()));
}

Get::Get(const QString &peer, const QString &objectId, QObject *parent) :
    QObject(parent),
    peer(peer),
    objectId(objectId),
    socket(new QUdpSocket(this))
{
    socket->bind(10500);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagram()));

}

void Get::setPeer(const QString &peer)
{
    this->peer = peer;
}

void Get::setObjectId(const QString &objectId)
{
    this->objectId = objectId;
}

void Get::execute()
{
    SnmpVersion *version = new SnmpVersion(SnmpVersion::SNMPv2);
    OctetString *community = new OctetString("public"); //#TODO: Make configurable.

    Integer *requestId = new Integer(1);
    Integer *error = new Integer(0);
    Integer *errorIndex = new Integer(0);

    ObjectIdentifier *objectIdentifier = new ObjectIdentifier(objectId);
    AbstractSyntaxNotationOne *value = new Null();
    VarbindList *varbindList = new VarbindList(SequenceData{new Varbind(objectIdentifier, value)});

    ProtocolDataUnit *getRequestPDU = new GetRequestPDU(requestId, error, errorIndex, varbindList);

    SnmpMessage snmpMessage(version, community, getRequestPDU);
    QByteArray datagram = snmpMessage.encode();
    qDebug() << "datagram:" << datagram << datagram.toHex();
    socket->writeDatagram(datagram, QHostAddress(peer), 161);
}

QList<ResponseStruct> Get::getResponse() const
{
    return response;
}

void Get::readPendingDatagram()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram, remove;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        remove.append('\x82');
        remove.append('\x00');
        datagram.replace(remove, "");

        QDataStream stream(&datagram, QIODevice::ReadOnly);
        Type::AbstractSyntaxNotationOneType type;
        stream >> type;

        if (type != Type::Sequence)
            throw SnmpException("Wrong response");

        quint8 bytesRead = 0;
        SnmpMessage sequence;
        bytesRead += sequence.decode(stream);

        ResponseStruct responses;
        VarbindList *varbindList = sequence.getProtocolDataUnit()->getVarbindList();
        foreach (Varbind *varbind, varbindList->getVarbinds()) {
            responses.objectIdentifier = varbind->getObjectIdentifier()->toString();
            responses.type = DataTypeFactory::getTypeName(varbind->getValue()->getType());
            responses.value = varbind->getValue()->toString();
            response.append(responses);
        }

        emit readedDatagram();

        foreach(responses, response)
        {
            qDebug() << QString("SNMP datagram has been read!\n"
                                "Version: %1\n"
                                "Community: %2\n"
                                "Responses: %3 = %4 : %5\n")
                                .arg(sequence.getSnmpVersion()->getVersion() + 1)
                                .arg(sequence.getCommunity()->getValue())
                                .arg(responses.objectIdentifier).arg(responses.type).arg(responses.value);
        }
    }
}
