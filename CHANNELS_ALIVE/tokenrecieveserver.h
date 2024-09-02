#ifndef TOKENRECIEVESERVER_H
#define TOKENRECIEVESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

class TokenRecieveServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TokenRecieveServer(QObject* parent);
	void incomingConnection(qintptr Handle) override;
public slots:
	void onReadyRead();
	void onDisconnected();
signals:
	void onRecieve(QByteArray ReplyContent);
};

#endif // TOKENRECIEVESERVER_H
