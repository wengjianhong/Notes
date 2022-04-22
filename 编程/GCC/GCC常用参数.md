gcc and g++现在是gnu中最主要和最流行的c & c++编译器 gcc/g++在执行编译工作的时候，总共需要以下几步:
1.预处理,生成.i的文件[预处理器cpp]
2.将预处理后的文件不转换成汇编语言,生成文件.s[编译器egcs]
3.由汇编变为目标代码(机器代码)生成.o的文件[汇编器as]
4.连接目标代码,生成可执行程序[链接器ld]

GCC能够处理的后缀有：

- \*.C (C语言)
- .cxx \*.cc (C++语言)
- \*.m (面向对象的C)
- \*.i (预处理后的C语言源文件)
- \*.ii (预处理后的C++语言源文件)
- \*.s \*.S (汇编语言)
- \*.h (头文件)

目标文件可以是：

- \*.o 编译连接后的目标文件*
- \*.a 库文件 

 

## 参数详解

`无选项编译链接`

```shell
gcc test.c      // 将test.c预处理、汇编、编译并链接形成可执行文件。这里未指定输出文件，默认输出为a.out。
```

`无选项链接`

```
gcc test.o -o test  // 将编译输出文件test.o链接成最终可执行文件test。
```

`-x language filename`
　 设定文件所使用的语言,使后缀名无效,对以后的多个有效.也就是根据约定C语言的后缀名称是.c的，而C++的后缀名是.C或者.cpp,如果你很个性，决定你的C代码文件的后缀名是.pig 哈哈，那你就要用这个参数,这个参数对他后面的文件名都起作用，除非到了下一个参数的使用。
　　可以使用的参数吗有下面的这些
　　`c`, `objective-c`, `c-header`, `c++`, `cpp-output`, `assembler`, and `assembler-with-cpp`.
　　看到英文，应该可以理解的。
　　例子用法:
　　gcc -x c hello.pig

`-x none filename`
　　关掉上一个选项，也就是让gcc根据文件名后缀，自动识别文件类型
　　例子用法:
　　gcc -x c hello.pig -x none hello2.c

`-c`
　　只激活预处理,编译,和汇编,也就是他只把程序做成obj文件
　　例子用法:
　　gcc -c hello.c
　　他将生成.o的obj文件

```
gcc -c test.s       // 将汇编输出文件test.s编译输出test.o文件。
```



`-S`
　　只激活预处理和编译，就是指把文件编译成为汇编代码。

```shell
# gcc -S hello.c    // 将hello.c文件生成test.s汇编文件
# gcc -S test.i     // 将预处理输出文件test.i汇编成test.s文件 
```



`-E`
　　只激活预处理,这个不生成文件,你需要把它重定向到一个输出文件里面.
　　例子用法: 
　　gcc -E hello.c >; pianoapan.txt
　　gcc -E hello.c | more

```
gcc -E test.c -o test.i
```



`-o`
　　制定目标名称,缺省的时候,gcc 编译出来的文件是a.out,很难听,如果你和我有同感，改掉它,哈哈
　　例子用法:
　　gcc -o hello.exe hello.c (哦,windows用习惯了)
　　gcc -o hello.asm -S hello.c

`-pipe`
　　使用管道代替编译中临时文件,在使用非gnu汇编工具的时候,可能有些问题
　　gcc -pipe -o hello.exe hello.c

`-ansi`
　　关闭gnu c中与ansi c不兼容的特性,激活ansi c的专有特性(包括禁止一些asm inline typeof关键字,以及UNIX,vax等预处理宏,

`-fno-asm`
　　此选项实现ansi选项的功能的一部分，它禁止将asm,inline和typeof用作关键字。 　　　　
`-fno-strict-prototype`
　　只对g++起作用,使用这个选项,g++将对不带参数的函数,都认为是没有显式的对参数的个数和类型说明,而不是没有参数.
　　而gcc无论是否使用这个参数,都将对没有带参数的函数,认为城没有显式说明的类型

`-fthis-is-varialble`
　　就是向传统c++看齐,可以使用this当一般变量使用.

`-fcond-mismatch`
　　允许条件表达式的第二和第三参数类型不匹配,表达式的值将为void类型

`-funsigned-char`
`-fno-signed-char`
`-fsigned-char`
`-fno-unsigned-char`
　　这四个参数是对char类型进行设置,决定将char类型设置成unsigned char(前两个参数)或者 signed char(后两个参数)

`-include file`
　　包含某个代码,简单来说,就是便以某个文件,需要另一个文件的时候,就可以用它设定,功能就相当于在代码中使用#include<filename>;
　　例子用法:
　　gcc hello.c -include /root/pianopan.h

`-imacros file`
　　将file文件的宏,扩展到gcc/g++的输入文件,宏定义本身并不出现在输入文件中

`-Dmacro` 
以字符串“1”定义 MACRO 宏 
　　相当于C语言中的#define macro

`-Dmacro=defn`
以字符串“DEFN”定义 MACRO 宏
　　相当于C语言中的#define macro defn

`-Umacro` 
取消对 MACRO 宏的定义 
　　相当于C语言中的#undef macro

`-undef`
　　取消对任何非标准宏的定义

`-Idir`
　　在你是用#include"file"的时候,gcc/g++会先在当前目录查找你所制定的头文件,如果没有找到,他回到缺省的头文件目录找,如果使用-I制定了目录,他
　　回先在你所制定的目录查找,然后再按常规的顺序去找.
　　对于#include<file>;,gcc/g++会到-I制定的目录查找,查找不到,然后将到系统的缺省的头文件目录查找

`-I-`
　　就是取消前一个参数的功能,所以一般在-Idir之后使用

`-idirafter dir`
　　在-I的目录里面查找失败,讲到这个目录里面查找.

`-iprefix prefix`
`-iwithprefix dir`
　　一般一起使用,当-I的目录查找失败,会到prefix+dir下查找

`-nostdinc`
　　使编译器不再系统缺省的头文件目录里面找头文件,一般和-I联合使用,明确限定头文件的位置

`-nostdin C++`
　　规定不在g++指定的标准路经中搜索,但仍在其他路径中搜索,.此选项在创libg++库使用

`-C`
　　在预处理的时候,不删除注释信息,一般和-E使用,有时候分析程序，用这个很方便的

`-M`
　　生成文件关联的信息。包含目标文件所依赖的所有源代码你可以用gcc -M hello.c来测试一下，很简单。

`-MM`
　　和上面的那个一样，但是它将忽略由#include<file>;造成的依赖关系。

`-MD`
　　和-M相同，但是输出将导入到.d的文件里面

-MMD
　　和-MM相同，但是输出将导入到.d的文件里面

`-Wa,option`
　　此选项传递option给汇编程序;如果option中间有逗号,就将option分成多个选项,然后传递给会汇编程序

`-Wl.option`
　　此选项传递option给连接程序;如果option中间有逗号,就将option分成多个选项,然后传递给会连接程序.

`-llibrary`
　　制定编译的时候使用的库
　　例子用法
　　gcc -lcurses hello.c
　　使用ncurses库编译程序

`-Ldir`
　　制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然
　　编译器将只在标准库的目录找。这个dir就是目录的名称。

`-O0/-O1/-O2/-O3`：编译器的优化选项的4个级别，-O0表示没有优化,-O1为缺省值，-O3优化级别最高
例子用法：

```shell
# gcc -O1 test.c -o test    // 使用编译优化级别1编译程序。级别为1~3，级别越大优化效果越好，但编译时间越长
```

`-g`：只是编译器，在编译的时候，产生调试信息。

`-gstabs`：此选项以stabs格式声称调试信息,但是不包括gdb调试信息.

`-gstabs+`：此选项以stabs格式声称调试信息,并且包含仅供gdb使用的额外调试信息.

`-ggdb`：此选项将尽可能的生成gdb的可以使用的调试信息.

`-static`：此选项将禁止使用动态库，所以，编译出来的东西，一般都很大，也不需要什么
动态连接库，就可以运行.

`-share`：此选项将尽量使用动态库，所以生成文件比较小，但是需要系统由动态库.

`-traditional`：　　试图让编译器支持传统的C语言特性 

`-IDIRECTORY` ：指定额外的头文件搜索路径DIRECTORY

`-LDIRECTORY` ：指定额外的函数库搜索路径DIRECTORY

`-lLIBRARY` ：连接时搜索指定的函数库LIBRARY

`-m486`：针对 486 进行代码优化 

`-shared` ：生成共享目标文件。通常用在建立共享库时

`-static`：禁止使用共享连接

`-w` ：不生成任何警告信息

`-Wall` ：生成所有警告信息

 

-save-temps
一次获得全部的中文输出文件，正常的进行编译连接，.i、.s、.o为后缀，文件名相同


-fsyntax-only
不会执行预处理、编译、汇编、连接，只会测试输入文件的语法是否正确

-std
指定C方言，如:-std=c99,gcc默认的方言是GNU C 



多源文件的编译方法：

假设有两个源文件为test.c和testfun.c

1. 多个文件一起编译

```shell
# gcc testfun.c test.c -o test      // 将testfun.c和test.c分别编译后链接成test可执行文件。
```

2. 分别编译各个源文件，之后对编译后输出的目标文件链接。

```shell
# gcc -c testfun.c                   //将testfun.c编译成testfun.o
# gcc -c test.c                      //将test.c编译成test.o
# gcc -o testfun.o test.o -o test    //将testfun.o和test.o链接成test
```

以上两种方法相比较，第一中方法编译时需要所有文件重新编译，而第二种方法可以只重新编译修改的文件，未修改的文件不用重新编译。 



## FAQ

### 1. gcc与g++有什么区别？ 

gcc和g++都是GNU(组织)的一个编译器。

误区一:gcc只能编译c代码,g++只能编译c++代码
两者都可以，但是请注意：
1.后缀为.c的，gcc把它当作是C程序，而g++当作是c++程序；后缀为.cpp的，两者都会认为是c++程序，注意，虽然c++是c的超集，但是两者对语法的要求是有区别的。C++的语法规则更加严谨一些。
2.编译阶段，g++会调用gcc，对于c++代码，两者是等价的，但是因为gcc命令不能自动和C＋＋程序使用的库联接，所以通常用g++来完成链接，为了统一起见，干脆编译/链接统统用g++了，这就给人一种错觉，好像cpp程序只能用g++似的。

误区二:gcc不会定义__cplusplus宏，而g++会
实际上，这个宏只是标志着编译器将会把代码按C还是C++语法来解释，如上所述，如果后缀为.c，并且采用gcc编译器，则该宏就是未定义的，否则，就是已定义。

误区三:编译只能用gcc，链接只能用g++
严格来说，这句话不算错误，但是它混淆了概念，应该这样说：编译可以用gcc/g++，而链接可以用g++或者gcc -lstdc++。因为gcc命令不能自动和C＋＋程序使用的库联接，所以通常使用g++来完成联接。但在编译阶段，g++会自动调用gcc，二者等价

### 1、为什么会出现undefined reference to 'xxxxx'错误？

首先这是链接错误，不是编译错误，也就是说如果只有这个错误，说明你的程序源码本身没有问题，是你用编译器编译时参数用得不对，你没有指定链接程序要用到得库，比如你的程序里用到了一些数学函数，那么你就要在编译参数里指定程序要链接数学库，方法是在编译命令行里加入-lm。

### 2、-l参数和-L参数

-l参数就是用来指定程序要链接的库，-l参数紧接着就是库名，那么库名跟真正的库文
件名有什么关系呢？
就拿数学库来说，他的库名是m，他的库文件名是libm.so，很容易看出，把库文件名的头lib和尾.so去掉就是库名了。

好了现在我们知道怎么得到库名了，比如我们自已要用到一个第三方提供的库名字叫libtest.so，那么我们只要把libtest.so拷贝到/usr/lib里，编译时加上-ltest参数，我们就能用上libtest.so库了（当然要用libtest.so库里的函数，我们还需要与libtest.so配套的头文件）。

放在/lib和/usr/lib和/usr/local/lib里的库直接用-l参数就能链接了，但如果库文件
没放在这三个目录里，而是放在其他目录里，这时我们只用-l参数的话，链接还是会出错，出错信息大概是：“/usr/bin/ld: cannot find -lxxx”，也就是链接程序ld在那3个目录里找不到libxxx.so，这时另外一个参数-L就派上用场了，比如常用的X11的库，它放在/usr/X11R6/lib目录下，我们编译时就要用-L/usr/X11R6/lib -lX11参数，-L参数跟着的是库文件所在的目录名。
再比如我们把libtest.so放在/aaa/bbb/ccc目录下，那链接参数就是-L/aaa/bbb/ccc -ltest

另外，大部分libxxxx.so只是一个链接，以RH9为例，比如libm.so它链接到/lib/libm.so.x，/lib/libm.so.6又链到/lib/libm-2.3.2.so，如果没有这样的链接，还是会出错，因为ld只会找libxxxx.so，所以如果你要用到xxxx库，而只有libxxxx.so.x或者libxxxx-x.x.x.so，做一个链接就可以了ln -s libxxxx-x.x.x.so libxxxx.so,手工来写链接参数总是很麻烦的，还好很多库开发包提供了生成链接参数的程序，名字一般叫xxxx-config，一般放在/usr/bin目录下，比如gtk1.2的链接参数生成程序是gtk-config，执行gtk-config --libs就能得到以下输出"-L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm"，这就是编译一个gtk1.2程序所需的gtk链接参数，xxx-config除了--libs参数外还有一个参数是--cflags用来生成头文件包含目录的，也就是-I参数，在下面我们将会讲到。你可以试试执行gtk-config --libs --cflags，看看输出结果。现在的问题就是怎样用这些输出结果了，最笨的方法就是复制粘贴或者照抄，聪明的办法是在编译命令行里加入这个`xxxx-config --libs --cflags`，比如编译一个gtk程序：gcc gtktest.c `gtk-config --libs --cflags`这样就差不多了。注意`不是单引号，而是1键左边那个键。除了xxx-config以外，现在新的开发包一般都用pkg-config来生成链接参数，使用方法跟xxx-config类似，但xxx-config是针对特定的开发包，但pkg-config包含很多开发包的链接参数的生成，用pkg-config --list-all命令可以列出所支持的所有开发包，pkg-config的用法就是pkg-config pagName --libs --cflags，其中pagName是包名，是pkg-config--list-all里列出名单中的一个，比如gtk1.2的名字就是gtk+，pkg-config gtk+ --libs --cflags的作用跟gtk-config --libs --cflags是一样的。比如：gcc gtktest.c `pkg-config gtk+ --libs --cflags`。

 

 

### 3、-include和-I参数

-include用来包含头文件，但一般情况下包含头文件都在源码里用#include xxxxxx实现，-include参数很少用。-I参数是用来指定头文件目录，/usr/include目录一般是不用指定的，gcc知道去那里找，但是如果头文件不在/usr/include里我们就要用-I参数指定了，比如头文件放在/myinclude目录里，那编译命令行就要加上-I/myinclude参数了，如果不加你会得到一个"xxxx.h: No such file or directory"的错误。-I参数可以用相对路径，比如头文件在当前目录，可以用-I.来指定。上面我们提到的--cflags参数就是用来生成-I参数的。

### 4、几个相关的环境变量

PKG_CONFIG_PATH：用来指定pkg-config用到的pc文件的路径，默认是/usr/lib/pkgconfig，pc文件是文本文件，扩展名是.pc，里面定义开发包的安装路径，Libs参数和Cflags参数等等。
CC：用来指定c编译器。
CXX：用来指定cxx编译器。
LIBS：跟上面的--libs作用差不多。
CFLAGS:跟上面的--cflags作用差不多。
CC，CXX，LIBS，CFLAGS手动编译时一般用不上，在做configure时有时用到，一般情况下不用管。
环境变量设定方法：export ENV_NAME=xxxxxxxxxxxxxxxxx

CPATH、C_INCLUDE_PATH：用逗号隔开的目录列表，提供头文件搜索位置


COMPILER_PATH：用逗号隔开的目录列表，以提供GCC子程序的搜索位置


GCC_EXEC_PREFIX：当GCC调用子程序时，需要“加在前面”的前置名称


LIBRARY_PATH：用逗号隔开的目录列表，以提供连接库的位置


LD_LIBRARY_PATH：用逗号隔开的目录列表，以提供共享库文件的搜索位置


TMPDIR：临时文件所使用的目录

 

### 5、关于交叉编译

交叉编译通俗地讲就是在一种平台上编译出能运行在体系结构不同的另一种平台上，比如在我们地PC平台(X86 CPU)上编译出能运行在sparc CPU平台上的程序，编译得到的程序在X86 CPU平台上是不能运行的，必须放到sparc CPU平台上才能运行。当然两个平台用的都是linux。

这种方法在异平台移植和嵌入式开发时用得非常普遍。