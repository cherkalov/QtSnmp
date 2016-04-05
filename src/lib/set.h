#ifndef SET_H
#define SET_H

#include <QObject>
#include <QUdpSocket>

class Set : public QObject
{
    Q_OBJECT
public:
    Set(QObject *parent = 0);
    void setPeer(const QString &peer = "127.0.0.1");
    void execute(const QString &_objectId, const char &_type, const QVariant &_value);

private:
    QString _peer;
    QUdpSocket *_socket;

signals:

public slots:
};

#endif // SET_H
