#ifndef DATABASEIO_H
#define DATABASEIO_H

#include <QObject>
#include <QtSql>
#include <QStandardPaths>
#include <QList>
#include "channel.h"


class DatabaseIO : public QObject
{
	Q_OBJECT
	QSqlDatabase db;
public:
	explicit DatabaseIO(QObject *parent = nullptr);
	~DatabaseIO() override;
	bool AddChannel(const Channel& Chann);
	bool UpdateChannel(const Channel& Chann);
	bool DeleteChannel(QString Name);
	bool TurnOnNotify(QString Name);
	bool TurnOffNotify(QString Name);
	Channel GetChannel(QString Name);
	QList<Channel> GetAllChannels();

signals:

};

#endif // DATABASEIO_H
