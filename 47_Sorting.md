# 47 - C++ 中的排序 (Sorting in C++)

## 1. 基本概念

排序就是把一组数据按照某种规则重新排列。

在 C++ 中，最常用的排序工具是标准库里的：

```cpp
std::sort
```

它定义在：

```cpp
#include <algorithm>
```

`std::sort` 可以对一个范围内的元素排序，例如 `std::vector`、`std::array`、普通数组等。

默认情况下，`std::sort` 会按照从小到大的顺序排序，也就是使用元素的 `operator<`。

例如：

```cpp
std::sort(values.begin(), values.end());
```

这里的核心思想是：

- `values.begin()` 表示排序范围的起点；
- `values.end()` 表示排序范围的终点；
- `std::sort` 会排序 `[begin, end)` 这个区间；
- `end()` 本身不包含在范围内。

---

## 2. 为什么需要它

排序是非常常见的数据处理操作。

例如：

- 把分数从高到低排列；
- 把名字按字母顺序排列；
- 把游戏对象按距离排序；
- 把 UI 元素按显示层级排序；
- 把文件按时间或大小排序；
- 把搜索结果按相关性排序。

如果不用标准库，自己写排序算法会很麻烦，而且容易出错。

例如你当然可以手写冒泡排序：

```cpp
for (int i = 0; i < count; i++)
{
    for (int j = 0; j < count - 1; j++)
    {
        if (values[j] > values[j + 1])
        {
            std::swap(values[j], values[j + 1]);
        }
    }
}
```

但真实项目中通常不应该这么做。标准库的 `std::sort` 已经足够高效、通用、可靠。

我们真正需要掌握的是：

- 怎么调用 `std::sort`；
- 怎么指定排序范围；
- 怎么自定义排序规则；
- comparator 应该怎么写。

---

## 3. 核心语法 (Core Syntax)

### 3.1 默认升序排序

```cpp
std::sort(values.begin(), values.end());
```

适用于支持 `<` 比较的类型，例如：

```cpp
int
float
double
std::string
```

---

### 3.2 使用 comparator 自定义排序

```cpp
std::sort(values.begin(), values.end(), comparator);
```

第三个参数是比较函数，也叫 **comparator**。

例如从大到小排序：

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    return a > b;
});
```

这个 lambda 的含义是：

```cpp
return a > b;
```

如果返回 `true`，说明 `a` 应该排在 `b` 前面。

---

### 3.3 使用 `std::greater`

从大到小也可以写：

```cpp
std::sort(values.begin(), values.end(), std::greater<int>());
```

需要包含：

```cpp
#include <functional>
```

但在实际项目中，lambda 更灵活，也更容易表达复杂规则。

---

### 3.4 排序普通数组

普通数组也可以排序：

```cpp
int values[] = { 5, 1, 4, 2, 3 };

std::sort(std::begin(values), std::end(values));
```

需要包含：

```cpp
#include <iterator>
```

或者直接用指针范围：

```cpp
std::sort(values, values + 5);
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> values = { 3, 5, 1, 4, 2 };

    std::sort(values.begin(), values.end());

    std::cout << "Ascending: ";
    for (int value : values)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::sort(values.begin(), values.end(), [](int a, int b)
    {
        return a > b;
    });

    std::cout << "Descending: ";
    for (int value : values)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::sort(values.begin(), values.end(), [](int a, int b)
    {
        if (a == 1)
            return false;

        if (b == 1)
            return true;

        return a < b;
    });

    std::cout << "Custom: ";
    for (int value : values)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::sort(values.begin(), values.end())`

```cpp
std::sort(values.begin(), values.end());
```

这行代码会把 `values` 中的元素按默认规则排序。

对 `int` 来说，默认规则就是从小到大。

排序后：

```cpp
3 5 1 4 2
```

会变成：

```cpp
1 2 3 4 5
```

---

### 5.2 `begin()` 和 `end()` 是 iterator

```cpp
values.begin()
values.end()
```

它们返回的是 **iterator**。

iterator 可以理解成一种“指向容器中元素的位置”的对象。

`std::sort` 不直接关心你用的是 `std::vector` 还是其他容器，它只需要一段可以随机访问的 iterator 范围。

注意：`std::sort` 需要 **random access iterator**，所以它适合：

```cpp
std::vector
std::array
普通数组
std::deque
```

但不适合直接用于：

```cpp
std::list
```

`std::list` 有自己的成员函数：

```cpp
list.sort();
```

---

### 5.3 lambda comparator 的含义

```cpp
[](int a, int b)
{
    return a > b;
}
```

这是一个 lambda。

它接受两个 `int`：

```cpp
int a, int b
```

返回一个 `bool`：

```cpp
return a > b;
```

如果返回 `true`，表示 `a` 应该排在 `b` 前面。

所以：

```cpp
return a > b;
```

表示较大的数排前面，也就是降序排序。

---

### 5.4 自定义规则：把 `1` 放到最后

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    if (a == 1)
        return false;

    if (b == 1)
        return true;

    return a < b;
});
```

这个规则的意思是：

1. 如果 `a` 是 `1`，就不要让 `a` 排在前面；
2. 如果 `b` 是 `1`，就让 `a` 排在前面；
3. 其他情况下按升序排序。

结果类似：

```text
2 3 4 5 1
```

这说明 comparator 不只能做升序或降序，也可以表达特殊业务规则。

---

### 5.5 comparator 不是“交换条件”

初学者容易把 comparator 理解成：

> 如果返回 true，就交换两个元素。

这是不准确的。

更好的理解是：

> 如果 comparator(a, b) 返回 true，说明 a 应该排在 b 前面。

所以：

```cpp
return a < b;
```

表示升序。

```cpp
return a > b;
```

表示降序。

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 标准库提供，可靠高效 | comparator 写错会导致结果不正确 |
| 语法简洁 | 需要理解 iterator 范围 |
| 支持自定义排序规则 | 不适合所有容器，例如 `std::list` |
| 可以排序基本类型和自定义类型 | 排序会改变原容器中的元素顺序 |
| lambda 写法灵活 | 复杂 comparator 可能降低可读性 |

---

## 8. 和其他相关概念的区别

### 8.1 `std::sort` vs 手写排序

手写排序适合学习算法原理。

实际项目中，优先使用：

```cpp
std::sort
```

因为它通常比初学者手写的排序更快、更安全、更通用。

---

### 8.2 升序 vs 降序

升序：

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    return a < b;
});
```

降序：

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    return a > b;
});
```

关键在于 comparator 表达的是：

```text
a 是否应该排在 b 前面
```

---

### 8.3 `std::sort` vs `std::stable_sort`

`std::sort` 不保证相等元素的原始顺序。

`std::stable_sort` 会保留相等元素的原始相对顺序。

例如：

```cpp
std::stable_sort(players.begin(), players.end(), [](const Player& a, const Player& b)
{
    return a.Score > b.Score;
});
```

如果两个玩家分数相同，`std::stable_sort` 会让它们保持排序前的先后顺序。

一般情况用 `std::sort` 即可；如果需要保留相等元素顺序，用 `std::stable_sort`。

---

### 8.4 `operator<` vs comparator

如果类型本身定义了 `operator<`，可以直接排序：

```cpp
std::sort(objects.begin(), objects.end());
```

如果只是临时需要某种排序规则，更推荐传 comparator：

```cpp
std::sort(objects.begin(), objects.end(), [](const Object& a, const Object& b)
{
    return a.Value < b.Value;
});
```

简单判断：

- 类型有自然排序规则：可以考虑 `operator<`；
- 只是某个场景下的排序需求：使用 lambda comparator。

---

### 8.5 `std::vector` vs `std::list` 排序

`std::vector` 可以用：

```cpp
std::sort(vector.begin(), vector.end());
```

`std::list` 不能直接用 `std::sort`，因为 `std::list` 的 iterator 不是 random access iterator。

`std::list` 应该用：

```cpp
list.sort();
```

---

## 9. 核心思想

### 重点 1：排序范围是 `[begin, end)`

`std::sort` 排序的是从 `begin` 到 `end` 之前的元素。

```cpp
std::sort(values.begin(), values.end());
```

`end()` 指向的是最后一个元素的后一个位置，不是最后一个元素本身。

---

### 重点 2：comparator 决定排序规则

默认排序只适合简单情况。

一旦需要降序、按字段排序、特殊规则排序，就应该传入 comparator。

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    return a > b;
});
```

---

### 重点 3：comparator 表示“谁应该在前面”

记住这句话即可：

```text
如果 comparator(a, b) 返回 true，a 排在 b 前面。
```

---

### 重点 4：优先使用标准库

排序是基础算法，不需要每次都自己实现。

真实工程中，优先使用：

```cpp
std::sort
```

这样代码更短，也更可靠。

---

## 10. 实战建议 (Practical Advice)

### 10.1 普通排序直接用 `std::sort`

```cpp
std::sort(values.begin(), values.end());
```

不要为了简单排序手写循环。

---

### 10.2 自定义排序优先用 lambda

推荐：

```cpp
std::sort(players.begin(), players.end(), [](const Player& a, const Player& b)
{
    return a.Score > b.Score;
});
```

lambda 把排序规则直接写在调用处，阅读起来更清楚。

---

### 10.3 对自定义类型用 `const reference`

排序对象时，不要这样写：

```cpp
[](Player a, Player b)
```

这会复制对象。

更推荐：

```cpp
[](const Player& a, const Player& b)
```

---

### 10.4 comparator 要保持逻辑一致

不要写不稳定、互相矛盾的规则。

错误示例：

```cpp
std::sort(values.begin(), values.end(), [](int a, int b)
{
    return true;
});
```

这表示任何 `a` 都应该排在任何 `b` 前面，逻辑不成立。

---

### 10.5 不要在 comparator 里做昂贵操作

comparator 会被调用很多次。

不推荐：

```cpp
std::sort(files.begin(), files.end(), [](const File& a, const File& b)
{
    return LoadFileSizeFromDisk(a.Path) < LoadFileSizeFromDisk(b.Path);
});
```

因为排序过程中会反复调用它。

更好的做法是提前计算好需要比较的数据，再排序。

---

### 10.6 需要稳定顺序时用 `std::stable_sort`

如果相等元素的原始顺序很重要，用：

```cpp
std::stable_sort(values.begin(), values.end(), comparator);
```

否则一般用 `std::sort`。