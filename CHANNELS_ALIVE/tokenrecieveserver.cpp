#include "tokenrecieveserver.h"

TokenRecieveServer::TokenRecieveServer(QObject* parent) : QTcpServer(parent)
{
	if(listen(QHostAddress::LocalHost, 53354))
	{
		qDebug() << "\nServer is listening";
	}
	else
	{
		qDebug() << "\nServer is down : " << errorString();
	}
}

void TokenRecieveServer::incomingConnection(qintptr Handle)
{
	QTcpSocket* Socket = new QTcpSocket();
	Socket->setSocketDescriptor(Handle);
	connect(Socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(Socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void TokenRecieveServer::onReadyRead()
{
	QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
	if(Socket)
	{
		QByteArray UserRequest = Socket->readAll();
		QString Response = "HTTP/1.1 200 OK\r\n\r\n<script type=\"text/javascript\">window.close() ;</script>";
		Socket->write(Response.toLocal8Bit());
		Socket->disconnectFromHost();
		if(!UserRequest.isEmpty())
		{
			emit onRecieve(UserRequest);
		}
	}
}

void TokenRecieveServer::onDisconnected()
{
	QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
	if(Socket)
	{
		Socket->close();
		Socket->deleteLater();
	}
}
