#include "DxLib.h"
#include "SoundManager.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Game/Parent.h"

namespace Sequence {

Parent* Parent::mInstance = 0;

Parent::Parent() : mNextSequence(NEXT_NONE), mChild(0) {
	mChild = new Title();
}

Parent::~Parent() {
	deleteChild();
}

//�V���O���g������
void Parent::create() {
	if(!mInstance) {
		mInstance = new Parent();
	}
}

//�V���O���g���j��
void Parent::destroy() {
	if(mInstance) {
		delete mInstance;
		mInstance = 0;
	}
}

Parent* Parent::instance() {
	return mInstance;
}

void Parent::update() {
	mChild->update(this);

	switch(mNextSequence) {
		case NEXT_TITLE:
			deleteChild();
			mChild = new Title();
			break;
		case NEXT_GAMEOVER:
			deleteChild();
			mChild = new GameOver();
			break;
		case NEXT_ENDING:
			deleteChild();
			mChild = new Ending();
			break;
		case NEXT_GAME:
			deleteChild();
			mChild = new Game::Parent();
			break;
	}

	//���������Ă����Ȃ��ƁA���֐i�߂Ȃ�
	mNextSequence = NEXT_NONE;
}

//��ʑJ�ڂ��X�V
void Parent::moveToNextSequence(NextSequence next) {
	mNextSequence = next;
}

//��ʑJ�ڂ���ۂɁA���̉�ʂ�j������
void Parent::deleteChild() {
	delete mChild;
	mChild = 0;
}

}