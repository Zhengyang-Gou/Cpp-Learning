# 09 - C++ 静态关键字 (Static)

## 1. 基本概念 (Basic Concepts)

在 C++ 类或结构体中，`static` 关键字改变了成员变量的生命周期和作用域。静态成员不再属于某个具体的对象实例，而是属于类本身。

| 成员类型 | 存储位置 | 生命周期 | 访问方式 |
| :--- | :--- | :--- | :--- |
| **普通成员 (Normal)** | 随对象分配在堆栈中 | 随对象创建而生，随对象销毁而死 | `object.member` |
| **静态成员 (Static)** | 全局/静态存储区 | 程序启动时分配，程序结束时销毁 | `ClassName::member` |



## 2. 静态成员的核心规则 (Key Rules)

* **全局唯一性**：无论创建多少个对象，静态成员在内存中只有一份副本。
* **共享性**：所有对象实例共享同一个静态变量，一个对象对其修改，其他对象可见。
* **类外初始化**：在类内声明的静态变量，必须在类外进行定义和初始化，否则会导致链接错误。
* **独立性**：静态成员不依赖于对象存在，可以通过类名直接访问。

---

## 示例代码

```cpp
#include <iostream>

// 1. 普通结构体：每个对象独立拥有 x 和 y
struct Entity {
    int x, y;

    void print() {
        std::cout << "Entity (Normal) -> x: " << x << ", y: " << y << std::endl;
    }
};

// 2. 带有静态成员的结构体：所有对象共享同一个 x 和 y
struct EntityStatic {
    static int x, y; 

    void print() {
        std::cout << "Entity (Static) -> x: " << x << ", y: " << y << std::endl;
    }
};

// 【重要】必须在类外定义静态变量，为其分配内存
int EntityStatic::x;
int EntityStatic::y;

int main() {
    std::cout << "--- 普通变量测试 ---" << std::endl;
    Entity e1; e1.x = 5; e1.y = 8;
    Entity e2; e2.x = 10; e2.y = 16;

    e1.print(); // 输出 5, 8
    e2.print(); // 输出 10, 16（互不干扰）

    std::cout << "\n--- 静态变量测试 ---" << std::endl;
    EntityStatic s1;
    s1.x = 5; s1.y = 8; 

    EntityStatic s2;
    s2.x = 10; s2.y = 16; // 这里实际上修改了全局唯一的 x 和 y

    s1.print(); // 输出 10, 16 (受 s2 修改影响)
    s2.print(); // 输出 10, 16

    // 推荐用法：直接通过类名访问，不依赖实例
    EntityStatic::x = 100;
    std::cout << "\n直接通过类名访问: " << EntityStatic::x << std::endl;

    return 0;
}