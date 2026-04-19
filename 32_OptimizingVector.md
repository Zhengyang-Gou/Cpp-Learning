# 32 - C++ 优化 Vector (Optimizing std::vector)

## 1. 底层机制与性能痛点 (Underlying Mechanism & Bottlenecks)

`std::vector` 的工作机制是在堆（Heap）内存中维护一个**连续的数组**。当你向其中添加元素且当前容量（Capacity）不足时，它会自动进行扩容。这个扩容过程代价极其昂贵，包含以下三个强制步骤：

| 扩容步骤 | 底层行为说明 |
| :--- | :--- |
| **1. 分配 (Allocate)** | 在堆内存中寻找并分配一块更大的连续新内存空间。 |
| **2. 搬迁 (Copy/Move)** | 将旧内存中的所有元素逐一**拷贝**（或移动）到新内存中。 |
| **3. 释放 (Free)** | 销毁旧内存中的对象，并释放那块旧的内存空间。 |

> **优化的核心思想**：尽一切可能**减少内存的重新分配**，以及**减少不必要的拷贝**。

## 2. 核心优化策略 (Core Optimization Strategies)

针对上述痛点，C++ 提供了两个极其重要的 API 来彻底压榨 `vector` 的性能。

| 优化维度 | 推荐 API | 机制与对比 | 性能收益 |
| :--- | :--- | :--- | :--- |
| **1. 避免重新分配** | `reserve(size)` | **痛点**：不断 `push_back` 会触发多次扩容搬迁。<br>**优化**：如果你提前知道大概要装多少个元素，直接用 `reserve()` 一次性分配好足够的内存。 | 彻底消除中途的扩容操作，避免海量元素的无意义搬迁。 |
| **2. 避免多余拷贝** | `emplace_back(...)` | **痛点**：`push_back(Vertex(1,2,3))` 会先在当前的栈上构造一个临时对象，然后再把它**拷贝**进 Vector 的内存中。<br>**优化**：`emplace_back` 直接接收构造参数，并在 Vector 自己的内存里**原地构造**对象。 | 直接省去临时对象的创建和拷贝过程，实现**零拷贝**。 |

---

## 示例代码

```cpp
// 优化的核心思想是：减少内存重新分配和减少不必要的拷贝
// std::vector 的工作机制是在堆内存中维护一个连续的数组
// 当你添加元素且当前容量不足时，它会执行以下三步（分配、搬迁、释放）

#include <iostream>
#include <vector>

struct Vertex {
    float x, y, z;

    // 默认构造函数
    Vertex(float x, float y, float z)
        : x(x), y(y), z(z) {
    }

    // 拷贝构造函数（只要发生拷贝，就会在控制台打印，方便我们观察性能开销）
    Vertex(const Vertex& vertex)
        : x(vertex.x), y(vertex.y), z(vertex.z) {
        std::cout << "Copied!" << std::endl;
    }
};

int main() {
    // 场景 1：最糟糕的写法
    std::vector<Vertex> vertices;
    // 连续放入 3 个元素
    // 结果：会触发多次内存重新分配，且每次 push_back 都会发生拷贝
    // 终端会疯狂打印 "Copied!" (一共会拷贝 6 次)
    vertices.push_back(Vertex(1, 2, 3));
    vertices.push_back(Vertex(4, 5, 6));
    vertices.push_back(Vertex(7, 8, 9));

    std::cout << "----------------------------" << std::endl;

    // 场景 2：优化了一半
    std::vector<Vertex> vertices1;
    // 提前分配能容纳 3 个元素的内存，不再触发扩容搬迁
    vertices1.reserve(3); 

    // 结果：没有扩容开销了，但因为传递的是临时对象，依然会打印 3 次 "Copied!"
    vertices1.push_back(Vertex(1, 2, 3));
    vertices1.push_back(Vertex(4, 5, 6));
    vertices1.push_back(Vertex(7, 8, 9));

    std::cout << "----------------------------" << std::endl;

    // 场景 3：终极优化写法
    std::vector<Vertex> vertices2;
    // 1. 提前分配内存，避免重新分配
    vertices2.reserve(3); 

    // 2. 使用 emplace_back 原地构造，避免多余拷贝
    // 不再传入 Vertex 对象，而是直接传入构造参数！
    // 结果：控制台干干净净，1 次拷贝都不会发生！(零拷贝)
    vertices2.emplace_back(1, 2, 3);
    vertices2.emplace_back(4, 5, 6);
    vertices2.emplace_back(7, 8, 9);

    std::cin.get();
    return 0;
}
```