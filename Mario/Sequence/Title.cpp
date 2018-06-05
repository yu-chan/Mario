#include "Common.h"
#include "DxLib.h"
#include "Image.h"
#include "InputInterface.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"

namespace Sequence {

Title::Title() : mImage(0) {
	mImage = new Image("data/image/title.png");
}

Title::~Title() {
	delete mImage;
	mImage = 0;
}

void Title::update(Parent* parent) {
	//スペースキーを押したら、準備画面に遷移する
	if(InputInterface::isOn(KEY_INPUT_SPACE)) {
		parent->moveToNextSequence(Parent::NEXT_GAME);
	}

	mImage->draw();
	DrawString(WINDOW_HALF_WIDTH - 75, 340, "Press Space Key", GetColor(255, 0, 0));
}

}