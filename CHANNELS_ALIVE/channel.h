#ifndef CHANNEL_H
#define CHANNEL_H

#include <QString>
#include <QObject>

class Channel
{
public:
	QString Name = "";
	QString BroadcasterId = "";
	QString Followers = "0";
	QString Viewers = "0";
	QString StartedAt = "0";
	QString ImageUrl = "";
	bool isLive = false;
	bool isNotified = true;
};

#endif // CHANNEL_H
