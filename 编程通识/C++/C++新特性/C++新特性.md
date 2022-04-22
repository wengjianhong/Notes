[TOC]

# C++11/14/17/20 特性介绍

> 本文按照 [cppreference](https://links.jianshu.com/go?to=https%3A%2F%2Fen.cppreference.com%2Fw%2F) 列出的特性列表依次介绍。
> 本篇文章较长，而且都是内容「离散」的语言特性介绍，所以是很有必要加上目录的，但发布到「简书」时才发现「简书」是没法自动生成目录的。
> 但是幸运的是，前一段时间在 github 上发现了一个非常非常赞的开源插件：[简悦](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2FKenshin%2Fsimpread)。
> 它可以对页面进行二次处理从而提供风格统一且体验一致的页面，同时它也支持**自动生成目录**。插件也非常有产品的设计感，感觉作者践行了一句话：「不会设计的产品经理不是好的开发」。所以非常推荐读者安装这个插件（按照上述链接的官方教程安装即可），相信会给你一个不同的阅读体验。

## C++11 新特性

### 01 auto 与 decltype

**auto**: 对于变量，指定要从其初始化器⾃动推导出其类型。示例：

```c
auto a = 10;    // 自动推导 a 为 int
auto b = 10.2;  // 自动推导 b 为 double
auto c = &a;    // 自动推导 c 为 int*
auto d = "xxx"; // 自动推导 d 为 const char*
```

**decltype**: 推导实体的声明类型，或表达式的类型。为了解决 auto 关键字只能对变量进⾏类型推导的缺陷⽽出现。⽰例：

```c
int a = 0;

decltype(a) b = 1;        // b 被推导为 int 类型
decltype(10.8) c = 5.5;   // c 被推导为 double 类型
decltype(c + 100) d;      // d 被推导为 double

struct { double x; } aa;
decltype(aa.x) y;         // y 被推导为 double 类型
decltype(aa) bb;          // 推断匿名结构体类型
```

C++11 中 auto 和 decltype 结合再借助「尾置返回类型」还可推导函数的返回类型。⽰例：



```c
// 利⽤ auto 关键字将返回类型后置
template<typename T, typename U>
auto add1(T x, U y) -> decltype(x + y) {
  return x + y;
}
```

从 **C++14** 开始⽀持仅⽤ auto 并实现返回类型推导，见下⽂ C++14 章节。

### 02 defaulted 与 deleted 函数

在 C++ 中，如果程序员没有⾃定义，那么编译器会**默认**为程序员⽣成 「**构造函数**」、「**拷贝构造函数**」、「**拷贝赋值函数**」 等。

但如果程序员⾃定义了上述函数，编译器则不会⾃动⽣成这些函数。

⽽在实际开发过程中，我们有时需要**在保留⼀些默认函数的同时禁⽌⼀些默认函数**。

例如创建 「**不允许拷贝的类**」 时，在传统 C++ 中，我们经常有如下的惯例代码：



```c
// 除非特别熟悉编译器自动生成特殊成员函数的所有规则，否则意图是不明确的
class noncopyable   {
public:
  // 由于下⽅有⾃定义的构造函数（拷⻉构造函数）
  // 编译器不再⽣成默认构造函数，所以这⾥需要⼿动定义构造函数
  // 但这种⼿动声明的构造函数没有编译器⾃动⽣成的默认构造函数执⾏效率⾼
  noncopyable() {};
private: 
  // 将拷⻉构造函数和拷⻉赋值函数设置为 private
  // 但却⽆法阻⽌友元函数以及类成员函数的调⽤
  noncopyable(const noncopyable&);
  noncopyable& operator=(const noncopyable&);
};
```

传统 C++ 的惯例处理⽅式存在如下缺陷：

1. 由于⾃定义了「拷贝构造函数」，编译器不再⽣成「默认构造函数」，需要⼿动的显式定义「无参构造函数」
2. ⼿动显式定义的「无参构造函数」效率低于「默认构造函数」
3. 虽然「拷贝构造函数」和「拷贝赋值函数」是私有的，对外部隐藏。但⽆法阻⽌友元函数和类成员函数的调⽤
4. 除⾮特别熟悉编译器⾃动⽣成特殊成员函数的所有规则，否则意图是不明确的

为此，C++11 引⼊了 **default** 和 **delete** 关键字，来显式保留或禁止特殊成员函数：



```c
class noncopyable {
 public:
  noncopyable() = default;
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};
```

### 03 final 与 override

在传统 C++ 中，按照如下⽅式覆盖⽗类虚函数：



```c
struct Base {
  virtual void foo();
};
struct SubClass: Base {
  void foo();
};
```

上述代码存在⼀定的隐患：

- 程序员并⾮想覆盖⽗类虚函数，⽽是 **定义了⼀个重名的成员函数**。由于没有编译器的检查导致了意外覆盖且难以发现
- ⽗类的虚函数被删除后，编译器不会进⾏检查和警告，这可能引发严重的错误

为此，C++11 引⼊ **override** 显式的声明**要覆盖基类的虚函数**，如果不存在这样的虚函数，将不会通过编译：



```c
class Parent {
  virtual void watchTv(int);
};
class Child : Parent {
  virtual void watchTv(int) override;    // 合法
  virtual void watchTv(double) override; // 非法，父类没有此虚函数
};
```

⽽ **final** 则终⽌虚类被继承或虚函数被覆盖：



```c
class Parent2 {
  virtual void eat() final;
};

class Child2 final : Parent2 {};  // 合法

class Grandson : Child2 {};       // 非法，Child2 已经 Final，不可被继承

class Child3 : Parent2 {
  void eat() override; // 非法，foo 已 final
};
```

### 04 尾置返回类型

看一个比较复杂的函数定义：



```c
// func1(int arr[][3], int n) 为函数名和参数
// (* func1(int arr[][3], int n)) 表示对返回值进⾏解引⽤操作
// (* func1(int arr[][3], int n))[3] 表示返回值解引⽤后为⼀个⻓度为 3 的数组
// int (* func1(int arr[][3], int n))[3] 表示返回值解引⽤后为⼀个⻓度为 3 的 int 数组
int (* func1(int arr[][3], int n))[3] {
  return &arr[n];
}
```

C++11 引⼊「尾置返回类型」，将「函数返回类型」通过 **-> 符号**连接到函数后面，配合 auto 简化上述复杂函数的定义：



```c
// 返回指向数组的指针
auto fun1(int arr[][3], int n) -> int(*)[3] {
  return &arr[n];
}
```

尾置返回类型经常在 「**lambda 表达式**」、「**模板函数返回**」中使⽤：



```c
// 使⽤尾置返回类型来声明 lambda 表达式的返回类型
[capture list] (params list) mutable exception->return_type { function body }

// 在模板函数返回中结合 auto\decltype 声明模板函数返回值类型
template<typename T, typename U>
auto add(T x, U y) -> decltype(x + y) {
  return x + y;
}
```

### 05 右值引用

#### 05.1 何为左值与右值

- **左值**：内存中有确定存储地址的对象的表达式的值
- **右值**：所有不是左值的表达式的值。右值可分为「**传统纯右值**」和「**将亡值**」

上述的「传统纯右值」和「将亡值」又是什么？

- **纯右值**：即 C++11 之前的右值。包括：
  1. 常见的字面量如 0、"123"、或表达式为字面量
  2. 不具名的临时对象，如函数返回临时对象
- **将亡值**：随着 C++11 引入的右值引用而来的概念。包括：
  1. 「返回右值引用的函数」的返回值。如返回类型为 T&& 的函数的返回值
  2. 「转换为右值引用的转换函数」的返回值，如 std::move() 函数的返回值

同时，左值 + 将亡值又被称为「泛左值」。这几个概念对于刚接触的同学可能会比较混乱，我们梳理一下，如下图所示：



![img](C:\Users\wengjianhong\Desktop\C++新特性.assets\webp.webp)

value_type.png

左值还是右值可以通过**取地址运算符 &** 来进⾏判断，能够通过 & 正确取得地址的为左值，反之为右值。



```c
int i = 0;
int* p_i = &i;            // 可通过 & 取出地址，固 i 为左值
cout << p_i << endl;

int* p_i_plus = &(i + 1); // 非法，i + 1 为右值
int* p_i_const = &(0);    // 非法，0 为右值
```

#### 05.2 何为左值引用与右值引用

C++11 之前，我们就经常使⽤对左值的引⽤，即左值引⽤，使用 & 符号声明：



```c
int j = 0;
int& ref_j = j;           // ref_j 为左值引⽤
int& ref_ret = getVal();  // ref_ret 为左值引用

int& ref_j_plus = j + 1;  // ⾮法，左值引⽤不能作⽤于右值
int& ref_const = 0;       // 非法，左值引用不能作用于右值
```

如上例代码所示，**ref_j_plus** 和 **ref_const** 为传统 C++ 中经常使用的左值引用，无法作用于 **j+1** 或 **0** 这样的右值。

C++11 引⼊了针对右值的引⽤，即右值引⽤，使用 && 符号声明：



```c
int&& ref_k_plus = (i + 1); // ref_k_plus 为右值引用，它绑定了右值 i + 1
int&& ref_k = 0;            // ref_k 为右值引用，它绑定了右值 0 
```

#### 05.3 右值引用的特点

以下述代码为例：



```c
int getVal() {
  return 1;
}

int main() {
  // 这里存在两个值：
  //    1. val(左值)
  //    2. getVal() 返回的临时变量（右值）
  // 其中 getVal() 返回的临时变量赋值给 val 后会被销毁
  int val = getVal();
  return 0;
}
```

上述代码中，getVal 函数产⽣的 **「临时变量」** 需要先复制给左值 val，然后再被销毁。

但是如果使⽤右值引⽤：



```c
// 使用 && 来表明 val 的类型为右值引用
// 这样 getVal() 返回的临时对象（右值） 将被「续命」
// 拥有与 val 一样长的生命周期
int&& val = getVal();
```

上述代码体现了右值引⽤的**第⼀个特点**：

> 通过右值引⽤的声明，右值可「重⽣」，⽣命周期与右值引⽤类型的变量⽣命周期⼀样长。

再看如下例⼦：



```c
template<typename T>
void f(T&& t) {}

f(10);      // t 为右值

int x = 10;
f(x);       // t 为左值
```

上述例⼦体现了右值引⽤的**第⼆个特点**：

> 在 **⾃动类型推断（如模板函数等）的场景下**，T&& t 是**未定的引⽤类型**，即 t 并⾮⼀定为右值。如果它被左值初始化，那么 t 就为左值。如果它被右值初始化，则它为右值。

正是由于上述特点，C++11 引入右值引⽤可以实现如下⽬的：

- **实现移动语义**。解决临时对象的低效率拷贝问题
- **实现完美转发**。解决函数转发右值特征丢失的问题

#### 05.4 右值引用带来的移动语义

在 C++11 之前，临时对象的赋值采⽤的是低效的拷贝。

举例来讲，整个过程如同将⼀个冰箱⾥的⼤象搬到另⼀个冰箱，传统 C++ 的做法是第⼆个冰箱⾥复制⼀个⼀摸⼀样的⼤象，再把第⼀个冰箱的⼤象销毁，这显然不是⼀个⾃然的操作⽅式。

看如下例⼦：



```c
class HasPtrMem1 {
 public:
  HasPtrMem1() : d(new int(0)) {}
  ~HasPtrMem1() { delete d; }
  int* d;
};

int main() {
  HasPtrMem1 a1;
  HasPtrMem1 b1(a1);

  cout << *a1.d << endl;
  cout << *b1.d << endl;

  return 0;
}
```

上述代码中 `HasPtrMem1 b(a)` 将调⽤编译器默认⽣成的「拷贝构造函数」进⾏拷贝，且进⾏的是按位拷贝（浅拷贝），这将导致**悬挂指针问题[1]**。

> **悬挂指针问题[1]**: 上述代码在执⾏ main 函数后，将销毁 a、b 对象，于是调⽤对应的析构函数执⾏ delete d 操作。但由 于 a、b 对象中的成员 d 指针同⼀块内存，于是在其中⼀个对象被析构后，另⼀个对象中的指针 d 不再指向有效内存，这个对象的 d 就变成了悬挂指针。

在悬挂指针上释放内存将导致严重的错误。所以针对上述场景必须进⾏深拷贝：



```c
class HasPtrMem2 {
 public:
  HasPtrMem2() : d(new int(0)) {}
  HasPtrMem2(const HasPtrMem2& h) :
      d(new int(*h.d)) {}
  ~HasPtrMem2() { delete d; }
  int* d;
};

int main() {
  HasPtrMem2 a2;
  HasPtrMem2 b2(a2);

  cout << *a2.d << endl;
  cout << *b2.d << endl;

  return 0 ;
}
```

在上述代码中，我们⾃定义了拷贝构造函数的实现，我们通过 new 分配新的内存实现了深度拷贝，避免了「悬挂指针」的问题，但也引出了新的问题。

拷贝构造函数为指针成员分配新的内存并进⾏拷贝的做法是传统 C++ 编程中是⼗分常见的。但有些时候我们并不需要这样的拷贝：



```c
HasPtrMem2 GetTemp() {
  return HasPtrMem2();
}

int main() {
  HasPtrMem2 a = GetTemp();
}
```

上述代码中，GetTemp 返回的临时对象进⾏深度拷贝操作，然后再被销毁。如下图所⽰：

![img](C++%E6%96%B0%E7%89%B9%E6%80%A7.assets/6009978-5b7aa7695c62df06-163823122836626.png)

如果 HasPtrMem2 中的指针成员是复杂和庞⼤的数据类型，那么就会导致⼤量的性能消耗。

再回到⼤象移动的类⽐，其实更⾼效的做法是将⼤象直接从第⼀个冰箱拿出，然后放⼊第⼆个冰箱。同样的，我们在将临时对象赋值给某个变量时是否可以不⽤拷贝构造函数？答案是肯定的，如下图所⽰：

![img](C++%E6%96%B0%E7%89%B9%E6%80%A7.assets/6009978-74a85eb1207e624a.png)

在 C++11 中，像这样「偷⾛」资源的构造函数，称为 「**移动构造函数**」，这种「偷」的⾏为，称为 「**移动语义**」，可理解为「移为⼰⽤」。

当然实现时需要在代码中定义对应的「移动构造函数」：



```c
class HasPtrMem3 {
  public:
    HasPtrMem3() : d(new int(0)) {}
    HasPtrMem3(const HasPtrMem3& h) : 
        d(new int(*h.d)) {}
    HasPtrMem3(HasPtrMem3&& h) : d(h.d) {
      h.d = nullptr;
    }
    ~HasPtrMem3() { delete d; }
    int* d;
};
```

注意「移动构造函数」依然会存在悬挂指针问题，所以在通过移动构造函数「偷」完资源后，要把临时对象的 h.d 指针置为空，避免两个指针指向同⼀个内存，在析构时被析构两次。

「移动构造函数」中的参数为 HasPtrMem3&& h 为**右值类型[2]**，⽽返回值的临时对象就是右值类型，这也是为什么返回值临时对象能够匹配到「移动构造函数」的原因。

> **右值类型[2]**: 注意和上⾯提到的右值引⽤第⼆个特点做区分，这⾥不是类型推导的场景，HasPtrMem3 是确定的类型，所以 HasPtrMem3&& h 就是确定的右值类型。

上述的移动语义是通过右值引⽤来匹配临时值的，那么左值是否可以借助移动语义来优化性能呢？C++11 为我们 提供了 **std::move** 函数来实现这⼀⽬标：



```c
{
  std::list<std::string> tokens;              // tokens 为左值
  // 省略初始化...
  std::list<std::string> t = tokens;          // 这里存在拷贝
}

std::list<std::string> tokens;
std::list<std::string> t = std::move(tokens); // 这里不存在拷贝
```

std::move 函数实际没有移动任何资源，它唯⼀做的就是**将⼀个左值强制转换成右值引⽤**，从而匹配到「移动构造函数」或「移动赋值运算符」，应⽤移动语义实现资源移动。⽽ C++11 中所有的容器都实现了移动语义，所以使用了 list 容器的上述代码能够避免拷贝，提⾼性能。

#### 05.5 右值引用带来的完美转发

传统 C++ 中右值参数后被转换成左值，即不能按照参数原先的类型进⾏转发，如下所⽰：

![img](C++%E6%96%B0%E7%89%B9%E6%80%A7.assets/6009978-b1c4907141bc9ea8-163823116643922-163823116789524.png)

```c
template<typename T>
void forwardValue1(T& val) {
  // 右值参数变为左值
  processValue(val);
}

template<typename T>
void forwardValue1(const T& val) {
  processValue(val); // 参数都变成常量左值引用了
}
```

如何保持参数的左值、右值特征，C++11 引⼊了 std::forward，它将按照参数的实际类型进⾏转发：



```c
void processValue(int& a) {
  cout << "lvalue" << endl;
}

void processValue(int&& a) {
  cout << "rvalue" << endl;
}

template<typename T>
void forwardValue2(T&& val) {
  // 照参数本来的类型进⾏转发
  processValue(std::forward<T>(val));
}

int main() {
  int i = 0;

  forwardValue2(i); // 传入左值，函数执行输出 lvalue
  forwardValue2(0); // 传入右值，函数执行输出 rvalue

  return 0;
}
```

### 06 移动构造函数与移动赋值运算符

在规则 [#05](https://www.jianshu.com/p/8c4952e9edec#05) 已经提及，不再赘述。

### 07 有作用域枚举

传统 C++ 的枚举类型存在如下问题：

- 每⼀个枚举值在其作⽤域内都是可见，容易引起命名冲突



```c
// Color 下的 BLUE 和 Feeling 下的 BLUE 命名冲突
enum Color { RED, BLUE };
enum Feeling { EXCITED, BLUE };
```

- 会被隐式转换成 int，这在那些不该转换成 int 的场景下可能导致错误
- 不可指定枚举的数据类型，导致代码不易理解、不可进⾏前向声明等

在传统 C++ 中也有⼀些间接⽅案可以适当解决或缓解上述问题，例如使⽤**命名空间**：

```c
namespace Color { enum Type { RED, YELLOW, BLUE }; };
```

或使⽤类、结构体：

```c
struct Color { enum Type { RED, YELLOW, BLUE }; };
```

但上述⽅案通常值解决了**作用域问题**，隐式转换以及数据类型的问题⽆法解决。

C++11 引⼊了枚举类解决上述问题：

```c
// 定义枚举值为 char 类型的枚举类
enum class Color:char { RED, BLACK };

// 使⽤
Color c = Color::RED;
```

### 08 constexpr 与字面类型

**constexpr**: 在编译期将表达式或函数编译为常量结果

constexpr 修饰变量、函数：

```c
// 修饰变量
constexpr int a = 1 + 2 + 3;
char arr[a]; // 合法，a 是编译期常量

// 修饰函数，使函数在编译期会成为常量表达式（如果可以）
// 如果 constexpr 函数返回的值不能在编译器确定，则 constexpr 函数就会退化为运行期函数（这样做的初衷是避免在为编译期和运行期写两份相同代码）
// constexpr 函数的设计其实不够严谨，所以 C++20 引入了 consteval （详见下文 C++20 部分）
// C++11 中，constexpr 修饰的函数只能包含 using 指令、typedef 语句以及 static_assert 
// C++14 实现了对其他语句的支持
constexpr int len_foo_constexpr() {
  return 5;
}
```

### 09 初始化列表 - 扩展「初始化列表」的适用范围

在 C++98/03 中，普通数组或 **POD 类型** 可以通过初始化列表的⽅式进⾏初始化，例如：

> POD 类型见下文的 #18 条

```c
int arr1[3] = { 1, 2, 3 };

long arr2[] = { 1, 3, 2, 4 };
struct A { 
  int x;
  int y;
} a = { 1, 2 };
```

C++11 扩展了「初始化列表」的适⽤范围，使之可以适⽤于所有类型对象的初始化：

```c
class Dog {
 public:
  Dog(string name, int age) {
    cout << name << " "; cout << age << endl;
  }
};

Dog dog1 = {"cat1", 1};
Dog dog2 {"cat2", 2};
```

还可以通过 **std::initializer_list** 来实现更强⼤的「初始化列表」，例如：

```c
class Dog {
 public:
  Dog(initializer_list<int> list) {
   for (initializer_list<int>::iterator it = list.begin();
          it != list.end(); ++it) {
      cout << *it << " ";
    } 
    cout << endl;
  }
};

Dog dog3 = {1, 2, 3, 4, 5};
```

同时，初始化列表还可以⽤作普通函数的**形参**或**返回值**：

```c
// 形参
void watch(Dog dog) {
  cout << "watch" << endl;
}

watch({"watch_dog", 4});

// Dog 作为返回值
Dog getDefaultDog() {
  return {"default", 3};
}

getDefaultDog();
```

### 10 委托与继承的构造函数

**委托构造**：在⼀个构造函数中调⽤同⼀个类的另⼀个构造函数
**继承构造**：在 C++11 之前的 C++ 中，⼦类需要依次声明⽗类拥有的构造函数，并传递相应的初始化参数。C++11 利⽤关键字 using 引⼊了继承构造函数，使⽤⼀⾏语句让编译器⾃动完成上述⼯作。

```c
class Parent {
  public:
    int value1;
    int value2;

  Parent() {
    value1 = 1;
  }

  Parent(int value) : Parent() { // 委托 Parent() 构造函数
    value2 = value;
  }
}

class Child : public Parent {
  public: 
    using Parent::Parent;         // 继承构造
}
```

### 11 花括号或等号初始化器

上⽂已提及，不再赘述

### 12 nullptr

传统 C++ 中 NULL 的定义存在很多缺陷，编译器在实现时常常将其定义为 0，这会导致重载混乱。考虑如下代码；

```c
void foo(char*);
void foo(int);
```

当调⽤ foo(NULL) 时将匹配到 foo(int) 函数，这显然会让⼈感到迷惑。

C++11 引⼊了 **nullptr** （类型为 nullptr_t）关键字，以便区分空指针与 0，且 nullptr 能够隐式的转换为任何指针或成员指针的类型。

### 13 long long

**long**: ⽬标类型将有⾄少 **32 位**的宽度
**long long**: ⽬标类型将有⾄少 **64 位**的宽度

> 如同 long 类型后缀需要 「l」 或 「L」，long long 类型后缀需要加上「ll」或「LL」。

### 14 char16_t 与 char32_t

C++98 中为了表达 Unicode 字符串，引⼊了 wchar_t 类型，以解决 1 字节的 char 只能 256 个字符的问题。

但是由于 wchar_t 类型在不同平台上实现的长度不同，在代码移植⽅⾯有⼀定的影响。于是 C++11 引⼊ **char16_t** 和 **char32_t**，他们拥有的固定的长度，分别为 **2 个字节**和 **4 个字节**。

**char16_t**: UTF-16 字符表⽰的类型，要求⼤到⾜以表⽰任何 UTF-16 编码单元（ 16 位）。它与 `std::uint_least16_t` 具有相同的⼤⼩、符号性和对齐，但它是独⽴的类型。

**char32_t**: - UTF-32 字符表⽰的类型，要求⼤到⾜以表⽰任何 UTF-32 编码单元（ 32 位）。它与 `std::uint_least32_t` 具有相同的⼤⼩、符号性和对齐，但它是独⽴的类型。

同时 C++11 还定义了 3 个常量字符串前缀：

- u8 代表 UTF-8 编码
- u 代表 UTF-16 编码
- U 代表 UTF-32 编码



```c
char16_t UTF16[] = u"中国"; // 使用 UTF-16 编码存储

char32_t UTF16[] = U"中国"; // 使用 UTF-32 编码存储
```

### 15 类型别名

传统 C++ 中使⽤ typedef 来为类型定义⼀个新的名称，C++11 中我们可以使⽤ using 达到同样的效果，如下所⽰：

```c
typedef std::ios_base::fmtflags flags;
using flags = std::ios_base::fmtflags;
```

既然有了 typedef 为什么还引⼊ using？当然是因为 using ⽐起 typedef 还能做更多。

typedef 是只能为「类型」定义新名称，⽽模板则是 「**用来产生类型**」的，所以以下代码是⾮法的：

```c
template<typename T, typename U>
class DogTemplate {
  public: 
    T attr1;
    U aatr2;
};

// 不合法
template<typename T>
typedef DogTemplate<std::vector<T>, std::string> DogT;
```

但使⽤ using 则可以为模板定义别名：

```c
template<typename T>
using DogT = DogTemplate<std::vector<T>, std::string>;
```

### 16 变长参数模板

在传统 C++ 中，类模板或函数模板只能接受固定数量的模板参数。

**⽽ C++11 允许任意多个、任意类别的模板参数，同时在定义时⽆需固定参数个数**。如下所⽰：

```c
template<typename... T> class DogT;

// 传⼊多个不同类型的模板参数
class DogT<int, 
            std::vector<int>,
            std::map<std::string,
            std::vector<int>>> dogT;

// 不传⼊参数（ 0 个参数）
class DogT<> nothing;

// 第⼀个参数必传，之后为变⻓参数
template<typename require, typename... Args> class CatT;
```

同样的可⽀持模板函数：

```c
template<typename... Args>
void my_print(const std::string& str, Args... args) {
  // 使⽤ sizeof... 计算参数个数
  std::cout << sizeof...(args) << std::endl;
}
```

### 17 推广的（非平凡）联合体

联合体 Union 为我们提供了在⼀个结构内定义多种不同类型的成员的能⼒，但在传统 C++ 中，并不是所有的数据类型都能成为联合体的数据成员。例如:

```c
struct Dog {
  Dog(int a, int b) : age(a), size(b) {}
  int age;
  int size;
}

union T {
  // C++11 之前为非法（d 不是 POD 类型）
  // C++11 之后合法
  Dog d;
  int id;
}
```

有关 POD 类型参考下⽂的 #18 条

C++11 **去除**了上述联合体的限制**[3]**，标准规定了**任何⾮引⽤类型都可以成为联合体的数据成员**。

> **[3]** 去除的原因是经过长期的实践证明为了兼容 C 所做的限制没有必要。

### 18 推广的 POD （平凡类型与标准布局类型）

POD 为 Plain Old Data 的缩写，Plain 突出其为⼀种普通数据类型，Old 体现其具有与 C 的兼容性，例如可以使⽤ memcpy() 函数进⾏复制、使⽤ memset() 函数进⾏初始化等。

具体地，C++11 将 POD 划分为两个概念的合集：平凡的（trival）和标准布局的（standard layout）。

其中平凡的类或结构体应该符合如下要求：

1. 拥有平凡的默认构造函数和析构函数。即不⾃定义任何构造函数，或通过 =default 来显⽰指定使⽤默认构造函数
2. 拥有平凡的拷贝构造函数和移动构造函数
3. 拥有平凡的拷贝赋值运算符和移动赋值运算符
4. 不包含虚函数以及虚基类

C++11 同时提供了辅助类模板 is_trivial 来实现是否平凡的判断：

```c
cout << is_trivial<DogT>::value << endl;
```

POD 包含的另⼀个概念则是「标准布局」。标准布局的类或结构体需要符合如下要求：

1. 所有⾮静态成员有相同的访问权限（public、private、protected）

2. 类或结构体继承时满⾜如下两个条件之⼀：

   2.1 ⼦类中有⾮静态成员，且只有⼀个仅包含静态成员的基类

   ```c
   struct B1 { static int a; };
   struct B2 { static int b; };
   ```
   
   2.2 基类有⾮静态成员，则⼦类没有⾮静态成员

   ```c
   struct B2 { int a; } ;
   struct D2 : B2 { static int d; };
   ```
   
   从上述条件可知，1. 只要⼦类和基类同时都有⾮静态成员 2. ⼦类继承多个基类，有多个基类同时有⾮静态成员。 这两种情况都不属于标准布局。
   
3. 类中第⼀个⾮静态成员的类型与其基类不同

```c
struct A : B { B b; };        // 非标准布局，第一个非静态成员 b 就是基本类型
struct A : B { int a; B b; }; // 标准布局，第一个非静态成员 a 不是基类 B 类型
```

1. 没有虚函数或虚基类
2. 所有⾮静态数据成员均符合标准布局类型，其基类也符合标准布局（递归定义）

同样 C++11 提供了辅助类模板 is_standard_layout 帮助我们判断：

```c
cout << is_standard_layout<Dog>::value << endl;
```

最后，C++11 也提供了⼀次性判断是否为 POD 的辅助类模板 is_pod:

```c
cout << is_pod<Dog>::value << endl;
```

了解 POD 的基本概念，POD 到底有怎样的作⽤或好处呢？POD 能够给我们带来如下优点：

1. 字节赋值。安全的使⽤ memset 和 memcpy 对 POD 类型进⾏初始化和拷贝等操作
2. 兼容 C 内存布局。以便与 C 函数进⾏互操作
3. 保证静态初始化的安全。⽽静态初始化可有效提⾼程序性能

### 19 Unicode 字符串字面量

在 #14 已有所提及，C++11 定义了 3 个常量字符串前缀：

- u8 代表 UTF-8 编码
- u 代表 UTF-16 编码
- U 代表 UTF-32 编码

另外 C++11 还引⼊了⼀个字符串前缀 R 表⽰ 「**原生字符串字面量**」，所谓「原⽣字符串字⾯量」即表⽰字符串⽆需通过转义处理特殊字符，所见即所得：

```c
// ⽤法： R"分隔符 (原始字符 )分隔符"
string path = R"(D:\workspace\vscode\java_demo)";

// - 作为分隔符，
// 因为原始字符串含有 )"，如果不添加 - 作为分隔符，则会导致字符串错误标示结束位置
// 分隔符应该尽量使用原始字符串中未出现的字符，以便正确标示开始与结尾
string path2 = R"-(a\b\c)"\daaa\e)-";
```

### 20 用户定义字面量

⽤户定义字⾯量即⽀持⽤户定义类型的字⾯量。

传统 C++ 提供了多种字⾯量，例如 "12.5" 为⼀个 double 类型字⾯量。"12.5f" 为⼀个 float 类型字⾯量。这些字⾯量是 C++ 标准中定义和规定的字⾯量，程序和⽤户⽆法⾃定义新的**字⾯量类型**或**后缀**。

C++11 则是引⼊了⽤户⾃定义字⾯量的能⼒。主要通过定义「字⾯量运算符函数」或函数模板实现。该运算符名称由⼀对相邻双引号前导。字⾯量运算符通常在⽤户定义字⾯量的地⽅被隐式调⽤。例如：

```c
struct S {
  int value;
};

// 用户定义字面量运算符的实现
S operator ""_mysuffix(unsigned long long v) {
  S s_;
  S_.value = (int) v;
  return s_;
}

// 使用
S sv;
// 101 为类型为 S 的字面量
// _mysuffix 是我们自定义的后缀，如同 float 的 f 一般
sv = 101_mysuffix;
```

⽤户⾃定义字⾯量通常由以下⼏种类型：

1. 数值型字面量
   1.1 整数型字面量
   1.2 浮点型字面量

```c
OutputType operator "" _suffix(unsigned long long);
OutputType operator "" _suffix(long double);
 
// Uses the 'unsigned long long' overload.
OutputType some_variable = 1234_suffix;
// Uses the 'long double' overload.
OutputType another_variable = 3.1416_suffix; 
```

2. 字符串字面量

```c
OutputType operator "" _ssuffix(const char     * string_values, size_t num_chars);
OutputType operator "" _ssuffix(const wchar_t  * string_values, size_t num_chars);
OutputType operator "" _ssuffix(const char16_t * string_values, size_t num_chars);
OutputType operator "" _ssuffix(const char32_t * string_values, size_t num_chars);

// Uses the 'const char *' overload.
OutputType some_variable =   "1234"_ssuffix; 
// Uses the 'const char *' overload.
OutputType some_variable = u8"1234"_ssuffix;
// Uses the 'const wchar_t *'  overload. 
OutputType some_variable =  L"1234"_ssuffix; 
// Uses the 'const char16_t *' overload.
OutputType some_variable =  u"1234"_ssuffix; 
// Uses the 'const char32_t *' overload.
OutputType some_variable =  U"1234"_ssuffix; 
```

3. 字符字面量

```c
S operator "" _mysuffix(char value) {
  const char cv[] {value,'\0'};
  S sv_ (cv);
  return sv_;
}

S cv {'h'_mysuffix};
```

> 尽整些花里胡哨的特性

### 21 属性

C++11 引⼊了所谓的 「**属性**」来让程序员在代码中提供额外信息，例如：

```c
// f() 永不返回
void f [[ noreturn ]] () {
  throw "error";  // 虽然不能返回，但可以抛出异常
}
```

上述例⼦的展现了属性的基本形式，**noreturn** 表⽰该函数永不返回。

C++11 引⼊了两个属性：

|        属性        | 版本  |  修饰⽬标  |                             作⽤                             |
| :----------------: | :---: | :--------: | :----------------------------------------------------------: |
|      noreturn      | C++11 |    函数    | 指⽰函数不返回，没有return语句，不正常执⾏完毕，但是可以通过出异常或 者exit()函数退出 |
| carries_dependency | C++11 | 函数、变量 |  指⽰释放消费 std::memory_order 中的依赖链传⼊和传出该函数   |

> 概念与功能与 Java 中的注解有些类似

### 22 Lambda 表达式

Lambda 表达式基本语法：

```c
// [捕获列表]：捕获外部变量，详见下文
// (参数列表): 函数参数列表
// mutable: 是否可以修改值捕获的外部变量
// 异常属性：exception 异常声明
[捕获列表](参数列表) mutable( 可选 ) 异常属性 -> 返回类型 {
  // 函数体
}
```

例如：

```c
bool cmp(int a, int b) {
  return a < b;
}

int main() {
  int x = 0;
  // 传统做法
  sort(vec.begin(), vec.end(), cmp);

  // 使用 lambda
  sort(vec.begin(), vec.end(), [x](int a, int b) -> bool { return a < b; });
  return 0;
}
```

lambda 表达式中的「捕获列表」可以让 lambda 表达式内部使用其可见范围的外部变量，例如上例中的 `x`。捕获列表一般有以下几种类型：
**1. 值捕获**
与参数传递中值传递类似，被捕获的变量以值拷贝的方式传入：

```c
int a = 1;
auto f1 = [a] () { a+= 1; cout << a << endl;};

a = 3;
f1();

cout << a << endl;
```

**2. 引用捕获**
加上 & 符号，即可通过引用捕获外部变量：

```c
int a = 1;
// 引用捕获
auto f2 = [&a] () { cout << a << endl; };

a = 3;
f2();
```

**3. 隐式捕获**
无需显示列出所有需要捕获的外部变量，通过 `[=]` 可以通过「值捕获」的方式捕获所有外部变量，`[&]` 可以通过「引用捕获」的方式捕获所有外部变量：

```c
int a = 1;
auto f3 = [=] { cout << a << endl; };    // 值捕获
f3(); // 输出：1

auto f4 = [&] { cout << a << endl; };    // 引用捕获
a = 2;
f4(); // 输出：2
```

**4. 混合方式**
以上方式的混合，`[=, &x]` 表示变量 x 以引用形式捕获，其余变量以传值形式捕获。

最终 lambda 捕获外部变量总结如下表所示：

|  捕获形式   |                             说明                             |
| :---------: | :----------------------------------------------------------: |
|     []      |                      不捕获任何外部变量                      |
| [变量名, …] | 默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符） |
|   [this]    |                    以值的形式捕获this指针                    |
|     [=]     |                  以值的形式捕获所有外部变量                  |
|     [&]     |                  以引用形式捕获所有外部变量                  |
|   [=, &x]   |         变量x以引用形式捕获，其余变量以传值形式捕获          |
|   [&, x]    |         变量x以值的形式捕获，其余变量以引用形式捕获          |

### 23 noexcept 说明符与 noexcept 运算符

C++11 将异常的声明简化为以下两种情况：

1. 函数可能抛出任何异常

```c
void func(); // 可能抛出异常
```

1. 函数不可能抛出任何异常

```c
void func() noexcept; // 不可能抛出异常
```

使⽤ **noexcept** 能够让编译器更好的优化代码，同时 **noexcept** 修饰的函数如果抛出异常将会导致调⽤ `std::terminate()` ⽴即终⽌程序。

noexcept 还可作为运算符使⽤，来判断⼀个表达式是否产⽣异常：

```c
cout << noexcept(func()) << endl;
```

### 24 alignof 与 alignas

C++11 引⼊了 **alignof** 和 **alignas** 来实现对内存对齐的控制。

**alignof**: 能够获取对齐⽅式
**alignas**: ⾃定义结构的对齐⽅式：

```c
struct A {
  char a;
  int b;
};

struct alignas(std::max_align_t) B {
  char a;
  int b;
  float c;
};

cout << alignof(A) << endl;
cout << alignof(B) << endl;
```

### 25 多线程内存模型

请参见 [LevelDB 中的跳表实现](https://www.jianshu.com/p/a25e0754e2b9) 中的 「**C++ 中的 atomic 和 memory_order**」一节。

### 26 线程局部存储

在多线程程序中，全局以及静态变量会被多个线程共享，这在某些场景下是符合期望和需求的。

但在另⼀些场景下，我们希望能有线程级的变量，这种变量是线程独享的，不受其他线程影响。我们称之为**线程局部存储（TLS， thread local storage）**。

C++11 引⼊了 **thread_local** ⽤来声明线程局部存储，如下所⽰：

```c
int thread_local num;
```

### 27 GC 接口

众所周知 C++ 是⼀门显式堆内存管理的语⾔，程序员需要时时刻刻关注⾃⼰对内存空间的分配和销毁。 ⽽如果程序员没有正确进⾏堆内存管理，就会造成程序的异常、错误、崩溃等。从语⾔层⾯是讲，这些不正确的内存管理主要有：

- **野指针**：内存已经被销毁，但指向它的指针依然被使⽤
- **重复释放**：释放已经被释放过的内存，或者释放被重新分配过的内存，导致重复释放错误
- **内存泄漏**：程序中不再需要的内存空间却没有被及时释放，导致随着程序不断运⾏内存不断被⽆谓消耗

显式内存管理可以为程序员提供极⼤的编程灵活性，但也提⾼了出错的概率。为此，C++11 进⼀步改造了智能指针，同时也提供了⼀个 **「最小垃圾回收」**的标准。

⽬前⾮常多的现代语⾔都全⾯⽀持「垃圾回收」，例如 Java、Python、C#、Ruby、PHP 等都⽀持「垃圾回收」。 为实现「垃圾回收」，最重要的⼀点就是**判断对象或内存何时能够被回收**。判断对象或内存是否可回收的⽅法主要有：

1. **引用计数**
2. **跟踪处理（跟踪对象关系图）**。如 Java 中的「**对象可达性分析**」。

确定了对象或内存可被回收后，就需要进⾏回收，⽽这⾥又存在不同的回收策略和回收算法（简单描述）：

1. **标记-清除**
   第⼀步对对象和内存进⾏标记是否可回收，第⼆步对标记的内存进⾏回收。显然这种⽅法将导致⼤量的内存碎⽚
2. **标记-整理**
   第⼀步同样是标记。但是第⼆步不是直接清理，⽽是将「活对象」向左靠齐（整理）。但移动⼤量对象，将导致程 序中的引⽤需要进⾏更新。如果对象死亡的⽐较多，就要进⾏⽐较多的移动操作。所以适合「长寿」的对象。
3. **复制算法**。将堆空间分为两个部分：**from** 和 **to**。from 空间⽤满后启动扫描标记，找出其中活着的对象，将其复制到 to 空间， 然后清空 from 空间。之后原先 to 变成了 from 空间供程序分配内存，原先的 from 变成 to，等待下⼀次垃圾回收收容那些「幸存者」。如果有⼤量幸存者，那么拷贝将导致较⼤性能消耗。因此适合短寿「**朝生暮死**」的对象。

⽽在实现时通常采⽤「分代收集」算法，即将堆空间分为 **「新生代」**和 **「老年代」**，新⽣代朝⽣暮死适合「拷贝算法」，老年代长寿适合「标记清理」或「标记整理」。

上述介绍了垃圾回收的相关算法，C++11 则是制定了「最⼩垃圾回收」的标准，所谓「最⼩」指的其实就是它压根就不是⼀个完整的 GC，而是为了后续的 GC 铺垫，⽬前也只是提供了⼀些库函数来辅助 GC，如：
declare_reachable(声明⼀个对象不能被回收)、undeclare_reachable(声明⼀个对象可以被回收)。

由于 C++ 中的指针⼗分灵活，这种灵活性将导致 GC 误判从⽽回收内存，因此提供这两个函数保护对象：

```c
int* p1 = new int(1);
p1 += 10;             // 将导致 GC 回收内存空间
p1 -= 10;             // 指针的灵活性：又移动回来了
*p1 = 10;             // 内存已被回收，导致程序错误

// 使用 declare_reachable 保护对象不被 GC
int* p2 = new int(2);
declare_reachable(p2); // p2 不可回收

p2 += 10;              // GC 不会回收
p2 -= 10;

*p2 = 10;              // 程序正常
```

从上可知，这两个函数就是为旧程序兼容即将到来**[4]**的 C++ GC 而设计的。

> **[4]** 看样子是不会到来了。

上述介绍了这么多，最后再来介绍最尴尬的⼀点：**现在还没有编译器实现 C++11 有关 GC 的标准**。

> 可以暂时忽略这条 GC 特性，实际上 C++ 的很多特性都可以忽略

### 28 范围 for

类似 Java 中的 foreach 循环

```c
std::vector<int> vec = {1, 2, 3, 4};
for (auto element : vec) {
  std::cout << element << std::endl;  // read only
}
```

### 29 static_assert

我们常⽤ assert，即运⾏时断⾔。但很多事情不该在运⾏时采取判断和检查，而应该在编译期就进⾏严格断⾔，例如数组的长度等。

C++11 引⼊了 static_assert 实现编译期断⾔：

```c
static_assert(sizeof(void *) == 4,"64位系统不支持");
```

### 30 智能指针

C++98 提供了模板类型「auto_ptr」来实现智能指针。auto_ptr 以对象的⽅式管理分配的内存，并在适当的时机释放内存。程序员只需要将 new 操作返回的指针作为 auto_ptr 的初始值即可，如下所⽰：



```c
auto_ptr(new int);
```

但 auto_ptr 存在「进⾏拷贝时会将原指针置为 NULL」等缺陷，因此 C++11 引⼊了 **unique_ptr、shared_ptr、 weak_ptr** 三种智能指针。

- **unique_ptr**: unique_ptr 和指定对象的内存空间紧密绑定，不允许与其他 unique_ptr 指针共享同⼀个对象内存。即**内存所有权在同⼀个时间内是唯⼀的**，但所有权却可以通过 #05 条中提及的 move 和移动语义进⾏来实现「所有权」 转移。如下所⽰：

```c
unique_ptr<int> p1(new int(111));

unique_ptr<int> p2 = p1;        // ⾮法，不可共享内存所有权
unique_ptr<int> p3 = move(p1);  // 合法，移交所有权。p1 将丧失所有权

p3.reset();                     // 显式释放内存
```

- **shared_ptr**：与 unique_ptr 相对，可以共享内存所有权，即多个 shared_ptr 可以指向同⼀个对象的内存。同时 shared_ptr 采⽤**引⽤计数法**来判断内存是否还被需要，从⽽判断是否需要进⾏回收。

```c
shared_ptr<int> p4(new int(222));
shared_ptr<int> p5 = p4;  // 合法

p4.reset();               // 「释放」内存

// 由于采⽤引⽤计数法，p4.reset() 仅仅使得引⽤数减⼀
// 所指向的内存由于仍有 p5 所指向，所以不会被回收
// 访问 *p5 是合法且有效的
cout << *p5 << endl;      // 输出 222
```

- **weak_ptr**：weak_ptr 可以指向 shared_ptr 指向的内存，且在必要时可以通过成员 lock 来返回⼀个指向当前内存的 shared_ptr 指针，如果当前内存已经被释放，那么将 lock() 返回 nullptr。⽽另⼀个重点则是 weak_ptr **不参与引⽤计数**。如同⼀个「虚拟指针」⼀样指向 shared_ptr 指向的对象内存，⼀⽅⾯不妨碍内存的释放，另⼀⽅⾯又可以通过 weak_ptr 判断内存是否有效以及是否已经被释放：

```c
shared_ptr<int> p6(new int(333));
shared_ptr<int> p7 = p6;
weak_ptr<int> weak_p8 = p7;

shared_ptr<int> p9_from_weak_p8 = weak_p8.lock();

if (p9_from_weak_p8 != nullptr) {
  cout << "内存有效" << endl;
} else {
  cout << "内存已被释放" << endl;
}

p6.reset();
p7.reset(); // weak_p8

// 内存已被释放，即使 weak_p8 还「指向」该内存
```

weak_ptr 还有⼀个⾮常重要的应⽤并是解决 shared_ptr 引⽤计数法所带来的 **「循环引用」**问题。所谓「循环引⽤」 如下图所示：

![img](C++%E6%96%B0%E7%89%B9%E6%80%A7.assets/6009978-d824fd439f8feec7.png)

由于 ObjA 和 ObjB 内部有成员变量相互引⽤，即使将 P1 和 P2 引⽤去除，这两个对象的引⽤计数仍然不为 0。但实际上两个对象已经不可访问，理应被回收。

使⽤ weak_ptr 来实现上⾯两个对象的相互引⽤则可以解决该问题，如下图所⽰：

![img](C++%E6%96%B0%E7%89%B9%E6%80%A7.assets/6009978-d057dfeb9d25e49f.png)

将 P1 和 P2 引⽤去除，此时 ObjA 和 ObjB 内部是通过 weak_ptr 相互引用的，由于 **weak_ptr 不参与引用计数**，因此 ObjA 和 ObjB 的引用计数被判断为 0，ObjA 和 ObjB 将被正确回收。

## C++14 新特性

### 01 变量模板

我们已经有了类模板、函数模板，现在 C++14 为我们带来了变量模板：

```c
template<class T>
constexpr T pi = T(3.1415926535897932385);

int main() {
  cout << pi<int> << endl;

  cout << pi<float> << endl;

  cout << pi<double> << endl;

  return 0;
}

// 当然在以前也可以通过函数模板来模拟
// 函数模板
template<class T>
constexpr T pi_fn() {
  return T(3.1415926535897932385);
}
```

### 02 泛型 lambda

所谓「泛型 lambda」，就是在形参声明中使用 auto 类型指示说明符的 lambda。例如：

```c
auto lambda = [](auto x, auto y) { return x + y; };
```

### 03 lambda 初始化捕获

C++11 lambda 已经为我们提供了值捕获和引⽤捕获，但针对的实际都是左值，⽽**右值对象⽆法被捕获**，这个问题在 C++14 中得到了解决:

```c
int a = 1;
auto lambda1 = [value = 1 + a] {return value;};

std::unique_ptr ptr(new int(10));

// 移动捕获
auto lambda2 = [value = std::move(ptr)] {return *value;};
```

### 04 new/delete elision

> 不知怎么翻译好，new/delete 消除？new/delete 省略？
> 在 [cppreference c++14](https://links.jianshu.com/go?to=https%3A%2F%2Fzh.cppreference.com%2Fw%2Fcpp%2F14) 列出了这条，但没有详细说明。

由于 C++14 新提供了 make_unique 函数，unique_ptr 可在析构是自动删除，再加上 make_shared 和 shared_ptr，基本可以覆盖大多数场景和需求了。所以从 C++14 开始， new/delete 的使用应该会大幅度减少。

### 05 constexpr 函数上放松的限制

在 C++11 的 #08 条中已经提及 constexpr 修饰的函数除了可以包含 using 指令、typedef 语句以及 static_assert 断⾔ 外，只能包含⼀条 return 语句。

⽽ C++14 则放开了该限制，constexpr 修饰的函数可包含 **if/switch 等条件语句，也可包含 for 循环**。

### 06 ⼆进制字面量

C++14 的数字可⽤⼆进制形式表达，前缀使⽤ `0b` 或 `0B`。

```c
int a = 0b101010; // C++14
```

### 07 数字分隔符

使⽤单引号 `'` 来提⾼数字可读性：

```c
auto integer_literal = 100'0000;
```

> GC、模块、协程等重大特性唯唯诺诺，可有可无的特性 C++ 重拳出击！

### 08 函数的返回类型推导

上文提及了 C++11 中使用 auto/decltype 配合尾置返回值实现了函数返回值的推导，C++14 实现了一个 auto 并自动推导返回值类型：

```c
auto Func(); // 返回类型由编译器推断
```

### 09 带默认成员初始化器的聚合类

C++11 增加了默认成员初始化器，如果构造函数没有初始化某个成员，并且这个成员拥有默认成员初始化器，就会⽤默认成员初始化器来初始化成员。

而在 C++11 中，聚合类（aggregate type）的定义被改为「明确排除任何含有默认成员初始化器」的类型。

因此，在 C++11 中，如果⼀个类含有默认成员初始化器，就不允许使⽤聚合初始化。C++14放松了这⼀限制：

```c
struct CXX14_aggregate {
  int x;
  int y = 42;  // 带有默认成员初始化器
};

// C++11 中不允许
// 但 C++14允许 且 a.y 将被初始化为42
CXX14_aggregate a = { 1 }; 
```

### 10 decltype(auto)

> 允许 auto 的类型声明使⽤ decltype 的规则。也即，允许不必显式指定作为decltype参数的表达式，而使用decltype对于给定表达式的推断规则。
> —— From [Wikipedia C++14](https://links.jianshu.com/go?to=https%3A%2F%2Fzh.wikipedia.org%2Fwiki%2FC%2B%2B14)

看一个例子：

```c
// 在另一个函数中对下面两个函数进行转发调用
std::string  lookup1();
std::string& lookup2();

// 在 C++11 中，需要这么实现
std::string look_up_a_string_1() {
    returnlookup1();
}
std::string& look_up_a_string_2() {
    returnlookup2();
}

// 在 C++14 中，可以通过 decltype(auto) 实现
decltype(auto) look_up_a_string_1() {
    return lookup1();
}
decltype(auto) look_up_a_string_2() {
    return lookup2();
}
```

## C++17 新特性

### 01 折叠表达式

上文介绍了 C++11 中介绍了「变长参数模板」(C++11 第 #16 条)。在 C++11 中对变长参数进行展开比较麻烦，通常采用递归函数的方式进行展开：

```c
void print() {  // 递归终止函数
   cout << "last" << endl;
}

template <class T, class ...Args>
void print(T head, Args... rest) {
   cout << "parameter " << head << endl;
   print(rest...); // 递归展开 rest 变长参数
}
```

C++17 引入「折叠表达式」来进一步支持变长参数的展开：

```c
// ⼀元左折叠
// 只有一个操作符 「-」，且展开符 ... 位于参数包 args 的左侧，固为一元左折叠
template<typename... Args>
auto sub_val_left(Args&&... args) {
  return (... - args);
}
auto t = sub_val_left(2, 3, 4);   // ((2 - 3) - 4) = -5;

// 一元右折叠
// 只有一个操作符 「-」，且展开符 ... 位于参数包 args 的右侧，固为一元右折叠
template<typename... Args>
auto sub_val_right(Args&&... args) {
  return (args - ...);
}
auto t = sub_val_right(2, 3, 4);  // (2 - (3 - 4)) = 3;

// 二元左折叠
// 左右有两个操作符 ，且展开符 ... 位于参数包 args 的左侧，固为二元左折叠
template<typename... Args>
auto sub_one_left(Args&&... args) {
  return (1 - ... - args);
}
auto t  = sub_one_left(2, 3, 4);  // ((1 - 2) - 3) - 4 = -8

// 二元右折叠
// 左右有两个操作符，且展开符 ... 位于参数包 args 的右侧，固为二元右折叠
template<typename... Args>
auto sub_one_right(Args&&... args) {
  return (args - ... - 1);        
}
auto t  = sub_one_right(2, 3, 4); //  2 - (3 - (4 - 1)) = 2
```

### 02 类模板实参推导

C++17 之前类模板⽆法进⾏参数推导：

```c
std::pair<int, string> a{ 1, "a"s }; // 需要指明 int, string 类型
```

C++17 实现了类模板的实参类型推导：

```c
std::pair a{ 1, "a"s }; // C++17，类模板可自行推导实参类型
```

### 03 auto 占位的⾮类型模板形参

```c
template<auto n> struct B { /* ... */ }

B<5> b1;    // OK: 非类型模板形参类型为 int
B<'a'> b2;  // OK: 非类型模板形参类型为 char
B<2.5> b3;  // 错误（C++20前）：非类型模板形参类型不能是 double
```

### 04 编译期的 constexpr if 语句

C++17 将 constexpr 这个关键字引⼊到 if 语句中，允许在代码中声明常量表达式的判断条件

```c
template<typename T>
auto print_info(const T& t) {
  if constexpr (std::is_integral<T>::value) {
    return t + 1;
  } else {
    return t + 1.1;
  }
}
```

上述代码将在**编译期**进行 if 语句的判断，从而在**编译期**选定其中一条分支。

### 05 内联变量（inline 变量）

看一个例子：

```c
// student.h
extern int age;  // 全局变量

struct  Student {
   static int age;  // 静态成员变量
};

// student.cpp
int age = 18;
int Student::foo = 18;
```

在 C++17 之前，如果想要使用全局变量或类的静态成员变量，需要在头文件中声明，然后在每个 cpp 文件中定义。

C++17 支持声明**内联变量**达到相同的效果：

```c
// student.h
inline int age = 18;

struct Student {
   static inline int age = 18;
};
```

### 06 结构化绑定

> 类似于 JavaScript 中的解构赋值

⽰例：

```c
tuple<int, double, string> f() {
  return make_tuple(1, 2.3, "456");
}

int main() {
  int arr[2] = {1,2};
  // 创建 e[2]
  // 复制 arr 到 e, 然后 a1 指代 e[0], b1 指代 e[1]
  auto [a1, b1] = arr;
  cout << a1 << ", " << b1 << endl;

  // a2 指代 arr[0], b2 指代 arr[1]
  auto& [a2, b2] = arr;
  cout << a2 << "," << b2<< endl;

  // 结构化绑定 tuple
  auto [x, y, z] = f();
  cout << x << ", " << y << ", " << z << endl;

  return 0;
}
```

### 07 if/switch 语句的变量初始化

if/switch 语句声明并初始化变量，形式为：if (init; condition) 和 switch (init; condition)。例⼦：

```c
for (int i = 0; i < 10; i++) {
  // int count = 5; 这条初始化语句直接写在 if 语句中
  if (int count = 5; i > count) {
    cout << i << endl;
  }
}

// char c(getchar()); 这条初始化语句直接写在 switch 语句中
switch (char c(getchar()); c) {
  case 'a': left(); break;
  case 'd': right(); break;
  default: break;
}
```

### 08 u8-char

字符前缀：

```c
u8'c-字符' // UTF-8 字符字面量
```

> 注意和上文的「字符串前缀」相区分，C++11 引入的 u8 是**字符串**前缀，C++17 补充 u8 可作为**字符**的前缀。

### 09 简化的嵌套命名空间

```c
namespace X { namespace Y { … }}  // 传统
namespace X::Y { … }              // C++17 简化命名空间
```

### 10 using 声明语句可以声明多个名称

```c
struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};
struct B {
    void f(double) {cout << "B::f(double)" << endl;}
};
struct S : A, B {
    using A::f, B::f; // C++17
};
```

### 11 将 noexcept 作为类型系统的一部分

与返回类型相似，异常说明成为函数类型的一部分，但**不是函数签名的一部分**：

```c
// 下面函数是不同类型函数，但拥有相同的函数签名
void g() noexcept(false);
void g() noexcept(true);
```

### 12 新的求值顺序规则

在 C++17 之前，为了满足各个编译器在不同平台上做相应的优化，C++ 对一些求值顺序未做严格规定。最典型的例子如下：

```c
cout << i << i++;  // C++17 之前，未定义行为
a[i] = i++;              // C++17 之前，未定义行为
f(++i, ++i);           // C++17 之前，未定义行为
```

具体的，C++17 规定了以下求值顺序：

- a.b
- a->b
- a->*b
- a(b1, b2, b3)
- b @= a
- a[b]
- a << b
- a >> b

顺序规则为：**a 的求值和所有副作用先序于 b，但同一个字母的顺序不定**

### 13 强制的复制消除（guaranteed copy elision）

C++17 引入「强制的复制消除」，以便在**满足一定条件下**能够**确保**消除对象的复制。

在 C++11 之前已经存在所谓的复制消除技术（copy elision），即编译器的返回值优化 RVO/NRVO。

> RVO(return value optimization): 返回值优化
> NRVO(named return value optimization)：具名返回值优化

看下面的例子：

```c
T Func() {
  return T();
}
```

在传统的复制消除（copy elision）规则下，上述代码将会产生一个临时对象，并将其拷贝给「返回值」。这个过程可能会被优化掉，也就是拷贝/移动函数根本不会被调用。但程序还是必须提供相应的拷贝函数。

再看如下代码：

```c
T t = Func();
```

上述代码会将返回值拷贝给 t，这个拷贝操作依然可能被优化掉，但同样的，程序依然需要提供相应的拷贝函数。

从上文可知，在传统的复制消除规则下，下面代码是非法的：

```c
// 传统的复制消除即使优化了拷贝函数的调用
// 但还是会检查是否定义了拷贝函数等
struct T {
    T() noexcept = default;
    T(const T&) = delete; // C++11 中如果不提供相应的拷贝函数将会导致 return 与 赋值错误
    T(T&&) = delete;
};

T Func() {
  return T();
}

int main() {
  T t = Func();
}
```

而「强制复制消除」对于纯右值 prvalue**[5]**，将会真正消除上述复制过程**[6]**，也不会检查是否提供了拷贝/移动函数，所以上述代码在 C++17 中是合法的。

> **[5]** 在 C++17 之前，纯右值为临时对象，而 C++17 对纯右值 prvalue 的定义进行了扩展：能够产生临时对象但还未产生临时对象的表达式，如上例代码中的 Func();
> **[6]** 消除的原理：在满足「纯右值赋值给泛左值」这个条件时，T t = Func(); 会被优化成类似于 T t = T(); 这中间不会产生临时对象。

但另一方面，对于「具名临时对象」，不会进行「强制复制消除」：

```c
T Func() {
   T t = ...;
   ...
   return t;
}
```

T 还是必须提供拷贝/移动函数，所以 C++17 对于具名返回值优化 NRVO (named return value optimization) 没有变化。

关于强制复制消除，可以参考下面链接的第一个回答，回答的很清楚：
[How does guaranteed copy elision work?](https://links.jianshu.com/go?to=https%3A%2F%2Fstackoverflow.com%2Fquestions%2F38043319%2Fhow-does-guaranteed-copy-elision-work)

> 这一切是否来源于 C++ 的初始设计问题： = 运算符的默认重载，赋予了 = 运算符对象拷贝的语义。

### 14 lambda 表达式捕获 *this

```c
#include <iostream>
 
struct Baz {
  auto foo() {
    // 通过 this 捕获对象，之后在 lambda 即可访问对象的成员变量 s
    return[this]{ std::cout << s << std::endl; };
  }
 
  std::string s;
};
 
int main() {
  auto f1 = Baz{ "ala" }.foo();
  auto f2 = Baz{ "ula" }.foo();
  f1();
  f2();
}
```

但上述代码存在一个缺陷：捕获的是当前对象，如果 lambda 表达式对成员变量的访问超出了当前对象的生命周期，就会导致问题。

C++17 提供了 `*this` 捕获当前对象的**副本**：

```c
auto foo() {
  return[*this]{ std::cout << s << std::endl; };
}
```

### 15 constexpr 的 lambda 表达式

C++17 的 lambda 声明为 constexpr 类型，这样的 lambda 表达式可以用在其他需要 constexpr 类型的上下文中。

```c
int y = 32;

auto func = [y]() constexpr {
  int x = 10;

  return y + x;
};
```

### 16 属性命名空间不必重复

在上文的 C++11 #21 条中已经介绍了属性的概念，对于由实现定义的行为的非标准属性，可能会带有命名空间：

```c
[[gnu::always_inline, gnu::const, gnu::hot, nodiscard]]
inline int f(); // 声明 f 带四个属性

[[gnu::always_inline, gnu::const, gnu::hot, nodiscard]]
int f(); // 同上，但使用含有四个属性的单个属性说明符
```

C++11 中上述属性的命名空间需要重复声明，C++17 简化了属性命名空间的定义：

```c
[[using gnu : const, always_inline, hot]] [[nodiscard]]
int f[[gnu::always_inline]](); // 属性可出现于多个说明符中
```

### 17 新属性 [[fallthrough]] [[nodiscard]] 和 [[maybe_unused]]

C++11 仅自带了两个标准属性，C++17 继续扩展了几个标准属性。

**fallthrough**

```c
// 以下代码因为没有 case 中没有 break;
// 所以将会发生 case 穿透
// 编译时编译器将会发出警告
int x = 2;
switch (x) {
  case 2:
    result++;
  case 0:
    result++;
  default:
    result++;
}

// 有时候我们需要 case 穿透，如匹配到 2 就一直执行后续的 case
// 此时可以使用属性 [[fallthrough]]，使用后，编译器将不会发出警告
switch (x) {
    case 2:
      result++;
      [[fallthrough]];  // Added
    case 0:
      result++;
      [[fallthrough]];  // Added
    default:
      result++;
  }
```

**nodiscard**
在开发过程中经常需要对函数返回值进行检查，这一步骤在不少业务场景下是必须的，例如：

```c
// 许多人会遗漏对返回值进行检查的步骤
// 导致了很多业务层面潜在的缺陷
if (CallService() != ret) {
  // ... 
}

// C++17 引入 [[nodiscard]] 属性来「提醒」调用者检查函数的返回值
[[nodiscard]] int CallService() {
  return CallServiceRemote();
}

CallService();              // 如果只调用而不检查，编译器将发出警告
if (CallService() != ret) { // pass
  // ...
}
```

**maybe_unused**
如果我们以 -Wunused 与 -Wunused-parameter 编译以下代码，编译器则可能报出警告：

```c
int test(int a, int b, int c) {
  int result = a + b;

#ifdef ENABLE_FEATURE_C
  result += c;
#endif
  return result;
}
```

原因是编译器认为 c 是未用到的变量，但实际上并非无用。C++17 中可以使用 [[maybe_unused]] 来抑制「针对未使用实体」的警告:

```c
int test(int a, int b, [[maybe_unused]] int c) {
  int result = a + b;

#ifdef ENABLE_FEATURE_C
  result += c;
#endif
  return result;
}
```

### 18 __has_include

表明指定名称的头或源文件是否存在：

```c
#if __has_include("has_include.h")
  #define NUM 1
#else
  #define NUM 0   
#endif
```

## C++20 新特性

### 01 特性测试宏

为 C++11 和其后所引入的 C++ 语言和程序库的功能特性定义了一组预处理器宏。使之成为检测这些功能特性是否存在的一种简单且可移植的方式。例如：

```c
__has_cpp_attribute(fallthrough)     // 判断是否支持 fallthrough 属性
#ifdef __cpp_binary_literals              // 检查「二进制字面量」特性是否存在 
#ifdef __cpp_char8_t                          // char8 t
#ifdef __cpp_coroutines                     // 协程
// ...
```

### 02 三路比较运算符 <=>

```c
// 若 lhs < rhs 则 (a <=> b) < 0
// 若 lhs > rhs 则 (a <=> b) > 0
// 而若 lhs 和 rhs 相等/等价则 (a <=> b) == 0

lhs <=> rhs
```

### 04 范围 for 中的初始化语句和初始化器

C++17 引入了 if/switch 的初始化语句，C++20 引入了范围 for 的初始化：

```c
// 将 auto list = getList(); 初始化语句直接放在了范围 for 语句中
for (auto list = getList(); auto& ele : list) {
    // ele = ....
}
```

另外 C++20 的范围 for 还可支持一定的函数式编程风格，例如引入管道符 | 实现函数组合：

```c
// 范围库
auto even = [](int i){ return 0 == i % 2; };
auto square = [](int i) { return i * i; };
// ints 输出到 std::view::filter(even) ，处理后得到所有偶数
// 上一个结果输出到 std::view::transform(square)，将所有偶数求平方
// 循环遍历所有偶数的平方
for (int i : ints | std::view::filter(even) | 
                      std::view::transform(square)) {
 // ...
}
```

### 05 char8_t

C++20 新增加 char8_t 类型。

char8_t 用来表示 UTF-8 字符，要求大到足以表示任何 UTF-8 编码单元（ 8 位）。

### 06 [[no_unique_address]]

[[no_unique_address]] 属性修饰的数据成员可以被优化为**不占空间**:

```c
struct Empty {}; // 空类
struct X {
  int i;
  Empty e;
};
struct Y {
  int i;
  [[no_unique_address]] Empty e;
};
struct Z {
  char c;
  [[no_unique_address]] Empty e1, e2;
};
struct W {
  char c[2];
  [[no_unique_address]] Empty e1, e2;
};

int main() {
  // 任何空类类型对象的大小至少为 1
  static_assert(sizeof(Empty) >= 1);

  // 至少需要多一个字节以给 e 唯一地址
  static_assert(sizeof(X) >= sizeof(int) + 1);

  // 优化掉空成员
  std::cout << "sizeof(Y) == sizeof(int) is " << std::boolalpha << (sizeof(Y) == sizeof(int)) << '\n';

  // e1 与 e2 不能共享同一地址，因为它们拥有相同类型，尽管它们标记有 [[no_unique_address]]。
  // 然而，其中一者可以与 c 共享地址。
  static_assert(sizeof(Z) >= 2);

  // e1 与 e2 不能拥有同一地址，但它们之一能与 c[0] 共享，而另一者与 c[1] 共享
  std::cout << "sizeof(W) == 2 is " << (sizeof(W) == 2) << '\n';
}
```

### 07 [[likely]]

[[likely]] 属性用来告诉编译器哪条分支执行的概率会更大，从而帮助编译器进行代码编译的优化

```c
if (a > b) [[likely]] {
  // ...
}
```

> 第一直觉真的是奇葩特性，好奇能优化到什么程度以至于专门增加语言特性来要求程序员配合这种优化
> 包括下文的头文件，让我觉得 C++ 很多时候不是编译器为程序员服务，而是程序员为编译器服务

### 08 [[unlikely]]

与 [[likely]] 相对应：

```c
if (a>b) [[unlikely]] {
  // ...
}
```

### 09 lambda 初始化捕获中的包展开

在 C++20 之前，lambda 表达式对与包展开无法进行初始化捕获，如果想要对包展开进行初始化捕获，需要通过 make_tuple 和 apply 来实现，如下所示：

```c
template <class... Args>
auto delay_invoke_foo(Args... args) {
    // 对 args 进行 make_tuple，然后再用 apply 恢复
    return [tup=std::make_tuple(std::move(args)...)]() -> decltype(auto) {
        return std::apply([](auto const&... args) -> decltype(auto) {
            return foo(args...);
        }, tup);
    };
}
```

C++20 将直接支持 lambda 对包展开进行初始化捕获，如下所示：

```c
template <class... Args>
auto delay_invoke_foo(Args... args) {
    // 直接 ...args = xxxxx
    return [...args=std::move(args)]() -> decltype(auto) {
        return foo(args...);
        
    };
}
```

### 10 移除了在多种上下文语境中，使用 typename 关键字以消除类型歧义的要求

> [P0634R3](https://links.jianshu.com/go?to=http%3A%2F%2Fwww.open-std.org%2Fjtc1%2Fsc22%2Fwg21%2Fdocs%2Fpapers%2F2018%2Fp0634r3.html)
> C++20 之前，在使用了模板类型的地方需要使用 typename 来消除歧义，如下所示：

```c
template<typename T>
typename std::vector<T>::iterator // std::vector<T>::iterator 之前必须使用 typename 关键字
```

C++20 则允许在一些上下文语境中省略 typename，如下所示：

```c
template<typename T>
std::vector<T>::iterator // 省略 typename 关键字
```

### 11 consteval、constinit

**consteval**
上文提及过 constexpr 函数可以在编译期运行，也可以在运行期执行。C++20 为了更加明确场景和语义，提供了只能在编译期执行的 consteval，consteval 修饰的函数返回的值如果不能在编译器确定，则编译无法通过。

**constinit**
在 C++ 中，对于静态存储期的变量的初始化，通常会有两种情况：

- 在编译期初始化
- 在被第一次加载声明时初始化

其中第二种情况由于静态变量初始化顺序的原因存在着隐藏的风险。

所以 C++20 提供了 constinit，以便使某些应该在编译期初始化的变量被确保的在编译期初始化。

### 12 更为宽松的 constexpr 要求

从 C++11 一直到 C++20 就一直在给 constexpr 「打补丁」，就不能一次性扩展其能力吗

> 引用自 [C++20 新增特性](https://links.jianshu.com/go?to=http%3A%2F%2Fjimmysue.me%2Fc-20-features%2F%23constexpr-string--vector)
> C++20 中 constexpr 扩展的能力：
>
> - constexpr虚函数
>   - constexpr 的虚函数可以重写非 constexpr 的虚函数
>   - 非 constexpr 虚函数将重载 constexpr 的虚函数
> - constexpr 函数支持:
>   - 使用 dynamic_cast() 和 typeid
>   - 动态内存分配
>   - 更改union成员的值
>   - 包含 try/catch
>     - 但是不允许 throw 语句
>     - 在触发常量求值的时候 try/catch 不发生作用
>     - 需要开启 constexpr std::vector
> - constexpr 支持 string & vector 类型

### 13 规定有符号整数以补码实现

在 C++20 之前，有符号整数的实现没有明确以标准的形式规定（虽然在实现时基本都采用补码）。C++20 明确规定了有符号整数使用补码实现。

### 14 使用圆括号的聚合初始化

C++20 引入了一些新的聚合初始化形式，如下所示：

```c
T object = { .designator = arg1 , .designator { arg2 } ... };  //(since C++20)
T object { .designator = arg1 , .designator { arg2 } ... };     // (since C++20)
T object (arg1, arg2, ...);                                                           // (since C++20)
```

其中之前没有过的就是第三种形式： `T object (arg1, arg2, ...)`，使用圆括号进行初始化。

### 15 协程

**进程**：操作系统资源分配的基本单元。调度涉及到用户空间和内核空间的切换，资源消耗较大。
**线程**：操作系统运行的基本单元。在同一个进程资源的框架下，实现抢占式多任务，相对进程，降低了执行单元切换的资源消耗。
**协程**：和线程非常类似。但是转变一个思路实现协作式多任务，由用户来实现协作式调度（主动交出控制权）

高德纳 Donald Knuth：

> 子程序就是协程的一种特例

协程是广义的函数（子程序），只是它的流程由用户进行一定程度的函数过程切换和控制

举一个例子：

```python
# 协程实现的生产者和消费者
def consumer():
  r = ''
  while True:
    n = yield r
    if not n:
      return
    print('[CONSUMER] Consuming %s...' % n)
    time.sleep(1)
    r = '200 OK'

def produce(c):
  c.next()
  n = 0
  while n < 5:
    n = n + 1
    print('[PRODUCER] Producing %s...' % n)
    r = c.send(n)
    print('[PRODUCER] Consumer return: %s' % r)
  c.close()

if __name__=='__main__':
  c = consumer()
  produce(c)
```

生产者生产消息，待消费者执行完毕后，通过 yield 让出控制权切换回生产者继续生产。

> **yield**: 执行到这里主动让出控制权，返回一个值，并等待上一个上下文对自己的进一步调度

上面是**协程的**的纯粹概念，但是很多语言对协程会有不同的实现和封装，导致协程的概念被进一步扩展和延伸。

例如 **golang** 中的 Goroutines 其实并不是一个纯粹的协程概念，而是对协程和线程的封装和实现，可以说在用户状态下的执行单元调度，同时又解决了传统协程无法利用多核能力的缺陷。所以很多资料将其称为 「**轻量级线程**」或 「**用户态线程**」。

另外，在异步编程方面，协程有一个特别的优势：
**通过更符合人类直觉的顺序执行来表达异步逻辑**。

在 JS 生态中（尤其以 Node.js 为代表）我们编写异步逻辑，经常使用回调来实现结果返回。而如果是多层级异步调用的场景，容易陷入 「**callback hell 回调地狱**」。

如下所示：

```js
fs.readFile(fileA, function (err, data) {
  fs.readFile(fileB, function (err, data) {
    // ...
  });
});
```

JS 后续引入了 Promise，简化回调调用形式，如下所示：

```c
readFile(fileA)
.then(function(data){
  console.log(data.toString());
})
.then(function(){
  return readFile(fileB);
})
.then(function(data){
  console.log(data.toString());
})
.catch(function(err) {
  console.log(err);
});
```

再后续引入了协程的一种实现——**Generator 生成器**：

```c
var fetch = require('node-fetch');

function* gen(){
  var url = 'https://api.github.com/users/github';
  var result = yield fetch(url);
  console.log(result.bio);
}

var g = gen();
var result = g.next();

result.value.then(function(data){
  return data.json();
}).then(function(data){
  g.next(data);
});
```

> Generator 函数可以暂停执行(yield)和恢复执行(next)，这是它能用来实现异步编程的根本原因

而 JS 后续底层通过 yield/generator 实现的 async & await 异步编程体验，也会使得 JS 程序员对协程的直观感受为「回调调度器」。

而 C++20 引入的则是相对纯粹的协程，例如可以实现一个 generator函数或者生成器：

```c
experimental::generator<int> GetSequenceGenerator( 
    int startValue, 
    size_t numberOfValues) { 
    for (int i = 0 startValue; i < startValue + numberOfValues; ++i){ 
        time_t t = system_clock::to_time_t(system_clock::now()); 
        cout << std:: ctime(&t); co_yield i; 
    } 
} 
int main() {
    auto gen = GetSequenceGenerator(10, 5); 
    for (const auto& value : gen) { 
        cout << value << "(Press enter for next value)" << endl; 
        cin.ignore(); 
    } 
}
```

### 16 模块

**历史包袱-头文件**

请看如下代码：

```c
// person.cpp
int rest() {
  Play();
  return 0;
}

// game.cpp
int play() {
  LaunchSteam();
  return 0;
}
```

1. 由于 C/C++ 时代 .obj 等结果文件可能来自于其他语言。固每个源文件不与其他源文件产生关联，需独立编译。在这样的背景下，我们站在编译器的角度尝试编译 person.cpp ，会发现编译将无法进行。原因是 Play 的**返回类型、参数类型等元信息**无法获取。那么是否可以生成外部符号等待链接阶段呢？
2. 答案是否定的。即无法推迟到链接阶段。原因是 C++ 编译时不会将函数的返回值、参数等元信息编译进 .obj 等结果，固在**链接阶段**依然获取不到 Play 函数相关的元信息。之所以没有像 Java/C# 等现代语言这样将元信息写到编译结果中，是因为 C/C++ 时代内存等资源稀缺，所以想方设法的节省各种资源。

而由于上述历史原因，导致了 C++ 最终将这种不便转交给了程序员。程序员在调用另一个源文件的函数时需要**事先声明函数原型**，而如果在每个使用到相应函数的源文件中都重复声明一次就太过于低级，于是出现了所谓的头文件，简化声明工作。

另一方面，头文件从一定程度起到了接口描述的作用，但有些人把头文件当作是「实现与接口分离的设计思想」下的成果就非常的牵强了。

头文件本质上是围绕着编译期的一种概念，是 C/C++ 由于历史原因不得不由程序员使用头文件辅助编译器完成编译工作。

而接口的概念是围绕着业务开发或编程阶段的，是另一层面的事情。

如果不好理解，可以思考一下，Java/C# 没有头文件的语言是如何实现所谓「头文件提供接口」这一功能的？

如果需要实现，编译器可以直接从源码文件抽离出接口信息生成接口文件即可，而且还可以根据访问权限来决定哪些该对外暴露，哪些不能暴露。甚至可以以 .h 为后缀让那些觉得「**头文件起到接口作用**」的程序员好受些。

C++20 引入了模块，模块的其中一个作用就是将 `header` 与`编译单元`统一在了一起。

```c
// example 模块
export module example; //声明一个模块名字为example
export int add(int first, int second) { //可以导出的函数
  return first + second;
}

// 使用 example 模块
import example; //导入上述定义的模块
int main() {
  add(1, 2); //调用example模块中的函数
}
```

### 17 限定与概念(concepts)

concepts 是 C++20 的重要更新之一，它是模板能力的扩展。在 C++20 之前，我们的模板参数是没有明确限定的，如下所示：

```c
template<class L, class T>
void find(const L& list, const T& t); // 从 list 列表中查找 t
```

上面的参数类型 L 与 T 没有任何的限制，但实际上是**存在着隐含的限定条件**的：

- L 应该是一个可迭代类型
- L 中的元素类型应该和 T 类型相同
- L 中的元素应该和 T 类型可进行相等比较

程序员应当知晓上述隐含条件，否则编译器就会输出一堆错误。而现在可以通过 concepts 将上述限定条件告知编译器，在使用错误将得到直观的错误原因。

例如使用 concepts 限定参数可 hash：

```c
// 定义概念
template<typename T>
concept Hashable = requires(T a) {
  // 下面语句的限定含义为：
  // 限定  std::hash(a) 返回值可转换成 std::size_t
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// 使用概念对模板参数进行限定
template<typename T>
auto my_hash(T) requires Hashable<T> {
  // ....
}
```

对于上述的 `my_hash` 函数也可通过简化的方式进行：

```c
// 简化
template<Hashable T>
auto my_hash(T) {
  // ....
}
```

### 18 缩略函数模板

通常声明函数模板的形式如下：

```c
template<class T> void f(T);
template<C1 T> void f2(T); // C1 如果是一个 concept 概念
// ...
```

C++20 可以采用 `auto` 或 `concept auto` 来实现更为简短的函数模板声明形式：

```c
void f1(auto);       // 等同于 template<class T> void f(T);
void f2(C1 auto); // template<C1 T> void f2(T);
// ...
```

### 19 数组长度推导

C++20 将允许 `new int[]{1, 2, 3}` 的写法，编译器可自动推导数组长度。

## 小结

C++ 最根本的设计理念就是为了**运行效率**服务，甚至专门增加新特性要求程序员配合编译器来做优化。但另一方面， C++ 后期一直从 Java/JavaScript/Go/Python 等语言中借鉴特性，而其中很多是无关紧要的语法糖，对于真正至关重要的特性却又一直拖到了 2020 年才推出标准。

> C++ 20 真正在业界扎稳又是要到何年何月，至于形成与其他现代语言一样完善、统一的生态更是遥不可期

这导致本就繁杂的 C++ 的语法随着时间推移变得更加混乱，这进一步提高了 C++ 的学习与使用成本。唯一的好处就是提高了部分现有 C++ 程序员的自豪感，毕竟部分程序员是以自己掌握的**工具难度**为傲的。这些人不仅将「工具的难度」与「技术水平」挂钩，有时甚至以此标榜自己的智商。建议有此想法的人**阅读并背诵新华字典全典**或者**用汇编完成所有工作**。

C++ 有其对应的应用场景，在一些运行效率要求极高的基础组件的开发上，在绝大多数的游戏开发场景下，C++ 有其不可替代性。但在一些上层的应用场景，尤其是在更接近用户的互联网业务上使用 C++ 基本都是由于历史债务**[7]**。

> **[7]**: 例如我现在所在的部门。即使围绕 C++ 积累了大量的组件、工具和平台，不少人自认为开发效率也不低，但实际上和业界在开发效率上至少有 2 ～ 3 年以上的差距**[8]**，而且就目前能预见的，这差距只会越来越大。
>
> **[8]**: 游戏和基础组件场景除外，这里指那些不该使用 C++ 却由于历史原因使用 C++而现在又没有决心改变的场景和业务。当然这也只是从技术层面上讲，实际情况可能是业务上难以改变，或者改变的风险大于收益。但这不该是心安理得的理由，开发效率落后于业界依然是一个客观事实。

## 相关代码

针对上述特性编写了一些例子代码（更新中），有需要可[点此查看](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2FYueHub%2Fdemo%2Ftree%2Fmaster%2Fcpp-demo)**[9]**

> **[9]**: 文件名为 **cpp版本号_特性序号_特性名称**

## 参考资料

[现代C++教程：高速上手C++11/14/17/20](https://links.jianshu.com/go?to=https%3A%2F%2Fchangkun.de%2Fmodern-cpp%2Fzh-cn%2F02-usability%2Findex.html)
[cppreference](https://links.jianshu.com/go?to=https%3A%2F%2Fen.cppreference.com%2F)
[cppreference 中文](https://links.jianshu.com/go?to=https%3A%2F%2Fzh.cppreference.com%2F)
[C++17 STL Cook Book](https://links.jianshu.com/go?to=https%3A%2F%2Fchenxiaowei.gitbook.io%2Fc-17-stl-cook-book%2F)
[Changes between C++14 and C++17](https://links.jianshu.com/go?to=http%3A%2F%2Fopen-std.org%2FJTC1%2FSC22%2FWG21%2Fdocs%2Fpapers%2F2017%2Fp0636r2.html)
[深入理解 C++11 新特性解析与应用](https://www.jianshu.com/p/8c4952e9edec)
[C++20 - 下一个大版本功能确定](https://links.jianshu.com/go?to=https%3A%2F%2Fskyscribe.github.io%2Fpost%2F2019%2F06%2F23%2Fcpp-20-modules-concepts-coroutine%2F)