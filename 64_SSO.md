# 64 - 小字符串优化 (Small String Optimization in C++)

## 1. 基本概念

`std::string` 本质上是一个管理字符数组的对象。正常情况下，如果字符串内容较长，它会在 `heap` 上分配内存来保存字符。

但很多字符串其实很短，例如：

```cpp
"cat"
"hello"
"Cherno"
```

如果每个短字符串都去 `heap` 分配内存，成本会很高。

所以很多标准库实现会对短字符串做优化：  
当字符串足够短时，直接把字符存储在 `std::string` 对象内部，而不是去 `heap` 分配内存。

这就是 **Small String Optimization**。

---

## 2. 为什么需要它 

`heap allocation` 比普通的 stack / inline 存储更慢。

如果程序频繁创建短字符串，例如：

```cpp
std::string name = "Tom";
std::string type = "NPC";
std::string tag = "Enemy";
```

如果这些都发生 heap allocation，性能会变差。

SSO 的目的就是：

> 对短字符串避免 heap allocation，让 `std::string` 更快。

---

## 3. 核心语法

SSO 不是一种需要你手动调用的语法。

你仍然正常使用：

```cpp
std::string text = "Hello";
```

区别在于标准库内部可能会自动优化：

```cpp
std::string shortText = "Hello"; 
```

短字符串通常不会分配 heap 内存。

```cpp
std::string longText = "This is a very long string that probably allocates memory";
```

长字符串通常会分配 heap 内存。

注意：

> SSO 是标准库实现细节，C++ 标准不规定具体能存多少字符。

不同编译器、不同标准库、不同平台，SSO 的阈值可能不同。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <string>
#include <cstdlib>

static int s_AllocationCount = 0;

void* operator new(std::size_t size)
{
    s_AllocationCount++;
    return std::malloc(size);
}

void operator delete(void* memory) noexcept
{
    std::free(memory);
}

int main()
{
    s_AllocationCount = 0;
    std::string shortString = "Hello";
    std::cout << "Short string: " << shortString << std::endl;
    std::cout << "Allocations: " << s_AllocationCount << std::endl;

    s_AllocationCount = 0;
    std::string longString = "This is a long string that will probably allocate memory on the heap";
    std::cout << "Long string: " << longString << std::endl;
    std::cout << "Allocations: " << s_AllocationCount << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 重载 `operator new`

```cpp
void* operator new(std::size_t size)
```

这段代码用来统计程序中发生了多少次 heap allocation。

每次有对象通过 `new` 或标准库内部申请 heap 内存时，`s_AllocationCount` 就会增加。

---

### 5.2 短字符串可能没有 allocation

```cpp
std::string shortString = "Hello";
```

如果标准库启用了 SSO，`"Hello"` 会直接存储在 `std::string` 对象内部。

所以它可能不会触发 heap allocation。

---

### 5.3 长字符串通常会 allocation

```cpp
std::string longString = "This is a long string...";
```

当字符串长度超过 `std::string` 内部可容纳的短字符串空间时，它就必须去 `heap` 上申请内存。

这时 allocation count 通常会增加。

---

### 5.4 SSO 和 `std::string_view` 不一样

`std::string_view` 是“不拥有字符串，只查看字符串”。

而 SSO 仍然是 `std::string`。

区别是：

| 类型 | 是否拥有数据 | 是否可能避免 allocation |
| :--- | :--- | :--- |
| `std::string` + SSO | 拥有 | 短字符串可能避免 |
| `std::string_view` | 不拥有 | 本身不复制字符串 |

简单说：

```cpp
std::string
```

负责管理字符串。

```cpp
std::string_view
```

只是看字符串。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 短字符串更快 | 具体阈值不固定 |
| 减少 heap allocation | 属于实现细节 |
| 使用方式不变 | 不能依赖某个平台的具体大小 |
| 对大量短字符串很有帮助 | 长字符串仍然需要 heap allocation |

---

## 7. 核心思想

### 重点 1：SSO 是自动优化

你不需要写特殊代码启用 SSO。

只要使用 `std::string`，标准库实现可能会自动对短字符串做优化。

---

### 重点 2：短字符串不一定分配 heap 内存

初学者常以为：

```cpp
std::string text = "Hello";
```

一定会在 heap 上分配内存。

实际上，很多情况下短字符串会被直接存在 `std::string` 对象内部。

---

### 重点 3：不要依赖具体 SSO 长度

有的实现可能能存 15 个字符，有的可能不同。

所以不要写依赖特定长度的代码。

SSO 应该被看作性能优化，而不是程序逻辑的一部分。

---

### 重点 4：性能优化要测量

这一集也延续了前几集的思想：

> 不要猜性能，要 benchmark。

如果你关心字符串是否发生 allocation，可以用类似 `operator new` 统计的方式观察。

