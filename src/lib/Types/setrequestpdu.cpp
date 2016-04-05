#include "setrequestpdu.h"

SetRequestPDU::SetRequestPDU(QObject *parent) :
    ProtocolDataUnit(Type::SetRequestPDU, parent)
{
}

SetRequestPDU::SetRequestPDU(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::SetRequestPDU, requestId, error, errorIndex, varbindList)
{
}
