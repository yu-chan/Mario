#ifndef INCLUDED_GAME_STAGE_H
#define INCLUDED_GAME_STAGE_H

#include "Array2D.h"

class Image;
class Character;
class Enemy;
class Player;
class StaticObject;

class Stage {
public:
	Stage(const char* stageData, const int size);
	~Stage();

	void reset();
	void setSize(const char* stageData, const int size);
	void initStageWidth();

	void update();
	void stageScroll(const int, const int);

	void draw() const;

	bool clear();
	bool gameover();
	
private:
	Array2D<StaticObject> mStaticObjects;
	
	Image* mImage;
	Image* mBackground;
	Character* mCharacters;
	int mCharactersNumber;
	int mWidth;
	int mHeight;
	int mStageWidth;
	int mOffset;
	int mSize;
	char* mData;
	bool isClear;
	bool isGameover;
};

#endif