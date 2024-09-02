#ifndef NOTIFICATIONCLIENT_H
#define NOTIFICATIONCLIENT_H

#include <QObject>

class NotificationClient : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)
public:
	explicit NotificationClient(QObject *parent = nullptr);

	void setNotification(const QString &notification);
	QString notification() const;

signals:
	void notificationChanged();

private slots:
	void updateAndroidNotification();

private:
	QString notificationNested;
};

#endif // NOTIFICATIONCLIENT_H
