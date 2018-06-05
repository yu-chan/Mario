#ifndef INCLUDED_STATICOBJECT_H
#define INCLUDED_STATICOBJECT_H

class Image;
class Character;

class StaticObject {
public:
	StaticObject();

	//マップチップの種類
	enum Flag {
		FLAG_NONE          =       0 ,
		FLAG_BRICK         = (1 << 0),
		FLAG_DYNAMIC_BRICK = (1 << 1),
		FLAG_CLEAR_BRICK   = (1 << 2),
		FLAG_FLAG          = (1 << 3),
	};
	bool checkFlag(unsigned) const;
	void setFlag(unsigned);
	void resetFlag(unsigned);
	unsigned flag() const;

	void set(int, int, unsigned);
	void scroll(const int);

	void draw(const Image* image) const;

	void setPosition(int, int);
	void getPosition(int*, int*);
	int convertPixelToInner(const int) const;
	int convertInnerToPixel(const int) const;

private:
	unsigned mFlag;
	int mX;
	int mY;
};

#endif