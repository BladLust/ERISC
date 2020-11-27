
# ERISC

## General
## 技术细节
### 指令编码
本项目将模拟实际汇编语言的编译，讲程序翻译成为16进制指令存储于`instructionStack`中。`instructionStack`大小为0x400010（4M），程序中全部跳转与函数调用指令将直接在编译阶段被翻译为8位地址码，指示跳转后应执行的程序行在`instructionSet`中的位置。
#### 指令

Instruction | Code | Syntax | Meaning|
|---|---|---|---|
|`load`|0x10|
|`store`|0x11|
|`push`|0x20|
|`pop`|0x21|
|`mov`|0x30|
|`add`|0x40|
|`sub`|0x41|
|`mul`|0x50|
|`div`|0x51|
|`rem`|0x52|
|`and`|0x60|
|`or`|0x61|
|*:|Address|N\A|Will not appear in final parsed instruction binary.
|`jal`|0x70|
|`beq`|0x80
|`bne`|0x81
|`blt`|0x82
|`bge`|0x83
|`call`|0x90
|`ret`|0x91
|`draw`|0xa0
|`end`|0x00

### 关于本块README空间的使用

- 基本上作为大家交流想法/记录进度以及制定规则，分享资源的空间。
- 如果有对README进行更新希望大家查看可以在群里吼一声
  
## 档案结构

- 使用makefile编译，规则自行学习修改
- 由于是个多人合作大project，在单一cpp文件中完成是不现实的。请将程序中不同功能的各个模块分开在不同文件中编写，尽量保证单一文件不超过500行。
- 务必学习多文件编译（之后应该会讲，但这个project一定会用到）将希望别人调用的函数声明放在.hpp文档中，并且将函数的实现放在同名.cpp文件中。其中，cpp文档中不希望别人调用的函数应该使用static关键词修饰并**不在**.hpp中暴露其声明。

## 开发注意事项

### Naming Conventions （各种东西的命名规则）

存在的意义是保证程序具有对于除作者以外组员的可读性。保证测试/他人在参考的时候不会想杀人。以下是我之前在参与大型project开发时使用的Naming Convention，个人觉得十分值得借鉴

- 命名时使用具有实际意义的单词/缩写，拒绝一切单一字母命名法。使用的编辑器应该有自动补全功能，所以较长的变量名称不应是问题。
- 命名变量/函数名称时使用lower camel case. 例如：
```cpp
int varName;
int funcName(const int& paramOne);
```
- 命名class, struct, namespace或文件名时使用upper camel case。例如：`class Fraction{}`或`SyntaxParser.cpp`
- 命名常量、pre-process变量时使用全大写并以下划线分割各单词:`const int INT_MAX_VALUE = 2147483647;`
- 时常使用编辑器中代码格式化的功能，养成格式化->保存的习惯

### 关于合作与git的使用

#### 关于函数

- 在多人合作任务中常常需要调用他人撰写的函数，请务必给每一个函数进行注释。推荐使用doxygen进行注释编辑（在VSCode中就是按两个按钮装个插件的事）例如某快速排序：
```cpp
/**
 * @brief Memory based quick sort
 * 
 * @param start pointer to the first element
 * @param end pointer to the last element
 * @param width width of each element
 * @param comp custom compare funciton. 
 */
void qSort(const void *start, const void *end, int width, int (*comp)(const void *first, const void *second));
```
- 如果不能使用doxygen可以不用整理的这么花哨，但简介即各个parameter的介绍还是要写。这个例子没有return，如果有的话也要写。

#### 关于分支的使用
- master分支不使用，仅在做完100分内容及最后卷完之后merge入。相当于外界软件的“稳定版本的概念”
- develop分支作为主要开发的分支，在自己写完新功能，完成测试后先在本地merge，去除所有冲突后，确定无问题后并入。（可以讨论下要不要设置pull request）相当于软件的beta版本，即应该没有已知bug才并入。
- 进行新功能的撰写时应从develop checkout新分支，命名规则为`feature_blah_blah`之后可以在新分支上随便造作，最后完成工程并测试后进行并入develop的步骤

#### 其他

- 欢迎浏览其他人写的代码，如果发现问题，在github网页上的issue界面中提出issue，标记代码作者。写问题的时候不用客气简洁即可。

## 资源分享

- markdown 教程：https://www.runoob.com/markdown/md-tutorial.html    
- 命令行教程：自行百度/google“系统名字 命令行教程”    
- git教程：https://git-scm.com/book/zh/v2/起步-命令行
- 多文件编译：https://www.jianshu.com/p/bd4b563c2c77    
*注：其中使用的#ifndef...#define...#endif写法也可以用#pragma once 代替*    
- makefile(可以先不看，我没看到适合当前进度的教程)：https://course.cse.ust.hk/comp2012/notes/self-study-makefile-full.pdf
