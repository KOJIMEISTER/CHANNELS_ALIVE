#include "databaseio.h"

DatabaseIO::DatabaseIO(QObject *parent)
	: QObject{parent}
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("db.db");
	db.open();
//	QSqlQuery createQuery;
//	createQuery.exec("drop table channels");
//	createQuery.clear();
//	db.commit();
	QSqlQuery query;
	query.exec("create table channels"
		  "(name varchar(64) primary key, "
		   "broadcaster_id varchar(64), "
		   "followers integer, "
		   "viewers integer, "
		   "is_live boolean, "
		   "is_notified boolean,"
		   "started_at integer, "
		   "image_url varchar(256))");
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	db.commit();
	query.clear();
}

DatabaseIO::~DatabaseIO()
{
	db.commit();
	db.close();
}

bool DatabaseIO::AddChannel(const Channel& Chann)
{
	QSqlQuery query;
	query.exec(QString("insert into channels values ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')")
		   .arg(Chann.Name).arg(Chann.BroadcasterId).arg(Chann.Followers).arg(Chann.Viewers).arg(Chann.isLive)
		   .arg(Chann.isNotified).arg(Chann.StartedAt).arg(Chann.ImageUrl));
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
		query.clear();
		return false;
	}
	query.clear();
	db.commit();
	return true;
}

bool DatabaseIO::UpdateChannel(const Channel& Chann)
{
	QSqlQuery query;
	query.exec(QString("update channels set followers='%2', viewers='%3',"
			   "is_live='%4', started_at='%5' where name='%1'")
		   .arg(Chann.Name).arg(Chann.Followers).arg(Chann.Viewers).arg(Chann.isLive)
		   .arg(Chann.StartedAt));
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
		query.clear();
		return false;
	}
	query.clear();
	db.commit();
	return true;
}

Channel DatabaseIO::GetChannel(QString Name)
{
	Channel Chann;
	QSqlQuery query;
	query.exec(QString("select * from channels where name='%1'").arg(Name));
	if(query.next())
	{
		Chann.BroadcasterId = query.value(1).toString();
		Chann.Followers = QString::number(query.value(2).toInt());
		Chann.Viewers = QString::number(query.value(3).toInt());
		Chann.isLive = query.value(4).toBool();
		Chann.isNotified = query.value(5).toBool();
		Chann.StartedAt = QString::number(query.value(6).toInt());
		Chann.ImageUrl = query.value(7).toString();
	}
	else
	{
		qDebug() << query.lastError();
	}
	query.clear();
	return Chann;
}

QList<Channel> DatabaseIO::GetAllChannels()
{
	QList<Channel> Channels;
	QSqlQuery query;
	query.exec("select * from channels");
	while(query.next())
	{
		Channel Chann;
		Chann.Name = query.value(0).toString();
		Chann.BroadcasterId = query.value(1).toString();
		Chann.Followers = QString::number(query.value(2).toInt());
		Chann.Viewers = QString::number(query.value(3).toInt());
		Chann.isLive = query.value(4).toBool();
		Chann.isNotified = query.value(5).toBool();
		Chann.StartedAt = QString::number(query.value(6).toInt());
		Chann.ImageUrl = query.value(7).toString();
		Channels.push_back(Chann);
	}
	query.clear();
	return Channels;
}

bool DatabaseIO::TurnOnNotify(QString Name)
{
	QSqlQuery query;
	query.exec(QString("update channels set is_notified='%1' where name='%2'").arg("true").arg(Name));
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
		query.clear();
		return false;
	}
	query.clear();
	db.commit();
	return true;
}

bool DatabaseIO::TurnOffNotify(QString Name)
{
	QSqlQuery query;
	query.exec(QString("update channels set is_notified='%1' where name='%2'").arg("false").arg(Name));
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
		query.clear();
		return false;
	}
	query.clear();
	db.commit();
	return true;
}

bool DatabaseIO::DeleteChannel(QString Name)
{
	QSqlQuery query;
	query.exec(QString("delete from channels where name='%1'").arg(Name));
	if(query.lastError().isValid())
	{
		qDebug() << query.lastError();
		query.clear();
		return false;
	}
	query.clear();
	db.commit();
	return true;
}
