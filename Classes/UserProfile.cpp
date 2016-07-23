#include "UserProfile.h"

USING_NS_CC;
using namespace cocos2d::network;

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
ServerCommunicator* ServerCommunicator::instance = nullptr;

ServerCommunicator::ServerCommunicator()
{
}

ServerCommunicator::~ServerCommunicator()
{
}

ServerCommunicator * ServerCommunicator::getInstance()
{
	if (instance == nullptr)
		instance = new (std::nothrow)ServerCommunicator();

	return instance;
}

void ServerCommunicator::end()
{
	if(instance != nullptr)
		delete instance;
}

void ServerCommunicator::registerUser(std::string id, std::string nick_name)
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl("http://52.78.94.176:8000/user/");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(ServerCommunicator::onHttpRequestCompleted, this));

	// write the post data
	
	const char* postData = String::createWithFormat("user_id=%s&user_nickname=%s&csrfmiddlewaretoken=CSRF_TOKEN", id.c_str(), nick_name.c_str())->getCString();
	
	request->setRequestData(postData, strlen(postData));
	
	request->setTag("POST test1");
	HttpClient::getInstance()->send(request);

	request->release();
}

void ServerCommunicator::onHttpRequestCompleted(HttpClient * sender, HttpResponse * response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	
	log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	log("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		log("%c", (*buffer)[i]);
	}
	log("\n");
	if (response->getHttpRequest()->getReferenceCount() != 2)
	{
		log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
	}
}
