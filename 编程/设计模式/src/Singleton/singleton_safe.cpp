// 单例模式（线程安全）
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

class Singleton {
protected:
	string value;
	static mutex mtx;
	static Singleton* instance;

	Singleton(const string& v) : value(v) {
	}

public:
	Singleton(Singleton& s) = delete;				// 删除拷贝函数
	void operator=(const Singleton& s) = delete;    // 删除复制函数

	static Singleton* GetInstance(const string& v) {
		unique_lock<mutex> lck(mtx);                // 线程安全
		if (instance == nullptr) {                  // 饿汉模式
			instance = new Singleton(v);
		}
		return instance;
	}

	void Print() {
		cout << value << endl;
	}
};
mutex Singleton::mtx;
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
}