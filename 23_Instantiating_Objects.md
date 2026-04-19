# 23 - C++ 实例化对象 (Instantiating Objects)

## 1. 核心概念 (Core Concepts)

在 C++ 中，当你定义了一个类之后，你需要将其“实例化”才能在内存中真正创建这个对象。与 C# 或 Java 不同，C++ 赋予了开发者对内存的绝对控制权，你可以自由选择将对象创建在**栈 (Stack)** 上，或者创建在**堆 (Heap)** 上。

## 2. 栈分配 vs 堆分配 (Stack vs. Heap Allocation)

在 C++ 的最佳实践中，**只要有可能，就应该首选栈分配**。只有当遇到特定需求时（如对象过大或需要跨作用域存活），才考虑堆分配。

| 分配方式 | 语法特征 | 性能与内存管理 | 适用场景 / 核心痛点 |
| :--- | :--- | :--- | :--- |
| **栈分配 (Stack)** | `Entity e;` | **极致的性能**。由所在的作用域（`{}`）严格控制生命周期，一旦离开作用域，内存会**自动清理**。 | **默认首选**。适用于绝大多数常规对象，特别是只在当前函数内使用的临时对象。 |
| **堆分配 (Heap)** | `new Entity();` | 分配速度较慢。`new` 返回的是内存指针，生命周期完全由开发者手动控制，**必须手动清理**。 | 对象数据非常庞大（为避免栈溢出），或者希望对象在跳出当前函数作用域后依然存活。 |

> **语法避坑指南**：在 C++ 中进行栈分配并调用**无参默认构造函数**时，千万不要加空括号（例如 `Entity entity();` 是错误的）！编译器会把它误认为是一个返回 `Entity` 类型的函数声明。正确的写法是直接写 `Entity entity;`。

---

## 示例代码

```cpp
#include <iostream>
#include <string>

// 如何在内存中创建你的类
class Entity {
private:
    std::string m_Name;
public:
    Entity() : m_Name("Unknown") {}
    Entity(const std::string& name) : m_Name(name) {}
};

// 1. 栈分配 (Stack Allocation)
// 极致的性能，自动清理
void CreateObject() {
    // 方式一：调用默认构造函数
    Entity entity; 
    
    // 方式二：调用带参数的构造函数
    Entity entity2("Cherno"); 

    // 当函数执行完毕（遇到右大括号 '}'）时
    // 栈上的 entity 和 entity2 会被自动销毁，内存立即回收
}

// 2. 堆分配 (Heap Allocation)
// 当你需要控制对象的生命周期，或者对象太大时使用
void CreateObjectOnHeap() {
    // 使用 new 关键字，就是在堆上动态分配内存
    // 它在堆上寻找合适的空间，调用构造函数，并返回一个指向该内存首地址的指针
    Entity* entity = new Entity("Cherno");
    
    // 当你不再需要它时，必须使用 delete 关键字手动释放它。
    delete entity; 
}

int main() {
    CreateObject();
    CreateObjectOnHeap();
    
    std::cin.get();
    return 0;
}
```