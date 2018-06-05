#ifndef INCLUDED_SOUNDMANAGER_H
#define INCLUDED_SOUNDMANAGER_H

class SoundManager {
public:
	static void create();
	static void destroy();
	static SoundManager* instance();

	void loadBgm(const char* filename);
	void playBgm();
	void stopBgm();
	void deleteBgm();
	bool checkBgm() const;

private:
	SoundManager();
	~SoundManager();
	static SoundManager* mInstance;

	int mBgm;
	int mSe;
};

#endif