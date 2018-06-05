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
	//ƒ^ƒCƒgƒ‹‘JˆÚŽžŠÔ‚É‚È‚Á‚½‚Ì‚ÅA‘JˆÚ‚·‚é
	if(mCnt >= TITLE_SEQUENCE_TIME) {
		parent->moveToNextSequence(Parent::NEXT_TITLE);
	}

	mImage->draw();

	mCnt++;
}

}
}