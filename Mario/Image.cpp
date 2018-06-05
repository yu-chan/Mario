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

//�摜���ۂ��ƕ\���������Ƃ��Ɏg��
void Image::draw() const {
	draw(0, 0, 0, 0, mWidth, mHeigt);
}

/*
data/image/object.png�̂悤�ɁA�G�𕪊��\���������Ƃ��Ɏg��
 detX,   detY : �`����W
 srcX         : �摜��OBJECT_SIZE(Common.h���Q��)�Ŋ������Ƃ��A���ォ��E�Ɍ������āA0���琔�����Ƃ��̔ԍ�
 srcY         : srcX���l�A���ォ�牺�ɐ������Ƃ��̔ԍ�
width, height : srcX�AsrcY����\���������G�̕��ƍ���
*/
void Image::draw(int detX, int detY, int srcX, int srcY, int width, int height) const {
	DrawRectGraph(detX, detY, srcX * width, srcY * height, width, height, mData, TRUE, FALSE);
}