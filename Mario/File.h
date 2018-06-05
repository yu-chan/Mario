#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File {
public:
	File(const char* filename);
	~File();

	int size() const;
	char* data() const;

private:
	int mSize;
	char* mData;
};

#endif