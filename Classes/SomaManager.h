#pragma once

#include "cocos2d.h"

/*
	¼Ò¸¶ ±ÛÀÚµéÀ» º¸¿©ÁÖ´Â Å¬·¡½º
*/
class SomaWordViewer : public cocos2d::Node
{
public:
	bool init() override;

	//value °ª¿¡ µû¶ó '¼Ò' ¶Ç´Â '¸¶'¸¦ Â÷·Ê´ë·Î Ãâ·ÂÇÑ´Ù.
	void printWords(const unsigned int value,int length);
	void hideAllWords();
	//¸Ç µÚÀÇ ÇÑ°³ÀÇ word¸¦ visible·Î ¹Ù²Û´Ù.
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

	//¸ÂÃá°³¼ö
	int numberOfCorrect;
public:
	unsigned int getCurrentQuestion() const;
	float getCurrentWordShowTime() const;
	float getCurrentWaitTime() const;
	float getCurrentLength() const;

	bool isCorrect(const int value);
	//´Ù ¸Â­Ÿ´Ï?
	bool isAllCorrect();

	void refresh();

	bool init();
	CREATE_FUNC(SomaWordManager);
};