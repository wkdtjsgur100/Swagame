#include "UserProfile.h"

USING_NS_CC;

UserProfile* UserProfile::instance = nullptr;

UserProfile::UserProfile()
{
}

UserProfile::~UserProfile()
{
}

UserProfile * UserProfile::getInstance()
{
	if (instance == nullptr)
		instance = new (std::nothrow)UserProfile();

	return instance;
}

void UserProfile::end()
{
	delete instance;
	instance = nullptr;
}

bool UserProfile::isLoggedIn() const
{
	return UserDefault::getInstance()->getStringForKey("user_id", "").compare("") != 0;
}

std::string UserProfile::getUserNickName() const
{
	return UserDefault::getInstance()->getStringForKey("user_nickname", "");
}
