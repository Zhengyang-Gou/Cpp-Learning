# 26 - C++ 运算符及其重载 (Operators & Operator Overloading)

## 1. 核心概念 (Core Concepts)

在 C++ 中，像 `+`、`-`、`*`、`/` 甚至 `==` 这样的运算符，**本质上只不过是函数而已**。运算符重载允许我们为自定义的类或结构体重新定义这些基本运算符的行为，从而让对象之间的操作像处理基础数据类型（如 `int` 或 `float`）一样自然。

## 2. 传统函数与运算符重载对比 (Methods vs. Operators)

对于某些特定的数据结构（特别是像向量、矩阵这样的数学概念），如果我们只能依赖传统的函数调用，复杂的计算公式会变得极其难以阅读。

| 调用方式 | 语法机制 | 代码示例表现 | 可读性对比 |
| :--- | :--- | :--- | :--- |
| **普通成员函数** | 显式定义诸如 `Add` 等函数进行计算 | `Vector2 result = a.Add(b.Multiply(c));` | 逻辑明确，但在公式嵌套较深时非常繁琐、难以阅读。 |
| **运算符重载** | 使用 `operator+` 等关键字重载符号 | `Vector2 result = a + b * c;` | **极其简洁**，符合人类的数学直觉和常识。 |

## 3. 运算符重载的黄金准则 (The Golden Rule)

> **强烈警告**：重载运算符的唯一目的是**为了让代码更易读、更符合直觉**。
> 如果你重载的运算符，不能让人第一眼就凭常识猜出它是干什么的，那就**绝对不要使用重载**，请老老实实去写一个普通的具名函数。
> *(例如：给向量重载 `+` 表示位置相加是合理的；但如果在重载的 `+` 里写了清空数据或打印日志的诡异逻辑，那就是糟糕的设计。)*

---

## 示例代码

```cpp
#include <iostream>

// ==========================================
// 1. 传统写法：没有重载运算符
// ==========================================
struct Vector2_no_operator {
    float x, y;
    Vector2_no_operator(float x, float y) : x(x), y(y) {}

    Vector2_no_operator Add(const Vector2_no_operator& other) const {
        return Vector2_no_operator(x + other.x, y + other.y);
    }

    Vector2_no_operator Multiply(const Vector2_no_operator& other) const {
        return Vector2_no_operator(x * other.x, y * other.y);
    }
};

// ==========================================
// 2. 推荐写法：使用运算符重载
// ==========================================
struct Vector2_operator {
    float x, y;
    Vector2_operator(float x, float y) : x(x), y(y) {}

    // 重载 + 运算符
    // 本质上就是一个名为 operator+ 的函数
    Vector2_operator operator+(const Vector2_operator& other) const {
        return Vector2_operator(x + other.x, y + other.y);
    }

    // 重载 * 运算符
    Vector2_operator operator*(const Vector2_operator& other) const {
        return Vector2_operator(x * other.x, y * other.y);
    }
};

int main() {
    // 传统函数调用演示
    Vector2_no_operator a(1.0f, 2.0f);
    Vector2_no_operator b(3.0f, 4.0f);
    Vector2_no_operator c = a.Add(b); // 使用成员函数进行加法
    std::cout << "Without operator overloading: " << c.x << ", " << c.y << std::endl;

    // 运算符重载演示 (代码更加优雅直观)
    Vector2_operator d(1.0f, 2.0f);
    Vector2_operator e(3.0f, 4.0f);
    Vector2_operator f = d + e; // 直接使用重载的 + 运算符进行加法！
    std::cout << "With operator overloading: " << f.x << ", " << f.y << std::endl;

    return 0;
}
```