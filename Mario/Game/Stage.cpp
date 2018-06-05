#include "Common.h"
#include "Image.h"
#include "Game/Character.h"
#include "Game/Stage.h"
#include "Game/StaticObject.h"
#include <Windows.h>

Stage::Stage(const char* stageData, const int size) :
mImage(0),
mBackground(0),
mCharacters(0),
mCharactersNumber(0),
mWidth(0),
mHeight(0),
mStageWidth(0),
mOffset(0),
mSize(size),
mData(0),
isClear(false),
isGameover(false) {
	//ステージ情報
	mData = new char[mSize];
	for(int i = 0; i < mSize; i++) {
		mData[i] = stageData[i];
	}

	mImage = new Image("data/image/object.png");
	mBackground = new Image("data/image/back.png");

	reset();
}

Stage::~Stage() {
	delete mImage;
	mImage = 0;
	delete mBackground;
	mBackground = 0;
	delete[] mData;
	mData = 0;
}

//キャラやブロックの初期化
void Stage::reset() {
	setSize(mData, mSize);
	initStageWidth();
	mStaticObjects.setSize(mWidth, mHeight);

	//ブロックの初期化
	int x = 0, y = 0;
	for(int i = 0; i < mSize; i++) {
		switch(mData[i]) {
			case '0':
				mStaticObjects(x, y).setFlag(0);
				x++;
				break;
			case '1':
				mStaticObjects(x, y).set(x * OBJECT_SIZE, y * OBJECT_SIZE, StaticObject::FLAG_BRICK);
				x++;
				break;
			case '2':
				mStaticObjects(x, y).set(x * OBJECT_SIZE, y * OBJECT_SIZE, StaticObject::FLAG_DYNAMIC_BRICK);
				x++;
				break;
			case '3':
				mStaticObjects(x, y).set(x * OBJECT_SIZE, y * OBJECT_SIZE, StaticObject::FLAG_CLEAR_BRICK);
				x++;
				break;
			case '4':
				mStaticObjects(x, y).set(x * OBJECT_SIZE, y * OBJECT_SIZE, StaticObject::FLAG_FLAG);
				x++;
				break;
			case 'P':
			case 'E':
				mCharactersNumber++;
				x++;
				break;
			case '\n':
				x = 0;
				y++;
				break;
			default:
				break;
		}
	}

	//キャラの初期化
	if(mCharacters) {
		delete[] mCharacters;
		mCharacters = 0;
	}
	mCharacters = new Character[mCharactersNumber];
	
	int j = 1;
	x = y = 0;
	for(int i = 0; i < mSize; i++) {
		switch(mData[i]) {
			case 'P':
				mCharacters[0].set(x * OBJECT_SIZE, y * OBJECT_SIZE, Character::CHARACTERTYPE_PLAYER);
				x++;
				break;
			case 'E':
				mCharacters[j].set(x * OBJECT_SIZE, y * OBJECT_SIZE, Character::CHARACTERTYPE_ENEMY);
				x++;
				j++;
				break;
			case '\n':
				x = 0;
				y++;
				break;
			default:
				x++;
				break;
		}
	}
}

/*
読み込んだステージ情報の幅と高さを求める
stageData : ステージデータ
size      : データサイズ
*/
void Stage::setSize(const char* stageData, const int size) {
	mWidth = mHeight = 0;
	int x = 0, y = 0;
	for(int i = 0; i < size; i++) {
		switch(stageData[i]) {
			case '0':
			case '1':
			case '2':
			case '3':
				x++;
				break;
			case '\n':
				y++;
				mWidth = (mWidth < x) ? x : mWidth;
				mHeight++;
				x = 0;
				break;
			default:
				break;
		}
	}
}

//ステージの幅を初期化
void Stage::initStageWidth() {
	mStageWidth = mWidth * OBJECT_SIZE;
}

//ステージの更新
void Stage::update() {
	//プレイヤーが死んでるなら、処理しない
	if(mCharacters[0].isDead()) {
		isGameover = true;
		return;
	}
	//クリアしているなら、処理しない
	if(mCharacters[0].clear()) {
		isClear = true;
		return;
	}
	int preX, preY; //プレイヤーが動く前の位置
	mCharacters[0].getPosition(&preX, &preY);

	//キャラの動きを更新
	for(int i = 0; i < mCharactersNumber; i++) {
		mCharacters[i].update(mStaticObjects, mCharacters, mCharactersNumber, mOffset);
	}

	//マップスクロール
	int curX, curY; //プレイヤーが動いた後の位置
	mCharacters[0].getPosition(&curX, &curY);
	int scrollX = curX - preX;
	//動いたら、スクロール
	if(scrollX != 0) {
		stageScroll(scrollX, curX);
	}
}

//オフセットを用いて、ステージをスクロールする
void Stage::stageScroll(const int scrollX, const int curX) {
	if(WINDOW_HALF_WIDTH <= mOffset + curX && mOffset + curX <= mStageWidth - WINDOW_HALF_WIDTH) {
		//動かない物体をスクロール
		for(int y = 0; y < mHeight; y++) {
			for(int x = 0; x < mWidth; x++) {
				mStaticObjects(x, y).scroll(scrollX);
			}
		}

		//キャラをスクロール
		for(int i = 0; i < mCharactersNumber; i++) {
			switch(i) {
				case 0:
					//プレイヤーは常にスクリーン中央
					mCharacters[i].setPosition(WINDOW_HALF_WIDTH);
					break;
				default:
					mCharacters[i].scroll(scrollX);
					break;
			}
		}

		//スクロール量を更新
		mOffset += scrollX;
	}
}

//ステージを描画
void Stage::draw() const {
	//背景
	mBackground->draw();
	
	//動かない物体
	for(int y = 0; y < mHeight; y++) {
		for(int x = 0; x < mWidth; x++) {
			mStaticObjects(x, y).draw(mImage);
		}
	}

	//キャラ
	for(int i = 0; i < mCharactersNumber; i++) {
		mCharacters[i].draw(mImage);
	}
}

bool Stage::clear() {
	return isClear;
}

bool Stage::gameover() {
	return isGameover;
}