#pragma once

#include "cocos2d.h"

/*
	�Ҹ� ���ڵ��� �����ִ� Ŭ����
*/
class SomaWordViewer : public cocos2d::Node
{
public:
	bool init() override;

	//value ���� ���� '��' �Ǵ� '��'�� ���ʴ�� ����Ѵ�.
	void printWords(const unsigned int value,int length);
	void hideAllWords();
	//�� ���� �Ѱ��� word�� visible�� �ٲ۴�.
	void showWord();

	CREATE_FUNC(SomaWordViewer);
};

class SomaWordManager : public cocos2d::Ref
{
	unsigned int currentQuestion;
	float wordShowTime;
	float correctWaitTime;
	int wordLength;
	int level;

	//���ᰳ��
	int numberOfCorrect;
public:
	unsigned int getCurrentQuestion() const;
	float getCurrentWordShowTime() const;
	float getCurrentWaitTime() const;
	float getCurrentLength() const;

	bool isCorrect(const int value);
	//�� �­���?
	bool isAllCorrect();

	void refresh();

	bool init();
	CREATE_FUNC(SomaWordManager);
};