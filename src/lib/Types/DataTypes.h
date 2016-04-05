#pragma once

#include <QObject>
#include <QDataStream>
#include <QHash>

namespace Type{
enum AbstractSyntaxNotationOneType {
    Integer = 0x02,
    Gauge32 = 0x42,
    OctetString = 0x04,
    Null = 0x05,
    ObjectIdentifier = 0x06,
    Sequence = 0x30,
    GetRequestPDU = 0xa0,
    GetResponsePDU = 0xa2,
    SetRequestPDU = 0xa3,

    NullObject = 0xff
};
}

inline QDataStream &operator >>(QDataStream &inputStream, Type::AbstractSyntaxNotationOneType &type) {
    quint8 byte = 0;
    inputStream >> byte;
    type = static_cast<Type::AbstractSyntaxNotationOneType>(byte);
    return inputStream;
}
