#pragma once

#include "cocos2d.h"

class UserProfile
{
	static UserProfile* instance;
public:
	static UserProfile* getInstance();
};