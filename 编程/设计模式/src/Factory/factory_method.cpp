// factory_method.cpp
// 工厂方法模式
#include<iostream>
using namespace std;

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

// 抽象工厂类
class KeyboardFactory {
public:
    static Keyboard* CreateKeyboard();
};

class HPKeyboardFactory : public KeyboardFactory {
public:
    static Keyboard* CreateKeyboard(){
        return new HPKeyboard();
    }
};

class DellKeyboardFactory : public KeyboardFactory {
public:
    static Keyboard* CreateKeyboard(){
        return new DellKeyboard();
    }
};

int main() {
    HPKeyboard* ka = (HPKeyboard*)HPKeyboardFactory::CreateKeyboard();
    DellKeyboard* kb = (DellKeyboard*)DellKeyboardFactory::CreateKeyboard();

	ka->print();
	kb->print();
	return 0;
}

/** OUTPUT:

*/