#include "DxLib.h"
#include "InputInterface.h"

int InputInterface::key[256];
bool InputInterface::isOn(int id) {
	bool flag = false;
	//updateKey();
	if(key[id]) {
		flag = true;
	}
	return flag;
}

void InputInterface::updateKey() {
	char stateKey[256];
	GetHitKeyStateAll(stateKey);
	for(int i = 0; i < 256; i++) {
		if(stateKey[i] != 0) {
			key[i]++;
		} else {
			key[i] = 0;
		}
	}
}