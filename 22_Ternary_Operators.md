# 22 - C++ 三元运算符 (Ternary Operators)

## 1. 核心概念 (Core Concepts)

三元运算符（也称为条件运算符）是 C++ 中唯一一个需要三个操作数的运算符。它通常被用作简单 `if-else` 语句的语法糖，能够让代码更加简洁易读。

## 2. 语法结构 (Syntax)

| 结构部分 | 符号 | 含义 |
| :--- | :--- | :--- |
| **条件判断** | `?` 前面 | 需要进行判断的布尔条件。如果为 `true`，则返回第一部分的值；如果为 `false`，则返回第二部分的值。 |
| **真值返回** | `?` 和 `:` 之间 | 条件成立时执行的代码或返回的值。 |
| **假值返回** | `:` 后面 | 条件不成立时执行的代码或返回的值。 |

**基本语法：** `condition ? expression_if_true : expression_if_false;`

## 3. 核心优势 (Key Advantages)

使用三元运算符不仅仅是为了少写几行代码，它在特定场景下有着传统的 `if-else` 无法比拟的优势：

| 优势点 | 详细说明 |
| :--- | :--- |
| **支持 Const 初始化** | 在传统的 `if-else` 中，你必须先声明变量然后再赋值，这意味着该变量**不能**被标记为 `const`。而三元运算符允许你在声明变量的**同时**进行条件判断并赋值，从而完美支持 `const` 常量初始化。 |
| **提高代码密度** | 将多行的条件分支逻辑压缩到一行，提高代码的阅读效率（但请注意：严禁滥用多重嵌套的三元运算符，否则会严重降低代码可读性）。 |

---

## 示例代码

```cpp
#include <iostream>

int main() {
    int level = 6;
    
    // 1. 传统 if-else 写法
    // 必须先声明变量，无法使用 const
    int speed_normal;

    if (level > 5) {
        speed_normal = 10;
    }
    else {
        speed_normal = 5;
    }

    // 2. 三元运算符写法
    const int speed_ternary = level > 5 ? 10 : 5;
    
    std::cout << "Normal: " << speed_normal << std::endl;
    std::cout << "Ternary: " << speed_ternary << std::endl;

    return 0;
}
```