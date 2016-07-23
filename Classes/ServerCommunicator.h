#pragma once

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

/*
	서버와 통신하는 class
	Singleton 패턴 클래스.
*/
class ServerCommunicator
{
	static ServerCommunicator* instance;

	ServerCommunicator();
	~ServerCommunicator();

	void sendDataToServer(std::string data, std::string tag, cocos2d::network::HttpRequest::Type type);
public:
	static ServerCommunicator* getInstance();
	void end();

	void registerUser(std::string id, std::string nick_name);
	void userScoreUpdate(std::string userScore);
	void userConnectTimeUpdate();
	void shutgameTimeUpdate();
	
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};