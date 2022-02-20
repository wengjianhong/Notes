// visitor.cpp 访问者模式
#include <list>
#include <iostream>
using namespace std;

class Paper;
class Cuprum;

// 抽象访问者
class Company{
public:
    virtual string Create(Paper* p) = 0;
    virtual string Create(Cuprum* c) = 0;
};
// 具体访问者：艺术公司
class ArtCompany  : public Company{
public:
    string Create(Paper* p){
        return "生成七十二贤图\n";
    }
    string Create(Cuprum* c){
        return "生成孔子铜像\n";
    }
};
// 具体访问者：造币公司
class Mint : public Company{
public:
    string Create(Paper* p){
        return "生成纸币\n";
    }
    string Create(Cuprum* c){
        return "生成铜币\n";
    }
};


// 抽象元素：材料
class Material {
public:
    // 被访问函数
    virtual string Process(Company* m) = 0;
};
// 具体元素：铜
class Cuprum : public Material{
public:
    string Process(Company* m){
        return "铜 " + m->Create(this);
    }
};
// 具体元素：纸张
class Paper : public Material{
public:
    string Process(Company* m){
        return "纸张 " + m->Create(this);
    }
};

void ClientCode(list<Material*>& materials, Company* com){
    for(auto m : materials){
        cout << "\t" << m->Process(com);
    }
}

int main(){
    list<Material*> materials;
    materials.push_back(new Paper);
    materials.push_back(new Cuprum);
    
    cout << "造币公司：" << endl;
    ClientCode(materials, new Mint);

    cout << "艺术公司：" << endl;
    ClientCode(materials, new ArtCompany);

    return 0;
}
