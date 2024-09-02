#ifndef TWITCHAPI_H
#define TWITCHAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QUrl>
#include <QUrlQuery>
#include <QDesktopServices>
#include <QSharedPointer>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QDateTime>
#include <QVariant>
#include <QHostInfo>
#include <QCryptographicHash>
#include "tokenrecieveserver.h"
#include "qaesencryption.h"

class TwitchAPI : public QObject
{
	Q_OBJECT
	QString AppToken = "";
	QString RefreshAppToken = "";
	QString Code = "";
	QString ClientId = "СЮДА КЛИЕНТ ID ПРИЛОЖЕНИЯ";
	QString ClientSecret = "СЮДА ТОКЕН КЛИЕНТ СЕКРЕТА";
	QString Original = "ORIGINAL";
	QDateTime ExpiresAt;
	QNetworkAccessManager* Manager;
	QNetworkRequest Request;
	QSslConfiguration SSLConfig;
	QByteArray HandleResponse(QNetworkReply* Reply);
	void LoadData();
	TokenRecieveServer* Server;
	QByteArray ReadData();
	void WriteData(QByteArray Data);
	void LoadClient();
	void RefreshToken();
	QByteArray MakeRequest(QString Url);

public:
	explicit TwitchAPI(QObject *parent = nullptr);
	QDateTime GetExpiresAt();
	QByteArray GetUserInfo(QString Username);
	QByteArray GetUserFollowers(QString BroadcasterId);
	QByteArray GetCurrentStreamInfo(QString Username);

public slots:

	void onRecieve(QByteArray ReplyContent);

signals:

	void onCodeChange();

};

#endif // TWITCHAPI_H
