#include "StageManager.h"

USING_NS_CC;

void StageManager::parsingData(std::string target)
{
	int s_pos = 0, e_pos = 0;

	std::string str;
	int cnt = 0;
	StageComponent component;

	for (size_t i = 0; i < target.size(); i++)
	{
		char chk_c = target.at(i);
		if (chk_c == ',')
		{
			switch (cnt % 3)
			{
			case 0:
				component.wordLength = atoi(str.c_str());
				break;
			case 1:
				component.wordShowTime = atof(str.c_str());
				break;
			case 2:
				component.correctWaitTime = atof(str.c_str());
				break;
			}
			cnt++;
			str = "";
		}
		else if (chk_c == '\n')
			stageDatas.push_back(component);
		else
			str += chk_c;
	}
}

void StageManager::loadCsvDatas()
{
	std::string getData = FileUtils::getInstance()->getStringFromFile("stage/stage.csv");

	if (getData.compare("") == 0)
	{
		log("empty data..");
		return;
	}
	parsingData(getData);
}
