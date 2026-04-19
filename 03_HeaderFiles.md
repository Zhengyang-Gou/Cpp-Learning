# 03 - C++ 头文件与声明 (Header Files & Declarations)

## 1. 头文件的作用 (Purpose of Header Files)

随着程序规模的扩大，把所有代码写在一个文件里会导致代码难以维护。C++ 通过头文件（`.h` 或 `.hpp`）将函数的**声明 (Declaration)** 与**定义 (Definition)** 分离开来，方便在不同的源文件中复用代码。

| 概念 | 含义 | 示例 |
| :--- | :--- | :--- |
| **头文件 (`.h`)** | 通常包含函数的声明、类的定义、宏定义等，告诉编译器“有这个东西存在，以及它的用法”。 | `Multiply.h` |
| **源文件 (`.cpp`)** | 包含函数的具体实现（定义），告诉编译器“这个东西具体是怎么工作的”。 | `03_HeaderFiles.cpp` |
| **包含指令 (`#include`)** | 预处理指令，在编译前将指定的头文件内容直接“复制粘贴”到当前位置。 | `#include "Multiply.h"` |

> **提示：尖括号与双引号的区别**
> * `#include <iostream>`：**尖括号**通常用于包含 C++ 标准库或系统提供的头文件，编译器会直接去系统目录查找。
> * `#include "Multiply.h"`：**双引号**通常用于包含你自己编写的相对路径头文件，编译器会优先在当前项目目录下查找。

## 2. 预处理指令 `#pragma once`

在头文件中，我们经常会看到首行写着 `#pragma once`。它的作用是**防止头文件被重复包含**。如果一个头文件在同一个源文件中被 `#include` 了多次（有时是间接包含），可能会导致“重复定义”的编译错误。`#pragma once` 确保该头文件在一次编译中只被引入一次。

---

## 示例代码

### 文件 1：`Multiply.h` (头文件：仅声明)
```cpp
#pragma once

// 声明 Multiply 函数：告诉编译器有这个函数，接收两个 int，返回一个 int
// 注意：这里只有分号，没有大括号和具体实现
int Multiply(int a, int b);
```

### 文件 2：`03_HeaderFiles.cpp` (源文件：调用)
```cpp
#include <iostream>
#include "Multiply.h" // 引用自定义的头文件，获取 Multiply 函数的声明

int main()
{
    // 因为包含了 Multiply.h，编译器知道 Multiply 的存在及其参数要求
    int result = Multiply(3, 4);
    
    std::cout << result << std::endl;
    
    return 0;
}
```

