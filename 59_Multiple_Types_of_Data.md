# 59 - 在单个变量中存储多种类型的数据 (Multiple TYPES of Data in a SINGLE VARIABLE in C++?)

## 1. 基本概念 
这一集的核心主题是 **`std::variant`**。

`std::variant` 是 C++17 引入的标准库类型，它允许一个变量在不同时间保存几种预先指定类型中的一种。

例如：

```cpp
std::variant<int, std::string> data;
```

这表示 `data` 可以是：

- 一个 `int`
- 或者一个 `std::string`

但要注意：  
它不是同时保存多个值，而是 **同一时刻只保存其中一种类型的值**。

也就是说：

```cpp
data = 10;              // 当前保存 int
data = "Hello C++";     // 当前保存 std::string
```

`std::variant` 可以理解为一种更安全、更现代的 `union`。

传统 `union` 也能让多个类型共享同一块内存，但它有几个问题：

- 不自动记录当前到底存的是哪种类型
- 对复杂类型支持麻烦，例如 `std::string`
- 容易访问错误类型导致 undefined behavior

而 `std::variant` 会记录当前保存的是哪一种类型，并提供类型安全的访问方式。

---

## 2. 为什么需要它 

在实际编程中，经常会遇到一个值可能有多种类型的情况。

例如，一个配置项可能是：

- `int`
- `float`
- `std::string`
- `bool`

如果没有 `std::variant`，我们可能会写出这样的代码：

```cpp
struct ConfigValue
{
    int intValue;
    float floatValue;
    std::string stringValue;
    bool boolValue;

    int type;
};
```

然后通过 `type` 判断当前应该使用哪个字段。

这种写法的问题是：

1. **浪费空间**

   每个对象都同时包含 `int`、`float`、`std::string`、`bool`，即使实际只需要其中一个。

2. **容易出错**

   程序员必须手动维护 `type`。

   例如：

   ```cpp
   value.type = 0;       // 表示 int
   value.stringValue = "Hello";
   ```

   这就会造成逻辑不一致。

3. **缺少类型安全**

   如果当前实际存的是 `std::string`，但你按 `int` 读取，程序可能产生错误。

4. **代码不够清晰**

   读代码的人必须理解 `type` 和各个字段之间的对应关系。

`std::variant` 解决的就是这个问题：

```cpp
std::variant<int, float, std::string, bool> value;
```

它把“一个变量可能是多种类型之一”这个意图直接写进类型系统里。

这就是 C++ 的一个重要思想：

> 能交给类型系统检查的事情，就不要只靠程序员手动记忆。

---

## 3. 核心语法

使用 `std::variant` 需要包含头文件：

```cpp
#include <variant>
```

如果要存储 `std::string`，还需要：

```cpp
#include <string>
```

---

### 3.1 定义一个 variant

```cpp
std::variant<int, std::string> data;
```

表示 `data` 可以保存 `int` 或 `std::string`。

---

### 3.2 给 variant 赋值

```cpp
data = 10;
data = std::string("Hello");
```

赋值之后，`std::variant` 内部保存的实际类型会改变。

---

### 3.3 使用 `std::get` 读取值

```cpp
int value = std::get<int>(data);
```

如果 `data` 当前确实保存的是 `int`，这段代码可以正常工作。

但如果当前保存的是 `std::string`，却写：

```cpp
int value = std::get<int>(data);
```

程序会抛出异常：

```cpp
std::bad_variant_access
```

所以 `std::get` 适合在你非常确定当前类型时使用。

---

### 3.4 使用 `std::get_if` 安全读取

```cpp
if (int* value = std::get_if<int>(&data))
{
    std::cout << *value << std::endl;
}
```

`std::get_if` 会返回一个 pointer：

- 如果当前保存的是目标类型，返回指向值的 pointer
- 如果当前不是目标类型，返回 `nullptr`

这种方式比直接 `std::get` 更安全。

---

### 3.5 使用 `std::holds_alternative` 判断类型

```cpp
if (std::holds_alternative<int>(data))
{
    std::cout << std::get<int>(data) << std::endl;
}
```

`std::holds_alternative<T>` 可以判断当前 `variant` 是否正在保存类型 `T`。

---

### 3.6 使用 `index()` 查看当前类型下标

```cpp
std::cout << data.index() << std::endl;
```

对于：

```cpp
std::variant<int, std::string, float> data;
```

类型下标是：

| 类型 | index |
| :--- | :--- |
| `int` | `0` |
| `std::string` | `1` |
| `float` | `2` |

如果当前保存的是 `std::string`，那么：

```cpp
data.index()
```

返回 `1`。

---

### 3.7 使用 `std::visit` 统一处理 variant

`std::visit` 可以根据 `variant` 当前保存的实际类型，自动调用对应的处理逻辑。

```cpp
std::visit([](auto&& value)
{
    std::cout << value << std::endl;
}, data);
```

这里的 `auto&&` 是一个 generic lambda 参数，它可以接收 `variant` 当前保存的任意类型。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <variant>
#include <string>
#include <type_traits>

void PrintData(const std::variant<int, std::string, bool>& data)
{
    std::visit([](const auto& value)
    {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, int>)
        {
            std::cout << "当前保存的是 int: " << value << std::endl;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            std::cout << "当前保存的是 std::string: " << value << std::endl;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            std::cout << "当前保存的是 bool: " << std::boolalpha << value << std::endl;
        }
    }, data);
}

int main()
{
    std::variant<int, std::string, bool> data;

    data = 42;
    PrintData(data);

    data = std::string("Hello Variant");
    PrintData(data);

    data = true;
    PrintData(data);

    if (std::holds_alternative<std::string>(data))
    {
        std::cout << "data 当前是 std::string" << std::endl;
    }
    else
    {
        std::cout << "data 当前不是 std::string" << std::endl;
    }

    data = std::string("C++17");

    if (const std::string* text = std::get_if<std::string>(&data))
    {
        std::cout << "安全读取 std::string: " << *text << std::endl;
    }

    try
    {
        int number = std::get<int>(data);
        std::cout << "读取 int: " << number << std::endl;
    }
    catch (const std::bad_variant_access& error)
    {
        std::cout << "读取失败: data 当前并不是 int" << std::endl;
    }

    std::cout << "当前类型 index: " << data.index() << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::variant<int, std::string, bool> data;`

这行代码定义了一个 `variant`。

它的意思是：

```cpp
data
```

可以保存三种类型之一：

```cpp
int
std::string
bool
```

但它不会同时保存三个值。

它只会在某一时刻保存其中一个。

---

### 5.2 默认构造时保存第一个类型

这段代码：

```cpp
std::variant<int, std::string, bool> data;
```

默认情况下会构造第一个类型，也就是 `int`。

因此，默认状态下 `data` 保存的是一个 `int`，值通常是 `0`。

如果你写成：

```cpp
std::variant<std::string, int, bool> data;
```

那么默认保存的就是一个空的 `std::string`。

这点很重要，因为 `std::variant` 默认会尝试构造第一个 alternative。

如果第一个类型不能默认构造，就可能导致编译错误。

---

### 5.3 赋值会改变当前保存的类型

```cpp
data = 42;
```

此时 `data` 保存的是 `int`。

```cpp
data = std::string("Hello Variant");
```

此时 `data` 原来保存的 `int` 会被替换，新的活动类型变成 `std::string`。

```cpp
data = true;
```

此时活动类型又变成 `bool`。

所以 `std::variant` 的本质不是“多个字段同时存在”，而是：

> 一个对象，在多个可能类型之间切换。

---

### 5.4 `std::get<T>` 要求当前类型必须正确

```cpp
int number = std::get<int>(data);
```

这表示：

> 请把 `data` 当前保存的 `int` 取出来。

如果 `data` 当前确实是 `int`，没有问题。

但如果当前是 `std::string`，这句代码就会失败并抛出：

```cpp
std::bad_variant_access
```

所以初学者要注意：

```cpp
std::get<int>(data)
```

不是“尝试把 data 转成 int”。

它的含义是：

> data 里面现在必须已经是 int。

`std::variant` 不会帮你做自动类型转换。

---

### 5.5 `std::get_if<T>` 更安全

```cpp
if (const std::string* text = std::get_if<std::string>(&data))
{
    std::cout << *text << std::endl;
}
```

这里 `std::get_if` 返回的是 pointer。

如果 `data` 当前保存的是 `std::string`，那么 `text` 不为 `nullptr`。

如果不是，`text` 就是 `nullptr`。

所以这种写法非常适合初学者：

```cpp
if (auto* value = std::get_if<int>(&data))
{
    // 当前确实是 int
}
```

这样就不用担心异常。

---

### 5.6 `std::holds_alternative<T>` 用来检查当前类型

```cpp
if (std::holds_alternative<std::string>(data))
{
    std::cout << std::get<std::string>(data) << std::endl;
}
```

这是一种两步写法：

1. 先判断当前是不是 `std::string`
2. 再用 `std::get<std::string>` 获取值

这种方式清楚、直接，适合教学和简单场景。

但如果分支很多，代码可能会变得啰嗦，这时更适合用 `std::visit`。

---

### 5.7 `std::visit` 是处理 variant 的常见方式

```cpp
std::visit([](const auto& value)
{
    std::cout << value << std::endl;
}, data);
```

`std::visit` 会根据 `data` 当前保存的实际类型，调用 lambda。

如果当前是 `int`，`value` 就是 `int`。

如果当前是 `std::string`，`value` 就是 `std::string`。

如果当前是 `bool`，`value` 就是 `bool`。

这就是 `std::visit` 的核心作用：

> 不需要自己手动判断类型，让 `variant` 根据当前类型分发逻辑。

---

### 5.8 `if constexpr` 用于编译期分支

示例代码里有这一段：

```cpp
if constexpr (std::is_same_v<T, int>)
{
    std::cout << "当前保存的是 int: " << value << std::endl;
}
```

这里的 `if constexpr` 是 C++17 引入的编译期条件判断。

它和普通 `if` 的区别是：

- 普通 `if` 是运行时判断
- `if constexpr` 是编译期判断

在 `std::visit` 里，`value` 可能是不同类型。

如果不同类型需要不同处理，就可以结合：

```cpp
decltype
std::decay_t
std::is_same_v
if constexpr
```

来写类型分支。

---

### 5.9 `std::variant` 和 `union` 的区别

传统 `union` 示例：

```cpp
union Data
{
    int number;
    float decimal;
};
```

`union` 的成员共享同一块内存。

但问题是：

```cpp
Data data;
data.number = 10;

std::cout << data.decimal << std::endl;
```

这段代码从逻辑上是错误的，因为你写入的是 `number`，却读取了 `decimal`。

`union` 本身不会告诉你当前哪个成员是有效的。

而 `std::variant` 会记录当前活动类型。

这就是它比 `union` 更安全的地方。

---

### 5.10 `std::variant` 和 `std::optional` 的区别

`std::optional<T>` 表示：

```cpp
一个值要么存在，要么不存在
```

例如：

```cpp
std::optional<int> result;
```

它的意思是：

- 要么有一个 `int`
- 要么没有值

而 `std::variant<A, B, C>` 表示：

```cpp
一个值可能是 A、B、C 中的一种
```

例如：

```cpp
std::variant<int, std::string, bool> value;
```

它的意思是：

- 当前可能是 `int`
- 也可能是 `std::string`
- 也可能是 `bool`

简单总结：

| 类型 | 表达含义 |
| :--- | :--- |
| `std::optional<T>` | 有或者没有一个 `T` |
| `std::variant<A, B>` | 是 `A` 或者是 `B` |
| `std::any` | 可以是几乎任意类型 |

---

### 5.11 `std::variant` 和 `std::any` 的区别

`std::variant` 的类型范围是固定的：

```cpp
std::variant<int, std::string> data;
```

这表示它只能保存：

```cpp
int
std::string
```

不能突然保存 `double`：

```cpp
data = 3.14; // 可能无法按预期工作，取决于类型匹配和转换规则
```

而 `std::any` 更自由：

```cpp
std::any data;
data = 10;
data = std::string("Hello");
data = 3.14;
```

但 `std::any` 的问题是类型约束更弱，读取时也更依赖程序员自己知道里面是什么。

所以一般来说：

- 类型集合明确时，优先考虑 `std::variant`
- 类型完全不确定时，才考虑 `std::any`

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 类型安全，比 `union` 更安全 | 语法比普通变量复杂 |
| 可以在一个变量中表示多种可能类型 | 类型列表必须在编译期确定 |
| 不需要手动维护 `type` 字段 | 如果类型太多，代码可读性会下降 |
| 支持 `std::string` 等复杂类型 | 使用 `std::get` 取错类型会抛异常 |
| 适合表达“几种类型之一”的数据模型 | `std::visit` 对初学者来说比较抽象 |
| C++17 标准库提供，不需要第三方库 | 不能随意保存未声明在类型列表中的类型 |

---

## 7. 核心思想

### 重点 1：`std::variant` 表示“多种类型之一”

`std::variant` 不是让一个变量同时拥有多个值。

它表达的是：

```cpp
这个变量当前可能是几种类型中的一种
```

例如：

```cpp
std::variant<int, std::string> value;
```

意思是：

```cpp
value 当前要么是 int，要么是 std::string
```

这是一种非常常见的数据建模方式。

---

### 重点 2：`std::variant` 是更安全的 `union`

传统 `union` 可以让不同类型共享内存，但它不记录当前哪个类型有效。

`std::variant` 不仅能保存不同类型，还会记录当前活动类型。

所以它可以提供：

```cpp
std::get
std::get_if
std::holds_alternative
std::visit
```

这些安全接口。

这就是现代 C++ 的风格：

> 尽量用标准库提供的类型安全工具，而不是手写容易出错的底层结构。

---

### 重点 3：读取 variant 时必须知道当前类型

这段代码：

```cpp
std::get<int>(data)
```

并不是类型转换。

它不是把 `data` 转成 `int`。

它的意思是：

```cpp
data 当前必须已经是 int
```

如果不是，就会出错。

所以在不确定当前类型时，更推荐：

```cpp
std::get_if<int>(&data)
```

或者：

```cpp
std::visit(...)
```

---

### 重点 4：类型集合应该明确

`std::variant` 最适合这种场景：

```cpp
一个值只可能是几种已知类型之一
```

比如：

```cpp
std::variant<int, float, std::string, bool> configValue;
```

如果你不知道它可能是什么类型，或者类型完全开放，那么 `std::variant` 可能不是最合适的工具。

这时可以考虑：

```cpp
std::any
```

但 `std::any` 会牺牲更多类型安全和可读性。

---

### 重点 5：优先用 `std::visit` 处理复杂逻辑

如果只是简单读取，可以用：

```cpp
std::get_if
std::holds_alternative
```

但如果你要对不同类型做不同操作，推荐学习并使用：

```cpp
std::visit
```

它是处理 `std::variant` 的标准方式。

例如：

```cpp
std::visit([](const auto& value)
{
    std::cout << value << std::endl;
}, data);
```

这段代码可以处理 `variant` 当前保存的任意类型。