#include "OctetString.h"

#include <QDataStream>

#include <QDebug>

OctetString::OctetString(QObject *parent) :
    AbstractSyntaxNotationOne(Type::OctetString, parent)
{
}

OctetString::OctetString(const QString &value, QObject *parent) :
    AbstractSyntaxNotationOne(Type::OctetString, parent),
    value(value.toUtf8())
{
}

QString OctetString::getValue() const
{
    return QString::fromUtf8(value);
}

QString OctetString::toString() const
{
    return getValue();
}

QVariant OctetString::toVariant() const
{
    return QVariant::fromValue(value);
}

quint8 OctetString::getDataLength() const
{
    return value.length();
}

QByteArray OctetString::encodeData() const
{
    QByteArray code;
    code.append(value);
    return code;
}

void OctetString::decodeData(QDataStream &inputStream, quint8 length)
{
    quint8 bytesRead = 0;
    QByteArray buffer;

    while (bytesRead++ < length) {
        quint8 byte = 0;
        inputStream >> byte;
        buffer.append(byte);
    }

//    int n = buffer.size();
//    for (int i = 0; i < buffer.size(); i++) {
//        if (buffer[i] != '\x00') {
//            n = i;
//            break;
//        }
//    }
//    buffer = buffer.remove(0, n);
    value = buffer;
}
