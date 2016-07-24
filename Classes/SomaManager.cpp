#include "SomaManager.h"

USING_NS_CC;

bool SomaWordViewer::init()
{
	if(!Node::init())
		return false;

	return true;
}

void SomaWordViewer::printWords(const unsigned int value,int length)
{
	unsigned int cpy_value = value;

	this->removeAllChildren();

	Vec2 start_pos = Vec2(50,630);

	for (int i = 0; i < 4;i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Sprite* newSpr;

			if ((cpy_value & 1) == 0)
				newSpr = Sprite::create("font_so.png");
			else
				newSpr = Sprite::create("font_ma.png");

			Size sprSize = newSpr->getContentSize();
			newSpr->setPosition(start_pos.x + j*sprSize.width, start_pos.y - i*sprSize.height);

			addChild(newSpr);
			cpy_value >>= 1; 
			
			length--;
			if (length == 0)
				goto outLoop;
		}
	}
outLoop:
	return;
}

void SomaWordViewer::hideAllWords()
{
	for (const auto& child : _children)
		child->setVisible(false);
}

void SomaWordViewer::showWord()
{
	for (const auto& child : _children)
	{
		if (!child->isVisible())
		{
			child->setVisible(true);
			break;
		}
	}
}

unsigned int SomaWordManager::getLevel() const
{
	return level;
}

unsigned int SomaWordManager::getCurrentQuestion() const
{
	return currentQuestion;
}

float SomaWordManager::getCurrentWordShowTime() const
{
	return wordShowTime;
}

float SomaWordManager::getCurrentWaitTime() const
{
	return correctWaitTime;
}

float SomaWordManager::getCurrentLength() const
{
	return wordLength;
}

bool SomaWordManager::isCorrect(const int value)
{
	if (((currentQuestion >> numberOfCorrect) & 1) == value)
	{
		numberOfCorrect++;
		return true;
	}
	else
		return false;
}

bool SomaWordManager::isAllCorrect()
{
	return numberOfCorrect >= wordLength;
}

void SomaWordManager::refresh()
{
	level++;

	StageComponent stageDatas = stageManager.getDataFromLevel(level);

	numberOfCorrect = 0;
	wordLength = stageDatas.wordLength;
	wordShowTime = stageDatas.wordShowTime;
	correctWaitTime = stageDatas.correctWaitTime;

	currentQuestion = random(0, (1 << wordLength+1) - 1);
}

bool SomaWordManager::init()
{
	currentQuestion = 2;
	wordShowTime = 2.0f;
	correctWaitTime = 20.0f;
	wordLength = 2;
	level = 0;
	numberOfCorrect = 0;

	stageManager.loadCsvDatas();

	return true;
}

