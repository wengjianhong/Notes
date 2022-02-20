[TOC]

# 1. 初识GTest

gtest是一个跨平台的(Liunx、Mac OS X、Windows 、Cygwin 、Windows CE and Symbian ) C++单元测试框架，由google公司发布。gtest是为在不同平台上为编写C++测试而生成的。它提供了丰富的断言、致命和非致命判断、参数化、”死亡测试”等等。

- Git仓库：https://github.com/google/googletest
- 官方文档：https://google.github.io/googletest/



# 2. 安装使用

## Linux

安装命令

```shell
[root@QingYun ~]# yum install gtest gtest-devel -y
```

test.cpp

```cpp
#include <stdio.h>
#include <gtest/gtest.h>

int add(int a, int b) {
  return a + b;
}

TEST(MyTest, AddTest) {
  EXPECT_EQ(add(1, 2), 3);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

测试：

```shell
[root@QingYun ~]# g++ test.cpp -lgtest  -lpthread -o test
[root@QingYun ~]# ./test
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from MyTest
[ RUN      ] MyTest.AddTest
[       OK ] MyTest.AddTest (0 ms)
[----------] 1 test from MyTest (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
[root@QingYun ~]#
```

## Windows

Windows下可以考虑使用 `vcpkg` 安装:

```shell
MINGW64 /d/vcpkg (master)
$ ./vcpkg.exe list gtest
gtest:x64-windows           1.10.0       GoogleTest and GoogleMock testing frameworks
gtest:x86-windows           1.10.0       GoogleTest and GoogleMock testing frameworks
```



# 2. TEST宏

## TEST

`TEST` 宏是最简单的测试，作用是：在`TestSuiteName`测试套件中定义一个`TestName`测试，语法如下：

```cpp
TEST(TestSuiteName, TestName) {
  ... statements ...
}
```

其中，`TestSuiteName` 和 `TestName` 必须满足C++的变量命名规则，并且不能包含下划线(`_`)。

**示例**

```cpp
#include <queue>
#include <gtest/gtest.h>
using namespace std;

TEST(TestSuiteName, TestName1) {
    EXPECT_TRUE(1);
    EXPECT_EQ(1, 0);
    EXPECT_LE(1, 0);
}
TEST(TestSuiteName, TestName2) {
    EXPECT_EQ(1, 1);
    EXPECT_GE(1, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
```

输出

```shell
[root@QingYun gtest]# g++ test.cpp -lgtest   -o test
[root@QingYun gtest]# ./test 
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from TestSuiteName
[ RUN      ] TestSuiteName.TestName1
test.cpp:7: Failure
Value of: 0
Expected: 1
test.cpp:8: Failure
Expected: (1) <= (0), actual: 1 vs 0
[  FAILED  ] TestSuiteName.TestName1 (0 ms)
[ RUN      ] TestSuiteName.TestName2
[       OK ] TestSuiteName.TestName2 (0 ms)
[----------] 2 tests from TestSuiteName (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (1 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] TestSuiteName.TestName1

 1 FAILED TEST
```

> 在一个测试中，只有当所有的断言都成功整个测试才算成功，只要有一个断言失败则测试失败。



## TEST_F

`TEST_F(Test Fixtures)`  宏比 `TEST` 宏更高级，可以支持在测试前的执行初始化和测试后执行清理操作。

使用 `TEST_F` 步骤：

- 实现一个从 `::testing::Test` 继承的类，并且从`protected`开始
- 如果有需要，重载 `SetUp()` 并在函数内实现每个测试执行前需要执行的初始化操作
- 如果有需要，重载 `TearDown() ` 并在函数内实现每个测试执行完需要执行的清理操作



**示例**

```cpp
#include <queue>
#include <gtest/gtest.h>
using namespace std;

class QueueTest : public ::testing::Test {
protected:
    queue<int> q0_;
    void SetUp() override {
        q0_.push(10);
        cout << "setup..." << endl;
    }
    void TearDown() override {
        q0_.pop();
        cout << "TearDown..." << endl;
    }
};

TEST_F(QueueTest, test1) {
    EXPECT_EQ(q0_.front(), 10);
}

TEST_F(QueueTest, test2) {
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
```

输出

```shell
[root@QingYun gtest]# g++ test.cpp -lgtest   -o test
[root@QingYun gtest]# ./test 
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from QueueTest
[ RUN      ] QueueTest.test1
setup...                # 每个TEST_F测试执行前自动调用 SetUp()
TearDown...             # 每个TEST_F测试执行后自动调用 TearDown()
[       OK ] QueueTest.test1 (0 ms)
[ RUN      ] QueueTest.test2
setup...                # 每个TEST_F测试执行前自动调用 SetUp()
TearDown...             # 每个TEST_F测试执行后自动调用 TearDown()
[       OK ] QueueTest.test2 (0 ms)
[----------] 2 tests from QueueTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (0 ms total)
[  PASSED  ] 2 tests.
[root@QingYun gtest]# 
```



## TEST_P

`TEST_P` 支持参数化测试，其中`_P` 可以理解为`parameterized` 或`pattern` ，。

使用 `TEST_P` 可以支持自定义测试参数集，使用步骤如下：

- 定义一个测试类，必须同时继承 `testing::Test` 和 `testing::WithParamInterface<T>` 
- 在`TEST_P`宏内，实现需要的测试内容
- 使用`INSTANTIATE_TEST_SUITE_P`宏，定义参数集来实例化测试套件

> 为方便起见，可以直接只继承  `testing::TestWithParam<T>`，该类本身派生自`testing::Test` 和`testing::WithParamInterface<T>`。



示例

```cpp
#include <iostream>
#include "gtest/gtest.h"
using namespace std;

// 1.定义一个测试类
class FooTest : public testing::TestWithParam<int> {
    // You can implement all the usual fixture class members here.
    // To access the test parameter, call GetParam() from class
    // TestWithParam<T>.
};
// 2. 定义测试内容
TEST_P(FooTest, DoesBlah) {
    // Inside a test, access the test parameter with the GetParam() method
    // of the TestWithParam<T> class:
    EXPECT_GT(3, GetParam());
}
// 3. 定义参数集
INSTANTIATE_TEST_SUITE_P(DoesBlahx, FooTest, testing::Range(2, 5));


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

输出

```shell
[root@QingYun testzone]# g++ test.cpp -lgtest --std=c++11 -lpthread -o test
[root@QingYun testzone]# ./test 
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from DoesBlahx/FooTest
[ RUN      ] DoesBlahx/FooTest.DoesBlah/0
[       OK ] DoesBlahx/FooTest.DoesBlah/0 (0 ms)
[ RUN      ] DoesBlahx/FooTest.DoesBlah/1
test.cpp:14: Failure
Expected: (3) > (GetParam()), actual: 3 vs 3
[  FAILED  ] DoesBlahx/FooTest.DoesBlah/1, where GetParam() = 3 (0 ms)
[ RUN      ] DoesBlahx/FooTest.DoesBlah/2
test.cpp:14: Failure
Expected: (3) > (GetParam()), actual: 3 vs 4
[  FAILED  ] DoesBlahx/FooTest.DoesBlah/2, where GetParam() = 4 (0 ms)
[----------] 3 tests from DoesBlahx/FooTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 2 tests, listed below:
[  FAILED  ] DoesBlahx/FooTest.DoesBlah/1, where GetParam() = 3
[  FAILED  ] DoesBlahx/FooTest.DoesBlah/2, where GetParam() = 4

 2 FAILED TESTS
[root@QingYun testzone]# 
```



>使用 `INSTANTIATE_TEST_SUITE_P` 要求 `gtest`版本不低于 `1.10` ，低版本使用`INSTANTIATE_TEST_CASE_P`！！！

# 3. 断言

GTest中断言的宏可以分为两类：一类是ASSERT宏，另一类就是EXPECT宏了。

- ASSERT宏：如果当前点检测失败则退出当前函数
- EXPECT宏：如果当前点检测失败则继续往下执行

## ASSERT/EXPECT

### bool比较

#### EXPECT_TRUE

```
EXPECT_TRUE(condition)
ASSERT_TRUE(condition)
```

验证*`condition`*是真的。

#### EXPECT_FALSE

```
EXPECT_FALSE(condition)
ASSERT_FALSE(condition)
```

验证*`condition`*是假的。



#### 示例

```cpp
#include <gtest/gtest.h>
using namespace std;

bool IsEven1(int n) {
  return ((n % 2) == 0);
}
testing::AssertionResult IsEven2(int n) {
  if ((n % 2) == 0)
    return testing::AssertionSuccess();
  else
    return testing::AssertionFailure() << n << " is odd";
}
int main(){
    int a = 1;
    EXPECT_TRUE(IsEven1(a));
    cout << endl;
    
    EXPECT_TRUE(IsEven2(a));
    return 0;
}
```

输出：

```shell
[root@QingYun gtest]# g++ test.cpp -lgtest   -o test
[root@QingYun gtest]# ./test 
test.cpp:15: Failure
Value of: IsEven1(a)
  Actual: false
Expected: true

test.cpp:18: Failure
Value of: IsEven2(a)
  Actual: false (1 is odd)
Expected: true
```



### 二进制比较

#### EXPECT_EQ

```
EXPECT_EQ(val1,val2)
ASSERT_EQ(val1,val2)
```

验证 *`val1`*`==`*`val2`*

#### EXPECT_NE

```
EXPECT_NE(val1,val2)
ASSERT_NE(val1,val2)
```

验证 *`val1`*`!=`*`val2`*

#### EXPECT_LT

```
EXPECT_LT(val1,val2)
ASSERT_LT(val1,val2)
```

验证 *`val1`*`<`*`val2`*

#### EXPECT_LE

```
EXPECT_LE(val1,val2)
ASSERT_LE(val1,val2)
```

验证 *`val1`*`<=`*`val2`*

### EXPECT_GT

```
EXPECT_GT(val1,val2)
ASSERT_GT(val1,val2)
```

验证 *`val1`*`>`*`val2`*

### EXPECT_GE

```
EXPECT_GE(val1,val2)
ASSERT_GE(val1,val2)
```

验证 *`val1`*`>=`*`val2`*

### 字符串比较

#### EXPECT_STREQ

```cpp
EXPECT_STREQ(str1, str2)
ASSERT_STREQ(str1, str2)
```

验证两个 C 字符串*`str1`*并*`str2`*具有相同的内容。

#### EXPECT_STRNE

```
EXPECT_STRNE(str1,str2)
ASSERT_STRNE(str1,str2)
```

验证两个 C 字符串*`str1`*并*`str2`*具有不同的内容。

#### EXPECT_STRCASEEQ

```
EXPECT_STRCASEEQ(str1,str2)
ASSERT_STRCASEEQ(str1,str2)
```

验证两个 C 字符串*`str1`*并*`str2`*具有相同的内容，忽略大小写。

#### EXPECT_STRCASENE

```
EXPECT_STRCASENE(str1,str2)
ASSERT_STRCASENE(str1,str2)
```

验证两个 C 字符串*`str1`*并*`str2`*具有不同的内容，忽略大小写。

### 浮点比较

#### EXPECT_FLOAT_EQ

```cpp
EXPECT_FLOAT_EQ(val1, val2)
ASSERT_FLOAT_EQ(val1, val2)
```

验证两个`float`值*`val1`*和*`val2`*是否大致相等，彼此相差不超过 4 个 ULP。

#### EXPECT_DOUBLE_EQ

```cpp
EXPECT_DOUBLE_EQ(val1, val2)
ASSERT_DOUBLE_EQ(val1, val2)
```

验证两个`double`值*`val1`*和*`val2`*是否大致相等，彼此相差不超过 4 个 ULP。

#### EXPECT_NEAR

```cpp
EXPECT_NEAR(val1,val2, abs_error)
ASSERT_NEAR(val1,val2, abs_error)
```

验证*`val1`*和之间的差异*`val2`*不超过绝对误差界限*`abs_error`*。

## 广义断言

### EXPECT_THAT

```
EXPECT_THAT(value,matcher)
ASSERT_THAT(value,matcher)
```

验证*`value`*匹配*`matcher`*（ 其中， *`matcher`*为[匹配器](https://google.github.io/googletest/reference/matchers.html)）

示例：

- 验证字符串是否以`value1`开头 `"Hello"``
- 验证``value2`匹配正则表达式
- 验证`value3`介于 5 和 10 之间：

```cpp
#include "gmock/gmock.h"

using ::testing::AllOf;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::StartsWith;

...
EXPECT_THAT(value1, StartsWith("Hello"));
EXPECT_THAT(value2, MatchesRegex("Line \\d+"));
ASSERT_THAT(value3, AllOf(Gt(5), Lt(10)));
```

匹配器使这种形式的断言读起来像英语，并生成信息丰富的失败消息。例如，如果上述断言`value1` 失败，则结果消息将类似于以下内容：

```shell
Value of: value1
  Actual: "Hi, world!"
Expected: starts with "Hello"
```



GoogleTest 提供了一个内置的匹配器库——请参阅 [匹配器参考](https://google.github.io/googletest/reference/matchers.html)

## 异常断言



## 谓词断言

使用 `谓词断言` 有两个好处：

- 允许我们自建谓词逻辑，实现更复制的断言逻辑
- 允许自定义错误信息，提供更加完善的失败信息

`谓词断言` 的两个宏命令：`EXPECT_PRED*` 和 `EXPECT_PRED_FORMAT*`

### EXPECT_PRED*

```cpp
EXPECT_PRED1(pred,val1)
EXPECT_PRED2(pred,val1,val2)
EXPECT_PRED3(pred,val1,val2,val3)
EXPECT_PRED4(pred,val1,val2,val3,val4)
...
ASSERT_PRED1(pred,val1)
ASSERT_PRED2(pred,val1,val2)
ASSERT_PRED3(pred,val1,val2,val3)
ASSERT_PRED4(pred,val1,val2,val3,val4)
ASSERT_PRED5(pred,val1,val2,val3,val4,val5)
```

`EXPECT_PRED*` 验证 `pred` 是否返回 `true` ，其中：

- `*` 表示参数的个数
- `pred` 表示接收 `*` 个参数并且返回bool的函数指针，`val1`、`val2`、 ... `valn` 是 `pred` 的参数

### 示例

```cpp
#include <gtest/gtest.h>
using namespace std;

bool IsEven1(int n) {
  return ((n % 2) == 0);
}
testing::AssertionResult IsEven2(int n) {
  if ((n % 2) == 0)
    return testing::AssertionSuccess();
  else
    return testing::AssertionFailure() << n << " is odd";
}

int main(){
    int a = 1;
    EXPECT_PRED1(IsEven1, a);
    cout << endl;

    EXPECT_PRED1(IsEven2, a);
    return 0;
}
```

输出

```shell
[root@QingYun gtest]# g++ test.cpp -lgtest   -o test
[root@QingYun gtest]# ./test 
test.cpp:16: Failure
IsEven1(a) evaluates to false, where
a evaluates to 1

test.cpp:19: Failure
IsEven2(a) evaluates to false, where
a evaluates to 1
```



### EXPECT_PRED_FORMAT*

```cpp
EXPECT_PRED_FORMAT1(pred_formatter,val1)
EXPECT_PRED_FORMAT2(pred_formatter,val1,val2)
EXPECT_PRED_FORMAT3(pred_formatter,val1,val2,val3)
EXPECT_PRED_FORMAT4(pred_formatter,val1,val2,val3,val4)
EXPECT_PRED_FORMAT5(pred_formatter,val1,val2,val3,val4,val5)
...
ASSERT_PRED_FORMAT1(pred_formatter,val1)
ASSERT_PRED_FORMAT2(pred_formatter,val1,val2)
ASSERT_PRED_FORMAT3(pred_formatter,val1,val2,val3)
ASSERT_PRED_FORMAT4(pred_formatter,val1,val2,val3,val4)
ASSERT_PRED_FORMAT5(pred_formatter,val1,val2,val3,val4,val5)
```

`EXPECT_PRED_FORMAT*` 验证 `pred_formatter` 是否返回 `true` ，其中：

- `*` 表示参数的个数
- `pred_formatter` 表示函数指针，函数的形式如下：

```cpp
testing::AssertionResult pred_formatter(const char* expr1,
                                            const char* expr2,
                                            ...
                                            const char* exprn,
                                            T1 val1,
                                            T2 val2,
                                            ...
                                            Tn valn);
```

其中：

- 函数的返回类型是 `AssertionResult`
- `val1`， `val2`， …， `valn` 是参数值
- `expr1`， `expr2`， …， `exprn` 是 `val1`， `val2`， …， `valn`  在源码中相应的表达式

示例：

```cpp
#include <gtest/gtest.h>
using namespace std;

testing::AssertionResult IsEven3(const char* m_expr, int n) {
  if ((n % 2) == 0)
    return testing::AssertionSuccess();
  else
    return testing::AssertionFailure() << "Custom information: " << m_expr <<" = "<< n << " is odd";
}

int main(){
    int a = 2;
    EXPECT_PRED_FORMAT1(IsEven3, a * 100 + 1);
    return 0;
}
```

输出

```shell
[root@QingYun gtest]# g++ test.cpp -lgtest   -o test
[root@QingYun gtest]# ./test 
test.cpp:13: Failure
Custom information: a * 100 + 1 = 201 is odd
```



## 死亡断言





# 4. 事件机制

“事件” 本质是框架给你提供了一个机会, 让你能在这样的几个机会来执行你自己定制的代码, 来给测试用例准备/清理数据。gtest提供了多种事件机制，总结一下gtest的事件一共有三种：

## TestSuite事件

需要写一个类，继承testing::Test，然后实现两个静态方法：SetUpTestCase 方法在第一个TestCase之前执行；TearDownTestCase方法在最后一个TestCase之后执行。

## TestCase事件

是挂在每个案例执行前后的，需要实现的是SetUp方法和TearDown方法。SetUp方法在每个TestCase之前执行；TearDown方法在每个TestCase之后执行。

## 全局事件

要实现全局事件，必须写一个类，继承testing::Environment类，实现里面的SetUp和TearDown方法。SetUp方法在所有案例执行前执行；TearDown方法在所有案例执行后执行。

例如全局事件可以按照下列方式来使用：

除了要继承testing::Environment类，还要定义一个该全局环境的一个对象并将该对象添加到全局环境测试中去。



# 5. 参数化





# 6. 死亡测试

这里的 `死亡` 指的是程序的奔溃。通常在测试的过程中，我们需要考虑各种各样的输入，有的输入可能直接导致程序奔溃，这个时候我们就要检查程序是否按照预期的方式挂掉，这也就是所谓的 `死亡测试` 。

死亡测试所用到的宏：

1、ASSERT_DEATH(参数1，参数2)，程序挂了并且错误信息和参数2匹配，此时认为测试通过。如果参数2为空字符串，则只需要看程序挂没挂即可。

2、ASSERT_EXIT(参数1，参数2，参数3)，语句停止并且错误信息和被提前给的信息匹配。





