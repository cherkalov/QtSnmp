#include "NullObject.h"

NullObject::NullObject(QObject *parent) :
    AbstractSyntaxNotationOne(Type::NullObject, parent)
{
}

QString NullObject::toString() const
{
    throw SnmpException("No such element");
    return QString();
}

QVariant NullObject::toVariant() const
{
    throw SnmpException("No such element");
    return QVariant();
}

NullObject *NullObject::Instance()
{
    static NullObject nullObject;
    return &nullObject;
}

quint8 NullObject::getDataLength() const
{
    throw SnmpException("No such element");
    return 0;
}

QByteArray NullObject::encodeData() const
{
    throw SnmpException("No such element");
    return QByteArray();
}

void NullObject::decodeData(QDataStream &inputStream, quint8 length)
{
    Q_UNUSED(inputStream);
    Q_UNUSED(length);
    throw SnmpException("No such element");
}
