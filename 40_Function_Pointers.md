# 40 - C++ 函数指针 (Function Pointers)

## 1. 基本概念 (Basic Concepts)

函数指针（Function Pointer）的本质是：

> **一个变量，这个变量里存放的是函数的地址。**

你已经学过普通指针：

```cpp
int* ptr = &value;
```

这里 `ptr` 保存的是一个变量的地址。

函数指针也是同样的思路，只不过它保存的不是普通变量地址，而是：

> **某个函数在内存中的入口地址。**

也就是说，函数本身也可以被“指向”。

---

## 2. 为什么函数也可以有地址

程序运行时，代码也会被加载到内存中。  
普通函数并不是抽象概念，它们最终也对应一段机器指令。

例如：

```cpp
void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}
```

这个函数在程序里会有一个地址。  
而函数指针做的事情就是：

- 保存这个地址
- 以后通过这个地址去调用对应函数

所以：

> **函数名在很多场景下，本质上就代表这个函数的地址。**

---

## 3. 最基础的例子 (Basic Example)

### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    auto function = HelloWorld;
    function();

    return 0;
}
```

### 说明

这里：

```cpp
auto function = HelloWorld;
```

表示让 `function` 保存 `HelloWorld` 的函数地址。

然后：

```cpp
function();
```

就等价于调用这个函数。

### 关键点

这里没有写：

```cpp
HelloWorld()
```

而是写：

```cpp
HelloWorld
```

因为：

- `HelloWorld()` 是“调用函数”
- `HelloWorld` 是“函数本身/函数地址”

---

## 4. 函数指针的完整写法 (Full Syntax)

虽然 `auto` 很方便，但函数指针真正的类型写法才是这集的核心之一。

### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    void(*function)() = HelloWorld;
    function();

    return 0;
}
```

### 语法拆解

```cpp
void(*function)()
```

可以拆成三部分理解：

- `void`：这个函数返回 `void`
- `(*function)`：`function` 是一个指针
- `()`：这个指针指向的是一个“无参数函数”

所以整句话的意思是：

> **定义一个名为 `function` 的指针，它指向一个“无参数、返回值为 void”的函数。**

---

## 5. 为什么语法这么怪

函数指针的语法对初学者来说经常显得很怪，  
因为它把“指针”和“函数签名”混在了一起。

例如：

```cpp
void(*function)()
```

乍一看不如普通变量直观。  
但它实际上是在准确表达：

- 这是个指针
- 指向的是函数
- 这个函数的返回类型是 `void`
- 参数列表为空

所以函数指针的类型，不只跟返回值有关，  
还和**参数列表**有关。

---

## 6. 函数签名必须匹配 (Function Signature Must Match)

函数指针不是“能指任何函数”。  
它只能指向**签名匹配**的函数。

例如：

```cpp
void HelloWorld()
{
}
```

那么匹配它的函数指针类型必须是：

```cpp
void(*)()
```

如果函数是：

```cpp
int Multiply(int a, int b)
{
    return a * b;
}
```

那么函数指针类型就应该是：

```cpp
int(*)(int, int)
```

### 示例代码

```cpp
#include <iostream>

int Multiply(int a, int b)
{
    return a * b;
}

int main()
{
    int(*operation)(int, int) = Multiply;

    std::cout << operation(3, 4) << std::endl;
    return 0;
}
```

### 说明

这里：

- 返回类型是 `int`
- 参数是两个 `int`

所以函数指针必须完全匹配这个签名。

---

## 7. 使用 `typedef` / `using` 简化类型

很常见的一个做法，就是先给函数指针类型起别名。  
因为直接写原始类型会比较难读。

### 用 `typedef`

```cpp
typedef void(*HelloWorldFunction)();
```

这表示：

- `HelloWorldFunction` 是一个类型别名
- 它代表“指向无参数、返回 `void` 的函数的指针”

### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    typedef void(*HelloWorldFunction)();
    HelloWorldFunction function = HelloWorld;

    function();
    return 0;
}
```

---

### 用 `using`

现代 C++ 中更常见的是：

```cpp
using HelloWorldFunction = void(*)();
```

### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    using HelloWorldFunction = void(*)();
    HelloWorldFunction function = HelloWorld;

    function();
    return 0;
}
```

### 优点

- 可读性更好
- 更适合复杂类型别名
- 现代 C++ 风格更统一

---

## 8. 函数指针的调用方式

假设你有：

```cpp
void(*function)() = HelloWorld;
```

那么调用时可以写：

```cpp
function();
```

也可以写成：

```cpp
(*function)();
```

这两种写法都可以。

### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    void(*function)() = HelloWorld;

    function();
    (*function)();

    return 0;
}
```

### 说明

大多数时候直接写：

```cpp
function();
```

就够了。

---

## 9. 函数指针最重要的用途：回调 (Callbacks)

> **函数可以被当作参数传给别的函数。**

这就是函数指针最经典的用途之一：**回调（Callback）**。

意思是：

- 你把一个函数传进去
- 另一个函数在合适的时候调用它

---

## 10. 回调示例 (Callback Example)

### 示例代码

```cpp
#include <iostream>
#include <vector>

void PrintValue(int value)
{
    std::cout << "Value: " << value << std::endl;
}

void ForEach(const std::vector<int>& values, void(*func)(int))
{
    for (int value : values)
        func(value);
}

int main()
{
    std::vector<int> values = { 1, 5, 4, 2, 3 };

    ForEach(values, PrintValue);

    return 0;
}
```

### 说明

这里：

```cpp
void ForEach(const std::vector<int>& values, void(*func)(int))
```

表示 `ForEach` 接收两个参数：

1. 一个整数数组/容器
2. 一个函数指针 `func`

而这个 `func` 必须满足：

- 返回 `void`
- 接收一个 `int`

所以：

```cpp
ForEach(values, PrintValue);
```

就是把 `PrintValue` 这个函数传进去了。

然后在 `ForEach` 里：

```cpp
func(value);
```

就会调用这个回调函数。

---

## 11. 为什么这很重要

函数指针让程序拥有更强的“行为可配置性”。

如果没有函数指针，你可能只能把逻辑写死：

```cpp
void ForEach(const std::vector<int>& values)
{
    for (int value : values)
        std::cout << value << std::endl;
}
```

这样它就只能打印。

但有了函数指针后：

- 你可以传打印函数
- 可以传计数函数
- 可以传过滤函数
- 可以传处理函数

也就是说：

> **同一套遍历逻辑，可以配合不同的行为函数复用。**

这也是函数指针和后面 lambda 的衔接点。

---

## 12. 函数指针 vs 普通函数调用

| 特性 | 普通函数调用 | 函数指针 |
| :--- | :--- | :--- |
| **调用目标** | 写死在代码里 | 可以动态决定 |
| **可配置性** | 低 | 更高 |
| **语法复杂度** | 简单 | 稍复杂 |
| **典型用途** | 固定逻辑 | 回调、行为注入 |

### 举例

普通调用：

```cpp
HelloWorld();
```

函数指针调用：

```cpp
void(*function)() = HelloWorld;
function();
```

普通调用更简单，  
但函数指针更灵活。

---

## 13. 函数指针和普通指针的相同点与不同点

### 相同点

- 都是“地址”
- 都可以存进变量
- 都可以间接访问目标

### 不同点

| 项目 | 普通指针 | 函数指针 |
| :--- | :--- | :--- |
| **指向对象** | 普通变量/对象 | 函数 |
| **访问方式** | `*ptr` | `func()` 或 `(*func)()` |
| **类型匹配依据** | 指向对象类型 | 函数签名 |

---

## 14. `auto` 在函数指针中的用法
### 示例代码

```cpp
#include <iostream>

void HelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}

int main()
{
    auto function = HelloWorld;
    function();

    return 0;
}
```

### 优点

- 不用手写复杂类型
- 更干净
- 适合快速保存函数地址

### 但要注意

`auto` 虽然简洁，  
但如果你要真正理解函数指针，还是必须能看懂：

```cpp
void(*function)()
```

因为这才是底层类型本体。

