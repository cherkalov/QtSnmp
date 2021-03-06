#include "AbstractSyntaxNotationOne.h"

#include "DataTypeFactory.h"

#include <QDataStream>

#include <QDebug>

static const quint8 TYPE_SIZE = 1;
static const quint8 LENGTH_SIZE = 1;

AbstractSyntaxNotationOne::AbstractSyntaxNotationOne(Type::AbstractSyntaxNotationOneType type, QObject *parent) :
    QObject(parent),
    type(type)
{
}

QByteArray AbstractSyntaxNotationOne::encode() const
{
    quint8 dataLength = getDataLength();

    QByteArray code;
    code.append(type);
    code.append(dataLength);
    code.append(encodeData());

    qDebug() << dataLength << code.toHex();

    return code;
}

int AbstractSyntaxNotationOne::decode(QDataStream &inputStream)
{
    quint8 dataLength = 0;
    inputStream >> dataLength;
    qDebug() << "Start decoding:" << this;
    decodeData(inputStream, dataLength);
    qDebug() << "Decoded:" << this;
    return LENGTH_SIZE + dataLength;
}

quint8 AbstractSyntaxNotationOne::getLength() const
{
    return TYPE_SIZE + LENGTH_SIZE + getDataLength();
}

Type::AbstractSyntaxNotationOneType AbstractSyntaxNotationOne::getType() const
{
    return type;
}

QVariant AbstractSyntaxNotationOne::toVariant() const
{
    return QVariant();
}
