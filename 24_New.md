# 24 - C++ new 关键字 (The new Keyword)

## 1. 核心概念 (Core Concepts)

在 C++ 中，`new` 关键字用于在**堆 (Heap)** 上动态分配内存。当你使用 `new` 时，底层主要按顺序发生了两件核心事情：

| 执行阶段 | 具体机制 |
| :--- | :--- |
| **第一步：寻找内存** | 在堆内存中寻找一块大小合适的连续可用空间，并返回这块内存的首地址。 |
| **第二步：调用构造函数** | 如果你分配的类型是一个类 (Class) 或结构体 (Struct)，`new` 在分配好内存后，会自动调用该类型的**构造函数**来初始化对象。 |

## 2. 内存分配与释放 (Allocation & Deallocation)

堆内存最大的特点是**必须手动释放**。无论你用 `new` 分配了什么，用完后都必须使用 `delete` 归还内存，否则会导致**内存泄漏 (Memory Leak)**。

| 分配场景 | 分配语法 | 释放语法 (极其重要) | 内存说明 |
| :--- | :--- | :--- | :--- |
| **基本类型** | `int* b = new int;` | `delete b;` | 在堆上分配 4 个字节。 |
| **类对象** | `Entity* e = new Entity();` | `delete e;` | 分配对象大小的内存，**并调用构造函数**。 |
| **数组** | `int* arr = new int[50];` | `delete[] arr;` | 分配 200 字节，释放时**必须加上 `[]`**。 |

## 3. new 与 malloc 的对比 (new vs. malloc)

C++ 的 `new` 和 C 语言的 `malloc` 都可以用来在堆上分配内存，但它们有着本质的区别：

| 对比维度 | `new` (C++) | `malloc` (C 语言) |
| :--- | :--- | :--- |
| **本质** | 语言级别的**运算符 (Operator)** | 标准库中的**函数 (Function)** |
| **面向对象** | **会**自动调用构造函数初始化对象 | **不会**调用构造函数，只分配原始内存 (Raw Memory) |
| **返回类型** | 返回明确类型的指针 (如 `Entity*`) | 返回 `void*`，需要手动强制类型转换 |

---

## 示例代码

```cpp
#include <iostream>

class Entity { 
public:
    float X, Y;

    Entity() {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Entity Created!" << std::endl;
    }

    void Move(float xa, float ya) {
        X += xa;
        Y += ya;
    }
};

int main() {
    // 1. 分配基本类型
    int* b = new int; // 在堆上分配了 4 个字节，返回地址给 b
    *b = 5;

    // 2. 分配数组
    int* array = new int[50]; // 在堆上分配了 200 个字节（50 * 4）

    // 3. 分配类对象
    // 分配了 Entity 大小的内存，并且自动调用了 Entity() 构造函数！
    Entity* e = new Entity(); 
    e->Move(1.0f, 2.0f);

    // 4. 手动释放内存 (防止内存泄漏)
    delete b;
    delete e;
    delete[] array; // 注意：释放数组必须加 []
    
    return 0;
}
```