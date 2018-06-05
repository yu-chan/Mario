#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence {

class Child;

class Parent {
public:
	static void create();
	static void destroy();
	static Parent* instance();

	void update();

	enum NextSequence {
		NEXT_TITLE,
		NEXT_GAMEOVER,
		NEXT_ENDING,
		NEXT_GAME,

		NEXT_NONE
	};
	void moveToNextSequence(NextSequence);
	
	void deleteChild();

private:
	Parent();
	~Parent();
	static Parent* mInstance;

	NextSequence mNextSequence;
	Child* mChild;
};

}

#endif