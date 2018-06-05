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

//キャラクターの座標、種類をセットする
void Character::set(int x, int y, CharacterType type) {
	mX             = convertPixelToInner(x);
	mY             = convertPixelToInner(y);
	mCharacterType = type;
	//種類が敵なら、速度をランダムに決める
	if(mCharacterType == CHARACTERTYPE_ENEMY) {
		setRandomDetX();
	}
}

//X方向に対して、速度をランダムに決める
void Character::setRandomDetX() {
	double rnd = rand();
	if(rnd < 0.5) {
		mDetX = -1;
	} else {
		mDetX =  1;
	}
}

/*
座標の更新
arrayStaticObject : ステージのマップチップ
characters        : キャラクター
charactersNum     : ステージのキャラ数
offset            : マップスクロールした量
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

//座標取得
void Character::getPosition(
	int* x,
	int* y) {
		*x = mX;
		*y = mY;
}

/*
マップスクロールする際に使う
プレイヤーを常に、中央に置く
*/
void Character::setPosition(int x) {
	mX = x;
}

//マップスクロールしたとき、スクロールした分だけ移動させる
void Character::scroll(const int scrollX) {
	mX -= scrollX;
}

/*
プレイヤーを歩かせる
arrayStaticObject : ステージのマップチップ
characters        : キャラクター
charactersNum     : ステージのキャラ数
offset            : マップスクロールした量
*/
void Character::playerWalk(Array2D<StaticObject>& arrayStaticObject, const Character* characters, const int charactersNum, const int  offset) {
	int speed = 0;
	if(InputInterface::isOn(KEY_INPUT_RIGHT)) {       //右矢印キーを押している
		speed = PLAYER_WALK_SPEED;
		isDetRight = true;
	} else if(InputInterface::isOn(KEY_INPUT_LEFT)) { //左矢印キーを押している
		speed = -PLAYER_WALK_SPEED;
		isDetRight = false;
	}
	mDetX = speed;

	//キャラの周囲のマス
	int mas[3][3][2] = {
		{{-1, -1}, {-1,  0}, {-1,  1}},
		{{ 0, -1}, { 0,  0}, { 0,  1}},
		{{ 1, -1}, { 1,  0}, { 1,  1}},
	};
	//キャラがいるマス
	int masX = (offset + mX) / OBJECT_SIZE;
	int masY = mY / OBJECT_SIZE;
	//移動後の座標
	int movedX = mX + mDetX;
	int movedY = mY + mDetY;
	//移動した際にオブジェクトにあたっているかどうか
	bool hitX = false, hitY = false;
	//ブロックなどの衝突判定
	for(int y = 0; y < 3; y++) {
		for(int x = 0; x < 3; x++) {
			if(x == 1 && y == 1) { //キャラが居るマスなので、何もしない
				continue;
			}
			int tmpX = masX + mas[y][x][0];
			int tmpY = masY + mas[y][x][1];
			//X方向に対して、判定する
			if(collisionDetectionToStatic(movedX, mY, &arrayStaticObject(tmpX, tmpY))) {
				hitX = true;
				//ステージの旗に触れたら、クリア
				if(arrayStaticObject(tmpX, tmpY).checkFlag(StaticObject::FLAG_FLAG)) {
					isClear = true;
				}
			}
			//Y方向に対して、判定する
			if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(tmpX, tmpY))) {
				hitY = true;
			}
		}
	}
	//キャラ同士の衝突判定
	for(int i = 0; i < charactersNum; i++) {
		if(this == &characters[i]) {
			continue;
		}
		//プレイヤーが敵に当たると死ぬ
		if(collisionDetectionToCharacter(movedX,     mY, characters[i])) {
			dieCharacter();
			hitX = true;
		}
	}
	//歩いている途中に、下に段差がないなら、空中にいる
	//これをやらないと、空中でジャンプできてしまう
	if(!hitY) {
		isJump = true;
	}

	//プレイヤーは常にスクリーンにいる必要がある
	if((OBJECT_HALF_SIZE < movedX && movedX < WINDOW_WIDTH  - OBJECT_HALF_SIZE) && 
		(OBJECT_HALF_SIZE < movedY && movedY < WINDOW_HEIGHT - OBJECT_HALF_SIZE)) {
			if(hitX && !hitY) {         //Y方向のみ移動できる
				mY = movedY;
			} else if(!hitX && hitY) {  //X方向のみ移動できる
				mX = movedX;
			} else if(!hitX && !hitY) { //両方向に移動できる
				mX = movedX;
				mY = movedY;
			}
	}
}

/*
プレイヤーをジャンプさせる
arrayStaticObject : ステージのマップチップ
characters        : キャラクター
charactersNum     : ステージのキャラ数
offset            : マップスクロールした量
*/
void Character::playerJump(Array2D<StaticObject>& arrayStaticObject, const Character* characters, const int charactersNum, const int offset) {
	int keyIndex = KEY_INPUT_F;
	//ジャンプ開始
	if(InputInterface::key[keyIndex] == 1 && !isJump) {
		isJump = true;
		mDetY  = -PLAYER_JUMP_SPEED;
	}

	if(isJump) {
		int masX   = (offset + mX) / OBJECT_SIZE;
		int masY   = mY / OBJECT_SIZE;
		int movedY = mY + mDetY;
		//頭上の衝突判定
		if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(masX, masY - 1))) {
			mY     = convertPixelToInner(masY * OBJECT_SIZE);
			mDetY += GRAVITY;
		} else {
			mDetY += GRAVITY;
		}
		//下降中の衝突判定
		if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(masX, masY + 1))) {
			mY     = convertPixelToInner(masY * OBJECT_SIZE);
			mDetY  = GRAVITY;
			isJump = false;
		}
	}
}

/*
敵が動く
arrayStaticObject : ステージのマップチップ
characters        : キャラクター
charactersNum     : ステージのキャラ数
offset            : マップスクロールした量
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
			//X方向に対して、判定する
			if(collisionDetectionToStatic(movedX, mY, &arrayStaticObject(tmpX, tmpY))) {
				hitX = true;
			}
			//Y方向に対して、判定する
			if(collisionDetectionToStatic(mX, movedY, &arrayStaticObject(tmpX, tmpY))) {
				hitY = true;
			}
		}
	}
	//キャラ同士の衝突判定
	for(int i = 0; i < charactersNum; i++) {
		if(this == &characters[i]) {
			continue;
		}
		if(characters[i].isPlayer() && collisionDetectionToCharacter(movedX,     mY, characters[i])) {
			characters[i].dieCharacter();
			hitX = true;
		}
	}

	if(hitX && !hitY) {         //Y方向のみ移動できる
		mY = movedY;
	} else if(!hitX && hitY) {  //X方向のみ移動できる
		mX = movedX;
	} else if(!hitX && !hitY) { //両方向に移動できる
		mX = movedX;
		mY = movedY;
	} else {                    //移動できない
		mDetX *= -1;
	}
}

//キャラを描画
void Character::draw(const Image* image) const {
	if(isDead()) {
		return;
	}

	//左上座標基準で描画するので、中心から左上に変換
	int detX = convertInnerToPixel(mX);
	int detY = convertInnerToPixel(mY);

	/*
	一枚の画像に、たくさんの絵があるので、どれを書くか決める必要がある
	10カウントごとに、アニメーション
	*/
	int srcX, srcY;
	srcX = srcY = -1;
	int tmpIndex = mCnt / ANIMATION_INTERVAL;
	switch(mCharacterType) {
	case CHARACTERTYPE_PLAYER:
		if(isJump) {        //ジャンプ
			srcX = 3;
			if(isDetRight) {
				srcY = 0;
			} else {
				srcY = 1;
			}
		} else {
			if(mDetX == 0) { //デフォルト
				srcX = 0;
				if(isDetRight) {
					srcY = 0;
				} else {
					srcY = 1;
				}
			} else {         //動いている
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

//キャラクターの種類
unsigned Character::type() const {
	return mCharacterType;
}

//キャラクターをカウントする
void Character::count() {
	if(mCnt == INTERVAL) {
		mCnt = 0;
	}
	mCnt++;
}

//ブロックなどの動かない物体に対する衝突判定
bool Character::collisionDetectionToStatic(int movedX, int movedY, StaticObject* obj) const {
	int x, y;
	obj->getPosition(&x, &y);
	return collisiontDetection(movedX, movedY, x, y);
}

//キャラに対する衝突判定
bool Character::collisionDetectionToCharacter(const int movedX, const int movedY, const Character& c) const {
	return collisiontDetection(movedX, movedY, c.mX, c.mY);
}

//4頂点が別の物体の座標範囲に入っている
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

//プレイヤーが死んだとき、種類を更新
void Character::dieCharacter() {
	mCharacterType = CHARACTERTYPE_NONE;
}

/*
キャラやブロックの左上座標を中心に変換する。
画像を描画するときは左上頂点を基準としている
しかし、キャラやブロックの動かしたり衝突判定する際には、
座標が中心のほうがやりやすい
*/
int Character::convertPixelToInner(const int pos) const {
	return pos + OBJECT_HALF_SIZE;
}

/*
キャラやブロックの中心座標を左上に変換する
画像を描画する際、左上座標を基準としている
したがって、変換する必要がある
*/
int Character::convertInnerToPixel(const int pos) const {
	return pos - OBJECT_HALF_SIZE;
}