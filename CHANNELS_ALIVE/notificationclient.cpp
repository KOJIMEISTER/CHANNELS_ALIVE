#include "notificationclient.h"
#include <QtCore/qjniobject.h>
#include <QtCore/qcoreapplication.h>

NotificationClient::NotificationClient(QObject *parent)
	: QObject(parent)
{
	connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
}

void NotificationClient::setNotification(const QString &notification)
{
	notificationNested = notification;
	emit notificationChanged();
}

QString NotificationClient::notification() const
{
	return notificationNested;
}

void NotificationClient::updateAndroidNotification()
{
	QJniEnvironment env;
	jclass javaClass = env.findClass("NotificationClient");
	QJniObject javaNotification = QJniObject::fromString(notificationNested);
	QJniObject::callStaticMethod<void>(
				javaClass,
				"notify",
				"(Landroid/content/Context;Ljava/lang/String;)V",
				QNativeInterface::QAndroidApplication::context(),
				javaNotification.object<jstring>());
}
