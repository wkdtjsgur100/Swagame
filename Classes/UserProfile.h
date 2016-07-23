#pragma once

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

/*
	������ ������ �����͵��� �����մϴ�.
	Singleton Pattern�� �����
	�ٸ� Scene������ �����Ӱ� ������ �����ϵ��� �߽��ϴ�.
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
	������ ����ϴ� class
	Singleton ���� Ŭ����.
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