#pragma once

#include "cocos2d.h"

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