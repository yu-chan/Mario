#include "DxLib.h"
#include "Image.h"
#include "InputInterface.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Clear.h"

namespace Sequence {
namespace Game {

Clear::Clear() : mImage(0), mCnt(0) {
	mImage = new Image("data/image/gameclear.png");
}

Clear::~Clear() {
	delete mImage;
	mImage = 0;
}

void Clear::update(Parent* parent) {
	//�^�C�g���J�ڎ��ԂɂȂ����̂ŁA�J�ڂ���
	if(mCnt >= TITLE_SEQUENCE_TIME) {
		parent->moveToNextSequence(Parent::NEXT_TITLE);
	}

	mImage->draw();

	mCnt++;
}

}
}