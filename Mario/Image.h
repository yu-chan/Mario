#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

class Image {
public:
	Image(const char* filename);
	~Image();

	int width() const;
	int height() const;

	void draw() const;
	void draw(int detX, int detY, int srcX, int srcY, int width, int height) const;
	
private:
	int mWidth;
	int mHeigt;
	int mData;
};

#endif