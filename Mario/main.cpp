#include <time.h>
#include "Common.h"
#include "DxLib.h"
#include "File.h"
#include "Framerate.h"
#include "InputInterface.h"
#include "SoundManager.h"
#include "Sequence/Parent.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if(DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	srand((unsigned)time(NULL));

	//フレームレート制御
	if(!Framerate::instance()) {
		Framerate::create();
	}
	if(!SoundManager::instance()) {
		SoundManager::create();
	}
	if(!Sequence::Parent::instance()) {
		Sequence::Parent::create();
	}

	while(!InputInterface::isOn(KEY_INPUT_Q)) { //Qを押したら終了
		if(ProcessMessage() != 0) {
			break;
		}
		InputInterface::updateKey();
		Framerate::instance()->update();
		ClearDrawScreen();

		//ゲーム開始
		Sequence::Parent::instance()->update();

		ScreenFlip();
		Framerate::instance()->wait();
	}

	Sequence::Parent::destroy();
	SoundManager::destroy();
	Framerate::destroy();
	DxLib_End();

	return 0;
}