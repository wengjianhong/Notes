// bridge.cpp 桥接模式
#include <iostream>
using namespace std;

//实现化角色：设备
class Driver
{
protected:
    string name = "";
    bool enable = false;
    uint volume = 10;
    uint channel = 1;

public:
    virtual string GetName() { return name; }

    virtual void Enable() { enable = true; }
    virtual void Disable() { enable = false; }
    virtual bool IsEnabled() { return enable; }

    virtual uint GetVolume() { return volume; }
    virtual void SetVolume(uint v) { volume = v; }

    virtual uint GetChannel() { return channel; }
    virtual void SetChannel(uint c) { channel = c; }
};

//具体实现化角色：电视机
class TV : public Driver
{
public:
    TV() { name = "TV"; }
};

//具体实现化角色：收音机
class Radio : public Driver
{
public:
    Radio() { name = "Radio"; }
};

//抽象化角色：遥控器
class RemoteControl
{
protected:
    Driver *driver;

public:
    RemoteControl(Driver *d) : driver(d) {}

    virtual string GetDriverName() { return driver->GetName(); }

    virtual void Enable() { driver->Enable(); }
    virtual void Disable() { driver->Disable(); }
    virtual bool IsEnabled() { return driver->IsEnabled(); }

    virtual uint GetVolume() { return driver->GetVolume(); }
    virtual void SetVolume(uint v) { driver->SetVolume(v); }

    virtual uint GetChannel() { return driver->GetChannel(); }
    virtual void SetChannel(uint c) { driver->SetChannel(c); }
};
//扩展抽象化角色：智能遥控
class AdvancedRemoteControl : public RemoteControl
{
public:
    AdvancedRemoteControl(Driver* d) : RemoteControl(d) {}
    string GetMute() { return "mute list"; }
};

void ClientCode(RemoteControl* rc)
{
    // ...
    cout << "Driver Name: " << rc->GetDriverName() << endl;
    std::cout << "\tIsEnabled: " << rc->IsEnabled() << endl;
    
    rc->Enable();
    std::cout << "\tEnable Driver." << endl;
    
    std::cout << "\tGetVolume: " << rc->GetVolume() << endl;
    std::cout << "\tGetChannel: " << rc->GetChannel() << endl;
    
    rc->SetVolume(66);
    rc->SetChannel(66);
    std::cout << "\tSetVolume(10) && SetChannel(10)"  << endl;

    std::cout << "\tGetVolume: " << rc->GetVolume() << endl;
    std::cout << "\tGetChannel: " << rc->GetChannel() << endl;
    // ...
}

int main()
{
    Driver *tv = new TV;
    Driver *radio = new Radio;

    RemoteControl *rc = new RemoteControl(radio);
    AdvancedRemoteControl *arc = new AdvancedRemoteControl(tv);

    ClientCode(rc);
    cout << endl;

    ClientCode(arc);
    cout << "\tGetMute: " << arc->GetMute() << endl;

    delete rc;
    delete arc;

    delete tv;
    delete radio;
    return 0;
}