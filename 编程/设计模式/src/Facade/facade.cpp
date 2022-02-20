// facade.cpp 
#include <iostream>
using namespace std;

// 子系统角色：SDK API
class API {
public:
    void InsertOrder(const string& s){
        cout << "新增报单：" << s << endl;
    }
};
// 子系统角色：SDK SPI
class SPI {
public:
    string GetResult(const string& s) {
        cout << "查询交易：" << s << endl;
        return "【交易结果】";
    }
};
// 子系统角色：Logger
class Logger{
public:
    void PrintLog(const string& s){
        cout << "打印日志：" << s << endl;
    }
};

// 外观角色：交易者
class Trader{
protected:
    API* api;
    SPI* spi;
    Logger* logger;

public:
    Trader(API* a, SPI* s, Logger* l) : api(a), spi(s), logger(l) {}

    void InsertOrder(){
        if(!api || !spi || !logger)
            return;

        api->InsertOrder("【报单信息】");
        string res = spi->GetResult("【报单信息】");
        logger->PrintLog("【报单信息】 : " + res);
    }
};

void ClientCode(Trader *trader) {
  // ...
  trader->InsertOrder();
  // ...
}

int main(){
    API* api = new API;
    SPI* spi = new SPI;
    Logger* logger = new Logger;
    Trader* trader = new Trader(api, spi, logger);
    
    ClientCode(trader);

    delete api;
    delete spi;
    delete logger;
    delete trader;

    return 0;
}