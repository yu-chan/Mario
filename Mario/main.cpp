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

	//�t���[�����[�g����
	if(!Framerate::instance()) {
		Framerate::create();
	}
	if(!SoundManager::instance()) {
		SoundManager::create();
	}
	if(!Sequence::Parent::instance()) {
		Sequence::Parent::create();
	}

	while(!InputInterface::isOn(KEY_INPUT_Q)) { //Q����������I��
		if(ProcessMessage() != 0) {
			break;
		}
		InputInterface::updateKey();
		Framerate::instance()->update();
		ClearDrawScreen();

		//�Q�[���J�n
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