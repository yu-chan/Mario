#include "DxLib.h"
#include "Image.h"

Image::Image(const char* filename) : mWidth(0), mHeigt(0), mData(0) {
	mData = LoadGraph(filename);
	GetGraphSize(mData, &mWidth, &mHeigt);
}

Image::~Image() {
	DeleteGraph(mData);
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeigt;
}

//画像を丸ごと表示したいときに使う
void Image::draw() const {
	draw(0, 0, 0, 0, mWidth, mHeigt);
}

/*
data/image/object.pngのように、絵を分割表示したいときに使う
 detX,   detY : 描画座標
 srcX         : 画像をOBJECT_SIZE(Common.hを参照)で割ったとき、左上から右に向かって、0から数えたときの番号
 srcY         : srcX同様、左上から下に数えたときの番号
width, height : srcX、srcYから表示したい絵の幅と高さ
*/
void Image::draw(int detX, int detY, int srcX, int srcY, int width, int height) const {
	DrawRectGraph(detX, detY, srcX * width, srcY * height, width, height, mData, TRUE, FALSE);
}