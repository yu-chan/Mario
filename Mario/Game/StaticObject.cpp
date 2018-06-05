#include "Common.h"
#include "Image.h"
#include "Game/StaticObject.h"


StaticObject::StaticObject() :
mFlag(0),
mX(0),
mY(0) {
}

//マップチップの種類をチェック
bool StaticObject::checkFlag(unsigned flag) const {
	return (mFlag & flag) ? true : false;
}

//種類をセット
void StaticObject::setFlag(unsigned flag) {
	mFlag |= flag;
}

//種類をセットし直す
void StaticObject::resetFlag(unsigned flag) {
	mFlag &= ~flag;
}

unsigned StaticObject::flag() const {
	return mFlag;
}

//マップチップを初期化
void StaticObject::set(int x, int y, unsigned flag) {
	setPosition(x, y);
	setFlag(flag);
}

//マップチップをスクロール
void  StaticObject::scroll(const int scrollX) {
	mX -= scrollX;
}

//マップチップ描画
void StaticObject::draw(const Image* image) const {
	//画像の左上を基準に描画するので、中心から左上に変換
	int detX = convertInnerToPixel(mX);
	int detY = convertInnerToPixel(mY);

	int srcX, srcY;
	srcX = srcY = -1;
	switch(mFlag) {
		case FLAG_BRICK:
			srcX = 0;
			srcY = 4;
			break;
		case FLAG_DYNAMIC_BRICK:
			break;
		case FLAG_CLEAR_BRICK:
			break;
		case FLAG_FLAG:
			srcX = 3;
			srcY = 4;
			break;
	}

	if(srcX != -1 && srcY != -1 && mFlag != FLAG_NONE) {		
		if(!(detX + OBJECT_SIZE < 0 || WINDOW_WIDTH < detX)) {
			image->draw(detX, detY, srcX, srcY, OBJECT_SIZE, OBJECT_SIZE);
		}
	}
}

void StaticObject::setPosition(int x, int y) {
	mX = convertPixelToInner(x);
	mY = convertPixelToInner(y);
}

void StaticObject::getPosition(int* x, int* y) {
	*x = mX;
	*y = mY;
}

/*
キャラやブロックの左上座標を中心に変換する。
画像を描画するときは左上頂点を基準としている
しかし、キャラやブロックの動かしたり衝突判定する際には、
座標が中心のほうがやりやすい
*/
int StaticObject::convertPixelToInner(const int pos) const {
	return pos + OBJECT_HALF_SIZE;
}

/*
キャラやブロックの中心座標を左上に変換する
画像を描画する際、左上座標を基準としている
したがって、変換する必要がある
*/
int StaticObject::convertInnerToPixel(const int pos) const {
	return pos - OBJECT_HALF_SIZE;
}