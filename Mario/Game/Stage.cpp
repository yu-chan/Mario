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
	//�X�e�[�W���
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

//�L������u���b�N�̏�����
void Stage::reset() {
	setSize(mData, mSize);
	initStageWidth();
	mStaticObjects.setSize(mWidth, mHeight);

	//�u���b�N�̏�����
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

	//�L�����̏�����
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
�ǂݍ��񂾃X�e�[�W���̕��ƍ��������߂�
stageData : �X�e�[�W�f�[�^
size      : �f�[�^�T�C�Y
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

//�X�e�[�W�̕���������
void Stage::initStageWidth() {
	mStageWidth = mWidth * OBJECT_SIZE;
}

//�X�e�[�W�̍X�V
void Stage::update() {
	//�v���C���[������ł�Ȃ�A�������Ȃ�
	if(mCharacters[0].isDead()) {
		isGameover = true;
		return;
	}
	//�N���A���Ă���Ȃ�A�������Ȃ�
	if(mCharacters[0].clear()) {
		isClear = true;
		return;
	}
	int preX, preY; //�v���C���[�������O�̈ʒu
	mCharacters[0].getPosition(&preX, &preY);

	//�L�����̓������X�V
	for(int i = 0; i < mCharactersNumber; i++) {
		mCharacters[i].update(mStaticObjects, mCharacters, mCharactersNumber, mOffset);
	}

	//�}�b�v�X�N���[��
	int curX, curY; //�v���C���[����������̈ʒu
	mCharacters[0].getPosition(&curX, &curY);
	int scrollX = curX - preX;
	//��������A�X�N���[��
	if(scrollX != 0) {
		stageScroll(scrollX, curX);
	}
}

//�I�t�Z�b�g��p���āA�X�e�[�W���X�N���[������
void Stage::stageScroll(const int scrollX, const int curX) {
	if(WINDOW_HALF_WIDTH <= mOffset + curX && mOffset + curX <= mStageWidth - WINDOW_HALF_WIDTH) {
		//�����Ȃ����̂��X�N���[��
		for(int y = 0; y < mHeight; y++) {
			for(int x = 0; x < mWidth; x++) {
				mStaticObjects(x, y).scroll(scrollX);
			}
		}

		//�L�������X�N���[��
		for(int i = 0; i < mCharactersNumber; i++) {
			switch(i) {
				case 0:
					//�v���C���[�͏�ɃX�N���[������
					mCharacters[i].setPosition(WINDOW_HALF_WIDTH);
					break;
				default:
					mCharacters[i].scroll(scrollX);
					break;
			}
		}

		//�X�N���[���ʂ��X�V
		mOffset += scrollX;
	}
}

//�X�e�[�W��`��
void Stage::draw() const {
	//�w�i
	mBackground->draw();
	
	//�����Ȃ�����
	for(int y = 0; y < mHeight; y++) {
		for(int x = 0; x < mWidth; x++) {
			mStaticObjects(x, y).draw(mImage);
		}
	}

	//�L����
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