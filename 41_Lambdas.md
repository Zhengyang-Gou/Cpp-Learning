# 41 - C++ Lambda 表达式 (Lambdas)

## 1. 基本概念

Lambda 表达式（Lambda Expression）的本质是：

> **一种定义匿名函数（Anonymous Function）的方法。**

所谓匿名函数，就是：

- 它也是函数
- 也能接收参数
- 也能有返回值
- 也能被调用

但它和普通函数不同的地方在于：

> **它不一定需要一个单独的函数名。**

例如：

```cpp
[]()
{
    std::cout << "Hello" << std::endl;
}
```

这就是一个 lambda。

Lambda 最大的价值在于：

- 适合写临时的小函数
- 适合写一次性的逻辑
- 很适合配合算法、回调、遍历等场景使用

---

## 2. 为什么需要 Lambda 

在上一集你已经学过函数指针。  
函数指针让我们知道：

> **函数也可以像值一样被传来传去。**

但是函数指针有个问题：

- 你通常得先单独写一个函数
- 然后再把这个函数传进去

例如：

```cpp
void PrintValue(int value)
{
    std::cout << "Value: " << value << std::endl;
}
```

再传给某个函数：

```cpp
ForEach(values, PrintValue);
```

如果这个函数逻辑只用一次，  
那专门写一个具名函数会显得有点啰嗦。

这时候 lambda 就很合适：

> **把“只用一次的小函数”直接写在调用现场。**

---

## 3. 最基本的 Lambda 写法

### 示例代码

```cpp
#include <iostream>

int main()
{
    auto lambda = []()
    {
        std::cout << "Hello from lambda!" << std::endl;
    };

    lambda();

    return 0;
}
```

### 说明

这里：

```cpp
[]()
{
    std::cout << "Hello from lambda!" << std::endl;
}
```

就是一个 lambda 表达式。

它的结构可以拆成：

```cpp
[capture](parameters)
{
    body
}
```

也就是：

1. `[]`：捕获列表
2. `()`：参数列表
3. `{}`：函数体

然后我们把这个 lambda 赋值给变量：

```cpp
auto lambda = ...
```

再调用：

```cpp
lambda();
```

---

## 4. Lambda 看起来像函数，但它更像对象

从使用体验上看，lambda 很像一个小函数。  
但从语言机制上看，它更接近于一个“可调用对象”。

不过在学习初期，你可以先把它理解为：

> **一个能被保存和调用的匿名函数。**

---

## 5. Lambda 与普通函数的区别

| 特性 | 普通函数 | Lambda |
| :--- | :--- | :--- |
| **是否必须有名字** | 通常要有 | 可以没有 |
| **定义位置** | 一般在外部单独定义 | 可以直接写在使用位置 |
| **是否能捕获外部变量** | 不能直接捕获局部变量 | 可以 |
| **适合场景** | 通用逻辑、复用逻辑 | 一次性、小范围、临时逻辑 |

---

## 6. 带参数的 Lambda

Lambda 当然也可以接收参数。

### 示例代码

```cpp
#include <iostream>

int main()
{
    auto print = [](int value)
    {
        std::cout << "Value: " << value << std::endl;
    };

    print(5);
    print(10);

    return 0;
}
```

### 说明

这里：

```cpp
[](int value)
```

表示：

- 不捕获任何外部变量
- 接收一个 `int` 参数

和普通函数很像：

```cpp
void Print(int value)
{
    ...
}
```

只不过 lambda 没有单独的函数名。

---

## 7. 带返回值的 Lambda

Lambda 也可以返回值。

### 示例代码

```cpp
#include <iostream>

int main()
{
    auto multiply = [](int a, int b)
    {
        return a * b;
    };

    std::cout << multiply(3, 4) << std::endl;
    return 0;
}
```

### 说明

这里 lambda：

- 接收两个 `int`
- 返回它们的乘积

如果返回值足够明显，编译器通常能自动推导返回类型。

---

## 8. 显式指定返回类型 

有时候你也可以显式写出返回类型。

### 示例代码

```cpp
#include <iostream>

int main()
{
    auto divide = [](float a, float b) -> float
    {
        return a / b;
    };

    std::cout << divide(10.0f, 2.0f) << std::endl;
    return 0;
}
```

### 说明

这里：

```cpp
-> float
```

表示显式指定返回类型为 `float`。

一般在返回值不明显，  
或者你想强制某种返回类型时才会写出来。

---

## 9. Lambda 最重要的特点：捕获外部变量

这集真正的重点，不只是“匿名函数怎么写”，  
而是：

> **Lambda 可以使用它外部作用域中的变量。**

这就是捕获。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;

    auto lambda = [a]()
    {
        std::cout << a << std::endl;
    };

    lambda();
    return 0;
}
```

### 说明

这里：

```cpp
[a]
```

表示把外部变量 `a` 捕获进 lambda。

这样即使 lambda 自己没有参数，  
它也能在函数体里使用 `a`。

---

## 10. 值捕获

当你写：

```cpp
[a]
```

时，表示**按值捕获**。

也就是说：

- lambda 会得到 `a` 的一份副本
- 在 lambda 内部使用的是这份拷贝
- 外部原变量和内部副本不是同一个东西

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;

    auto lambda = [a]()
    {
        std::cout << a << std::endl;
    };

    a = 10;

    lambda(); // 输出 5
    return 0;
}
```

### 说明

虽然外部 `a` 后来变成了 `10`，  
但 lambda 捕获进去的是当时的副本 `5`。

所以结果仍然是：

```cpp
5
```

---

## 11. 引用捕获

如果希望 lambda 使用的是外部变量本身，  
而不是副本，就可以用引用捕获。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;

    auto lambda = [&a]()
    {
        std::cout << a << std::endl;
    };

    a = 10;

    lambda(); // 输出 10
    return 0;
}
```

### 说明

这里：

```cpp
[&a]
```

表示按引用捕获 `a`。

所以 lambda 里面访问的是外部变量本体。  
外面改了，里面看到的也是改后的值。

---

## 12. 修改外部变量 

引用捕获不仅能读取外部变量，  
还能直接修改它。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;

    auto increment = [&a]()
    {
        a++;
    };

    increment();
    increment();

    std::cout << a << std::endl; // 输出 7
    return 0;
}
```

### 说明

因为这里是：

```cpp
[&a]
```

所以 lambda 内部操作的就是外部那个 `a`。

---

## 13. 默认捕获方式

如果你不想一个个写变量名，  
也可以使用默认捕获。

### 1）按值默认捕获

```cpp
[=]
```

表示默认按值捕获用到的外部变量。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;
    int b = 10;

    auto lambda = [=]()
    {
        std::cout << a + b << std::endl;
    };

    lambda();
    return 0;
}
```

---

### 2）按引用默认捕获

```cpp
[&]
```

表示默认按引用捕获用到的外部变量。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;
    int b = 10;

    auto lambda = [&]()
    {
        a++;
        b++;
    };

    lambda();

    std::cout << a << ", " << b << std::endl; // 6, 11
    return 0;
}
```

---

## 14. 混合捕获 

你还可以组合使用。

例如：

```cpp
[a, &b]
```

表示：

- `a` 按值捕获
- `b` 按引用捕获

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;
    int b = 10;

    auto lambda = [a, &b]()
    {
        std::cout << a << std::endl;
        b++;
    };

    lambda();

    std::cout << b << std::endl; // 11
    return 0;
}
```

### 说明

这种写法很适合你明确区分：

- 哪些变量只读副本就够了
- 哪些变量需要真正修改外部值

---

## 15. 为什么值捕获里不能随便改变量

当 lambda 按值捕获变量时，  
默认情况下它的 `operator()` 是 `const` 的。

你可以先简单理解成：

> **默认值捕获更偏向“只读副本”。**

所以像这样通常会报错：

```cpp
int a = 5;

auto lambda = [a]()
{
    a++;
};
```

因为这里试图修改值捕获进来的副本。

---

## 16. `mutable` 关键字 (Mutable Lambda)

如果你真的想修改“按值捕获的副本”，  
可以加上 `mutable`。

### 示例代码

```cpp
#include <iostream>

int main()
{
    int a = 5;

    auto lambda = [a]() mutable
    {
        a++;
        std::cout << a << std::endl;
    };

    lambda();
    std::cout << a << std::endl; // 仍然是 5

    return 0;
}
```

### 说明

这里发生的是：

- lambda 内部那份副本被改成了 `6`
- 外部原来的 `a` 还是 `5`

所以：

> **`mutable` 允许你修改值捕获进来的副本，但不会影响外部原变量。**

---

## 17. Lambda 作为回调

上一集你学过函数指针，  
知道函数可以传给另一个函数当回调。

Lambda 在这方面更方便，  
因为你可以直接把逻辑写在调用处。

### 示例代码

```cpp
#include <iostream>
#include <vector>

void ForEach(const std::vector<int>& values, void(*func)(int))
{
    for (int value : values)
        func(value);
}

int main()
{
    std::vector<int> values = { 1, 2, 3, 4, 5 };

    ForEach(values, [](int value)
    {
        std::cout << "Value: " << value << std::endl;
    });

    return 0;
}
```

### 说明

这里没有单独写：

```cpp
void PrintValue(int value)
{
    ...
}
```

而是直接把 lambda 传进去了。

这就是 lambda 最典型的优势之一：

> **临时逻辑就地定义，就地使用。**

---

## 18. Lambda 为什么比普通函数指针更舒服

| 对比项 | 函数指针 | Lambda |
| :--- | :--- | :--- |
| **是否要先单独写函数** | 通常要 | 不一定 |
| **是否能直接写在调用现场** | 不方便 | 很方便 |
| **是否能捕获外部局部变量** | 不能 | 可以 |
| **适合临时逻辑** | 一般 | 非常适合 |

### 核心优势

Lambda 真正强的地方不是“少写几个字”，  
而是：

> **它可以把小逻辑和使用地点放在一起，还能顺便拿到周围上下文。**

