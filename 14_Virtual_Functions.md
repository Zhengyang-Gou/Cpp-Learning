# 14 - C++ 虚函数 (Virtual Functions)

## 1. 核心概念 (Core Concepts)

**虚函数**是实现**多态 (Polymorphism)** 的基础。在没有虚函数的情况下，C++ 默认使用“静态分派”，即根据指针或引用的类型来决定调用哪个函数。而虚函数允许程序在**运行时**根据对象的实际类型来寻找并执行正确的方法。

## 2. 为什么需要虚函数？

当一个函数接收父类指针（如 `Entity*`）作为参数时，如果传入的是子类对象（如 `Player`），默认情况下它只会调用父类定义的版本。

| 机制 | 描述 | 结果 |
| :--- | :--- | :--- |
| **静态分派 (Static Dispatch)** | 默认行为。根据指针的类型（编译时已知）来调用函数。 | 即使传入子类，也执行父类逻辑。 |
| **动态分派 (Dynamic Dispatch)** | 使用 `virtual` 关键字。在运行时查找“虚函数表 (V-Table)”来决定调用。 | 传入子类时，执行子类重写后的逻辑。 |


## 3. 关键字说明 (Key Keywords)

* **`virtual`**: 在父类的函数声明前添加。它告诉编译器不要在编译时硬编码调用路径，而是创建一个虚函数表。
* **`override`**: (C++11 标准引入) 建议在子类重写的方法后加上。它能让编译器帮你检查函数签名是否与父类一致，防止因拼写错误而没能成功重写。

---

## 示例代码

```cpp
#include <iostream>
#include <string>

class Entity {
public:
    // 1. 在父类方法前加上 virtual，开启动态分派
    virtual std::string GetName() { return "Entity"; }
};

class Player : public Entity {
private:
    std::string m_Name;
public:
    Player(const std::string& name) : m_Name(name) {}
    
    // 2. 在子类中使用 override 关键字
    // 这能确保你确实是在重写父类的方法，如果名字写错（如 GetNames），编译器会报错
    std::string GetName() override { return m_Name; } 
};

// 这是一个多态的典型用法：
// 该函数只认识 Entity*，但由于虚函数的存在，它能根据传入对象的真身打印正确名字
void PrintName(Entity* entity) { 
    std::cout << entity->GetName() << std::endl;
}

int main() {
    Entity* e = new Entity();
    PrintName(e); // 输出: Entity

    // 向上转型 (Upcasting)：Player* 被当作 Entity* 传入
    Player* p = new Player("Cherno");
    PrintName(p); // 输出: Cherno (如果没有 virtual，则会输出 Entity)

    delete e;
    delete p;
    std::cin.get();
}