#include "DxLib.h"
#include "SoundManager.h"

SoundManager* SoundManager::mInstance = 0;

SoundManager::SoundManager() : mBgm(0), mSe(0) {}

SoundManager::~SoundManager() {}

void SoundManager::create() {
	if(!mInstance) {
		mInstance = new SoundManager();
	}
}

void SoundManager::destroy() {
	if(mInstance) {
		delete mInstance;
		mInstance = 0;
	}
}

SoundManager* SoundManager::instance() {
	return mInstance;
}

void SoundManager::loadBgm(const char* filename) {
	mBgm = LoadSoundMem(filename);
}

void SoundManager::playBgm() {
	PlaySoundMem(mBgm, DX_PLAYTYPE_LOOP);
}

void SoundManager::stopBgm() {
	StopSoundMem(mBgm);
}

void SoundManager::deleteBgm() {
	DeleteSoundMem(mBgm);
}

bool SoundManager::checkBgm() const {
	return (CheckSoundMem(mBgm) == 1) ? true : false;
}