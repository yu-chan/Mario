#ifndef INCLUDED_SEQUENCE_GAME_CLEAR_H
#define INCLUDED_SEQUENCE_GAME_CLEAR_H

#include "Sequence/Game/Child.h"

class Image;

namespace Sequence {
namespace Game {

class Parent;

class Clear : public Child {
public:
	Clear();
	~Clear();
	void update(Parent*);

private:
	//クリア後、タイトル画面に遷移するまでの時間
	static const int TITLE_SEQUENCE_TIME = 120;

	Image* mImage;
	int mCnt;
};

}
}

#endif