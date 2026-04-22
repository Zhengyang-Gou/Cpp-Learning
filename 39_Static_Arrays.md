# 57 - C++ 静态数组 (Static Arrays / std::array)

## 1. 基本概念 (Basic Concepts)

静态数组（Static Array）的核心特点是：

> **数组大小在编译期就已经确定，并且之后不能再改变。**

最传统的静态数组写法是 C 风格数组：

```cpp
int arr[5];
```

而在现代 C++ 中，更推荐使用：

```cpp
std::array<int, 5> arr;
```

它们都表示：

- 存储 5 个 `int`
- 大小固定
- 不能像 `std::vector` 那样动态扩容

这一集的核心，不只是“数组怎么写”，而是理解：

> **什么时候应该用固定大小数组，为什么 `std::array` 通常比老式 C 数组更好。**

---

## 2. C 风格数组 vs `std::array`

### C 风格数组

```cpp
int arr[5];
```

### `std::array`

```cpp
#include <array>

std::array<int, 5> arr;
```

这两者都表示“固定长度为 5 的整数数组”。

但 `std::array` 更现代，原因是它本质上是一个标准库类型，  
因此它带来了更好的接口和更好的 STL 兼容性。

---

## 3. 为什么更推荐 `std::array`

最重要的观点之一就是：

> **如果你需要固定大小数组，优先考虑 `std::array`，而不是老式 C 数组。**

### 原因

### 1）有更好的接口

例如：

```cpp
arr.size()
```

而不是自己再去算数组长度。

---

### 2）支持迭代器

例如：

```cpp
arr.begin()
arr.end()
```

这意味着你可以直接配合 STL 算法使用。

---

### 3）更容易与现代 C++ 风格结合

例如：

- 范围 `for`
- STL 算法
- 模板
- 泛型代码

---

## 4. `std::array` 的语法

### 示例代码

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int, 5> data;

    data[0] = 2;
    data[1] = 4;
    data[2] = 6;
    data[3] = 8;
    data[4] = 10;

    for (int i = 0; i < data.size(); i++)
        std::cout << data[i] << std::endl;

    return 0;
}
```

### 说明

```cpp
std::array<int, 5> data;
```

这里有两个模板参数：

1. `int`：元素类型
2. `5`：数组长度

### 关键点

这说明：

> **数组大小是类型的一部分。**

也就是说：

```cpp
std::array<int, 5>
```

和

```cpp
std::array<int, 10>
```

是两种不同的类型。

---

## 5. 为什么数组大小属于类型的一部分

这一点在 `std::array` 里非常重要。

例如：

```cpp
std::array<int, 5> a;
std::array<int, 10> b;
```

虽然它们都是“int 数组”，  
但它们并不是同一种类型，因为长度不同。

所以：

- `std::array<int, 5>` 是一个类型
- `std::array<int, 10>` 是另一个类型

这也是后面函数传参时会遇到问题的原因之一。

---

## 6. 静态数组与动态数组的区别

| 特性 | `std::array` | `std::vector` |
| :--- | :--- | :--- |
| **大小是否固定** | 固定 | 可变 |
| **大小确定时机** | 编译期 | 运行期 |
| **元素存储位置** | 通常直接随对象存放 | 元素通常在堆上 |
| **是否可扩容** | 不可 | 可以 |
| **额外管理开销** | 少 | 更多 |
| **适合场景** | 固定大小数据 | 大小不确定的数据 |

---

## 7. `std::array` vs `std::vector`

### `std::array`

适合：

- 大小已知
- 不需要扩容
- 想要更少开销
- 想保留 STL 友好接口

### `std::vector`

适合：

- 大小在运行时才知道
- 需要插入、删除、扩容
- 需要更灵活的数据结构

所以可以这样理解：

> **`std::array` 是“固定大小的现代数组”，**
> **`std::vector` 是“可变大小的动态数组”。**

---

## 8. `std::array` 的内存特点 

> **`std::array` 通常是栈上分配的固定大小数组对象，而 `std::vector` 的元素通常是堆上分配的。**

例如：

```cpp
std::array<int, 5> arr;
```

如果这是一个局部变量，  
那这个数组对象通常就跟随当前作用域存在。

而：

```cpp
std::vector<int> vec;
```

`vec` 这个对象本身可以在栈上，  
但它真正存元素的内存通常在堆上。

### 这意味着什么？

- `std::array` 更轻量
- `std::array` 更直接
- `std::array` 不需要动态分配元素内存
- 但它也因此失去了动态扩容能力

---

## 9. `std::array` 的遍历方式

### 方式一：普通 `for`

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int, 5> values = { 1, 2, 3, 4, 5 };

    for (int i = 0; i < values.size(); i++)
        std::cout << values[i] << std::endl;

    return 0;
}
```

---

### 方式二：范围 `for`

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int, 5> values = { 1, 2, 3, 4, 5 };

    for (int value : values)
        std::cout << value << std::endl;

    return 0;
}
```

---

### 方式三：迭代器

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int, 5> values = { 1, 2, 3, 4, 5 };

    for (auto it = values.begin(); it != values.end(); it++)
        std::cout << *it << std::endl;

    return 0;
}
```

### 说明

由于 `std::array` 提供了：

- `begin()`
- `end()`

所以它能很好地融入 STL 风格代码。

---

## 10. `std::array` 支持 STL 算法

这是 `std::array` 相比 C 风格数组一个很舒服的地方。

### 示例代码

```cpp
#include <iostream>
#include <array>
#include <algorithm>

int main()
{
    std::array<int, 5> values = { 5, 3, 1, 4, 2 };

    std::sort(values.begin(), values.end());

    for (int value : values)
        std::cout << value << std::endl;

    return 0;
}
```

### 说明

因为它有标准迭代器接口，  
所以你可以直接使用：

- `std::sort`
- `std::find`
- `std::reverse`
- 其他 STL 算法

这也是现代 C++ 更偏向 `std::array` 的重要原因之一。

---

## 11. 函数传参时的问题 

这一集里很关键的一个问题是：

> **如果 `std::array` 的大小也是类型的一部分，那函数参数怎么写？**

例如下面这段：

```cpp
void PrintArray(std::array<int, 5> arr)
{
}
```

它只能接收：

```cpp
std::array<int, 5>
```

但如果传进来的是：

```cpp
std::array<int, 10>
```

就不是同一个类型了。

---

## 12. 用模板接收任意大小的 `std::array`

这就是为什么 Cherno 在这集里会把模板带进来。

### 写法一：类型和大小都泛化

```cpp
#include <iostream>
#include <array>

template<typename T, size_t N>
void PrintArray(const std::array<T, N>& arr)
{
    for (const auto& value : arr)
        std::cout << value << std::endl;
}

int main()
{
    std::array<int, 5> values = { 1, 2, 3, 4, 5 };
    PrintArray(values);

    return 0;
}
```

### 说明

这里：

- `T` 表示元素类型
- `N` 表示数组大小

这样函数就不依赖固定长度了。

---

### 写法二：固定元素类型，只放开大小

```cpp
#include <iostream>
#include <array>

template<size_t N>
void PrintIntArray(const std::array<int, N>& arr)
{
    for (const int& value : arr)
        std::cout << value << std::endl;
}

int main()
{
    std::array<int, 3> values = { 7, 8, 9 };
    PrintIntArray(values);

    return 0;
}
```

### 说明

这里的 `int` 是固定的，  
只有数组长度 `N` 是模板参数。

---

## 13. 为什么这里要用引用传参

上面的函数通常会写成：

```cpp
const std::array<T, N>& arr
```

而不是：

```cpp
std::array<T, N> arr
```

原因是：

- 避免复制整个数组
- 更高效
- 更符合传只读参数的习惯

这和前面你学过的“引用传递”是连起来的。

---

## 14. C 风格数组的问题 (Problems with C-style Arrays)

虽然老式数组也能用，但它们有不少问题。

### 1）接口差

没有：

- `.size()`
- `.begin()`
- `.end()`

### 2）更难与 STL 协作

你通常得手动处理长度和边界。

### 3）传参时容易退化为指针

例如：

```cpp
void Print(int arr[])
```

本质上会退化成指针语义，  
很多数组信息会丢掉。

### 4）代码表达力不如 `std::array`

现代 C++ 更鼓励用语义更清晰、接口更完整的标准库类型。

---

## 15. `std::array` 的优点

### 1）固定大小，开销小

适合大小已知的数据。

### 2）接口现代

提供：

- `size()`
- `begin()`
- `end()`

### 3）支持 STL 算法

很容易和现代 C++ 生态配合使用。

### 4）比 C 风格数组更安全、更清晰

至少从接口和表达层面上更友好。

### 5）通常没有额外性能负担

因为它本质上仍然是固定大小的数组封装，  
而数组长度已经在模板参数里确定了。

---

## 16. `std::array` 的缺点 

### 1）大小必须在编译期确定

如果你运行时才知道要多少元素，  
那就不适合。

### 2）不能动态扩容

不像 `std::vector` 那样能 `push_back()` 变大。

### 3）长度不同就是不同类型

这虽然在类型系统上很严谨，  
但有时也会让接口变得麻烦，所以才需要模板配合。

---

## 17. 核心思想

### 重点 1：静态数组的大小是固定的

只要你使用固定大小数组，  
就要接受“长度不能变”这个事实。

---

### 重点 2：`std::array` 是现代 C++ 中更推荐的固定大小数组

它几乎就是“更好用的静态数组”。

---

### 重点 3：`std::array` 的长度是类型的一部分

这解释了为什么函数传参时经常需要模板。

---

### 重点 4：如果你不需要动态分配，就不要为了灵活性强行上 `std::vector`

固定大小场景下，  
`std::array` 往往更自然。

