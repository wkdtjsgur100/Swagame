#pragma once

#include "cocos2d.h"

class StageComponent
{
public:
	int wordLength; //글자 길이
	float wordShowTime; //글자 보여주는 시간
	float correctWaitTime; //맞출 떄까지 기다리는 시간
};
/*
Stage를 관리하는 역할
*/

class StageManager
{
	std::vector<StageComponent> stageDatas;

	void parsingData(std::string target);
public:
	//레벨은 1레벨 부터 시작합니다.
	StageComponent getDataFromLevel(int level);
	void loadCsvDatas();
};