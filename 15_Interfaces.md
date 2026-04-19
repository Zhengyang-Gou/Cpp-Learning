# 15 - C++ 接口 (Interfaces)

## 1. 纯虚函数与抽象类 (Pure Virtual Functions & Abstract Classes)

在 C++ 中，接口并不是像其他高级语言那样的专门关键字，而是通过**纯虚函数**（Pure Virtual Function）来实现的。包含至少一个纯虚函数的类被称为**抽象类**（Abstract Class）。接口的核心作用是只提供方法声明，强制子类实现具体的逻辑。

| 核心概念 | 特点 / 含义 | 代码示例 |
| :--- | :--- | :--- |
| **纯虚函数** | 在虚函数声明末尾加上 `= 0`，没有具体的函数体。 | `virtual std::string GetClassName() = 0;` |
| **抽象类** | 包含纯虚函数的类，**无法直接被实例化**（不能创建对象）。 | `class Printable { ... };` |

## 2. 接口的继承与实现 (Inheritance & Implementation)

继承了接口（抽象类）的子类，必须重写并实现父类中所有的纯虚函数。

| 规则要求 | 说明 |
| :--- | :--- |
| **强制实现** | 如果子类没有实现所有的纯虚函数，那么该子类也会变成抽象类，从而无法创建实例。 |
| **重写关键字** | 在实现纯虚函数时，使用 `override` 关键字来明确表示这是对父类方法的覆盖。 |

## 3. 多态应用 (Polymorphism Application)

接口最大的优势在于实现**多态**。当我们将函数的参数定义为接收“接口指针”或“接口引用”时，该函数就可以安全地接收任何实现了该接口的子类对象。

---

## 示例代码

```cpp
/*
只提供接口，强制子类实现
*/
#include <iostream>
#include <string>

// 1. 定义一个接口（本质上是包含纯虚函数的类）
class Printable {
public:
    virtual std::string GetClassName() = 0; // 纯虚函数，类变成抽象类
};

// 2. Entity 继承了 Printable 接口
// 继承抽象类的子类必须实现所有的纯虚函数
class Entity : public Printable {
public:
    virtual std::string GetName() { return "Entity"; }

    // 如果不写这行，Entity 也会变成抽象类，无法创建实例！
    std::string GetClassName() override { return "Entity"; }
};

// 3. Player 继承了 Entity（间接继承了 Printable）
class Player : public Entity {
private:
    std::string m_Name;
public:
    Player(const std::string& name) { m_Name = name; }
    
    std::string GetName() override { return m_Name; } 

    // 【必须实现】覆盖父类的实现
    std::string GetClassName() override { return "Player"; }
};

// 4. 终极多态：这个函数现在可以接收任何“实现了 Printable 接口”的对象！
void PrintClassName(Printable* obj) { 
    std::cout << obj->GetClassName() << std::endl;
}

int main() {
    Entity* e = new Entity();
    Player* p = new Player("Cherno");

    // 只要是实现了 Printable 的对象，都可以安全地传进来
    PrintClassName(e); // 打印 Entity
    PrintClassName(p); // 打印 Player

    std::cin.get();
}
```