# 21 - C++ 成员初始化列表 (Member Initializer Lists)

## 1. 核心概念 (Core Concepts)

成员初始化列表是 C++ 中在构造函数初始化类成员变量的一种特定语法。对于 `int`、`float` 等基本数据类型，使用传统赋值和使用初始化列表在性能上没有任何区别。**但是，当成员变量是另一个类（对象）时，两者的性能区别会非常大。**

| 数据类型 | 传统赋值 (构造函数体内) | 初始化列表 (冒号语法) | 性能差异 |
| :--- | :--- | :--- | :--- |
| **基本数据类型**<br>(如 `int`, `float`) | 正常赋值 | 正常初始化 | **无区别** |
| **类对象**<br>(如 `std::string`, 自定义类) | 先调用该成员的**默认构造函数**，然后在体内进行**赋值操作**。 | 直接调用该成员**对应的构造函数**进行初始化。 | **差异巨大！**传统写法会白白浪费一次默认构造函数的开销。 |

## 2. 语法对比 (Syntax Comparison)

初始化列表位于构造函数的参数列表之后，函数体的大括号之前。

| 写法 | 特点与语法规则 | 代码片段 |
| :--- | :--- | :--- |
| **传统赋值写法** | 在构造函数的大括号 `{}` 内部使用等号 `=` 进行赋值。 | `Entity() { m_Score = 0; }` |
| **初始化列表写法** | 在参数列表后用冒号 `:` 开启，变量后跟括号 `()` 进行传参，多个成员用逗号 `,` 隔开。 | `Entity() : m_Score(0) {}` |

> **最佳实践**：在 C++ 中，**只要能用初始化列表，就永远使用初始化列表**。这不仅能保持代码整洁，还能避免隐藏的性能浪费。
>
> **注意**：初始化列表中的初始化顺序，应该与成员变量在类中声明的顺序严格保持一致，否则某些编译器会发出警告。

---

## 示例代码

```cpp
#include <iostream>
#include <string>

/*
对于像 int、float 这样的基本数据类型，两种写法在性能上没有任何区别
但是，当你的成员变量是另一个类时，区别就大了
*/

// 1. 传统写法
class Entity {
private:
    std::string m_Name;
    int m_Score;
public:
    // 传统写法：在构造函数体内赋值
    Entity() {
        m_Name = "Unknown";
        m_Score = 0;
    }
};

// 2. 初始化列表写法 (推荐)
class Entity_Init {
private:
    std::string m_Name;
    int m_Score;
public:
    // 初始化列表写法
    Entity_Init()
        : m_Name("Unknown"), m_Score(0) // 注意这里是用括号()来初始化，且用逗号分隔
    {
        // 构造函数体现在是空的
    }
};



class Example {
public:
    Example() {
        std::cout << "Created Entity!" << std::endl;
    }
    Example(int x) {
        std::cout << "Created Entity with " << x << "!" << std::endl;
    }
};

// 错误示范：浪费性能
class Entity_Test {
private:
    Example m_Example;
public:
    Entity_Test() {
        // 走到这里之前，m_Example 已经调用了无参构造函数 Example()
        m_Example = Example(8); // 在这里又创建了一个临时对象并赋值，极其浪费性能
    }
};

// 正确示范：高效初始化
class Entity_Right {
private:
    Example m_Example;
public:
    Entity_Right()
        : m_Example(Example(8)) // 或者简写为 : m_Example(8)
    {
        // 直接一步到位，只调用一次 Example(int) 构造函数
    }
};

int main() {
    std::cout << "--- 测试 Entity_Test (传统写法) ---" << std::endl;
    Entity_Test();  // 会打印两次：先无参构造，再带参构造
    
    std::cout << "\n--- 测试 Entity_Right (初始化列表) ---" << std::endl;
    Entity_Right(); // 只会打印一次：直接带参构造
}
```