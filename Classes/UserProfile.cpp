#include "UserProfile.h"

USING_NS_CC;

UserProfile* UserProfile::instance = nullptr;

UserProfile * UserProfile::getInstance()
{
	if (instance == nullptr)
		instance = new (std::nothrow)UserProfile();

	return instance;
}
