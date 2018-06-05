#ifndef INCLUDED_SEQUENCE_GAME_READY_H
#define INCLUDED_SEQUENCE_GAME_READY_H

#include "Sequence/Game/Child.h"

class Image;

namespace Sequence {
namespace Game {

class Parent;

class Ready : public Child {
public:
	Ready();
	~Ready();
	void update(Parent*);

private:
	static const int PLAY_READY_TIME =  60;
	static const int PLAY_START_TIME = 120;

	int mCnt;
	bool mStarted;
};

}
}

#endif