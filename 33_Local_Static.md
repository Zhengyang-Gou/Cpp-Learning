# 48 - C++ 局部静态变量 (Local Static)

## 1. 基本概念 (Basic Concepts)

局部静态变量是指**定义在函数内部，但使用 `static` 修饰的变量**。  
它和普通局部变量最大的区别不在于“能不能访问到”，而在于**生命周期 (Lifetime)**。

普通局部变量在函数每次调用时创建，函数结束时销毁。  
而局部静态变量**只会初始化一次**，并且会一直存在到整个程序结束。

也就是说：

- **作用域 (Scope)**：仍然只在当前函数内部可见
- **生命周期 (Lifetime)**：贯穿整个程序运行期间

---

## 2. Scope 和 Lifetime 的区别

Cherno 在这一节里重点强调，要区分下面两个概念：

| 概念 | 含义 |
| :--- | :--- |
| **作用域 (Scope)** | 变量在哪些地方可以被访问 |
| **生命周期 (Lifetime)** | 变量在内存中存在多久 |

局部静态变量的关键点就在于：

- 它的 **Scope** 没变，还是局部的
- 但它的 **Lifetime** 改变了，变成整个程序运行期

---

## 3. 普通局部变量 vs 局部静态变量

| 特性 | 普通局部变量 (Local Variable) | 局部静态变量 (Local Static Variable) |
| :--- | :--- | :--- |
| **定义位置** | 函数内部 | 函数内部 |
| **作用域** | 当前函数内部 | 当前函数内部 |
| **初始化次数** | 每次调用函数都会初始化 | 只初始化一次 |
| **生命周期** | 函数调用期间 | 程序整个运行期间 |
| **是否保留上次值** | 不保留 | 会保留 |

---

## 4. 示例代码 

```cpp
#include <iostream>

// 1. 普通局部变量
void Function1()
{
    int i = 0;
    i++;
    std::cout << "Function1: " << i << std::endl;
}

// 2. 局部静态变量
void Function2()
{
    static int i = 0;
    i++;
    std::cout << "Function2: " << i << std::endl;
}

int main()
{
    Function1();
    Function1();
    Function1();

    std::cout << "--------" << std::endl;

    Function2();
    Function2();
    Function2();

    return 0;
}