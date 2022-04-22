// decorator.cpp
#include <iostream>
using namespace std;

class Logger{
public:
    virtual ~Logger() {}
    virtual string Format(const string& str) const = 0;
};

class SimpleLogger : public Logger{
public:
    string Format(const string& str) const override {
        return str;
    }
};

class Decorator : public Logger{
protected:
    Logger* logger;
public:
    Decorator(Logger* l) : logger(l){}
    string Format(const string& str) const override {
        return this->logger->Format(str);
    }
};

class TimeDecorator : public Decorator{
public:
    TimeDecorator(Logger* l) : Decorator(l){}
    string Format(const string& str) const override {
        return "[2022-01-01 08:00:00] " + this->logger->Format(str);
    }
};

class LocateDecorator : public Decorator{
    int line;
    string filename;
public:
    LocateDecorator(Logger* l) : Decorator(l){}
    void SetLocate(const string& f, int l){
        line = l;
        filename = f;
    };
    string Format(const string& str) const override{
        return filename + ":" + to_string(line) + ": " + this->logger->Format(str);
    }
};

void ClientCode(Logger* logger) {
    // ...
    cout << logger->Format("something to logging...");
    // ...
}

int main() {
    Logger* logger = new SimpleLogger;
    std::cout << "SimpleLogger: ";
    ClientCode(logger);
    std::cout << "\n\n";

    Logger* timeDecorator = new TimeDecorator(logger);
    std::cout << "TimeDecorator: ";
    ClientCode(timeDecorator);
    std::cout << "\n\n";

    Logger* locateDecorator = new LocateDecorator(timeDecorator);
    ((LocateDecorator*)locateDecorator)->SetLocate(__FILE__, __LINE__);
    std::cout << "LocateDecorator: ";
    ClientCode(locateDecorator);
    std::cout << "\n\n";
    
    delete logger;
    delete timeDecorator;
    delete locateDecorator;

    return 0;
}