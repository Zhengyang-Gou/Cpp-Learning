# 13 - C++ 继承 (Inheritance)

## 1. 核心目的 (Core Purpose)

继承是面向对象编程（OOP）的核心支柱之一。它的存在主要为了解决两个问题：
1. **消除重复代码**：将多个类中相同的属性和方法提取到“父类”中。
2. **建立层级关系**：在类与类之间建立一种包含关系（通常被称为 "Is-A" 关系，例如：Player **是一个** Entity）。

## 2. 语法与内存表现 (Syntax & Memory)

在 C++ 中，我们通过在类名后使用冒号 `:` 来表示继承。

| 术语 | 描述 |
| :--- | :--- |
| **父类 (Base Class)** | 被继承的类，存放通用属性（如 `Entity`）。 |
| **子类 (Subclass)** | 继承者，自动拥有父类所有非私有（Public/Protected）成员（如 `Player`）。 |
| **内存布局** | 子类的总内存 = 父类的成员占用 + 子类特有的成员占用。 |


## 3. 关键特性 (Key Features)

* **代码复用**：子类无需重新编写父类已有的逻辑，直接通过 `.` 即可访问。
* **扩展性**：可以在不修改父类代码的前提下，为子类增加特有的功能（如 `PrintName`）。
* **内存效率**：尽管子类继承了父类，但它们在内存中依然是连续存储的结构。

---

## 示例代码

```cpp
// 继承的核心目的就一个：消除重复代码，并在类与类之间建立一种包含关系
#include <iostream>

class Entity { // 父类 (Base Class)
public:
    float X, Y;

    void Move(float xa, float ya) {
        X += xa;
        Y += ya;
    }
};

// 使用冒号 : 加 public 关键字进行继承
// Player 继承了 Entity 的所有公开内容
class Player : public Entity {
public:
    const char* Name;

    void PrintName() {
        std::cout << Name << std::endl;
    }
};

int main() {
    // 观察内存占用情况
    std::cout << "Entity size: " << sizeof(Entity) << " bytes" << std::endl; // 输出 8 (2个 float)
    std::cout << "Player size: " << sizeof(Player) << " bytes" << std::endl; // 输出 16 (2个 float + 1个指针)

    Player player;
    
    // 1. 虽然 Player 没定义 Move，但由于继承，它可以直接使用
    player.Move(5.0f, 5.0f); 
    
    // 2. 直接访问父类的成员变量
    player.X = 2.0f;
    
    // 3. 使用子类特有的方法
    player.Name = "Cherno";
    player.PrintName();

    std::cin.get();
}