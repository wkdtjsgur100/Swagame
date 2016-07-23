#include "ServerCommunicator.h"

USING_NS_CC;
using namespace cocos2d::network;

ServerCommunicator* ServerCommunicator::instance = nullptr;

ServerCommunicator::ServerCommunicator()
{
}

ServerCommunicator::~ServerCommunicator()
{
}

void ServerCommunicator::sendDataToServer(std::string data, std::string tag, HttpRequest::Type type)
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl("http://52.78.94.176:8000/user/");
	request->setRequestType(type);
	request->setResponseCallback(CC_CALLBACK_2(ServerCommunicator::onHttpRequestCompleted, this));

	request->setRequestData(data.c_str(), strlen(data.c_str()));

	request->setTag(tag);
	HttpClient::getInstance()->send(request);

	request->release();
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
	const char* data = String::createWithFormat("user_id=%s&user_nickname=%s", id.c_str(), nick_name.c_str())->getCString();
	sendDataToServer(data, "register user", HttpRequest::Type::POST);
}

void ServerCommunicator::userScoreUpdate(std::string userScore)
{
	std::string id = UserDefault::getInstance()->getStringForKey("user_id", "");

	const char* data = String::createWithFormat("user_id=%s&user_score=%s&update_type=1", id.c_str(),userScore.c_str())->getCString();
	sendDataToServer(data, "user_score update", HttpRequest::Type::PUT);
}

void ServerCommunicator::userConnectTimeUpdate()
{
	std::string id = UserDefault::getInstance()->getStringForKey("user_id", "0");
	
	const char* data = String::createWithFormat("user_id=%s&update_type=2", id.c_str())->getCString();
	sendDataToServer(data, "connect time update", HttpRequest::Type::PUT);
}

void ServerCommunicator::shutgameTimeUpdate()
{
	std::string id = UserDefault::getInstance()->getStringForKey("user_id", "0");

	const char* data = String::createWithFormat("user_id=%s&update_type=3", id.c_str())->getCString();
	sendDataToServer(data, "shutgame time update", HttpRequest::Type::PUT);
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
		cocos2d::log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	
	cocos2d::log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	cocos2d::log("Http Test, dump data: ");

	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		cocos2d::log("%c", (*buffer)[i]);
	}
	cocos2d::log("\n");
	if (response->getHttpRequest()->getReferenceCount() != 2)
	{
		cocos2d::log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
	}
}
