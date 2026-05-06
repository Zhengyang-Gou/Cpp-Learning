# 70 - C++ 中的 Map：std::map 和 std::unordered_map (Maps in C++: std::map and std::unordered_map)

## 1. 基本概念

`map` 是一种用 **key-value** 形式存储数据的容器。

普通数组或 `std::vector` 用数字下标访问：

```cpp
scores[0]
scores[1]
```

而 `map` 用 key 访问 value：

```cpp
ages["Cherno"]
```

这里：

```text
key   = "Cherno"
value = 25
```

C++ 标准库里常用的 map 有两个：

```cpp
std::map
std::unordered_map
```

它们都用来存储 key-value 数据，但底层实现不同。

---

## 2. 为什么需要它

有些数据天然不是用数字下标访问的。

例如：

```text
名字 -> 年龄
单词 -> 出现次数
ID -> 玩家对象
文件名 -> 资源
字符串 -> 函数
```

如果用 `std::vector`，你需要自己查找：

```cpp
for (auto& person : people)
{
    if (person.name == "Cherno")
        return person.age;
}
```

这很麻烦，也可能很慢。

使用 map 可以直接写：

```cpp
ages["Cherno"]
```

这让代码更清楚。

---

## 3. 核心语法

### 3.1 `std::map`

```cpp
#include <map>

std::map<std::string, int> ages;
```

`std::map` 的 key 会自动排序。

通常底层是平衡二叉树。

---

### 3.2 `std::unordered_map`

```cpp
#include <unordered_map>

std::unordered_map<std::string, int> ages;
```

`std::unordered_map` 不保证顺序。

通常底层是 hash table。

大多数查找场景下，它通常比 `std::map` 更快。

---

### 3.3 插入和访问

```cpp
ages["Cherno"] = 25;
ages["C++"] = 40;
```

读取：

```cpp
std::cout << ages["Cherno"] << std::endl;
```

---

### 3.4 查找 key

```cpp
auto it = ages.find("Cherno");

if (it != ages.end())
{
    std::cout << it->second << std::endl;
}
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

int main()
{
    std::unordered_map<std::string, int> ages;

    ages["Cherno"] = 25;
    ages["Alice"] = 30;
    ages["Bob"] = 20;

    std::cout << "Cherno age: " << ages["Cherno"] << std::endl;

    auto it = ages.find("Alice");

    if (it != ages.end())
    {
        std::cout << "Alice age: " << it->second << std::endl;
    }

    for (const auto& [name, age] : ages)
    {
        std::cout << name << " = " << age << std::endl;
    }

    std::map<std::string, int> sortedAges;

    sortedAges["Cherno"] = 25;
    sortedAges["Alice"] = 30;
    sortedAges["Bob"] = 20;

    std::cout << "Sorted map:" << std::endl;

    for (const auto& [name, age] : sortedAges)
    {
        std::cout << name << " = " << age << std::endl;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 key-value 结构

```cpp
std::unordered_map<std::string, int> ages;
```

表示：

```text
key   是 std::string
value 是 int
```

例如：

```cpp
ages["Alice"] = 30;
```

表示把 `"Alice"` 这个 key 对应到 `30`。

---

### 5.2 `operator[]` 会自动插入

```cpp
ages["Cherno"]
```

如果 `"Cherno"` 已经存在，就返回对应 value。

如果不存在，它会自动创建一个默认 value。

例如：

```cpp
std::cout << ages["Unknown"] << std::endl;
```

如果 key 不存在，会插入 `"Unknown"`，value 默认为 `0`。

所以如果你只是想检查 key 是否存在，最好使用：

```cpp
find()
```

---

### 5.3 `find()` 更适合查找

```cpp
auto it = ages.find("Alice");
```

如果找到，返回指向元素的 iterator。

如果没找到，返回：

```cpp
ages.end()
```

所以常见写法是：

```cpp
if (it != ages.end())
```

---

### 5.4 `it->first` 和 `it->second`

map 中的每个元素本质上类似：

```cpp
std::pair<const Key, Value>
```

所以：

```cpp
it->first
```

是 key。

```cpp
it->second
```

是 value。

例如：

```cpp
std::cout << it->first << " = " << it->second << std::endl;
```

---

### 5.5 `std::map` 会排序

```cpp
std::map<std::string, int> sortedAges;
```

遍历 `std::map` 时，key 会按顺序排列。

例如字符串 key 会按字典序输出。

如果你需要有序结果，可以用 `std::map`。

---

### 5.6 `std::unordered_map` 不保证顺序

```cpp
std::unordered_map<std::string, int> ages;
```

遍历输出顺序不一定和插入顺序一样，也不一定排序。

它的优势通常是查找更快。

如果不需要排序，很多场景可以优先考虑 `std::unordered_map`。

---

## 6. 优点和缺点

| 容器 | 优点 | 缺点 |
| :--- | :--- | :--- |
| `std::map` | key 自动排序，遍历结果稳定 | 查找通常比 `unordered_map` 慢 |
| `std::unordered_map` | 查找通常更快 | 不保证顺序，需要 hash |
| 二者共同点 | 都适合 key-value 数据 | 比 `vector` 更占内存 |

---

## 7. 核心思想

### 重点 1：map 用 key 找 value

map 最重要的思想是：

```cpp
key -> value
```

例如：

```cpp
name -> age
id -> object
word -> count
```

当数据不是按数字下标访问时，map 很有用。

---

### 重点 2：`std::map` 有序，`std::unordered_map` 无序

选择时可以简单判断：

```text
需要排序：std::map
不需要排序，只想快速查找：std::unordered_map
```

---

### 重点 3：不要随便用 `operator[]` 做查询

```cpp
ages["Unknown"]
```

如果 key 不存在，会插入新元素。

如果只是检查是否存在，应使用：

```cpp
find()
```

---

### 重点 4：map 的元素是 pair

遍历时：

```cpp
for (const auto& [key, value] : map)
```

这是 C++17 的 structured binding，写法更清楚。

旧写法是：

```cpp
for (const auto& pair : map)
{
    std::cout << pair.first << pair.second << std::endl;
}
```