#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"

class Stage;

namespace Sequence {

class Parent;

namespace Game {

class Child;

class Parent : public Sequence::Child {
public:
	typedef Sequence::Parent GrandParent;

	Parent();
	~Parent();

	void update(GrandParent*);

	//‘JˆÚ‰æ–Ê‚ÌŽí—Þ
	enum NextSequence {
		NEXT_TITLE,
		NEXT_PLAY,
		NEXT_GAMEOVER,
		NEXT_ENDING,

		NEXT_READY,
		NEXT_CLEAR,
		NEXT_PAUSE,

		NEXT_NONE,
	};
	void moveToNextSequence(NextSequence);

	void draw() const;

	bool isCleared() const;

	void startLoading();

	void deleteChild();

	Stage* stage();

private:
	NextSequence mNextSequence;
	Game::Child* mChild;
	Stage* mStage;
};

}
}

#endif