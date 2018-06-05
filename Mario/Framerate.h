#ifndef INCLUDED_FRAMERATE_H
#define INCLUDED_FRAMERATE_H

class Framerate {
public:
	static void create();
	static void destroy();
	static Framerate* instance();

	void update();
	void wait();

	int cnt() const;

private:
	Framerate();
	~Framerate();
	static Framerate* mInstance;
	
	int mStartTime;
	int mCnt;
	float mFramerate;
};

#endif