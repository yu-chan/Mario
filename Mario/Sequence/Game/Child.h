#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

namespace Sequence {
namespace Game {

class Parent;

//画面遷移するのに利用
//このクラスを継承したクラス間で遷移する
class Child {
public:
	virtual ~Child() {}
	virtual void update(Parent*) = 0;
};

}
}

#endif