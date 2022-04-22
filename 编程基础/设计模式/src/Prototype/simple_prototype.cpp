// simple_prototype.cpp: 原型模式基本实现
#include <iostream>
using namespace std;

class Prototype  {
public:
    virtual Prototype* clone() const = 0;
};


class Keyboard : public Prototype {
private:
    string brand;           // 品牌
    string model;           // 型号
    float  prise;           // 零售价
    string identification;  // 唯一标识

public:
    Keyboard(const string& b, const string& m, float p, const string& id)
        : brand(b), model(m), prise(p), identification(id) { }
    
    void SetIdentification(const string& id) {
        identification = id;
    }

    Keyboard* clone() const override {
        Keyboard* res = new Keyboard(*this);
        return res;
    }

    void Show(){
        cout << "brand: " << brand << ", model: " << model << ", prise: " << prise
             << ", identification: " << identification << endl;
    }
};

int main(){
    Keyboard* keyboard1 = new Keyboard("iKBC", "V120", 100, "123-456-789");
    Keyboard* keyboard2 = keyboard1->clone();
    keyboard2->SetIdentification("123-456-790");
    
    keyboard1->Show();
    keyboard2->Show();
    return 0;
}