# 29 - C++ 智能指针 (Smart Pointers)

## 1. 核心概念 (Core Concepts)

智能指针是 C++ 引入的核心特性，用于**自动管理堆（Heap）上对象的生命周期**。它本质上是一个包装了原始指针（Raw Pointer）的类。当智能指针离开其作用域时，它会自动调用底层的 `delete` 来销毁对象，从而从根本上避免了**内存泄漏 (Memory Leak)** 和**野指针 (Dangling Pointer)**。

> **前置要求**：使用智能指针必须包含头文件 `#include <memory>`。

## 2. 智能指针的三种类型 (Types of Smart Pointers)

C++ 提供了三种主要的智能指针，分别应对不同的内存管理策略：

| 指针类型 | 所有权模型 | 核心机制与特点 | 代码创建规范 |
| :--- | :--- | :--- | :--- |
| **`unique_ptr`** | **独占型** (Unique) | 同一时刻只能有一个指针拥有该对象。**严禁复制 (Copy)**，只能通过 `std::move` **转移 (Move)** 所有权。性能最高，开销几乎等同于原始指针。 | 首选 `std::make_unique<T>()` |
| **`shared_ptr`** | **共享型** (Shared) | 允许多个指针共享同一个对象。底层使用**引用计数**机制：多一个指针指向它就 +1，销毁一个就 -1。当计数归零时，内存自动释放。 | 首选 `std::make_shared<T>()` |
| **`weak_ptr`** | **观察者** (Observer) | 不拥有对象，**不会增加引用计数**。通常配合 `shared_ptr` 使用，专门用来打破两个 `shared_ptr` 互相引用导致的“循环引用（内存死锁）”问题。 | `std::weak_ptr<T> w = shared_ptr;` |

---

## 示例代码

```cpp
// Smart Pointers 自动管理 heap 对象的生命周期，避免 memory leak 和野指针
#include <iostream>
#include <memory> // 包含智能指针的头文件

class Entity {
public:
    Entity() { std::cout << "Created\n"; }
    ~Entity() { std::cout << "Destroyed\n"; }
    void Print() { std::cout << "Hello from Entity!\n"; }
};

int main() {
    // 1. unique_ptr (独占指针)
    // 创建一个 unique_ptr，自动管理 Entity 对象的生命周期
    std::unique_ptr<Entity> entity = std::make_unique<Entity>(); 
    
    // 正常像普通指针一样使用 -> 访问对象成员
    entity->Print();

    // 核心特性：unique_ptr 只能 move（移动），不能 copy（复制）
    std::unique_ptr<Entity> e1 = std::make_unique<Entity>();
    // std::unique_ptr<Entity> e2 = e1; // 错误！编译器会直接报错
    std::unique_ptr<Entity> e2 = std::move(e1); // 正确！e1 把所有权转交给了 e2

    // 2. shared_ptr (共享指针)
    // shared_ptr 允许多个指针共享同一个对象
    std::shared_ptr<Entity> sharedEntity1 = std::make_shared<Entity>();
    
    // 这里并没有拷贝对象本身，而是让 sharedEntity2 指向同一个对象，并将内部的“引用计数”加 1
    std::shared_ptr<Entity> sharedEntity2 = sharedEntity1; 

    // 3. weak_ptr 
    // weak_ptr 不拥有对象，只是观察者，它不会增加 sharedEntity1 的引用计数
    // 主要作用是避免互相引用导致的内存泄漏
    std::weak_ptr<Entity> weakEntity = sharedEntity1;

} // <-- 当程序运行到这里离开作用域时：
  // 1. sharedEntity1 和 sharedEntity2 被销毁，引用计数归零，其管理的 Entity 被自动 delete
  // 2. e2 (以及 entity) 被销毁，其管理的 Entity 被自动 delete
```