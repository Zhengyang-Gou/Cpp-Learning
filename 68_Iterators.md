# 68 - C++ 中的迭代器 (ITERATORS in C++)

## 1. 基本概念

这一集的核心主题是 **iterator**。

`iterator` 可以理解为一种“广义 pointer”。  
它用来遍历容器中的元素，例如：

```cpp
std::vector<int>
std::array<int, 5>
std::map<std::string, int>
```

普通数组可以用 pointer 遍历：

```cpp
int* ptr = data;
```

而标准库容器通常用 iterator 遍历：

```cpp
std::vector<int>::iterator it = values.begin();
```

iterator 的核心作用是：

> 用统一的方式访问不同容器中的元素。

---

## 2. 为什么需要它 

不同容器内部结构不同。

例如：

```cpp
std::vector
```

内部是连续内存。

```cpp
std::map
```

内部通常是树结构。

如果每个容器都用完全不同的遍历方式，代码会很混乱。

iterator 提供了一种统一接口：

```cpp
for (auto it = container.begin(); it != container.end(); it++)
{
    std::cout << *it << std::endl;
}
```

无论底层是 array、vector、list 还是 map，都可以用类似方式遍历。

---

## 3. 核心语法 

### 3.1 获取 begin 和 end

```cpp
auto begin = values.begin();
auto end = values.end();
```

含义：

| 表达式 | 含义 |
| :--- | :--- |
| `begin()` | 指向第一个元素 |
| `end()` | 指向最后一个元素之后的位置 |

注意：

```cpp
end()
```

不是最后一个元素，而是“尾后位置”。

---

### 3.2 使用 iterator 遍历

```cpp
for (auto it = values.begin(); it != values.end(); it++)
{
    std::cout << *it << std::endl;
}
```

这里：

- `it++`：移动到下一个元素
- `*it`：访问当前元素
- `it != values.end()`：判断是否结束

---

### 3.3 const iterator

如果不想修改元素，可以使用：

```cpp
std::vector<int>::const_iterator it;
```

或者更常见：

```cpp
for (auto it = values.cbegin(); it != values.cend(); it++)
{
    std::cout << *it << std::endl;
}
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> names;
    names.push_back("Cherno");
    names.push_back("C++");
    names.push_back("Iterator");

    for (std::vector<std::string>::iterator it = names.begin();
         it != names.end();
         it++)
    {
        std::cout << *it << std::endl;
    }

    std::cout << "-----" << std::endl;

    for (auto it = names.begin(); it != names.end(); it++)
    {
        *it += "!";
        std::cout << *it << std::endl;
    }

    std::cout << "-----" << std::endl;

    for (const auto& name : names)
    {
        std::cout << name << std::endl;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 iterator 像 pointer 一样使用

```cpp
*it
```

表示访问 iterator 当前指向的元素。

这和 pointer 很像：

```cpp
*ptr
```

所以 iterator 可以理解为一种封装过的 pointer。

---

### 5.2 `begin()` 和 `end()` 构成一个范围

```cpp
names.begin()
names.end()
```

它们表示一个左闭右开的范围：

```cpp
[begin, end)
```

也就是：

- 包含 `begin`
- 不包含 `end`

所以循环条件通常写：

```cpp
it != names.end()
```

而不是：

```cpp
it <= names.end()
```

---

### 5.3 `auto` 可以简化 iterator 类型

完整写法很长：

```cpp
std::vector<std::string>::iterator it
```

所以实际开发中常写：

```cpp
auto it = names.begin();
```

这让代码更简洁，也更符合现代 C++ 风格。

---

### 5.4 range-based for 本质上也依赖 iterator

这段代码：

```cpp
for (const auto& name : names)
{
    std::cout << name << std::endl;
}
```

底层本质上也是通过：

```cpp
begin()
end()
```

来遍历容器。

所以理解 iterator，有助于理解 range-based for 的工作方式。

---

### 5.5 iterator 可以修改元素

如果 iterator 不是 `const_iterator`，可以通过它修改元素：

```cpp
*it += "!";
```

如果你只想读取，不想修改，推荐使用：

```cpp
const auto&
```

或：

```cpp
cbegin()
cend()
```

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 统一遍历不同容器 | 语法比普通 index 复杂 |
| 接近 pointer，效率高 | 初学者容易误用 `end()` |
| 支持标准库算法 | iterator 可能失效 |
| 可以配合 `auto` 简化代码 | 不同容器的 iterator 能力不同 |

---

## 7. 核心思想

### 重点 1：iterator 是访问容器元素的工具

iterator 不一定是真正的 pointer，但使用方式很像 pointer。

核心操作是：

```cpp
*it
it++
it != end
```

---

### 重点 2：`end()` 不是最后一个元素

`end()` 表示最后一个元素后面的位置。

所以不能直接解引用：

```cpp
*values.end() // 错误
```

---

### 重点 3：iterator 让算法和容器解耦

标准库算法经常接收 iterator：

```cpp
std::find(values.begin(), values.end(), 10);
```

算法不需要知道容器内部怎么存储，只需要知道从哪里开始、到哪里结束。

---

### 重点 4：range-based for 是更简洁的写法

如果只是遍历元素，通常写：

```cpp
for (const auto& value : values)
```

如果需要手动控制位置、删除元素、配合算法，才更常直接使用 iterator。

---

## 8. 常见错误

### 错误 1：解引用 `end()`

```cpp
auto it = values.end();
std::cout << *it << std::endl;
```

这是错误的，因为 `end()` 不指向有效元素。

---

### 错误 2：修改容器后继续使用旧 iterator

例如对 `std::vector` 执行 `push_back()` 后，如果发生扩容，旧 iterator 可能失效。

```cpp
auto it = values.begin();
values.push_back(100);
// it 可能已经无效
```

---

### 错误 3：能用 range-based for 时写复杂 iterator

如果只是读取：

```cpp
for (const auto& value : values)
```

通常比手写 iterator 更清楚。