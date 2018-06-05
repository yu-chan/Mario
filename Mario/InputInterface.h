#ifndef INCLUDED_INPUTINTERFACE_H
#define INCLUDED_INPUTINTERFACE_H

class InputInterface {
public:
	static int key[];
	static bool isOn(int id);
	static void updateKey();
};

#endif