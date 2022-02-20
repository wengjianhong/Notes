// safe_composite.cpp  安全模式
#include <iostream>
#include <string>
#include <map>
using namespace std;

// 行政区域
class Region {
protected:
    string name;               // 名称
    double area;               // 面积
    unsigned long population;  // 人口

public:
    Region(const string& n, double a, unsigned long p)
        : name(n), area(a), population(p) {}

    string GetName() { return name; }
    double GetArea() { return area; }
    unsigned long GetPopulation() { return population; }
    
    // 安全模式去掉子对象管理方法
    // virtual bool AddSubRegion(Region* region) = 0;
    // virtual bool RemoveSubRegion(const string& n) = 0;

    virtual double LandCensus() = 0;                // 土地普查
    virtual unsigned long PopulationCensus() = 0;   // 人口普查
};

class Country : public Region {
    map<string, Region*> provinces;

public:
    Country(const string& n, double a, unsigned long p) : Region(n, a, p) {}

    bool AddSubRegion(Region* p) {
        provinces[p->GetName()] = p;
        return true;
    }
    bool RemoveSubRegion(const string& n) {
        provinces.erase(n);
        return true;
    }

    double LandCensus() override {
        area = 0;
        for (auto item : provinces) {
            area += item.second->LandCensus();
        }
        return area;
    }
    unsigned long PopulationCensus() override {
        population = 0;
        for (auto item : provinces) {
            population += item.second->PopulationCensus();
        }
        return population;
    }
};

class Province : public Region {
    map<string, Region*> cities;

   public:
    Province(const string& n, double a, unsigned long p) : Region(n, a, p) {}

    bool AddSubRegion(Region* c) {
        cities[c->GetName()] = c;
        return true;
    }
    bool RemoveSubRegion(const string& n) {
        cities.erase(n);
        return true;
    }

    double LandCensus() override {
        area = 0;
        for (auto item : cities) {
            area += item.second->LandCensus();
        }
        return area;
    }
    unsigned long PopulationCensus() override {
        population = 0;
        for (auto item : cities) {
            population += item.second->PopulationCensus();
        }
        return population;
    }
};

class City : public Region {
public:
    City(const string& n, double a, unsigned long p) : Region(n, a, p) {}

    // 叶子节点去掉 子对象管理方法
    // bool AddSubRegion(Region* c) override { return false; }
    // bool RemoveSubRegion(const string& n) override { return false; }

    double LandCensus() override {
        return area;
    }
    unsigned long PopulationCensus() override {
        return population;
    }
};

void ClientCode(Region* region) {
    // ...
    std::cout  << region->GetName() << ": Land=" << region->LandCensus() << endl;
    std::cout  << region->GetName() << ": Population=" << region->PopulationCensus() << endl;
    // ...
}


int main() {
    Country* china = new Country("China", 0, 0);

    Province* fujian = new Province("Fujian", 0, 0);
    Province* zhejiang = new Province("Zhejiang", 0, 0);

    City* xiamen = new City("Xiamen", 1, 100000);
    City* fuzhou = new City("Fuzhou", 2, 200000);
    City* putian = new City("Putian", 3, 300000);

    City* wenzhou = new City("Wenzhou", 5, 500000);
    City* hangzhou = new City("Hangzhou", 6, 600000);

    fujian->AddSubRegion(xiamen);
    fujian->AddSubRegion(fuzhou);
    fujian->AddSubRegion(putian);
    china->AddSubRegion(fujian);

    zhejiang->AddSubRegion(wenzhou);
    zhejiang->AddSubRegion(hangzhou);
    china->AddSubRegion(zhejiang);

    ClientCode(china);

    return 0;
}