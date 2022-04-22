// abstract_factory.cpp
// 抽象工厂模式
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

// 显示器
class Monitor {
public:
	void play(){};
};
// HP显示器
class HPMonitor : public Monitor {
public:
	void play() { cout << "HPMonitor play()" << endl; };
};
// DELL显示器
class DellMonitor : public Monitor {
public:
	void play(){cout << "DellMonitor play()"<< endl;};
};

// 主机
class MainFrame {
public:
	void run(){};
};
// HP主机
class HPMainFrame : public MainFrame {
public:
	void run() { cout << "HPMainFrame run()" << endl; };
};
// DELL主机
class DellMainFrame : public MainFrame {
public:
	void run(){cout << "DellMainFrame run()"<< endl;};
};

// 抽象工厂类
class Factory {
public:
    static MainFrame* CreateMainFrame();
    static Monitor* CreateMonitor();
    static Keyboard* CreateKeyboard();
};
//HP工厂
class HPFactory : public Factory {
public:
    static MainFrame* CreateMainFrame(){
        return new HPMainFrame();
    }
    static Monitor* CreateMonitor(){
        return new HPMonitor();
    }
    static Keyboard* CreateKeyboard(){
        return new HPKeyboard();
    }
};
//Dell工厂
class DellFactory : public Factory {
public:
    static MainFrame* CreateMainFrame(){
        return new DellMainFrame();
    }
    static Monitor* CreateMonitor(){
        return new DellMonitor();
    }
    static Keyboard* CreateKeyboard(){
        return new DellKeyboard();
    }
};

int main() {
    Factory hpFactory = HPFactory();
    Factory dellFactory = DellFactory();

    ((HPKeyboard*)HPFactory::CreateKeyboard())->print();
    ((HPMainFrame*)HPFactory::CreateMainFrame())->run();
    ((HPMonitor*)HPFactory::CreateMonitor())->play();

    ((DellKeyboard*)DellFactory::CreateKeyboard())->print();
    ((DellMainFrame*)DellFactory::CreateMainFrame())->run();
    ((DellMonitor*)DellFactory::CreateMonitor())->play();
	return 0;
}

/** OUTPUT:
HPKeyboard print()
HPMainFrame run()
HPMonitor play()
DellKeyboard print()
DellMainFrame run()
DellMonitor play()
*/