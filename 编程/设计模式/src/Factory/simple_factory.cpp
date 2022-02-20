// simple_factory.cpp
// 简单工厂模式
#include<iostream>
using namespace std;

enum Keyboard_ID{
	HPKeyboard_ID = 0,
	DellKeyboard_ID = 1,
};

// 键盘
class Keyboard {
public:
	void print(){};
    void input(string context);
};
// HP键盘
class HPKeyboard : public Keyboard {
public:
	void print() { cout << "HPKeyboard print()" << endl; };
    void input(string context);
};
// DELL键盘
class DellKeyboard : public Keyboard {
public:
	void print(){cout << "DellKeyboard print()"<< endl;};
    void input(string context);
};

// 工厂类
class KeyboardFactory {
public:
    // 构造方法
	static Keyboard* CreateKeyboard(int id) {
		Keyboard* keyboard = nullptr;
		switch (id) {
			case Keyboard_ID::HPKeyboard_ID:
				keyboard = new HPKeyboard();
				break;
			case Keyboard_ID::DellKeyboard_ID:
				keyboard = new DellKeyboard();
				break;
			default:
				break;
		}
		return keyboard;
	}
};

int main() {
	HPKeyboard* ka = (HPKeyboard*)KeyboardFactory::CreateKeyboard(Keyboard_ID::HPKeyboard_ID);
	DellKeyboard* kb = (DellKeyboard*)KeyboardFactory::CreateKeyboard(Keyboard_ID::DellKeyboard_ID);
	ka->print();
	kb->print();
	return 0;
}

/** OUTPUT:

*/