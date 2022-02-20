// object_adapter.cpp  对象适配器模式结构
#include <iostream>
#include <algorithm>
using namespace std;

class SDK{
public:
    virtual int Request() const {
        return 172;
    }
};

class HSSDK{
public:
    string Request(){
        return "AC";
    }
};

class HSSDKAdapter : public SDK{
    HSSDK* sdk;

public:
    HSSDKAdapter(HSSDK* s) : sdk(s){}
    int Request()const override{
        return stoi(sdk->Request(), nullptr, 16);
    }
};

int main(){
    SDK *sdk = new SDK;
    cout << "SDK: " << sdk->Request() << endl;

    HSSDK *hssdk = new HSSDK;
    cout << "HSSDK: " << hssdk->Request() << endl;

    HSSDKAdapter *adapter = new HSSDKAdapter(hssdk);
    cout << "Adapter: " << adapter->Request() << endl;

    delete sdk;
    delete hssdk;
    delete adapter;
    return 0;
}