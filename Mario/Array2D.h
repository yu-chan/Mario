#ifndef INCLUDED_ARRAY2D_H
#define INCLUDED_ARRAY2D_H

template<class T> class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete mArray;
		mArray = 0;
	}

	void setSize(int xSize, int ySize) {
		delete mArray;
		mArray = 0;
		mXsize = xSize;
		mYsize = ySize;
		mArray = new T[mXsize * mYsize];
	}

	T& operator()(int x, int y) {
		return mArray[y * mXsize + x];
	}

	const T& operator()(int x, int y) const {
		return mArray[y * mXsize + x];
	}

private:
	T* mArray;
	int mXsize;
	int mYsize;
};

#endif