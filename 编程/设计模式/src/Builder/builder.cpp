// builder.cpp
// 建造者模式
#include<iostream>
using namespace std;

// 键盘
class Keyboard {
    string brand = "";  // 品牌

public:
    Keyboard(const string& b):brand(b){}
    void Show(){ cout << "    Keyboard: " << brand << endl; }
};

// 显示器
class Monitor {
    string brand = "";  // 品牌

public:
    Monitor(const string& b):brand(b){}
    void Show(){cout << "    Monitor: " << brand << endl;}
};
// 主机
class MainFrame {
    string brand = "";  // 品牌

public:
    MainFrame(const string& b):brand(b){}
    void Show(){ cout << "    MainFrame: " << brand << endl; }
};

// 产品
class Computer{
public:
    Monitor* monitor = nullptr;
    Keyboard* keyboard = nullptr;
    MainFrame* mainFrame = nullptr;

    ~Computer(){
        if(monitor) delete monitor;
        if(keyboard) delete keyboard;
        if(mainFrame) delete mainFrame;
    }
    void Show(){
        if(monitor) monitor->Show();
        if(keyboard) keyboard->Show();
        if(mainFrame) mainFrame->Show();
    }
};

// 抽象生成器
class Builder{
public:
    Computer* computer = nullptr;
    Builder(){
        computer = new Computer();
    }
    ~Builder(){
        if(computer) delete computer;
    }
    Computer* GetComputer(){
        return computer;
    }
    virtual void BuildMonitor() = 0;
    virtual void BuildKeyboard() = 0;
    virtual void BuildMainFrame() = 0;
};

// HP电脑生成器
class HPBuilder: public Builder{
public:
    void BuildMonitor() override {
        computer->monitor = new Monitor("HP");
    }
    void BuildKeyboard() override {
        computer->keyboard = new Keyboard("HP");
    }
    void BuildMainFrame() override {
        computer->mainFrame = new MainFrame("HP");
    }
};

// Dell电脑生成器
class DellBuilder: public Builder{
public:
    void BuildMonitor() override {
        computer->monitor = new Monitor("Dell");
    }
    void BuildKeyboard() override {
        computer->keyboard = new Keyboard("Dell");
    }
    void BuildMainFrame() override {
        computer->mainFrame = new MainFrame("Dell");
    }
};

// 定制电脑生成器
class CustomBuilder: public Builder{
public:
    void BuildMonitor() override {
        computer->monitor = new Monitor("Dell");
    }
    void BuildKeyboard() override {
        computer->keyboard = new Keyboard("HP");
    }
    void BuildMainFrame() override {
        computer->mainFrame = new MainFrame("Other");
    }
};

// 生成指导者
class Director{
public:
    void MakeHPComputer(HPBuilder* builder){
        builder->BuildMainFrame();
        builder->BuildKeyboard();
        builder->BuildMonitor();
    }

    void MakeDellComputer(DellBuilder* builder){
        builder->BuildMainFrame();
        builder->BuildKeyboard();
        builder->BuildMonitor();
    }

    void MakeCustomComputer(CustomBuilder* builder){
        builder->BuildMainFrame();
        builder->BuildKeyboard();
        builder->BuildMonitor();
    }
};


int main(){
    Director* director = new Director();
    HPBuilder* hpBuilder = new HPBuilder();
    DellBuilder* dellBuilder = new DellBuilder();
    CustomBuilder* customBuilder = new CustomBuilder();

    director->MakeHPComputer(hpBuilder);
    Computer* computer1 = hpBuilder->GetComputer();
    cout << "Computer1 Show():" << endl;
    computer1->Show();
    cout << endl;

    director->MakeDellComputer(dellBuilder);
    Computer* computer2 = dellBuilder->GetComputer();
    cout << "Computer2 Show():" << endl;
    computer2->Show();
    cout << endl;

    director->MakeCustomComputer(customBuilder);
    Computer* computer3 = customBuilder->GetComputer();
    cout << "Computer3 Show():" << endl;
    computer3->Show();
    return 0;
}