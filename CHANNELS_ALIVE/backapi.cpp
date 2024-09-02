#include "backapi.h"

BackAPI::BackAPI(QObject *parent)
	: QObject{parent}
{
	updateChannelsPageModel();
	updateMainPageModel();
}

bool BackAPI::addChannel(QString Name)
{
	Channel Chann;
	auto UserInfo = QJsonDocument::fromJson(API.GetUserInfo(Name));
	if(!UserInfo["data"][0]["login"].isUndefined())
	{
		Chann.Name = UserInfo["data"][0]["login"].toString();
		Chann.BroadcasterId = UserInfo["data"][0]["id"].toString();
		Chann.ImageUrl = UserInfo["data"][0]["profile_image_url"].toString();
		auto FollowersInfo = QJsonDocument::fromJson(API.GetUserFollowers(Chann.BroadcasterId));
		Chann.Followers = QString::number(FollowersInfo["total"].toDouble());
		auto StreamInfo = QJsonDocument::fromJson(API.GetCurrentStreamInfo(Name));
		if(!StreamInfo["data"][0]["viewer_count"].isUndefined())
		{
			Chann.isLive = true;
			Chann.Viewers = QString::number(StreamInfo["data"][0]["viewer_count"].toDouble());
			QDateTime StartedAt = QDateTime::fromString(StreamInfo["data"][0]["started_at"].toString(), Qt::DateFormat::ISODate);
			StartedAt = StartedAt.addSecs(10800);
			Chann.StartedAt = QString::number(StartedAt.toSecsSinceEpoch());
		}
		else
		{
			Chann.StartedAt = "0";
		}
		if(Database.AddChannel(Chann))
		{
			return true;
		}
	}
	return false;
}

bool BackAPI::deleteChannel(QString Name)
{
	return Database.DeleteChannel(Name);
}

void BackAPI::updateChannels()
{
	auto ChannelsList = Database.GetAllChannels();
	for(auto& Chann : ChannelsList)
	{
		auto FollowersInfo = QJsonDocument::fromJson(API.GetUserFollowers(Chann.BroadcasterId));
		Chann.Followers = QString::number(FollowersInfo["total"].toDouble());
		auto StreamInfo = QJsonDocument::fromJson(API.GetCurrentStreamInfo(Chann.Name));
		if(!StreamInfo["data"][0]["viewer_count"].isUndefined())
		{
			if(Chann.isLive == false and Chann.isNotified)
			{
				Notify.setNotification(Chann.Name + " запустил стрим!");
			}
			Chann.isLive = true;
			Chann.Viewers = QString::number(StreamInfo["data"][0]["viewer_count"].toDouble());
			QDateTime StartedAt = QDateTime::fromString(StreamInfo["data"][0]["started_at"].toString(), Qt::DateFormat::ISODate);
			StartedAt = StartedAt.addSecs(10800);
			Chann.StartedAt = QString::number(StartedAt.toSecsSinceEpoch());
		}
		else
		{
			Chann.isLive = false;
			Chann.Viewers = "0";
			Chann.StartedAt = "0";
		}
		Database.UpdateChannel(Chann);
	}
}

bool BackAPI::turnOnNotify(QString Name)
{
	return Database.TurnOnNotify(Name);
}

bool BackAPI::turnOffNotify(QString Name)
{
	return Database.TurnOffNotify(Name);
}

void BackAPI::updateChannelsPageModel()
{
	ChannelsModelNested.clear();
	auto ChannelsList = Database.GetAllChannels();
	for(auto& Chann : ChannelsList)
	{
		QVariantMap ChannelsMap;
		ChannelsMap.insert("name", Chann.Name);
		ChannelsMap.insert("image", Chann.ImageUrl);
		ChannelsMap.insert("subscribers", Chann.Followers);
		ChannelsMap.insert("isNotified", Chann.isNotified);
		ChannelsModelNested.push_back(ChannelsMap);
	}
	emit channelsModelChanged();
}

void BackAPI::updateMainPageModel()
{
	MainPageModelNested.clear();
	auto ChannelsList = Database.GetAllChannels();
	for(auto& Chann : ChannelsList)
	{
		QVariantMap ChannelsMap;
		ChannelsMap.insert("name", Chann.Name);
		ChannelsMap.insert("image", Chann.ImageUrl);
		ChannelsMap.insert("isLive", Chann.isLive);
		ChannelsMap.insert("viewers", Chann.Viewers);
		ChannelsMap.insert("time", QDateTime::fromSecsSinceEpoch(Chann.StartedAt.toInt()).toString(Qt::DateFormat::TextDate));
		MainPageModelNested.push_back(ChannelsMap);
	}
	emit mainPageModelChanged();
}

QVariantList& BackAPI::channelsListModel()
{
	return ChannelsModelNested;
}

QVariantList& BackAPI::mainPageListModel()
{
	return MainPageModelNested;
}
