// bridge.cpp 桥接模式
#include <iostream>
using namespace std;

// 座椅
class Seat
{
protected:
    int m_number;
    string m_name = "";

public:
    Seat(int number, string name) : m_number(number), m_name(name){};

    string String() const
    {
        return to_string(m_number) + "座 * " + m_name;
    }
};

// 真皮座椅
class CoriumSeat : public Seat
{
public:
    CoriumSeat(int number) : Seat(number, "真皮座椅"){}
};

// 织物座椅
class FabricSeat : public Seat
{
public:
    FabricSeat(int number) : Seat(number, "织物座椅"){}
};

// 轮胎
class Tire
{
protected:
    double m_width;
    double m_radius;

public:
    Tire(double width, double radius) : m_width(width), m_radius(radius){};

    string String() const
    {
        return "半径: " + to_string(m_radius) + ", 轮度: " + to_string(m_width);
    }
};

// T型号
class T_Tire : public Tire{
public:
    T_Tire() : Tire(30, 35) {}
};

// V型号
class V_Tire : public Tire{
public:
    V_Tire() : Tire(32, 39) {}
};

// 抽象:发动机
class Engine
{
protected:
    int m_cylinders;   // 气缸数
    double m_capacity; // 排量
    // ...
public:
    Engine(int cylinders, double capacity) : m_cylinders(cylinders), m_capacity(capacity){};

    string String() const
    {
        return "气缸数: " + to_string(m_cylinders) + ", 排量: " + to_string(m_capacity);
    }
};

// S型号
class S_Engine : public Engine
{
public:
    S_Engine() : Engine(3, 500) {}
};

// SS型号
class SS_Engine : public Engine
{
public:
    SS_Engine() : Engine(5, 800) {}
};

class Car
{
    string m_name = "";
    Tire *m_tire = nullptr;
    Seat *m_seat = nullptr;
    Engine *m_engine = nullptr;
    // ...

public:
    Car(const string &name) : m_name(name){};
    void SetTire(Tire * tire) { m_tire = tire; }
    void SetSeat(Seat * seat) { m_seat = seat; }
    void SetEngine(Engine *engine) { m_engine = engine; }

    string String() const
    {
        return m_name + "\n"
            + "轮胎: " + m_tire->String() + "\n"
            + "座椅: " + m_seat->String() + "\n"
            + "发动机: " + m_engine->String() + "\n";
    }

    ~Car() {
        delete m_tire;
        delete m_seat;
        delete m_engine;
    }
};

int main()
{
    Car* car1 = new Car("我的第一辆小车");
    car1->SetTire(new T_Tire());
    car1->SetSeat(new FabricSeat(5));
    car1->SetEngine(new S_Engine());
    cout << car1->String() << endl;

    Car* car2 = new Car("我的第二辆小车");
    car2->SetTire(new V_Tire());
    car2->SetSeat(new CoriumSeat(5));
    car2->SetEngine(new SS_Engine());
    cout << car2->String() << endl;
    return 0;
}