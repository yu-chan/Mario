#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

namespace Sequence {
namespace Game {

class Parent;

//��ʑJ�ڂ���̂ɗ��p
//���̃N���X���p�������N���X�ԂőJ�ڂ���
class Child {
public:
	virtual ~Child() {}
	virtual void update(Parent*) = 0;
};

}
}

#endif