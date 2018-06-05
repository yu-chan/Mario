#include "Common.h"
#include "Image.h"
#include "Game/StaticObject.h"


StaticObject::StaticObject() :
mFlag(0),
mX(0),
mY(0) {
}

//�}�b�v�`�b�v�̎�ނ��`�F�b�N
bool StaticObject::checkFlag(unsigned flag) const {
	return (mFlag & flag) ? true : false;
}

//��ނ��Z�b�g
void StaticObject::setFlag(unsigned flag) {
	mFlag |= flag;
}

//��ނ��Z�b�g������
void StaticObject::resetFlag(unsigned flag) {
	mFlag &= ~flag;
}

unsigned StaticObject::flag() const {
	return mFlag;
}

//�}�b�v�`�b�v��������
void StaticObject::set(int x, int y, unsigned flag) {
	setPosition(x, y);
	setFlag(flag);
}

//�}�b�v�`�b�v���X�N���[��
void  StaticObject::scroll(const int scrollX) {
	mX -= scrollX;
}

//�}�b�v�`�b�v�`��
void StaticObject::draw(const Image* image) const {
	//�摜�̍������ɕ`�悷��̂ŁA���S���獶��ɕϊ�
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
�L������u���b�N�̍�����W�𒆐S�ɕϊ�����B
�摜��`�悷��Ƃ��͍��㒸�_����Ƃ��Ă���
�������A�L������u���b�N�̓���������Փ˔��肷��ۂɂ́A
���W�����S�̂ق������₷��
*/
int StaticObject::convertPixelToInner(const int pos) const {
	return pos + OBJECT_HALF_SIZE;
}

/*
�L������u���b�N�̒��S���W������ɕϊ�����
�摜��`�悷��ہA������W����Ƃ��Ă���
���������āA�ϊ�����K�v������
*/
int StaticObject::convertInnerToPixel(const int pos) const {
	return pos - OBJECT_HALF_SIZE;
}