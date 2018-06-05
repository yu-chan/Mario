#ifndef INCLUDED_SEQUENCE_GAMEOVER_H
#define INCLUDED_SEQUENCE_GAMEOVER_H

#include "Sequence/Child.h"

class Image;

namespace Sequence {

class Parent;

class GameOver : public Child {
public:
	GameOver();
	~GameOver();
	void update(Parent*);

private:
	static const int TITLE_SEQUENCE_TIME = 120;

	Image* mImage;
	int mCnt;
};

}

#endif