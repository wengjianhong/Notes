// command.cpp 命令模式
#include <vector>
#include <iostream>
using namespace std;

// 接受者类
class Editor{
    string data = "";
public:
    void Save(){
        cout << "Save: " << data << endl;
    }
    void Insert(int pos, const string& s){
        data = data.substr(0, pos) + s + data.substr(pos);
    }
    string Cut (int start, int len){
        string res = data.substr(start, len);
        data = data.substr(0, start) + data.substr(start+len);
        return res;
    }
    void Paste(int pos, const string& s){
        data = data.substr(0, pos) + s + data.substr(pos);
    }
};

// 调用者类
class Application{
    vector<Editor*> editors;
    string clipboard;

public:
    void SetClipboard(const string& s){
        clipboard = s;
    }
    string GetClipboard(){
        return clipboard;
    }
    Editor* OpenNewEditor(){
        Editor* editor = new Editor;
        editors.push_back(editor);
        return editor;
    }
};

// 抽象命令类
class Command{
protected:
    Editor* editor;
    Application* app;

public:
    Command(Application* a, Editor* e) : app(a), editor(e){}

    virtual void execute() = 0;
};

// 具体命令类：保存
class SaveCommand : public Command{
public:
    SaveCommand(Application* a, Editor* e) : Command(a, e){}

    void execute() override {
        editor->Save();
    }
};
// 具体命令类：插入
class InsertCommand : public Command{
private:
    int pos;
    string context;
public:
    InsertCommand(Application* a, Editor* e, int p, const string& c) : pos(p), context(c), Command(a, e){}

    void execute() override {
        editor->Insert(pos, context);
    }
};
// 具体命令类：剪切
class CutCommand : public Command{
private:
    int start, len;
public:
    CutCommand(Application* a, Editor* e, int s, int l) : start(s), len(l), Command(a, e) {}

    void execute() override {
        app->SetClipboard(editor->Cut(start, len));
    }
};
// 具体命令类：粘贴
class PasteCommand : public Command{
    int pos;
public:
    PasteCommand(Application* a, Editor* e, int p): pos(p), Command(a, e) {}

    void execute() override {
        editor->Paste(pos, app->GetClipboard());
    }
};




void DoSomethins(Application* app, Editor* editor){
    Command* sm = new SaveCommand(app, editor);
    sm->execute();


    Command* im = new InsertCommand(app, editor, 0, "0126734589");
    im->execute();
    sm->execute();
    
    cout << "cut(7, 5)" << endl;
    Command* cm = new CutCommand(app, editor, 3, 2);
    cm->execute();
    sm->execute();

    cout << "paste(6)" << endl;
    Command* pm = new PasteCommand(app, editor, 6);
    pm->execute();
    sm->execute();
}


int main(){
    Application* app = new Application;
    Editor* editor = app->OpenNewEditor();

    DoSomethins(app, editor);
    return 0;
}