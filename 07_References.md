# 07 - C++ 引用 (References)

## 1. 基本概念 (Basic Concepts)

引用是变量的一个**别名 (Alias)**。它并不像指针那样是一个存储地址的独立变量，而是与原变量共享同一块内存地址。一旦引用被初始化，它就“绑定”到了该变量上，无法再指向其他变量。

| 特性 | 引用 (Reference) | 指针 (Pointer) |
| :--- | :--- | :--- |
| **定义语法** | `int& ref = var;` | `int* ptr = &var;` |
| **是否独立** | 不是独立变量，不占额外空间（逻辑上） | 是独立变量，存储内存地址 |
| **初始化** | **必须**在声明时初始化 | 可以先声明后赋值 |
| **空值** | 不能为空 (NULL) | 可以为 `nullptr` |
| **重定向** | 一旦绑定，不可更改指向 | 可以随时指向另一个地址 |

## 2. 函数传参：值传递 vs 引用传递

引用最强大的用途是在函数参数传递中。

| 传递方式 | 关键字/符号 | 内存表现 | 结果 |
| :--- | :--- | :--- | :--- |
| **值传递** | `(int Value)` | 复制一份数据的副本到函数栈 | 函数内修改不影响原变量 |
| **引用传递** | `(int& Value)` | 直接操作原变量的内存地址 | 函数内修改会同步改变原变量 |

---

## 示例代码

```cpp
#include <iostream>

// 1. 值传递 (Pass by Value)
// 这里的 Value 是 var 的一个克隆，修改 Value 不会改变 main 里的 var
void Plus1(int Value) 
{
    Value++;
}

// 2. 引用传递 (Pass by Reference)
// 这里的 value 是 var 的“绰号”，操作 value 就是在操作 var 本身
void Plus(int& value) 
{
    value++;
}

int main()
{
    int var = 8;

    // 调用引用传递：var 变成了 9
    Plus(var); 
    
    // 调用值传递：var 依然是 9 (Plus1 内部的增加不影响这里)
    Plus1(var);

    // 3. 定义引用
    // ref 只是 var 的另一个名字，它们指向同一个内存地址
    int& ref = var; 
    
    // 修改 ref 等同于修改 var
    ref = 100;

    std::cout << "var 的最终结果: " << var << std::endl; // 输出 100
    
    return 0;
}