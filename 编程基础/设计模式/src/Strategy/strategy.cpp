// strategy.cpp 策略模式
#include <iostream>
using namespace std;

// 抽象策略类：路线
class RouteStrategy{
public:
    virtual string BuildRoute(const string& from, const string& to) = 0;
};

// 具体策略类：驾车导航
class DrivingStrategy : public RouteStrategy{
public:
    string BuildRoute(const string& from, const string& to) override {
        return "驾车导航路线: " + from + "->...->泉州->...->" + to;
    }
};
// 具体策略类：步行导航
class WalkingStrategy : public RouteStrategy{
public:
    string BuildRoute(const string& from, const string& to) override {
        return "步行导航路线: " + from + "->...->福州->...->" + to;
    }
};
// 具体策略类：公共交通导航
class PublicTransportStrategy : public RouteStrategy{
public:
    string BuildRoute(const string& from, const string& to) override {
        return "公共交通导航路线: " + from + "->...->莆田->...->" + to;
    }
};

// 环境类：导航器
class Navigator{
    RouteStrategy* strategy;
public:
    Navigator(RouteStrategy* s) : strategy(s){}
    string BuildRoute(const string& from, const string& to){
        if(strategy)
            return strategy->BuildRoute(from, to);
        return "规划路线失败：策略类为空.";
    }
    void SetStrategy(RouteStrategy* s) {
        strategy = s;
    }
};

int main(){
    Navigator* navigator = new Navigator(nullptr);
    cout << navigator->BuildRoute("厦门", "杭州") << endl;


    RouteStrategy* s1 = new PublicTransportStrategy;
    navigator->SetStrategy(s1);
    cout << navigator->BuildRoute("厦门", "杭州") << endl;


    RouteStrategy* s2 = new DrivingStrategy;
    navigator->SetStrategy(s2);
    cout << navigator->BuildRoute("厦门", "杭州") << endl;

    RouteStrategy* s3 = new WalkingStrategy;
    navigator->SetStrategy(s3);
    cout << navigator->BuildRoute("厦门", "杭州") << endl;

    delete s3;
    delete s2;
    delete s1;
    delete navigator;
    return 0;
}