#include "DxLib.h"
#include "Image.h"
#include "InputInterface.h"
#include "Sequence/Parent.h"
#include "Sequence/GameOver.h"

namespace Sequence {

GameOver::GameOver() : mImage(0), mCnt(0) {
	mImage = new Image("data/image/gameover.png");
}

GameOver::~GameOver() {
	delete mImage;
	mImage = 0;
}

void GameOver::update(Parent* parent) {
	//時間が立ったら、タイトルに遷移する
	if(mCnt >= TITLE_SEQUENCE_TIME) {
		parent->moveToNextSequence(Parent::NEXT_TITLE);
	}

	mImage->draw();

	mCnt++;
}

}