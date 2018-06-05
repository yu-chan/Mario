#include "DxLib.h"
#include "File.h"
#include "Game/Stage.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Pause.h"

namespace Sequence {
namespace Game {

Parent::Parent() : mNextSequence(NEXT_NONE), mChild(0) {
	mChild = new Ready();
}

Parent::~Parent() {
	delete mChild;
	mChild = 0;
}

void Parent::update(GrandParent* parent) {
	//���݂̉�ʂ�����
	mChild->update(this);

	//��ʑJ�ڎ�ނɂ��A�������ς��
	switch(mNextSequence) {
		case NEXT_TITLE:
			deleteChild();
			parent->moveToNextSequence(GrandParent::NEXT_TITLE);
			break;
		case NEXT_PLAY:
			deleteChild();
			mChild = new Play();
			break;
		case NEXT_GAMEOVER:
			deleteChild();
			parent->moveToNextSequence(GrandParent::NEXT_GAMEOVER);
			break;
		case NEXT_ENDING:
			deleteChild();
			parent->moveToNextSequence(GrandParent::NEXT_ENDING);
			break;
		case NEXT_READY:
			deleteChild();
			mChild = new Ready();
		case NEXT_CLEAR:
			deleteChild();
			mChild = new Clear();
			break;
		case NEXT_PAUSE:
			deleteChild();
			mChild = new Pause();
			break;
	}
	
	//���������Ă����Ȃ��ƁA���֐i�߂Ȃ�
	mNextSequence = NEXT_NONE;
}

//��ʑJ�ڂ��X�V
void Parent::moveToNextSequence(NextSequence next) {
	mNextSequence = next;
}

void Parent::draw() const {
	mStage->draw();
}

bool Parent::isCleared() const {
	return true;
}

//�X�e�[�W����ǂݍ���
void Parent::startLoading() {
	delete mStage;
	mStage = 0;
	File f("data/stage/stageData.txt");
	mStage = new Stage(f.data(), f.size());
}

void Parent::deleteChild() {
	delete mChild;
	mChild = 0;
}

Stage* Parent::stage() {
	return mStage;
}

}
}