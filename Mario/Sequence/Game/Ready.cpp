#include "Common.h"
#include "DxLib.h"
#include "Image.h"
#include "InputInterface.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"

namespace Sequence {
namespace Game {

Ready::Ready() : mCnt(0), mStarted(false) {
}

Ready::~Ready() {}

void Ready::update(Parent* parent) {
	//�X�e�[�W����ǂݍ���
	if(!mStarted) {
		parent->startLoading();
		mStarted = true;
	}
	//�v���C���ԂɂȂ�����A�J�ڂ���
	if(mCnt >= PLAY_START_TIME) {
		parent->moveToNextSequence(Parent::NEXT_PLAY);
	}
	if(mCnt >= PLAY_READY_TIME) {
		DrawString(WINDOW_HALF_WIDTH - 20, WINDOW_HALF_HEIGHT, "Go!!", GetColor(255, 0, 0));
	} else {
		DrawString(WINDOW_HALF_WIDTH - 40, WINDOW_HALF_HEIGHT, "Ready...", GetColor(255, 0, 0));
	}

	mCnt++;
}

}
}