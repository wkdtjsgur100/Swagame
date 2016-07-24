#include "StageManager.h"

USING_NS_CC;

void StageManager::parsingData(std::string target)
{
	int s_pos = 0, e_pos = 0;

	std::string str;
	int cnt = 0;
	StageComponent component;
	size_t i;
	for (i = 0; target.at(i) != '\n'; i++);

	i += 1;
	for (; i < target.size(); i++)
	{
		char chk_c = target.at(i);
		if (chk_c == ',' || chk_c == '\n')
		{
			switch (cnt)
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
			if (cnt > 2)
				cnt = 0;

			if (chk_c == '\n')
				stageDatas.push_back(component);
			str = "";
		}
		else
			str += chk_c;
	}
}

StageComponent StageManager::getDataFromLevel(int level)
{
	if(level-1 < stageDatas.size())
		return stageDatas.at(level-1);
	else
	{
		//csv보다 레벨이 높으면 component를 정해진 공식대로 만들어서 반환
		StageComponent ret_sc;
		StageComponent last_stage = stageDatas.back();

		ret_sc.wordLength = last_stage.wordLength;
		ret_sc.correctWaitTime = last_stage.correctWaitTime;
		ret_sc.wordShowTime = last_stage.wordShowTime;

		return ret_sc;
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
