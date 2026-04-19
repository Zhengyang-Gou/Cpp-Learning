# 31 - C++ 动态数组 (Dynamic Arrays)

## 1. 原生数组的局限与 `std::vector`

C++ 原生的标准数组（比如在栈上的 `int arr[5];` 或者在堆上的 `new int[5];`）都有一个致命的缺陷：**它的大小是固定的**。一旦创建，你无法在运行时随意增加或减少它的容量。

为了解决这个问题，C++ 标准库提供了 `std::vector`。它本质上是一个**全自动完成内存管理的智能动态数组**，当你塞入的数据超过其当前容量时，它会自动在内存中寻找更大的空间并搬运数据。

> **前置要求**：使用动态数组必须包含头文件 `#include <vector>`。

## 2. 基础操作与 API (Basic Operations)

| 操作 / 方法 | 代码示例 | 功能说明 |
| :--- | :--- | :--- |
| **创建数组** | `std::vector<Vertex> v;` | 创建一个只装 `Vertex` 类型数据的动态数组。 |
| **添加元素** | `v.push_back(item);` | 将数据塞入数组的最尾部（自动处理扩容逻辑）。 |
| **访问元素** | `v[0]` | 和普通数组一样，可以通过中括号 `[]` 直接访问对应索引的元素。 |
| **获取大小** | `v.size()` | 返回当前数组中实际包含的元素个数。 |
| **清空数组** | `v.clear()` | 清空数组中的所有元素。 |

## 3. 遍历方式与性能陷阱 (Iteration & Performance)

在现代 C++ 中，遍历数组推荐使用 Range-based for 循环，但必须格外注意**按值传递**带来的性能损耗。

| 遍历方式 | 代码结构 | 特点与避坑指南 |
| :--- | :--- | :--- |
| **传统 for 循环** | `for (int i = 0; i < v.size(); i++)` | 通过索引 `i` 访问，语法较为传统。 |
| **现代范围 for 循环** | `for (Vertex& v : vertices)` | **极其重要：** 必须带上引用符号 `&`（如 `Vertex&` 或 `const Vertex&`）。如果不加 `&`，每一次循环都会把数组里的对象完完整整地**复制**一份到临时变量中，极其浪费性能！ |

---

## 示例代码

```cpp
// C++ 原生的标准数组（比如 int arr[5]; 或者 new int[5]）有一个致命的缺陷：它的大小是固定的
// std::vector 就是全自动完成的智能数组

#include <vector>
#include <iostream>

struct Vertex {
    float x, y, z;
};

int main() {
    // 1. 创建一个只装 Vertex 类型的动态数组
    std::vector<Vertex> vertices; 

    // 2. 使用 push_back 向数组尾部塞入数据
    vertices.push_back({ 1.0f, 2.0f, 3.0f });
    vertices.push_back({ 4.0f, 5.0f, 6.0f });
    vertices.push_back({ 7.0f, 8.0f, 9.0f });

    // 3. 数组遍历演示
    
    // 方式一：传统 for 循环遍历
    for (int i = 0; i < vertices.size(); i++) {
        // 像普通数组一样用 [] 访问
        std::cout << vertices[i].x << std::endl; 
    }

    // 方式二：现代 C++ 的 Range-based for 循环
    // 注意这里的引用 &，这是为了避免每次循环都复制一遍 Vertex 对象
    for (Vertex& v : vertices) { 
        std::cout << v.y << std::endl;
    }

    // 4. 清空数组
    vertices.clear();
    
    return 0;
}
```