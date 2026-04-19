# 20 - C++ Mutable 关键字 (Mutable)

## 1. 核心概念 (Core Concepts)

`mutable` 关键字在 C++ 中主要有两个截然不同但又非常实用的场景：一个是在类（Class）中配合 `const` 方法使用，另一个是在 Lambda 表达式中配合按值捕获使用。它的核心作用是**打破常规的常量（const）限制**，允许对特定变量进行修改。

## 2. 类中的 Mutable (Mutable in Classes)

| 场景特点 | 机制与说明 | 常见用途 |
| :--- | :--- | :--- |
| **突破 Const 限制** | 当类的方法被标记为 `const` 时，通常向编译器保证不会修改任何成员变量。但若将某个特定的成员变量标记为 `mutable`，它就可以在 `const` 方法中被合法地修改。 | 调试计数（记录 `const` 方法被调用的次数）、数据缓存（Cache 的延迟计算）、线程锁（Mutex 加锁）。 |

## 3. Lambda 表达式中的 Mutable (Mutable in Lambdas)

| 场景特点 | 机制与说明 | 核心注意点 |
| :--- | :--- | :--- |
| **修改值捕获的副本** | 当 Lambda 表达式使用 `[=]`（按值捕获外部变量）时，捕获进来的变量默认是只读的。在参数列表后加上 `mutable` 关键字，就可以在 Lambda 内部修改这些变量。 | **这仅仅是修改了拷贝的副本**。Lambda 内部的变化绝不会影响到外部作用域中原本的变量。 |

---

## 示例代码

```cpp
/*
Mutable 允许变量在 const 方法被修改
*/
#include <iostream>

class Entity {
private:
    int m_X;
    mutable int m_DebugCount;
public:
    int GetX() const {
        m_DebugCount++; // 可以修改
        return m_X;
    }
};

int main() {
    int x = 8;
    auto f = [=]() mutable{
        x++;
        std::cout << x << std::endl; // 输出 9
    };

    f();
    std::cout << x << std::endl; // 输出 8，外部的 x 并未改变
}
```