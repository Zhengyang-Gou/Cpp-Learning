# 51 - C++ 中的类型转换 (Casting in C++)

## 1. 基本概念

**casting** 指的是把一个值、pointer 或 reference 从一种 type 转换成另一种 type。

例如：

```cpp
double value = 5.25;
int number = (int)value;
```

这里把 `double` 转成了 `int`，小数部分会被截掉。

C++ 中常见的类型转换有两类：

1. **implicit cast**：编译器自动转换；
2. **explicit cast**：程序员明确要求转换。

例如：

```cpp
int a = 5;
double b = a;
```

这是 implicit cast，`int` 自动转成 `double`。

而：

```cpp
double a = 5.25;
int b = static_cast<int>(a);
```

这是 explicit cast，程序员明确告诉编译器：我要把 `double` 转成 `int`。

重点是：C++ 提供了比 C 风格 cast 更明确、更安全、更容易搜索的转换方式。

---

## 2. 为什么需要它

程序中经常需要把一种 type 当作另一种 type 使用，例如：

- 把 `double` 转成 `int`；
- 把 base class pointer 转成 derived class pointer；
- 把 `void*` 转成具体类型 pointer；
- 把普通 pointer 转成 byte pointer 观察 memory；
- 去掉或添加 `const`；
- 在底层系统代码中重新解释 memory。

如果没有 cast，很多底层和泛型代码就很难写。

但 cast 也很危险，因为它可能绕过类型系统。

例如：

```cpp
double value = 5.25;
int result = (int)value;
```

这段代码能工作，但它没有清楚说明转换意图。

C++ 更推荐：

```cpp
int result = static_cast<int>(value);
```

因为 `static_cast` 明确表达：这是一个普通、相对安全的类型转换。

---

## 3. 核心语法 

### 3.1 C 风格 cast

```cpp
int result = (int)value;
```

这是从 C 继承来的写法。

缺点是它太“万能”，可能同时执行多种转换，例如：

- numeric conversion；
- pointer conversion；
- 去掉 `const`；
- 重新解释 memory。

所以它不够明确。

---

### 3.2 C++ 风格 cast

C++ 提供了四种主要 cast：

```cpp
static_cast<T>(value)
dynamic_cast<T>(value)
reinterpret_cast<T>(value)
const_cast<T>(value)
```

它们的用途不同：

| cast | 主要用途 |
| :--- | :--- |
| `static_cast` | 普通类型转换，编译期检查 |
| `dynamic_cast` | polymorphism 中安全地向下转换 |
| `reinterpret_cast` | 底层 memory 重新解释 |
| `const_cast` | 添加或移除 `const` / `volatile` |

---

### 3.3 `static_cast`

常用于普通类型转换：

```cpp
double value = 5.25;
int result = static_cast<int>(value);
```

也可以用于 class 层级中编译期可判断的转换：

```cpp
Base* base = new Derived();
Derived* derived = static_cast<Derived*>(base);
```

但这里是否真的安全，要靠程序员保证。

---

### 3.4 `dynamic_cast`

用于 polymorphic class 的安全 downcast。

```cpp
Derived* derived = dynamic_cast<Derived*>(base);
```

如果 `base` 实际上不是 `Derived`，返回 `nullptr`。

要求 base class 至少有一个 `virtual` function。

---

### 3.5 `reinterpret_cast`

用于非常底层的类型重新解释：

```cpp
int value = 65;
char* ptr = reinterpret_cast<char*>(&value);
```

它不做普通意义上的数值转换，而是改变编译器解释 memory 的方式。

---

### 3.6 `const_cast`

用于移除或添加 `const`：

```cpp
const int value = 10;
int* ptr = const_cast<int*>(&value);
```

但如果原对象本身真的是 `const`，再通过 `ptr` 修改它，会导致 undefined behavior。

---

## 4. 示例代码

```cpp
#include <iostream>

class Entity
{
public:
    virtual ~Entity() = default;
};

class Player : public Entity
{
public:
    void Print()
    {
        std::cout << "Player\n";
    }
};

class Enemy : public Entity
{
};

void PrintValue(const int& value)
{
    std::cout << value << std::endl;
}

int main()
{
    double number = 5.75;
    int integer = static_cast<int>(number);

    std::cout << "static_cast result: " << integer << std::endl;

    Entity* entity = new Player();

    Player* player = dynamic_cast<Player*>(entity);
    if (player)
    {
        player->Print();
    }

    int value = 65;
    char* bytes = reinterpret_cast<char*>(&value);

    std::cout << "First byte as char: " << bytes[0] << std::endl;

    const int constant = 10;
    PrintValue(constant);

    int mutableValue = 20;
    const int* constPtr = &mutableValue;
    int* nonConstPtr = const_cast<int*>(constPtr);

    *nonConstPtr = 30;

    std::cout << "After const_cast: " << mutableValue << std::endl;

    delete entity;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `static_cast<int>(number)`

```cpp
int integer = static_cast<int>(number);
```

这里把 `double` 转成 `int`。

结果是：

```text
5
```

小数部分会被丢弃，不是四舍五入。

`static_cast` 适合这种普通、明确的转换。

---

### 5.2 `dynamic_cast<Player*>(entity)`

```cpp
Player* player = dynamic_cast<Player*>(entity);
```

`entity` 的静态类型是：

```cpp
Entity*
```

但它实际指向：

```cpp
Player
```

所以 `dynamic_cast` 成功，返回有效的 `Player*`。

如果写成：

```cpp
Enemy* enemy = dynamic_cast<Enemy*>(entity);
```

结果会是：

```cpp
nullptr
```

因为这个对象实际不是 `Enemy`。

---

### 5.3 `reinterpret_cast<char*>(&value)`

```cpp
char* bytes = reinterpret_cast<char*>(&value);
```

这不是把 `int` 的值转换成 `char`。

它是把 `value` 的地址重新解释成 `char*`，让我们可以按 byte 查看它的 memory。

`reinterpret_cast` 很底层，普通业务代码中应该少用。

---

### 5.4 `const_cast<int*>(constPtr)`

```cpp
int* nonConstPtr = const_cast<int*>(constPtr);
```

这里去掉了 pointer 上的 `const` 限制。

注意示例中真正的对象是：

```cpp
int mutableValue = 20;
```

它本身不是 `const`，所以通过 `nonConstPtr` 修改它是可以的。

但如果对象本身是：

```cpp
const int constant = 10;
```

再强行修改：

```cpp
int* ptr = const_cast<int*>(&constant);
*ptr = 20;
```

这是 undefined behavior。

---

## 6. 常见用法

### 6.1 数值类型转换

```cpp
float value = 3.14f;
int result = static_cast<int>(value);
```

用于明确表达数值转换。

---

### 6.2 base class 和 derived class 转换

```cpp
Entity* entity = new Player();
Player* player = dynamic_cast<Player*>(entity);
```

当你不确定 base pointer 实际指向哪种 derived type 时，可以用 `dynamic_cast` 检查。

---

### 6.3 处理 `void*`

底层 C API 经常返回 `void*`：

```cpp
void* memory = GetMemory();
int* data = static_cast<int*>(memory);
```

`void*` 转回具体 pointer type 时，经常需要 cast。

---

### 6.4 查看 raw memory

```cpp
int value = 123;
unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
```

可以逐 byte 查看对象的内存内容。

---

### 6.5 适配旧 API

有些旧接口没有正确使用 `const`，你可能需要临时使用：

```cpp
const_cast
```

但这通常是不得已的做法，不应该滥用。

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 可以表达程序员明确的转换意图 | 错误 cast 可能导致 undefined behavior |
| C++ 风格 cast 更容易搜索 | 某些 cast 会绕过类型系统 |
| `static_cast` 比 C 风格 cast 更清楚 | `reinterpret_cast` 很危险 |
| `dynamic_cast` 可以在运行时检查类型 | `dynamic_cast` 有运行时开销 |
| 不同 cast 分工明确 | 初学者容易混淆用途 |

---

## 8. 核心思想

### 重点 1：cast 是显式告诉编译器“我要转换 type”

cast 不应该随便写。

每次写 cast，都说明当前代码正在绕过或改变某种类型规则。

---

### 重点 2：优先使用 C++ 风格 cast

不要习惯性写：

```cpp
(int)value
```

更推荐：

```cpp
static_cast<int>(value)
```

C++ 风格虽然更长，但语义更明确。

---

### 重点 3：不同 cast 表达不同意图

常用判断：

```text
普通转换：static_cast
运行时类型检查：dynamic_cast
底层内存解释：reinterpret_cast
移除 const：const_cast
```

不要用一个 C 风格 cast 解决所有问题。

---

### 重点 4：cast 越底层，越要谨慎

危险程度大致可以这样理解：

```text
static_cast < dynamic_cast < const_cast < reinterpret_cast
```

不是绝对排序，但对初学者很有参考价值。

尤其是 `reinterpret_cast`，一般只应该出现在底层代码中。