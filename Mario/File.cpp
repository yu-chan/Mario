#include <fstream>
#include <iostream>
#include <Windows.h>
#include "File.h"

using namespace std;

File::File(const char* filename) : mSize(0), mData(0) {
	ifstream f(filename, ifstream::binary);
	if(f) {
		f.seekg(0, ifstream::end);
		mSize = static_cast<int>(f.tellg());
		f.seekg(0, ifstream::beg);
		mData = new char[mSize];
		f.read(mData, mSize);
		f.close();
	} else {
		OutputDebugString("ÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ");
	}
}

File::~File() {
	delete[] mData;
	mData = 0;
}

int File::size() const {
	return mSize;
}

char* File::data() const {
	return mData;
}