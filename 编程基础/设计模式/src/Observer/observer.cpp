// observer.cpp 观察者模式
#include <set>
#include <vector>
#include <iostream>
using namespace std;

// 抽象观察者类：读者
class Reader{
protected:
    string name;

public:
    Reader(const string& n) : name(n){}
    string GetName() { return name; }
    void Receive(const string& msg){
        cout << name << ", 收到新消息：" << msg << endl;
    }
};
// 具体观察者类：学生
class Student : public Reader{
public:
    Student(const string& n) : Reader(n){}
};
// 具体观察者类：教师
class Teacher : public Reader{
public:
    Teacher(const string& n) : Reader(n){}
};
// 具体观察者类：程序员
class Programmer : public Reader{
public:
    Programmer(const string& n) : Reader(n){}
};

// 抽象主题类：书
class Book{
protected:
    string name;
    set<Reader*> subscriber;

public:
    Book(const string& n) : name(n){}
    string GetName() { return name; }

    void Add(Reader* r){
        cout << r->GetName() << " 开始订阅 " << name << endl; 
        subscriber.insert(r);
    }
    void Remove(Reader* r){
        cout << r->GetName() << " 取消订阅 " << name << endl; 
        subscriber.erase(r);
    }
    void Publish(){
        for(auto r : subscriber){
            r->Receive( "《" + name + "》出新版了！");
        }
    }
};
// 具体主题类：教科书
class Textbook : public Book{
public:
    Textbook(const string& n) : Book(n){}
};
// 具体主题类：研究报告
class Report : public Book{
public:
    Report(const string& n) : Book(n){}
};
// 具体主题类：工具书
class ReferenceBook : public Book{
public:
    ReferenceBook(const string& n) : Book(n){}
};

void ClientCode(vector<Book*>& books, vector<Reader*>& readers){
    for(int i=0; i<books.size() && i<readers.size(); i++){
        books[i]->Add(readers[i]);
    }
    for(auto b: books){
        b->Publish();
    }
    
    cout << endl;
    for(int i=0;i<books.size() && i<readers.size();i++){
        if(i%2 == 0){
            books[i]->Remove(readers[i]);
            books[(i+1)%books.size()]->Add(readers[i]);
        }
    }
    for(auto b: books){
        b->Publish();
    }
}

int main(){
    vector<Book*> books;
    books.push_back(new Textbook("教科书1"));
    books.push_back(new Textbook("教科书2"));
    books.push_back(new Report("报告3"));
    books.push_back(new Report("报告4"));
    books.push_back(new ReferenceBook("工具书5"));
    books.push_back(new ReferenceBook("工具书6"));

    vector<Reader*> readers;
    readers.push_back(new Student("学生1"));
    readers.push_back(new Student("学生2"));
    readers.push_back(new Teacher("教师3"));
    readers.push_back(new Teacher("教师4"));
    readers.push_back(new Programmer("程序员5"));
    readers.push_back(new Programmer("程序员6"));

    ClientCode(books, readers);

}