# 62 - 如何让字符串更快 (How to make your STRINGS FASTER in C++!)

## 1. 基本概念

`std::string_view` 是 C++17 引入的标准库类型，它可以“查看”一段字符串内容，但不拥有这段内存。

普通 `std::string` 通常会拥有自己的字符数据：

```cpp
std::string name = "Cherno";
```

而 `std::string_view` 只是引用已有字符串的一段内容：

```cpp
std::string_view view = name;
```

它不会复制字符串，也通常不会产生 heap allocation。

可以把它理解为：

> `std::string_view` 是一个轻量的字符串窗口。

它内部通常只保存两样东西：

```cpp
const char* data;
size_t size;
```

所以它非常适合用来传递只读字符串。

---

## 2. 为什么需要它

很多时候，我们只是想读取字符串，而不是修改或保存它。

例如：

```cpp
void PrintName(std::string name)
{
    std::cout << name << std::endl;
}
```

这会复制一份字符串。

如果字符串很长，或者函数频繁调用，就会有不必要的开销。

可以改成：

```cpp
void PrintName(const std::string& name)
{
    std::cout << name << std::endl;
}
```

这避免了复制。

但它仍然只能方便地接收 `std::string`。

如果传入字符串字面量：

```cpp
PrintName("Cherno");
```

可能会临时构造 `std::string`。

更好的写法是：

```cpp
void PrintName(std::string_view name)
{
    std::cout << name << std::endl;
}
```

这样既可以接收：

```cpp
std::string
const char*
字符串字面量
字符串的一部分
```

又不需要复制字符串。

---

## 3. 核心语法 (Core Syntax)

使用 `std::string_view` 需要包含：

```cpp
#include <string_view>
```

基本用法：

```cpp
std::string_view name = "Cherno";
```

作为函数参数：

```cpp
void Print(std::string_view text)
{
    std::cout << text << std::endl;
}
```

截取子字符串：

```cpp
std::string_view first = text.substr(0, 5);
```

注意：

```cpp
std::string_view::substr()
```

不会复制字符，它只是创建一个新的 view。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <string>
#include <string_view>

void PrintString(std::string_view text)
{
    std::cout << text << std::endl;
}

void PrintNameParts(std::string_view fullName)
{
    size_t space = fullName.find(' ');

    if (space == std::string_view::npos)
    {
        std::cout << "Name: " << fullName << std::endl;
        return;
    }

    std::string_view firstName = fullName.substr(0, space);
    std::string_view lastName = fullName.substr(space + 1);

    std::cout << "First name: " << firstName << std::endl;
    std::cout << "Last name: " << lastName << std::endl;
}

int main()
{
    std::string name = "Cherno Hazel";

    PrintString(name);
    PrintString("Hello C++");

    PrintNameParts(name);
    PrintNameParts("The Cherno");

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::string_view` 不复制字符串

```cpp
void PrintString(std::string_view text)
```

这里传入字符串时，`text` 不会创建新的字符串副本。

它只是指向原来的字符数据。

所以相比：

```cpp
void PrintString(std::string text)
```

`std::string_view` 更轻量。

---

### 5.2 `substr()` 更便宜

普通 `std::string` 的 `substr()` 会创建一个新的字符串：

```cpp
std::string first = name.substr(0, 6);
```

这可能会分配新内存。

而 `std::string_view` 的 `substr()` 只是创建一个新的 view：

```cpp
std::string_view first = fullName.substr(0, space);
```

它不会复制字符。

这就是它能让字符串操作更快的主要原因之一。

---

### 5.3 `std::string_view` 不拥有数据

这是最重要的点。

```cpp
std::string_view view;
{
    std::string name = "Cherno";
    view = name;
}
```

离开作用域后，`name` 被销毁。

此时 `view` 还指向原来的内存，但那段内存已经无效。

这会造成 dangling reference。

所以不要让 `std::string_view` 活得比原始字符串更久。

---

### 5.4 `std::string_view` 不保证以 `\0` 结尾

C 风格字符串依赖 null terminator：

```cpp
const char* text = "Hello";
```

但 `std::string_view` 表示的是：

```cpp
起始位置 + 长度
```

它不要求最后一定有 `\0`。

所以不要随便把 `string_view.data()` 当作 C 字符串传给需要 null-terminated string 的 API。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 避免不必要的字符串复制 | 不拥有数据，容易出现 dangling |
| 传参轻量 | 不能直接修改字符串内容 |
| `substr()` 通常不分配内存 | 不保证以 `\0` 结尾 |
| 可以接收 `std::string` 和字符串字面量 | 生命周期需要程序员自己注意 |

---

## 7. 核心思想

### 重点 1：只读字符串参数优先考虑 `std::string_view`

如果函数只是读取字符串，不需要保存它，也不需要修改它，可以写成：

```cpp
void Print(std::string_view text);
```

这通常比按值传递 `std::string` 更高效。

---

### 重点 2：`std::string_view` 是 view，不是 owner

`std::string_view` 不负责管理内存。

它只是看别人拥有的字符串。

所以原始字符串必须还活着。

---

### 重点 3：性能提升来自减少 copy 和 allocation

字符串操作慢，很多时候不是因为字符比较本身，而是因为频繁创建临时 `std::string`，导致 copy 和 heap allocation。

`std::string_view` 的价值就在于：

```cpp
只看，不复制。
```