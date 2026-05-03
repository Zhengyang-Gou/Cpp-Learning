# 75 - C++ 中的结构化绑定 (Structured Bindings in C++)

## 1. 基本概念 

**structured binding** 是 C++17 引入的语法，用来把一个复合对象中的多个值直接拆出来，绑定到多个变量名上。

典型写法：

```cpp
auto [a, b] = value;
```

它常用于：

- `std::pair`
- `std::tuple`
- `std::array`
- 普通 `struct`
- `std::map` 遍历

例如：

```cpp
std::pair<int, std::string> data = { 1, "Apple" };

auto [id, name] = data;
```

这比传统写法更清楚：

```cpp
int id = data.first;
std::string name = data.second;
```

structured binding 的核心价值是：**让代码直接表达“我要把这个对象拆成几个部分使用”。**

---

## 2. 为什么需要它

C++ 中很多函数会返回多个值。

在没有 structured binding 之前，常见做法是返回 `std::pair` 或 `std::tuple`：

```cpp
std::pair<int, std::string> GetUser()
{
    return { 1, "Alice" };
}
```

传统接收方式：

```cpp
auto user = GetUser();

int id = user.first;
std::string name = user.second;
```

问题是：

```cpp
first
second
```

这两个名字没有业务含义，读代码时不够直观。

使用 structured binding：

```cpp
auto [id, name] = GetUser();
```

代码直接告诉读者：

```text
返回值中第一个是 id，第二个是 name。
```

它让代码更短，也更有表达力。

---

## 3. 核心语法

### 3.1 基本写法

```cpp
auto [a, b] = object;
```

左边的变量数量必须和右边对象能拆出来的元素数量匹配。

---

### 3.2 拆 `std::pair`

```cpp
std::pair<int, std::string> pair = { 10, "Hello" };

auto [number, text] = pair;
```

---

### 3.3 拆 `std::tuple`

```cpp
std::tuple<int, float, std::string> data = { 1, 2.5f, "Test" };

auto [id, value, name] = data;
```

---

### 3.4 拆普通 `struct`

```cpp
struct Vector2
{
    float x;
    float y;
};

Vector2 position = { 10.0f, 20.0f };

auto [x, y] = position;
```

注意：这种方式适用于有 public data members 的简单结构。

---

### 3.5 使用 reference 避免复制

```cpp
auto& [x, y] = position;
```

这样 `x` 和 `y` 是对 `position.x`、`position.y` 的引用。

修改 `x` 会修改原对象：

```cpp
x = 100.0f;
```

---

### 3.6 使用 `const reference`

```cpp
const auto& [x, y] = position;
```

适合只读取，不复制，也不修改原对象。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <string>
#include <tuple>
#include <map>

struct Vector2
{
    float x;
    float y;
};

std::tuple<std::string, int, float> CreatePlayer()
{
    return { "Player", 100, 4.5f };
}

int main()
{
    auto [name, health, speed] = CreatePlayer();

    std::cout << name << std::endl;
    std::cout << health << std::endl;
    std::cout << speed << std::endl;

    Vector2 position = { 10.0f, 20.0f };

    auto& [x, y] = position;
    x = 50.0f;

    std::cout << position.x << ", " << position.y << std::endl;

    std::map<std::string, int> scores;
    scores["Alice"] = 90;
    scores["Bob"] = 75;
    scores["Charlie"] = 100;

    for (const auto& [playerName, score] : scores)
    {
        std::cout << playerName << ": " << score << std::endl;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `auto [name, health, speed] = CreatePlayer();`

```cpp
auto [name, health, speed] = CreatePlayer();
```

`CreatePlayer()` 返回一个 `std::tuple`：

```cpp
std::tuple<std::string, int, float>
```

structured binding 把它拆成三个变量：

```cpp
name
health
speed
```

这比下面这种写法更直观：

```cpp
auto player = CreatePlayer();

std::get<0>(player);
std::get<1>(player);
std::get<2>(player);
```

`std::get<0>` 这种写法不容易看出每个值的含义，而 structured binding 可以给每个值命名。

---

### 5.2 `auto& [x, y] = position;`

```cpp
auto& [x, y] = position;
```

这里使用的是 reference。

所以：

```cpp
x = 50.0f;
```

会修改：

```cpp
position.x
```

如果写成：

```cpp
auto [x, y] = position;
```

那就是拷贝，修改 `x` 不会影响 `position.x`。

---

### 5.3 遍历 `std::map`

```cpp
for (const auto& [playerName, score] : scores)
{
    std::cout << playerName << ": " << score << std::endl;
}
```

`std::map` 中每个元素本质上类似：

```cpp
std::pair<const Key, Value>
```

传统写法：

```cpp
for (const auto& pair : scores)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
```

structured binding 后，代码更接近业务含义：

```cpp
playerName
score
```

而不是抽象的：

```cpp
first
second
```

---

### 5.4 `auto`、`auto&`、`const auto&` 的区别

```cpp
auto [x, y] = position;
```

复制数据。

```cpp
auto& [x, y] = position;
```

绑定到原对象，可以修改。

```cpp
const auto& [x, y] = position;
```

绑定到原对象，只读，不复制。

常用判断：

| 写法 | 含义 |
| :--- | :--- |
| `auto [a, b]` | 拷贝 |
| `auto& [a, b]` | 引用，可修改原对象 |
| `const auto& [a, b]` | 引用，只读，不复制 |

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 代码更简洁 | 变量数量必须匹配 |
| 替代 `first` / `second`，语义更清楚 | 过度使用可能隐藏原始类型 |
| 适合多个返回值 | 对复杂类型可能降低可读性 |
| 遍历 `std::map` 很方便 | 默认 `auto` 会复制，需要注意 |
| 可配合 reference 避免拷贝 | 需要 C++17 或更新版本 |

