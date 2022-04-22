// prototype_registry.cpp: 原型模式注册表实现
// simple_prototype.cpp: 原型模式基本实现
#include <iostream>
#include <unordered_map>
using namespace std;

class Prototype  {
public:
    virtual Prototype* Clone() const = 0;
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

    Keyboard* Clone() const override {
        Keyboard* res = new Keyboard(*this);
        return res;
    }

    void Show(){
        cout << "Keyboard: brand:" << brand << ", model:" << model << ", prise:" << prise
             << ", identification:" << identification << endl;
    }
};

class Monitor : public Prototype {
private:
    string brand;           // 品牌
    string model;           // 型号
    float  prise;           // 零售价
    string identification;  // 唯一标识

public:
    Monitor(const string& b, const string& m, float p, const string& id)
        : brand(b), model(m), prise(p), identification(id) { }
    
    void SetIdentification(const string& id) {
        identification = id;
    }

    Monitor* Clone() const override {
        Monitor* res = new Monitor(*this);
        return res;
    }

    void Show(){
        cout << "Monitor: brand:" << brand << ", model:" << model << ", prise:" << prise
             << ", identification:" << identification << endl;
    }
};

class PrototypeRegistry {
private:
    unordered_map<string, Prototype*> PrototypeMap;
public:
    void AddPrototype(const string& type, Prototype* prototype){
        PrototypeMap[type] = prototype;
    }
    Prototype* CreatePrototype(const string& type){
        return (PrototypeMap[type]->Clone());
    }
};

int main(){
    PrototypeRegistry* prototypeRegistry= new PrototypeRegistry();
    Monitor* monitor1 = new Monitor("Dell", "V120", 800, "0-123-456-789");
    Keyboard* keyboard1 = new Keyboard("iKBC", "X120", 100, "1-123-456-789");
    
    prototypeRegistry->AddPrototype("Monitor", monitor1);
    prototypeRegistry->AddPrototype("Keyboard", keyboard1);

    Monitor* monitor2 = (Monitor*)prototypeRegistry->CreatePrototype("Monitor");
    Keyboard* keyboard2 = (Keyboard*)prototypeRegistry->CreatePrototype("Keyboard");
    monitor2->SetIdentification("0-123-456-790");
    keyboard2->SetIdentification("1-123-456-790");

    monitor1->Show();
    monitor2->Show();

    keyboard1->Show();
    keyboard2->Show();
    return 0;
}