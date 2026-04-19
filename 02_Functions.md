# 02 - C++ 函数 (Functions)

## 1. 函数的基础概念 (Function Basics)

函数是执行特定任务的独立代码块。将代码逻辑封装成函数，可以极大地提高代码的复用性、可读性和可维护性。

| 概念 | 说明 | 示例代码中的体现 |
| :--- | :--- | :--- |
| **函数定义** | 规定了函数的返回类型、名称、参数列表和具体执行的操作。 | `int Multiply(int a, int b) { ... }` |
| **参数 (Parameters)** | 函数执行时需要传入的外部数据。 | `int a` 和 `int b` 是传入的两个整数。 |
| **返回值 (Return)** | 函数执行完毕后，交还给调用者的结果。 | `return a * b;` 返回乘积结果。 |
| **`main` 函数** | C++ 程序的固定入口点。 | 程序的执行从 `main` 开始，并且**默认隐式返回 0**，表示正常结束。 |

---

## 示例代码

```cpp
#include <iostream>

// 定义一个名为 Multiply 的函数，接收两个 int 类型参数，返回一个 int 类型结果
int Multiply(int a, int b) 
{
    return a * b;
}

int main()
{
    // 调用 Multiply 函数，并将结果赋值给变量 result
    int result = Multiply(3, 4);
    
    // 打印结果到控制台
    std::cout << result << std::endl;
    
    // main 函数默认返回 0
}
```
