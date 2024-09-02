#ifndef BACKAPI_H
#define BACKAPI_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QList>
#include <QJsonDocument>
#include "twitchapi.h"
#include "databaseio.h"
#include "notificationclient.h"

class BackAPI : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariantList channelsListModel READ channelsListModel)
	Q_PROPERTY(QVariantList mainPageListModel READ mainPageListModel)
public:
	explicit BackAPI(QObject *parent = nullptr);
	Q_INVOKABLE bool addChannel(QString Name);
	Q_INVOKABLE bool deleteChannel(QString Name);
	Q_INVOKABLE void updateChannels();
	Q_INVOKABLE bool turnOnNotify(QString Name);
	Q_INVOKABLE bool turnOffNotify(QString Name);
	Q_INVOKABLE void updateChannelsPageModel();
	Q_INVOKABLE void updateMainPageModel();
	QVariantList& channelsListModel();
	QVariantList& mainPageListModel();
signals:
	void mainPageModelChanged();
	void channelsModelChanged();
private:
	NotificationClient Notify;
	DatabaseIO Database;
	TwitchAPI API;
	QVariantList ChannelsModelNested;
	QVariantList MainPageModelNested;
};
#endif // BACKAPI_H
