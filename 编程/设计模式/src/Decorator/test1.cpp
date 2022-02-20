#include <iostream>
using namespace std;

class A{
public:
    void funA(){
        cout << "class A" << endl;
    }
    virtual void funB(){
        cout << "class A" << endl;
    }
};

class B: public A{
public:
    void funA(){
        cout << "class B" << endl;
    }
    void funB() override {
        cout << "class B" << endl;
    }
};

int main(){
    A* ptr = new B;
    ptr->funA();
    ((B*)ptr)->funA();

    ptr->funB();
    ptr->A::funB();
}