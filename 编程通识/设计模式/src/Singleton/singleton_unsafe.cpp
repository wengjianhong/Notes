// 单例模式（饿汉模式、线程不安全）
#include <thread>
#include <iostream>
using namespace std;

class Singleton {
protected:
	string value;
	static Singleton* instance;
	Singleton(const string& v) : value(v) { }

public:
	Singleton(Singleton& s) = delete;				// 删除拷贝函数
	void operator=(const Singleton& s) = delete;    // 删除复制函数

	static Singleton* GetInstance(const string& v) {
		if (instance == nullptr) { // 饿汉模式
			instance = new Singleton(v);
		}
		return instance;
	}

	void Print() {
		cout << value << endl;
	}
};
Singleton* Singleton::instance = nullptr;

void threadProcess(const string& s) {
	Singleton* t = Singleton::GetInstance(s);
	t->Print();
	cout << t << endl;
}

int main() {
	thread thd1 = thread(threadProcess, "bar");
	thread thd2 = thread(threadProcess, "foo");
	thd1.join();
	thd2.join();
	return 0;
}