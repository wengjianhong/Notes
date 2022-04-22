// class_adapter.cpp 类适配器模式结构（必须支持多重继承）
#include <iostream>
#include <algorithm>
using namespace std;

class SDK{
public:
    virtual int Request() const {
        return 160;
    }
};

class HSSDK{
public:
    string Request() const {
        return "A0";
    }
};

class HSSDKAdapter : public SDK, public HSSDK{
public:
    int Request () const override{
        return stoi(this->HSSDK::Request(), nullptr, 16);
    }
};

int main(){
    SDK *sdk = new SDK;
    cout << "SDK: " << sdk->Request() << endl;

    HSSDK *hssdk = new HSSDK;
    cout << "HSSDK: " << hssdk->Request() << endl;

    HSSDKAdapter *adapter = new HSSDKAdapter;
    cout << "Adapter: " << adapter->Request() << endl;

    delete sdk;
    delete hssdk;
    delete adapter;
    return 0;
}