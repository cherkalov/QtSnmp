#pragma once

#ifndef SETREQUESTPDU_H
#define SETREQUESTPDU_H

#include "ProtocolDataUnit.h"

class SetRequestPDU : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SetRequestPDU(QObject *parent = 0);
    SetRequestPDU(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

#endif // SETREQUESTPDU_H
