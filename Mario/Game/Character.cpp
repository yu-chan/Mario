#include <Windows.h>
#include "Common.h"
#include "DxLib.h"
#include "Image.h"
#include "InputInterface.h"
#include "Game/StaticObject.h"
#include "Game/Character.h"


Character::Character() :
	mCharacterType(CHARACTERTYPE_NONE),
	mX(0),
	mY(0),
	mDetX(0),
	mDetY(GRAVITY),
	isDetRight(true),
	isJump(false),
	isClear(false),
	mCnt(0) {
}

//�L�����N�^�[�̍��W�A��ނ��Z�b�g����
void Character::set(int x, int y, CharacterType type) {
	mX             = convertPixelToInner(x);
	mY             = convertPixelToInner(y);
	mCharacterType = type;
	//��ނ��G�Ȃ�A���x�������_���Ɍ��߂�
	if(mCharacterType == CHARACTERTYPE_ENEMY) {
		setRandomDetX();
	}
}

//X�����ɑ΂��āA���x�������_���Ɍ��߂�
void Character::setRandomDetX() {
	double rnd = rand();
	if(rnd < 0.5) {
		mDetX = -1;
	} else {
		mDetX =  1;
	}
}

/*
���W�̍X�V
arrayStaticObject : �X�e�[�W�̃}�b�v�`�b�v
characters        : �L�����N�^�[
charactersNum     : �X�e�[�W�̃L������
offset            : �}�b�v�X�N���[��������
*/
void Character::update(Array2D<StaticObject>& arrayStaticObject, Character* characters, const int charactersNum, const int offset) {
	if(!isDead()) {
		if(isPlayer()) {
			playerWalk(arrayStaticObject, characters, charactersNum, offset);
			playerJump(arrayStaticObject, characters, charactersNum, offset);
		} else if(isEnemy()) {
			enemyWalk(arrayStaticObject, characters, charactersNum, offset);
		}
		count();
	}
}

//���W�擾
void Character::getPosition(
	int* x,
	int* y) {
		*x = mX;
		*y = mY;
}

/*
�}�b�v�X�N���[������ۂɎg��
�v���C���[����ɁA�����ɒu��
*/
void Character::setPosition(int x) {
	mX = x;
}

//�}�b�v�X�N���[�������Ƃ��A�X�N���[�������������ړ�������
void Character::scroll(const int scrollX) {
	mX -= scrollX;
}

/*
�v���C���[���������
arrayStaticObject : �X�e�[�W�̃}�b�v�`�b�v
characters        : �L�����N�^�[
charactersNum     : �X�e�[�W�̃L������
offset            : �}�b�v�X�N���[��������
*/
void Character::playerWalk(Array2D<StaticObject>& arrayStaticObject, const Character* characters, const int charactersNum, const int  offset) {
	int speed = 0;
	if(InputInterface::isOn(KEY_INPUT_RIGHT)) {       //�E���L�[�������Ă���
		speed = PLAYER_WALK_SPEED;
		isDetRight = true;
	} else if(InputInterface::isOn(KEY_INPUT_LEFT)) { //�����L�[�������Ă���
		speed = -PLAYER_WALK_SPEED;
		isDetRight = false;
	}
	mDetX = speed;

	//�L�����̎��͂̃}�X
	int mas[3][3][2] = {
		{{-1, -1}, {-1,  0}, {-1,  1}},
		{{ 0, -1}, { 0,  0}, { 0,  1}},
		{{ 1, -1}, { 1,  0}, { 1,  1}},
	};
	//�L����������}�X
	int masX = (offset + mX) / OBJECT_SIZE;
	int masY = mY / OBJECT_SIZE;
	//�ړ���̍��W
	int movedX = mX + mDetX;
	int movedY = mY + mDetY;
	//�ړ������ۂɃI�u�W�F�N�g�ɂ������Ă��邩�ǂ���
	bool hitX = false, hitY = false;
	//�u���b�N�Ȃǂ̏Փ˔���
	for(int y = 0; y < 3; y++) {
		for(int x = 0; x < 3; x++) {
			if(x == 1 && y == 1) { //�L����������}�X�Ȃ̂ŁA�������Ȃ�
				continue;
			}
			int tmpX = masX + mas[y][x][0];
			int tmpY = masY + mas[y][x][1];
			//X�����ɑ΂��āA���肷��
			if(collisionDetectionToStatic(movedX, mY, &arrayStaticObject(tmpX, tmpY))) {
				hitX = true;
				//�X�e�[�W�̊��ɐG�ꂽ��A�N���A
				if(arrayStaticObject(tmpX, tmpY).checkFlag(StaticObject::FLAG_FLAG)) {
					isClear = true;
				}
			}
			//Y�����ɑ΂��āA���肷��
			if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(tmpX, tmpY))) {
				hitY = true;
			}
		}
	}
	//�L�������m�̏Փ˔���
	for(int i = 0; i < charactersNum; i++) {
		if(this == &characters[i]) {
			continue;
		}
		//�v���C���[���G�ɓ�����Ǝ���
		if(collisionDetectionToCharacter(movedX,     mY, characters[i])) {
			dieCharacter();
			hitX = true;
		}
	}
	//�����Ă���r���ɁA���ɒi�����Ȃ��Ȃ�A�󒆂ɂ���
	//��������Ȃ��ƁA�󒆂ŃW�����v�ł��Ă��܂�
	if(!hitY) {
		isJump = true;
	}

	//�v���C���[�͏�ɃX�N���[���ɂ���K�v������
	if((OBJECT_HALF_SIZE < movedX && movedX < WINDOW_WIDTH  - OBJECT_HALF_SIZE) && 
		(OBJECT_HALF_SIZE < movedY && movedY < WINDOW_HEIGHT - OBJECT_HALF_SIZE)) {
			if(hitX && !hitY) {         //Y�����݈̂ړ��ł���
				mY = movedY;
			} else if(!hitX && hitY) {  //X�����݈̂ړ��ł���
				mX = movedX;
			} else if(!hitX && !hitY) { //�������Ɉړ��ł���
				mX = movedX;
				mY = movedY;
			}
	}
}

/*
�v���C���[���W�����v������
arrayStaticObject : �X�e�[�W�̃}�b�v�`�b�v
characters        : �L�����N�^�[
charactersNum     : �X�e�[�W�̃L������
offset            : �}�b�v�X�N���[��������
*/
void Character::playerJump(Array2D<StaticObject>& arrayStaticObject, const Character* characters, const int charactersNum, const int offset) {
	int keyIndex = KEY_INPUT_F;
	//�W�����v�J�n
	if(InputInterface::key[keyIndex] == 1 && !isJump) {
		isJump = true;
		mDetY  = -PLAYER_JUMP_SPEED;
	}

	if(isJump) {
		int masX   = (offset + mX) / OBJECT_SIZE;
		int masY   = mY / OBJECT_SIZE;
		int movedY = mY + mDetY;
		//����̏Փ˔���
		if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(masX, masY - 1))) {
			mY     = convertPixelToInner(masY * OBJECT_SIZE);
			mDetY += GRAVITY;
		} else {
			mDetY += GRAVITY;
		}
		//���~���̏Փ˔���
		if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(masX, masY + 1))) {
			mY     = convertPixelToInner(masY * OBJECT_SIZE);
			mDetY  = GRAVITY;
			isJump = false;
		}
	}
}

/*
�G������
arrayStaticObject : �X�e�[�W�̃}�b�v�`�b�v
characters        : �L�����N�^�[
charactersNum     : �X�e�[�W�̃L������
offset            : �}�b�v�X�N���[��������
*/
void Character::enemyWalk(Array2D<StaticObject>& arrayStaticObject, Character* characters, const int charactersNum, const int offset) {
	int mas[3][3][2] = {
		{{-1, -1}, {-1,  0}, {-1,  1}},
		{{ 0, -1}, { 0,  0}, { 0,  1}},
		{{ 1, -1}, { 1,  0}, { 1,  1}},
	};
	int masX   = (offset + mX) / OBJECT_SIZE;
	int masY   = mY / OBJECT_SIZE;
	int movedX = mX + mDetX;
	int movedY = mY + mDetY;
	bool hitX = false, hitY = false;
	for(int y = 0; y < 3; y++) {
		for(int x = 0; x < 3; x++) {
			if(x == 1 && y == 1) {
				continue;
			}
			int tmpX = masX + mas[y][x][0];
			int tmpY = masY + mas[y][x][1];
			//X�����ɑ΂��āA���肷��
			if(collisionDetectionToStatic(movedX, mY, &arrayStaticObject(tmpX, tmpY))) {
				hitX = true;
			}
			//Y�����ɑ΂��āA���肷��
			if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(tmpX, tmpY))) {
				hitY = true;
			}
		}
	}
	//�L�������m�̏Փ˔���
	for(int i = 0; i < charactersNum; i++) {
		if(this == &characters[i]) {
			continue;
		}
		if(characters[i].isPlayer() && collisionDetectionToCharacter(movedX,     mY, characters[i])) {
			characters[i].dieCharacter();
			hitX = true;
		}
	}

	if(hitX && !hitY) {         //Y�����݈̂ړ��ł���
		mY = movedY;
	} else if(!hitX && hitY) {  //X�����݈̂ړ��ł���
		mX = movedX;
	} else if(!hitX && !hitY) { //�������Ɉړ��ł���
		mX = movedX;
		mY = movedY;
	} else {                    //�ړ��ł��Ȃ�
		mDetX *= -1;
	}
}

//�L������`��
void Character::draw(const Image* image) const {
	if(isDead()) {
		return;
	}

	//������W��ŕ`�悷��̂ŁA���S���獶��ɕϊ�
	int detX = convertInnerToPixel(mX);
	int detY = convertInnerToPixel(mY);

	/*
	�ꖇ�̉摜�ɁA��������̊G������̂ŁA�ǂ�����������߂�K�v������
	10�J�E���g���ƂɁA�A�j���[�V����
	*/
	int srcX, srcY;
	srcX = srcY = -1;
	int tmpIndex = mCnt / ANIMATION_INTERVAL;
	switch(mCharacterType) {
	case CHARACTERTYPE_PLAYER:
		if(isJump) {        //�W�����v
			srcX = 3;
			if(isDetRight) {
				srcY = 0;
			} else {
				srcY = 1;
			}
		} else {
			if(mDetX == 0) { //�f�t�H���g
				srcX = 0;
				if(isDetRight) {
					srcY = 0;
				} else {
					srcY = 1;
				}
			} else {         //�����Ă���
				if(tmpIndex % 2 == 0) {
					srcX = 1;
				} else {
					srcX = 2;
				}
				if(isDetRight) {
					srcY = 0;
				} else {
					srcY = 1;
				}
			}
		}
		break;
	case CHARACTERTYPE_ENEMY:
		if(tmpIndex % 2 == 0) {
			srcX = 0;
		} else {
			srcX = 1;
		}
		srcY = 3;
		break;
	}

	if(srcX != -1 && srcY != -1) {
		image->draw(detX, detY, srcX, srcY, OBJECT_SIZE, OBJECT_SIZE);
	}
}

//�L�����N�^�[�̎��
unsigned Character::type() const {
	return mCharacterType;
}

//�L�����N�^�[���J�E���g����
void Character::count() {
	if(mCnt == INTERVAL) {
		mCnt = 0;
	}
	mCnt++;
}

//�u���b�N�Ȃǂ̓����Ȃ����̂ɑ΂���Փ˔���
bool Character::collisionDetectionToStatic(int movedX, int movedY, StaticObject* obj) const {
	int x, y;
	obj->getPosition(&x, &y);
	return collisiontDetection(movedX, movedY, x, y);
}

//�L�����ɑ΂���Փ˔���
bool Character::collisionDetectionToCharacter(const int movedX, const int movedY, const Character& c) const {
	return collisiontDetection(movedX, movedY, c.mX, c.mY);
}

//4���_���ʂ̕��̂̍��W�͈͂ɓ����Ă���
bool Character::collisiontDetection(const int movedX, const int movedY, const int objX, const int objY) const {
	int aLeft  = movedX - OBJECT_HALF_SIZE;
	int aRight = movedX + OBJECT_HALF_SIZE;
	int bLeft  =   objX - OBJECT_HALF_SIZE;
	int bRight =   objX + OBJECT_HALF_SIZE;
	if( (aLeft < bRight) && (aRight > bLeft) ) {
		int aTop    = movedY - OBJECT_HALF_SIZE;
		int aBottom = movedY + OBJECT_HALF_SIZE;
		int bTop    =   objY - OBJECT_HALF_SIZE;
		int bBottom =   objY + OBJECT_HALF_SIZE;
		if( (aTop < bBottom) && (aBottom > bTop) ) {
			return true;
		}
	}

	return false;
}

bool Character::isPlayer() const {
	return (mCharacterType == CHARACTERTYPE_PLAYER);
}

bool Character::isEnemy() const {
	return (mCharacterType == CHARACTERTYPE_ENEMY);
}

bool Character::clear() const {
	return isClear;
}

bool Character::isDead() const {
	return (mCharacterType == CHARACTERTYPE_NONE);
}

//�v���C���[�����񂾂Ƃ��A��ނ��X�V
void Character::dieCharacter() {
	mCharacterType = CHARACTERTYPE_NONE;
}

/*
�L������u���b�N�̍�����W�𒆐S�ɕϊ�����B
�摜��`�悷��Ƃ��͍��㒸�_����Ƃ��Ă���
�������A�L������u���b�N�̓���������Փ˔��肷��ۂɂ́A
���W�����S�̂ق������₷��
*/
int Character::convertPixelToInner(const int pos) const {
	return pos + OBJECT_HALF_SIZE;
}

/*
�L������u���b�N�̒��S���W������ɕϊ�����
�摜��`�悷��ہA������W����Ƃ��Ă���
���������āA�ϊ�����K�v������
*/
int Character::convertInnerToPixel(const int pos) const {
	return pos - OBJECT_HALF_SIZE;
}