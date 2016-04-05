#pragma once

#include <QObject>
#include <QStringList>

class QUdpSocket;

struct ResponseStruct {
    QString objectIdentifier;
    QString type;
    QString value;
};

class Get : public QObject
{
    Q_OBJECT

    QString peer;
    QString objectId;
    QUdpSocket *socket;

    QList<ResponseStruct> response;
public:
    Get(QObject *parent = 0);
    Get(const QString &peer, const QString &objectId, QObject *parent = 0);

    void execute();
    void setPeer(const QString &peer);
    void setObjectId(const QString &objectId);

    QList<ResponseStruct> getResponse() const;

private Q_SLOTS:
    void readPendingDatagram();

signals:
    void readedDatagram();
};
