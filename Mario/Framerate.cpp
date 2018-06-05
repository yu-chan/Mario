#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Framerate.h"

Framerate* Framerate::mInstance = 0;

Framerate::Framerate() : mStartTime(0), mCnt(0), mFramerate(0) {}

Framerate::~Framerate() {}

void Framerate::create() {
	if(!mInstance) {
		mInstance = new Framerate();
	}
}

void Framerate::destroy() {
	if(mInstance) {
		delete mInstance;
		mInstance = 0;
	}
}

Framerate* Framerate::instance() {
	return mInstance;
}

//フレームを更新
void Framerate::update() {
	if(mCnt == 0) {
		mStartTime = GetNowCount();
	}
	if(mCnt == INTERVAL) {
		int t = GetNowCount();
		mFramerate = 1000.0f / ((t - mStartTime) / (float)INTERVAL);
		mCnt = 0;
		mStartTime = t;
	}
	mCnt++;
}

//フレームが早かったら、早いぶんだけ待つ
void Framerate::wait() {
	int t = GetNowCount() - mStartTime;
	int w = mCnt * 1000 / FPS - t;
	if(w > 0) {
		Sleep(w);
	}
}

int Framerate::cnt() const {
	return mCnt;
}