# 16 - C++ 可见性 (Visibility)

## 1. 核心概念 (Core Concepts)

可见性（或称为访问修饰符）是面向对象编程中用于控制类成员（变量和方法）访问权限的机制。主要是为了更好的代码架构设计和增强代码的可维护性。
    
> **重要提示**：可见性仅仅是一个语言语法层面的概念。它对程序的**性能**、**内存布局**以及最终编译出的**机器码**，**没有任何影响**。

## 2. 访问修饰符 (Access Modifiers)

C++ 中提供了三种基础的访问修饰符，它们的访问权限级别如下表所示：

| 关键字 | 访问权限 | 说明 |
| :--- | :--- | :--- |
| `private` | **类内部** | 私有成员。只有这个类本身的方法才能访问。类的外部和子类均无法访问。 |
| `protected` | **类内部 + 子类** | 受保护成员。在类外部看起来和 `private` 一模一样（外部无法访问），但继承它的子类（派生类）可以访问。 |
| `public` | **任何地方** | 公有成员。任何地方（类内部、外部、子类等）都可以随意访问。 |

---

## 示例代码

```cpp
/*
可见性对程序的性能、内存布局以及最终编译出的机器码，没有任何影响

private: 只有这个类本身的方法才能访问
protected: 在类外部看起来和 private 一模一样（外部无法访问），但子类可以访问
public: 随意访问 public 成员
*/
#include <iostream>

// 基类
class Entity {
private:
    int m_PrivateVar;   // 仅 Entity 内部可访问

protected:
    int m_ProtectedVar; // Entity 及继承它的子类可访问

public:
    int m_PublicVar;    // 任何地方都可访问

    Entity() {
        m_PrivateVar = 0;   // OK
        m_ProtectedVar = 0; // OK
        m_PublicVar = 0;    // OK
    }
};

// 子类
class Player : public Entity {
public:
    Player() {
        // m_PrivateVar = 1; // 错误！无法访问父类的 private 成员
        m_ProtectedVar = 1;  // OK：可以访问父类的 protected 成员
        m_PublicVar = 1;     // OK：可以访问父类的 public 成员
    }
};

int main() {
    Entity e;
    
    // e.m_PrivateVar = 2;   // 错误！外部无法访问 private 成员
    // e.m_ProtectedVar = 2; // 错误！外部无法访问 protected 成员
    
    e.m_PublicVar = 2;       // OK：外部可以随意访问 public 成员

    std::cin.get();
    return 0;
}
```