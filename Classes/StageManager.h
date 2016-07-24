#pragma once

#include "cocos2d.h"

class StageComponent
{
public:
	int wordLength; //���� ����
	float wordShowTime; //���� �����ִ� �ð�
	float correctWaitTime; //���� ������ ��ٸ��� �ð�
};
/*
Stage�� �����ϴ� ����
*/

class StageManager
{
	std::vector<StageComponent> stageDatas;

	void parsingData(std::string target);
public:
	//������ 1���� ���� �����մϴ�.
	StageComponent getDataFromLevel(int level);
	void loadCsvDatas();
};