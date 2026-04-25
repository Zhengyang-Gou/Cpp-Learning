# 49 - C++ 中的 union (Unions in C++)

## 1. 基本概念 

`union` 是 C++ 中的一种特殊类型。它和 `struct` 很像，都可以包含多个成员变量，但它们的内存分配方式不同。

普通 `struct` 中，每个成员都有自己的内存：

```cpp
struct Vector2
{
    float x;
    float y;
};
```

如果 `float` 是 4 bytes，那么这个 `struct` 通常占用：

```text
4 + 4 = 8 bytes
```

而 `union` 中，所有成员共享同一块内存：

```cpp
union Data
{
    int i;
    float f;
};
```

这里 `i` 和 `f` 使用的是同一段 memory。这个 `union` 的大小通常不是 `int + float`，而是其中最大成员的大小。

如果 `int` 和 `float` 都是 4 bytes，那么 `Data` 通常占用：

```text
4 bytes
```

`union` 的核心思想是：

> 多个成员共用同一块 memory，同一时间通常只应该把它当作其中一个成员来使用。

---

## 2. 为什么需要它 

`union` 主要用于节省内存，或者用不同方式解释同一块数据。

例如一个数据可能有两种表达方式：

```cpp
struct Vector2
{
    float x;
    float y;
};
```

我们可能既想用：

```cpp
vector.x
vector.y
```

也想用：

```cpp
vector.values[0]
vector.values[1]
```

如果不用 `union`，可能要同时存两份数据：

```cpp
struct Vector2
{
    float x;
    float y;
    float values[2];
};
```

这会浪费内存，而且 `x`、`y` 和 `values` 之间不会自动同步。

用 `union` 可以让它们共享同一块 memory：

```cpp
struct Vector2
{
    union
    {
        struct
        {
            float x;
            float y;
        };

        float values[2];
    };
};
```

这样：

```cpp
vector.x
```

和：

```cpp
vector.values[0]
```

访问的是同一块数据。

---

## 3. 核心语法 

### 3.1 基本 union

```cpp
union Data
{
    int i;
    float f;
};
```

使用：

```cpp
Data data;
data.i = 10;
std::cout << data.i << std::endl;
```

如果之后写入：

```cpp
data.f = 3.14f;
```

那么同一块 memory 会被当作 `float` 使用，之前的 `i` 内容会被覆盖。

---

### 3.2 union 的大小

```cpp
union Data
{
    int i;
    float f;
    double d;
};
```

这个 `union` 的大小通常等于最大成员 `double` 的大小。

可以用：

```cpp
std::cout << sizeof(Data) << std::endl;
```

查看实际大小。

---

### 3.3 匿名 union

`union` 可以不写名字，称为 anonymous union：

```cpp
struct Vector2
{
    union
    {
        struct
        {
            float x;
            float y;
        };

        float values[2];
    };
};
```

这样可以直接访问：

```cpp
Vector2 v;
v.x = 1.0f;
v.y = 2.0f;

std::cout << v.values[0] << std::endl;
```

如果 `union` 不是匿名的，就需要多写一层名字。

---

### 3.4 union 和 struct 组合

常见写法是把 `union` 放在 `struct` 里：

```cpp
struct Vector4
{
    union
    {
        struct
        {
            float x, y, z, w;
        };

        struct
        {
            float r, g, b, a;
        };

        float values[4];
    };
};
```

这样同一组 4 个 `float` 可以用不同语义访问：

```cpp
color.r
color.g
color.b
color.a
```

也可以用：

```cpp
color.x
color.y
color.z
color.w
```

或者：

```cpp
color.values[0]
```

---

## 4. 示例代码

```cpp
#include <iostream>

struct Vector2
{
    union
    {
        struct
        {
            float x;
            float y;
        };

        float values[2];
    };
};

struct Vector4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        struct
        {
            Vector2 a;
            Vector2 b;
        };

        float values[4];
    };
};

void PrintVector2(const Vector2& vector)
{
    std::cout << vector.x << ", " << vector.y << std::endl;
}

int main()
{
    Vector4 vector = { 1.0f, 2.0f, 3.0f, 4.0f };

    PrintVector2(vector.a);
    PrintVector2(vector.b);

    vector.z = 500.0f;

    std::cout << vector.values[2] << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `Vector2` 中的 union

```cpp
struct Vector2
{
    union
    {
        struct
        {
            float x;
            float y;
        };

        float values[2];
    };
};
```

这里 `x`、`y` 和 `values` 共享同一段 memory。

内存可以理解为：

```text
第 1 个 float：x / values[0]
第 2 个 float：y / values[1]
```

所以：

```cpp
v.x = 10.0f;
```

之后：

```cpp
v.values[0]
```

也会得到 `10.0f`。

---

### 5.2 `Vector4` 中的多种访问方式

```cpp
struct Vector4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        struct
        {
            Vector2 a;
            Vector2 b;
        };

        float values[4];
    };
};
```

这段代码让同一组 4 个 `float` 有三种访问方式：

```cpp
vector.x, vector.y, vector.z, vector.w
```

```cpp
vector.a, vector.b
```

```cpp
vector.values[0], vector.values[1], vector.values[2], vector.values[3]
```

它们不是三份数据，而是同一份数据的不同视图。

---

### 5.3 `vector.a` 和 `vector.b`

```cpp
Vector4 vector = { 1.0f, 2.0f, 3.0f, 4.0f };
```

此时内存大致是：

```text
1.0f  2.0f  3.0f  4.0f
```

`vector.a` 对应前两个 float：

```text
1.0f, 2.0f
```

`vector.b` 对应后两个 float：

```text
3.0f, 4.0f
```

所以：

```cpp
PrintVector2(vector.a);
PrintVector2(vector.b);
```

会输出类似：

```text
1, 2
3, 4
```

---

### 5.4 修改 `z` 会影响 `values[2]`

```cpp
vector.z = 500.0f;
```

`z` 对应第 3 个 float，也就是：

```cpp
vector.values[2]
```

所以：

```cpp
std::cout << vector.values[2] << std::endl;
```

会输出：

```text
500
```

这说明 union 内的成员只是同一块 memory 的不同访问方式。

---

### 5.5 union 不是普通的“多个变量”

这是初学者最容易误解的地方。

```cpp
union Data
{
    int i;
    float f;
};
```

不是说 `Data` 同时有一个独立的 `i` 和一个独立的 `f`。

它更像是：

```text
这 4 bytes 可以叫 i，也可以叫 f。
```

写入其中一个成员，会影响另一个成员看到的内容。

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 多个成员共享同一块 memory，节省空间 | 容易误用，读错成员会产生问题 |
| 可以给同一份数据提供多种访问方式 | 可读性不如普通 struct 直观 |
| 适合底层系统代码和数学类型封装 | 需要理解 object lifetime 和 active member |
| 可用于实现紧凑的数据结构 | 对非 trivial 类型使用更复杂 |
| 在向量、颜色、底层数据格式中很方便 | 现代 C++ 中很多场景可用 `std::variant` 替代 |

---

## 8. 和其他相关概念的区别

### 8.1 union vs struct

`struct` 中的成员各自占用内存：

```cpp
struct Data
{
    int i;
    float f;
};
```

大小通常是：

```text
sizeof(int) + sizeof(float)
```

`union` 中的成员共享内存：

```cpp
union Data
{
    int i;
    float f;
};
```

大小通常是最大成员的大小。

| 对比 | `struct` | `union` |
| :--- | :--- | :--- |
| 成员是否同时存在 | 是 | 通常只使用一个有效成员 |
| 内存关系 | 每个成员独立存储 | 所有成员共享同一块 memory |
| 大小 | 大致为所有成员大小之和，加上 padding | 大致为最大成员大小，加上 alignment |
| 常见用途 | 表达一个对象的多个属性 | 表达同一内存的多种解释 |

---

### 8.2 union vs type punning

type punning 是一种行为：把同一块 memory 当成不同 type 解释。

`union` 是实现这种行为的一种工具。

例如：

```cpp
union Data
{
    float f;
    int i;
};
```

这可以让同一块 memory 既被当成 `float`，也被当成 `int`。

但并不是所有 union 用法都是为了 type punning。有时只是为了节省内存或提供多个访问名字。

---

### 8.3 union vs `std::variant`

`std::variant` 是现代 C++ 中更安全的“多选一类型”。

```cpp
#include <variant>

std::variant<int, float, bool> value;
```

它会记录当前保存的是哪种类型。

而 `union` 本身不会自动记录当前有效成员。

| 对比 | `union` | `std::variant` |
| :--- | :--- | :--- |
| 是否自动记录当前类型 | 否 | 是 |
| 类型安全 | 较低 | 较高 |
| 使用复杂度 | 底层、灵活 | 更现代、更安全 |
| 性能和控制 | 更接近底层 | 有额外封装 |
| 适合场景 | 底层内存布局、性能敏感代码 | 业务逻辑中的多类型值 |

普通现代 C++ 代码中，优先考虑 `std::variant`。

---

### 8.4 union vs enum

`enum` 经常和 `union` 搭配使用。

因为 `union` 不知道当前有效成员是谁，所以可以用一个 `enum` 手动记录：

```cpp
enum class ValueType
{
    Int,
    Float
};

struct Value
{
    ValueType type;

    union
    {
        int i;
        float f;
    };
};
```

使用时先看 `type`：

```cpp
if (value.type == ValueType::Int)
{
    std::cout << value.i << std::endl;
}
```

这种模式叫 tagged union。`std::variant` 可以看作是更安全、更现代的 tagged union。

---

## 9. 核心思想

### 重点 1：union 的成员共享 memory

`union` 最大的特点就是：

```text
所有成员从同一个地址开始存储。
```

所以它的大小通常取决于最大成员，而不是所有成员之和。

---

### 重点 2：同一时间通常只应该使用一个有效成员

写入：

```cpp
data.i = 10;
```

再读取：

```cpp
data.f
```

不一定是安全或有意义的操作。

如果你写入的是 `i`，就应该按 `i` 使用；如果写入的是 `f`，就应该按 `f` 使用。

---

### 重点 3：union 适合表达“同一份数据的不同视图”

The Cherno 示例中，`Vector4` 可以被看作：

```cpp
x, y, z, w
```

也可以被看作：

```cpp
Vector2 a, Vector2 b
```

还可以被看作：

```cpp
values[4]
```

这种场景非常适合用 `union`。

---

### 重点 4：union 是底层工具，不是日常默认选择

`union` 很强，但它不如普通 `struct` 和现代容器安全。

如果只是普通业务代码，不要为了“省一点内存”就滥用 `union`。

