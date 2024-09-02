#include "twitchapi.h"

TwitchAPI::TwitchAPI(QObject *parent)
	: QObject{parent}
{
	SSLConfig.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
	SSLConfig.setProtocol(QSsl::TlsV1_2);
	SSLConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	Request.setSslConfiguration(SSLConfig);
	Request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, true);
	Manager = new QNetworkAccessManager(this);
	//LoadClient();
	LoadData();
}

QByteArray TwitchAPI::MakeRequest(QString Url)
{
	if(QDateTime::currentDateTime() >= ExpiresAt)
	{
		if(QDateTime::currentDateTime() < ExpiresAt.addDays(30))
		{
			RefreshToken();
		}
		else
		{
			QFile File("data.dat");
			File.remove();
			LoadData();
		}
	}
	Request.setUrl(QUrl(Url));
	Request.setRawHeader("Client-Id", ClientId.toLocal8Bit());
	Request.setRawHeader("Authorization", QString("Bearer %1").arg(AppToken).toLocal8Bit());
	QNetworkReply* Reply = Manager->get(Request);
	QEventLoop WaitResponse;
	connect(Reply, SIGNAL(finished()), &WaitResponse, SLOT(quit()));
	WaitResponse.exec();
	QByteArray ByteResponse = HandleResponse(Reply);
	Reply->deleteLater();
	return ByteResponse;
}

QByteArray TwitchAPI::HandleResponse(QNetworkReply* Reply)
{
	QByteArray ByteResponse = "";
	if(!Reply->error())
	{
		QByteArray Bytes = Reply->readAll();
		int StatusCode = Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if(StatusCode == 200)
		{
			ByteResponse = Bytes;
		}
		else
		{
			qDebug() << "\nNETWORK ERROR CODE : " << Reply->errorString() << " : " << Bytes;
		}
	}
	else
	{
		qDebug() << "\nNETWORK ERROR : " << Reply->errorString() << " : " << Reply->readAll();
	}
	Reply->deleteLater();
	return ByteResponse;
}

void TwitchAPI::LoadData()
{
	QByteArray Data = ReadData();
	if(!Data.isEmpty())
	{
		auto Closed = Data.indexOf("}");
		Data = Data.remove(Closed+1, (Data.size()-Closed-1));
		QJsonParseError Error;
		QJsonDocument JsonData = QJsonDocument::fromJson(Data, &Error);
		//qDebug() << Error.errorString();
		AppToken = JsonData["app_token"].toString();
		RefreshAppToken = JsonData["refresh_token"].toString();
		ExpiresAt = QDateTime::fromString(JsonData["expires_at"].toString());
		//QFile File("data.dat");
		//File.remove();
	}
	else
	{
		// Запускаем сервер для получения ответа от разрешения пользователя
		Server = new TokenRecieveServer(this);
		connect(Server, SIGNAL(onRecieve(QByteArray)), this, SLOT(onRecieve(QByteArray)));
		QString Url = QString("https://id.twitch.tv/oauth2/authorize?response_type=code&client_id=%1&redirect_uri=http://localhost:53354/authorize&scope=&force_verify=true").arg(ClientId);
		QEventLoop WaitCodeFill;
		connect(this, SIGNAL(onCodeChange()), &WaitCodeFill, SLOT(quit()));
		while(Code.isEmpty())
		{
			QDesktopServices::openUrl(Url);
			WaitCodeFill.exec();
		}

		// POST для получения токена приложения
		Request.setUrl(QUrl("https://id.twitch.tv/oauth2/token"));
		QByteArray UrlencodedData = QString("client_id=%1&client_secret=%2&code=%3&grant_type=authorization_code&redirect_uri=http://localhost:53354/authorize").
				arg(ClientId).arg(ClientSecret).arg(Code).toLocal8Bit();
		Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		Request.setHeader(QNetworkRequest::ContentLengthHeader, UrlencodedData.length());
		QNetworkReply* Reply = Manager->post(Request, UrlencodedData);
		QEventLoop WaitResponse;
		connect(Reply, SIGNAL(finished()), &WaitResponse, SLOT(quit()));
		WaitResponse.exec();
		QByteArray ReplyData = Reply->readAll();
		Reply->deleteLater();

		// Парсинг JSON ответа от POST и сохранение в файл в зашифрованном виде
		QJsonDocument Json = QJsonDocument::fromJson(ReplyData);
		if(!Json["access_token"].isNull())
		{
			AppToken = Json["access_token"].toString();
			RefreshAppToken = Json["refresh_token"].toString();
			QDateTime CurrentTime = QDateTime::currentDateTime();
			ExpiresAt = CurrentTime.addSecs(Json["expires_in"].toInt());
			QByteArray Data = QString("{\"app_token\":\"%1\",\"expires_at\":\"%2\",\"refresh_token\":\"%3\"}")
					.arg(AppToken).arg(ExpiresAt.toString()).arg(RefreshAppToken).toLocal8Bit();
			WriteData(Data);
		}
	}
}

void TwitchAPI::onRecieve(QByteArray ReplyContent)
{
	QString Content = QString::fromUtf8(ReplyContent);
	QRegularExpression GetRegex("GET \\/authorize\\?code=(?<GetContent>[\\d\\w=?]{1,})");
	if(Content.contains(GetRegex))
	{
		Code = GetRegex.match(Content).captured("GetContent");
		emit onCodeChange();
	}
}

void TwitchAPI::WriteData(QByteArray Data)
{
	QAESEncryption Encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
	QByteArray EncryptedData = Encryption.encode(Data, QCryptographicHash::hash(QHostInfo::localHostName().toLocal8Bit(), QCryptographicHash::Sha256));
	QFile File("data.dat");
	File.remove();
	if(File.open(QIODevice::WriteOnly))
	{
		File.write(EncryptedData);
		File.close();
	}
}

QByteArray TwitchAPI::ReadData()
{
	QByteArray DecryptedData = "";
	QFile File("data.dat");
	if(File.open(QIODevice::ReadOnly))
	{
		QByteArray Data = File.readAll();
		if(!Data.isEmpty())
		{
			QAESEncryption Encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
			DecryptedData = Encryption.decode(Data, QCryptographicHash::hash(QHostInfo::localHostName().toLocal8Bit(), QCryptographicHash::Sha256));
		}
		File.close();
	}
	return DecryptedData;
}

void TwitchAPI::LoadClient()
{
	QFile File("client.dat");
	if(File.open(QIODevice::ReadOnly))
	{
		QByteArray Data = File.readAll();
		File.close();
		if(!Data.isEmpty())
		{
			QAESEncryption Encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
			Data = Encryption.decode(Data, QCryptographicHash::hash(Original.toLocal8Bit(), QCryptographicHash::Sha256));
			QJsonDocument JsonData = QJsonDocument::fromJson(Data);
			ClientId = JsonData["client_id"].toString();
			ClientSecret = JsonData["client_secret"].toString();
		}
	}
}

QDateTime TwitchAPI::GetExpiresAt()
{
	return ExpiresAt;
}

void TwitchAPI::RefreshToken()
{
	Request.setUrl(QUrl("https://id.twitch.tv/oauth2/token"));
	QByteArray UrlencodedData = QString("client_id=%1&client_secret=%2&code=%3&grant_type=refresh_token&refresh_token=%4").
			arg(ClientId).arg(ClientSecret).arg(Code).arg(RefreshAppToken).toLocal8Bit();
	Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	Request.setHeader(QNetworkRequest::ContentLengthHeader, UrlencodedData.length());
	QNetworkReply* Reply = Manager->post(Request, UrlencodedData);
	QEventLoop WaitResponse;
	connect(Reply, SIGNAL(finished()), &WaitResponse, SLOT(quit()));
	WaitResponse.exec();
	QByteArray ReplyData = Reply->readAll();
	Reply->deleteLater();

	// Парсинг JSON ответа от POST и сохранение в файл в зашифрованном виде
	QJsonDocument Json = QJsonDocument::fromJson(ReplyData);
	AppToken = Json["access_token"].toString();
	RefreshAppToken = Json["refresh_token"].toString();
	QDateTime CurrentTime = QDateTime::currentDateTime();
	ExpiresAt = CurrentTime.addSecs(5000);
	QByteArray Data = QString("{\"app_token\":\"%1\",\"expires_at\":\"%2\",\"refresh_token\":\"%3\"}")
			.arg(AppToken).arg(ExpiresAt.toString()).arg(RefreshAppToken).toLocal8Bit();
	WriteData(Data);
}

QByteArray TwitchAPI::GetUserInfo(QString Username)
{
	return MakeRequest(QString("https://api.twitch.tv/helix/users?login=%1").arg(Username));
}

QByteArray TwitchAPI::GetUserFollowers(QString BroadcasterId)
{
	return MakeRequest(QString("https://api.twitch.tv/helix/channels/followers?broadcaster_id=%1").arg(BroadcasterId));
}

QByteArray TwitchAPI::GetCurrentStreamInfo(QString Username)
{
	return MakeRequest(QString("https://api.twitch.tv/helix/streams?user_login=%1").arg(Username));
}
