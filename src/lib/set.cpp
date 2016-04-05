#include "set.h"

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
#include "Types/setrequestpdu.h"

Set::Set(QObject *parent) : QObject(parent)
{
    _socket = new QUdpSocket;
}

void Set::setPeer(const QString &peer)
{
    this->_peer = peer;
}

void Set::execute(const QString &_objectId, const char &_type, const QVariant &_value)
{
    SnmpVersion *version = new SnmpVersion(SnmpVersion::SNMPv2);
    OctetString *community = new OctetString("private");

    Integer *requestId = new Integer(1);
    Integer *error = new Integer(0);
    Integer *errorIndex = new Integer(0);

    ObjectIdentifier *objectIdentifier = new ObjectIdentifier(_objectId);
    AbstractSyntaxNotationOne *value;
    switch(_type)
    {
    case 'i':
    case 'c':
        value = new Integer(_value.toInt());
        break;
    case 'u':
        value = new Integer(_value.toUInt());
        break;
    case 's':
    case 'd':
    case 'a':
        value = new OctetString(_value.toString());
        break;
    case 'x':
        value = new OctetString(_value.toByteArray().toHex());
        break;
    default:
        value = new Null();
    }

    VarbindList *varbindList = new VarbindList(SequenceData{new Varbind(objectIdentifier, value)});
    ProtocolDataUnit *getRequestPDU = new SetRequestPDU(requestId, error, errorIndex, varbindList);
    SnmpMessage snmpMessage(version, community, getRequestPDU);
    QByteArray datagram = snmpMessage.encode();
    qDebug() << "datagram:" << datagram << datagram.toHex();
    _socket->writeDatagram(datagram, QHostAddress(_peer), 161);
}
