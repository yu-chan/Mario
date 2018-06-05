#include "DxLib.h"
#include "InputInterface.h"
#include "Game/Stage.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Play.h"

namespace Sequence {
namespace Game {

Play::Play() {
}

Play::~Play() {
}

void Play::update(Parent* parent) {
	Stage* stage = parent->stage();
	//クリアもしくはゲームオーバーなら、タイトルに遷移する
	if(stage->clear()) {
		parent->moveToNextSequence(Parent::NEXT_CLEAR);
		return;
	}
	if(stage->gameover()) {
		parent->moveToNextSequence(Parent::NEXT_GAMEOVER);
	}

	stage->update();
	stage->draw();
}

}
}