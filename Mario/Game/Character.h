#ifndef INCLUDED_GAME_CHARACTER_H
#define INCLUDED_GAME_CHARACTER_H

#include "Array2D.h"

class Image;
class StaticObject;

class Character {
public:
	Character();

	enum CharacterType {
		CHARACTERTYPE_PLAYER,
		CHARACTERTYPE_ENEMY,

		CHARACTERTYPE_NONE,
	};
	
	void set(int x, int y, CharacterType);
	void setRandomDetX();
	void update(Array2D<StaticObject>&, Character*, const int, const int);
	void getPosition(int* x, int* y);
	void setPosition(int x);
	void scroll(const int);

	//プレイヤー専用
	void playerWalk(Array2D<StaticObject>&, const Character*, const int, const int);
	void playerJump(Array2D<StaticObject>&, const Character*, const int, const int);
	//敵専用
	void enemyWalk(Array2D<StaticObject>&, Character*, const int, const int);

	void draw(const Image*) const;

	unsigned type() const;
	void count();

	//衝突判定
	bool collisionDetectionToStatic(const int, const int, StaticObject*) const;
	bool collisionDetectionToCharacter(const int, const int, const Character&) const;
	bool collisiontDetection(const int movedX, const int movedY, const int objX, const int objY) const;

	bool isPlayer() const;
	bool isEnemy() const;
	
	bool clear() const;
	bool isDead() const;
	void dieCharacter();

	//座標変換
	int convertPixelToInner(const int) const;
	int convertInnerToPixel(const int) const;

private:
	static const int PLAYER_WALK_SPEED =  3;
	static const int PLAYER_RUN_SPEED  =  3;
	static const int PLAYER_JUMP_SPEED = 24;
	static const int GRAVITY           =  1;
	static const int ENEMY_WALK_SPEED  =  1;

	CharacterType mCharacterType;
	int mX;
	int mY;
	int mDetX;
	int mDetY;
	bool isDetRight;
	bool isJump;
	bool isClear;
	int mCnt;
};

#endif