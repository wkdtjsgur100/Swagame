#pragma once

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

/*
	유저의 프로필 데이터들을 관리합니다.
	Singleton Pattern을 사용해
	다른 Scene에서도 자유롭게 접근이 가능하도록 했습니다.
*/
class UserProfile
{
	static UserProfile* instance;

	UserProfile();
	~UserProfile();
public:
	static UserProfile* getInstance();
	void end();
	bool isLoggedIn() const;

	std::string getUserNickName() const;
};

/*
	서버와 통신하는 class
	Singleton 패턴 클래스.
*/
class ServerCommunicator
{
	static ServerCommunicator* instance;

	ServerCommunicator();
	~ServerCommunicator();
public:
	static ServerCommunicator* getInstance();
	void end();

	void registerUser(std::string id, std::string nick_name);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};