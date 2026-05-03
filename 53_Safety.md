# 53 - 现代 C++ 的安全性以及如何教授它 (Safety in Modern C++ and How to Teach It)

## 1. 基本概念

C++ 经常被认为“不安全”，主要是因为它允许程序员直接管理 memory、pointer、lifetime 和底层资源。例如：

```cpp
int* data = new int[10];

// 如果忘记 delete[]，就会 memory leak
delete[] data;
```

但现代 C++ 并不是鼓励大家到处写 `new`、`delete`、raw pointer 和 C 风格数组。相反，现代 C++ 更推荐使用：

- RAII
- smart pointer
- `std::vector`
- `std::array`
- `std::string`
- `std::unique_ptr`
- `std::shared_ptr`
- `std::optional`
- `std::variant`
- `const`
- reference
- range-based for loop

这些工具的目标是：**尽量让资源的生命周期由对象自动管理，而不是靠程序员手动记住每一步。**

所以现代 C++ 的安全性核心不是“禁止底层操作”，而是：

```text
默认写安全代码，需要时才进入底层。
```

---

## 2. 为什么需要它

C++ 的强大来自于控制力，但风险也来自于控制力。

例如，下面这些问题在 C++ 中很常见：

```cpp
int* value = new int(10);

// 忘记 delete，memory leak
```

```cpp
int* ptr = nullptr;
*ptr = 5; // crash 或 undefined behavior
```

```cpp
int* data = new int[5];
data[10] = 100; // out-of-bounds
delete[] data;
```

```cpp
int& GetValue()
{
    int value = 10;
    return value; // 返回局部变量 reference，危险
}
```

这些问题本质上都和 **lifetime**、**ownership**、**memory access** 有关。

现代 C++ 的很多特性都是为了解决这些问题：

- 用 `std::vector` 避免手动管理动态数组；
- 用 `std::unique_ptr` 表达独占所有权；
- 用 `std::shared_ptr` 表达共享所有权；
- 用 RAII 自动释放资源；
- 用 `const` 限制不该修改的数据；
- 用 reference 避免不必要的 pointer；
- 用标准库容器减少越界和 memory leak。

安全 C++ 的重点不是“什么都不能做”，而是让危险代码变得明显，让普通代码默认更可靠。

---

## 3. 核心语法 

### 3.1 用 `std::vector` 替代手动动态数组

不推荐：

```cpp
int* values = new int[10];

delete[] values;
```

推荐：

```cpp
std::vector<int> values(10);
```

`std::vector` 会自动管理 heap memory，离开作用域时自动释放。

---

### 3.2 用 `std::unique_ptr` 表达 ownership

```cpp
std::unique_ptr<Entity> entity = std::make_unique<Entity>();
```

含义是：

```text
这个对象只有一个 owner。
```

当 `entity` 离开作用域时，对象会自动销毁。

---

### 3.3 用 `std::shared_ptr` 表达共享 ownership

```cpp
std::shared_ptr<Entity> entity = std::make_shared<Entity>();
```

当多个对象都需要共享同一个资源时，可以使用 `std::shared_ptr`。

但不要滥用它，因为共享 ownership 会让 lifetime 更复杂。

---

### 3.4 用 `const` 表达不可修改

```cpp
void PrintName(const std::string& name)
{
    std::cout << name << std::endl;
}
```

这里的 `const std::string&` 表示：

- 不复制字符串；
- 函数不会修改它。

这是 C++ 中非常重要的安全习惯。

---

### 3.5 用 RAII 管理资源

RAII 的意思是：资源在 constructor 中获取，在 destructor 中释放。

```cpp
class File
{
public:
    File()
    {
        std::cout << "Open file\n";
    }

    ~File()
    {
        std::cout << "Close file\n";
    }
};
```

对象生命周期结束时，资源自动清理。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Entity
{
public:
    Entity(const std::string& name)
        : m_Name(name)
    {
        std::cout << "Entity created: " << m_Name << std::endl;
    }

    ~Entity()
    {
        std::cout << "Entity destroyed: " << m_Name << std::endl;
    }

    void PrintName() const
    {
        std::cout << m_Name << std::endl;
    }

private:
    std::string m_Name;
};

void PrintEntities(const std::vector<std::unique_ptr<Entity>>& entities)
{
    for (const auto& entity : entities)
    {
        if (entity)
        {
            entity->PrintName();
        }
    }
}

int main()
{
    std::vector<std::unique_ptr<Entity>> entities;

    entities.push_back(std::make_unique<Entity>("Player"));
    entities.push_back(std::make_unique<Entity>("Enemy"));
    entities.push_back(std::make_unique<Entity>("NPC"));

    PrintEntities(entities);

    return 0;
}
```

---

## 5. 重点解释

### 5.1 没有手动 `new` 和 `delete`

示例中没有写：

```cpp
new
delete
```

而是使用：

```cpp
std::make_unique<Entity>("Player")
```

这样对象会被 `std::unique_ptr` 管理。

当 `entities` 离开作用域时，`std::vector` 会销毁其中的 `std::unique_ptr`，每个 `std::unique_ptr` 又会自动销毁对应的 `Entity`。

这就是现代 C++ 中推荐的资源管理方式。

---

### 5.2 `std::unique_ptr` 表达独占 ownership

```cpp
std::vector<std::unique_ptr<Entity>> entities;
```

这里表示：`entities` 这个容器拥有这些 `Entity` 对象。

对象不应该随便被其他地方 `delete`。

ownership 很清楚，代码就更安全。

---

### 5.3 `const std::vector<std::unique_ptr<Entity>>&`

```cpp
void PrintEntities(const std::vector<std::unique_ptr<Entity>>& entities)
```

这表示：

- 通过 reference 传递，避免复制；
- 使用 `const`，函数不能修改容器；
- 函数只是读取数据。

这是安全接口设计的一部分。

---

### 5.4 `PrintName() const`

```cpp
void PrintName() const
```

函数末尾的 `const` 表示它不会修改当前对象的成员变量。

这让调用者更放心，也让编译器能帮助检查错误。

---

### 5.5 检查 pointer 是否有效

```cpp
if (entity)
{
    entity->PrintName();
}
```

虽然 `std::unique_ptr` 比 raw pointer 安全，但它仍然可能为空。

访问 pointer 前检查有效性，是好习惯。

---

## 6. 优点和缺点

| 做法 | 优点 | 缺点 |
| :--- | :--- | :--- |
| `std::vector` | 自动管理动态数组 | 某些底层场景不如裸数组直接 |
| `std::unique_ptr` | ownership 清晰，开销低 | 不能复制，只能 move |
| `std::shared_ptr` | 适合共享资源 | 容易让 lifetime 变复杂 |
| `const` | 防止误修改 | 初学者需要适应 const correctness |
| RAII | 自动释放资源 | 需要理解 constructor / destructor |
| raw pointer | 简单、底层、灵活 | 不表达 ownership，容易出错 |

现代 C++ 的方向是：普通代码优先使用安全抽象，底层代码才使用 raw pointer 和手动 memory management。

