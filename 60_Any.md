# 60 - 如何在 C++ 中存储任意数据 (How to store ANY data in C++)

## 1. 基本概念

`std::any` 是 C++17 引入的标准库类型，它可以在一个变量中存储几乎任意类型的数据。

例如：

```cpp
std::any data;

data = 10;
data = 3.14f;
data = std::string("Hello");
```

这表示 `data` 可以先保存一个 `int`，之后又可以保存一个 `float`，再之后又可以保存一个 `std::string`。

和普通变量不同，普通变量的类型在声明时就固定了：

```cpp
int value = 10;
value = "Hello"; // 错误
```

而 `std::any` 的目的就是让一个变量能够保存不同类型的值。

不过要注意：

> `std::any` 不是让 C++ 变成动态类型语言，它只是把具体类型隐藏到了运行时。

也就是说，`std::any` 内部仍然保存着一个真实的 C++ 类型，只是外层统一包装成了 `std::any`。

使用 `std::any` 需要包含头文件：

```cpp
#include <any>
```

如果要存储 `std::string`，还需要：

```cpp
#include <string>
```

---

## 2. 为什么需要它

在 C++ 中，变量通常必须有明确类型：

```cpp
int number = 10;
std::string text = "Hello";
float decimal = 3.14f;
```

这种设计的好处是：

- 类型安全
- 编译器可以优化
- 错误更容易在编译期发现

但是有些场景下，一个系统确实需要存储“不确定类型”的数据。

例如，一个属性系统：

```cpp
health = 100;
name = "Player";
speed = 4.5f;
isAlive = true;
```

这些属性的类型不同：

| 属性 | 类型 |
| :--- | :--- |
| `health` | `int` |
| `name` | `std::string` |
| `speed` | `float` |
| `isAlive` | `bool` |

如果不用 `std::any`，你可能需要写很多不同的容器：

```cpp
std::unordered_map<std::string, int> intValues;
std::unordered_map<std::string, float> floatValues;
std::unordered_map<std::string, std::string> stringValues;
std::unordered_map<std::string, bool> boolValues;
```

这样做会让代码变得复杂。

而使用 `std::any`，可以写成：

```cpp
std::unordered_map<std::string, std::any> properties;
```

这样同一个容器就可以存储不同类型的数据：

```cpp
properties["health"] = 100;
properties["name"] = std::string("Player");
properties["speed"] = 4.5f;
properties["isAlive"] = true;
```

这就是 `std::any` 解决的问题：

> 当你确实需要在一个统一接口中存储不同类型数据时，`std::any` 可以提供一种标准库级别的解决方案。

---

## 3. 核心语法

### 3.1 定义一个 `std::any`

```cpp
std::any data;
```

此时 `data` 还没有保存任何值。

可以用：

```cpp
data.has_value()
```

判断它是否有值。

---

### 3.2 给 `std::any` 赋值

```cpp
data = 10;
data = 3.14f;
data = std::string("Hello");
```

每一次赋值都会让 `std::any` 保存新的类型和值。

---

### 3.3 使用 `std::any_cast` 取出值

```cpp
int value = std::any_cast<int>(data);
```

如果 `data` 当前保存的确实是 `int`，这段代码可以正常工作。

如果 `data` 当前保存的不是 `int`，程序会抛出异常：

```cpp
std::bad_any_cast
```

例如：

```cpp
std::any data = std::string("Hello");

int value = std::any_cast<int>(data); // 错误，抛出 std::bad_any_cast
```

---

### 3.4 使用 pointer 形式的 `std::any_cast`

更安全的写法是：

```cpp
if (int* value = std::any_cast<int>(&data))
{
    std::cout << *value << std::endl;
}
```

这种写法不会抛异常。

如果类型正确，返回有效 pointer。

如果类型错误，返回 `nullptr`。

---

### 3.5 检查 `std::any` 是否有值

```cpp
if (data.has_value())
{
    std::cout << "data 有值" << std::endl;
}
```

---

### 3.6 清空 `std::any`

```cpp
data.reset();
```

调用 `reset()` 之后，`std::any` 不再保存任何值。

---

### 3.7 查看当前保存的类型

```cpp
std::cout << data.type().name() << std::endl;
```

`type()` 返回一个 `std::type_info` 对象，可以用来查看当前保存的类型信息。

不过要注意：

```cpp
data.type().name()
```

输出的字符串不一定是人类友好的名字，具体格式取决于编译器。

---

## 4. 示例代码

```cpp
#include <any>
#include <iostream>
#include <string>
#include <unordered_map>

void PrintAnyValue(const std::any& value)
{
    if (!value.has_value())
    {
        std::cout << "当前 std::any 没有保存任何值" << std::endl;
        return;
    }

    if (const int* number = std::any_cast<int>(&value))
    {
        std::cout << "int: " << *number << std::endl;
    }
    else if (const float* decimal = std::any_cast<float>(&value))
    {
        std::cout << "float: " << *decimal << std::endl;
    }
    else if (const bool* flag = std::any_cast<bool>(&value))
    {
        std::cout << "bool: " << std::boolalpha << *flag << std::endl;
    }
    else if (const std::string* text = std::any_cast<std::string>(&value))
    {
        std::cout << "std::string: " << *text << std::endl;
    }
    else
    {
        std::cout << "未知类型: " << value.type().name() << std::endl;
    }
}

int main()
{
    std::any data;

    data = 42;
    PrintAnyValue(data);

    data = 3.14f;
    PrintAnyValue(data);

    data = std::string("Hello std::any");
    PrintAnyValue(data);

    data = true;
    PrintAnyValue(data);

    std::unordered_map<std::string, std::any> properties;

    properties["health"] = 100;
    properties["speed"] = 4.5f;
    properties["name"] = std::string("Player");
    properties["isAlive"] = true;

    std::cout << "\nPlayer Properties:" << std::endl;

    for (const auto& [key, value] : properties)
    {
        std::cout << key << " = ";
        PrintAnyValue(value);
    }

    try
    {
        std::string name = std::any_cast<std::string>(properties["name"]);
        std::cout << "\n玩家名字: " << name << std::endl;

        int wrongType = std::any_cast<int>(properties["name"]);
        std::cout << wrongType << std::endl;
    }
    catch (const std::bad_any_cast& error)
    {
        std::cout << "std::any_cast 失败：类型不匹配" << std::endl;
    }

    data.reset();

    if (!data.has_value())
    {
        std::cout << "\ndata 已经被清空" << std::endl;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::any` 可以保存任意类型

```cpp
std::any data;
```

这行代码声明了一个可以保存任意类型对象的变量。

之后可以写：

```cpp
data = 42;
data = 3.14f;
data = std::string("Hello");
data = true;
```

每次赋值时，`std::any` 都会保存新的值，并记住当前值的真实类型。

---

### 5.2 `std::any` 同一时间只保存一个值

虽然 `std::any` 可以保存不同类型，但它不是同时保存多个值。

例如：

```cpp
std::any data;

data = 10;
data = std::string("Hello");
```

执行完第二次赋值后，`data` 中保存的是 `std::string`。

之前的 `int` 已经被替换掉了。

所以 `std::any` 的含义是：

```cpp
这个变量当前可以保存某种类型的一个值
```

而不是：

```cpp
这个变量同时保存很多不同类型的值
```

---

### 5.3 `std::any_cast<T>` 必须使用正确类型

假设：

```cpp
std::any data = std::string("Hello");
```

正确读取方式是：

```cpp
std::string text = std::any_cast<std::string>(data);
```

错误读取方式是：

```cpp
int number = std::any_cast<int>(data);
```

因为 `data` 当前保存的是 `std::string`，不是 `int`。

所以这段代码会抛出：

```cpp
std::bad_any_cast
```

初学者要特别注意：

> `std::any_cast<int>` 不是“把值转换成 int”，而是“按照 int 类型取出值”。

如果里面不是 `int`，它不会自动转换。

---

### 5.4 pointer 形式的 `std::any_cast` 更安全

```cpp
if (const int* number = std::any_cast<int>(&value))
{
    std::cout << *number << std::endl;
}
```

这是一种很推荐的写法。

它的逻辑是：

1. 尝试把 `value` 当作 `int` 读取
2. 如果成功，返回指向内部值的 pointer
3. 如果失败，返回 `nullptr`

因此可以用 `if` 判断是否成功。

这种方式不会抛异常，更适合写健壮代码。

---

### 5.5 `std::any` 会隐藏类型信息

普通 C++ 代码中，类型通常很明确：

```cpp
int health = 100;
std::string name = "Player";
```

看到变量声明，就知道变量是什么类型。

但使用 `std::any` 后：

```cpp
std::any value;
```

只看声明无法知道里面具体是什么。

所以 `std::any` 的灵活性是有代价的：

- 代码可读性可能下降
- 类型错误可能从编译期推迟到运行时
- 取值时必须自己知道或判断真实类型

这也是 The Cherno 在讲这类工具时经常强调的现代 C++ 思想：

> 灵活性不是免费的。越灵活的工具，越需要谨慎使用。

---

### 5.6 `std::any` 适合属性系统

示例中：

```cpp
std::unordered_map<std::string, std::any> properties;
```

表示创建一个属性表。

key 是属性名：

```cpp
"health"
"speed"
"name"
"isAlive"
```

value 是任意类型：

```cpp
100
4.5f
std::string("Player")
true
```

这样就可以把不同类型的属性放进同一个容器。

这类设计在以下场景中可能出现：

- 游戏引擎 entity 属性
- 编辑器属性面板
- 配置系统
- 脚本绑定
- 序列化系统
- 插件系统
- 事件系统

---

### 5.7 `std::any` 和 `void*` 的区别

在 C 风格代码中，也可以使用 `void*` 存储任意类型的 pointer：

```cpp
void* data;
```

但 `void*` 有很大问题：

1. 它只保存地址，不管理对象生命周期
2. 它不知道原始类型是什么
3. 转换错误很容易造成 undefined behavior
4. 对象析构需要程序员自己处理

例如：

```cpp
int value = 10;
void* data = &value;

std::string* text = static_cast<std::string*>(data); // 非常危险
```

这段代码编译可能通过，但运行时行为是错误的。

相比之下，`std::any` 更安全：

```cpp
std::any data = 10;

if (int* value = std::any_cast<int>(&data))
{
    std::cout << *value << std::endl;
}
```

`std::any` 至少知道自己当前保存的真实类型，并能在类型不匹配时返回 `nullptr` 或抛出异常。

---

### 5.8 `std::any` 和 `std::variant` 的区别

上一集讲的是 `std::variant`。

它和 `std::any` 很相似，但设计目的不同。

`std::variant` 的类型集合是固定的：

```cpp
std::variant<int, float, std::string> value;
```

这表示 `value` 只能保存：

```cpp
int
float
std::string
```

而 `std::any` 更开放：

```cpp
std::any value;

value = 10;
value = 3.14;
value = std::string("Hello");
value = std::vector<int>{1, 2, 3};
```

对比表：

| 特性 | `std::variant` | `std::any` |
| :--- | :--- | :--- |
| 类型集合 | 编译期固定 | 理论上任意类型 |
| 类型安全 | 更强 | 较弱 |
| 访问方式 | `std::get` / `std::visit` | `std::any_cast` |
| 是否适合已知类型集合 | 非常适合 | 不如 `std::variant` |
| 是否适合完全开放的数据 | 不适合 | 适合 |
| 错误发现时机 | 更多在编译期 | 更多在运行时 |

简单判断：

```cpp
如果你知道可能有哪些类型，优先用 std::variant。
如果你真的不知道类型，才考虑 std::any。
```

---

### 5.9 `std::any` 内部可能发生 heap allocation

`std::any` 需要保存任意类型对象。

如果对象比较小，标准库实现可能会使用 small object optimization，把对象直接存储在 `std::any` 内部。

如果对象比较大，可能会在 heap 上分配内存。

例如：

```cpp
std::any a = 10;
```

这种小对象通常比较轻量。

但如果存储大型对象：

```cpp
std::any b = std::vector<int>(100000);
```

就可能涉及 heap allocation。

所以不要因为 `std::any` 方便，就在性能敏感代码中大量滥用它。

尤其是在：

- 游戏主循环
- 渲染循环
- 高频事件系统
- 实时系统
- 大量对象容器

这些场景中，要特别关注 `std::any` 带来的运行时成本。

---

### 5.10 `std::any` 不适合替代正常类型设计

有些初学者看到 `std::any` 后，可能会想：

```cpp
std::any x;
std::any y;
std::any z;
```

然后所有变量都用 `std::any`。

这不是好习惯。

C++ 的优势之一就是强类型系统。

例如：

```cpp
int health;
float speed;
std::string name;
```

这些类型信息非常有价值。

它们能让编译器帮你检查错误，也能让读代码的人更容易理解程序。

如果全部换成：

```cpp
std::any health;
std::any speed;
std::any name;
```

代码反而更差。

因此，`std::any` 应该只在确实需要类型擦除的地方使用。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 可以在一个变量中保存任意类型 | 类型错误会推迟到运行时 |
| 标准库提供，不需要自己实现类型擦除 | 可读性可能下降 |
| 比 `void*` 更安全 | 需要使用 `std::any_cast` 取值 |
| 适合属性系统、配置系统、插件系统 | 可能产生额外性能开销 |
| 能统一存储不同类型数据 | 不适合滥用为普通变量 |
| 可以判断是否有值 | 不能像 `std::variant` 那样方便地用 `std::visit` 分发逻辑 |

---

## 7. 核心思想

### 重点 1：`std::any` 是“类型擦除”的工具

`std::any` 的核心思想是：

```cpp
我不在外层暴露具体类型，但内部仍然保存真实类型。
```

例如：

```cpp
std::any value = 10;
```

从外面看，`value` 是 `std::any`。

但它内部实际保存的是一个 `int`。

这种做法叫做 type erasure，也就是类型擦除。

---

### 重点 2：`std::any_cast` 不是类型转换

这点非常重要。

```cpp
std::any value = std::string("123");

int number = std::any_cast<int>(value);
```

这不是把字符串 `"123"` 转成整数 `123`。

它只是尝试把 `value` 里面的对象当成 `int` 取出来。

由于里面实际是 `std::string`，所以会失败。

如果你想做字符串转整数，应该使用：

```cpp
std::stoi
```

例如：

```cpp
std::string text = std::any_cast<std::string>(value);
int number = std::stoi(text);
```

---

### 重点 3：优先使用具体类型

如果一个变量明确就是整数，就写：

```cpp
int health = 100;
```

不要写：

```cpp
std::any health = 100;
```

如果一个变量明确就是字符串，就写：

```cpp
std::string name = "Player";
```

不要写：

```cpp
std::any name = std::string("Player");
```

`std::any` 是特殊场景下的工具，不是普通变量的替代品。

---

### 重点 4：已知类型集合时优先使用 `std::variant`

如果一个值只可能是几种已知类型之一：

```cpp
int
float
std::string
bool
```

那么通常更推荐：

```cpp
std::variant<int, float, std::string, bool>
```

而不是：

```cpp
std::any
```

因为 `std::variant` 的类型范围更明确，编译器能提供更多检查。

---

### 重点 5：`std::any` 用得越多，越要小心架构设计

`std::any` 很灵活，但它也会削弱类型系统。

如果一个项目里到处都是：

```cpp
std::any
```

那可能说明代码结构不够清晰。

更好的做法通常是：

- 能用普通类型就用普通类型
- 能用 `struct` / `class` 表达清楚就用 `struct` / `class`
- 类型集合固定时用 `std::variant`
- 只有类型真的开放时才用 `std::any`

