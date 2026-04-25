# 35 - C++ 模板 (Templates)

## 1. 基本概念 (Basic Concepts)

模板（Template）的本质就是：

> **让你写一份通用代码，然后由编译器根据你使用的类型，自动生成对应版本的代码。**

如果没有模板，我们经常会遇到这样的情况：

- 想写一个打印函数
- `int` 需要写一个版本
- `float` 需要写一个版本
- `std::string` 又要写一个版本

这样会产生很多重复代码。

模板的作用就是把“类型”变成参数，让代码可以复用。

---

## 2. 为什么需要模板 

假设我们要写一个打印函数。

如果不用模板，可能会写成这样：

```cpp
void Print(int value)
{
    std::cout << value << std::endl;
}

void Print(std::string value)
{
    std::cout << value << std::endl;
}

void Print(float value)
{
    std::cout << value << std::endl;
}
```

这些函数的逻辑几乎完全一样，  
唯一变化的只是参数类型。

这时候就会想到：

> **能不能把“类型”也抽象出来？**

这就是模板要解决的问题。

---

## 3. 函数模板 

函数模板允许我们为“多种类型”写同一套函数逻辑。

### 示例代码

```cpp
#include <iostream>
#include <string>

template<typename T>
void Print(T value)
{
    std::cout << value << std::endl;
}

int main()
{
    Print(5);
    Print(5.5f);
    Print("Hello");
    Print(std::string("Cherno"));

    return 0;
}
```

### 说明

```cpp
template<typename T>
```

这一行的意思是：

- 这里定义了一个模板
- `T` 是一个**类型占位符**
- 真正调用时，编译器会把 `T` 替换成实际类型

例如：

```cpp
Print(5);
```

编译器会推导出：

```cpp
Print<int>(5);
```

而：

```cpp
Print(5.5f);
```

会推导出：

```cpp
Print<float>(5.5f);
```

---

## 4. 模板并不是“万能函数”

模板不是说运行时动态判断类型，  
而是：

> **编译器在编译阶段，为不同类型生成不同版本的函数。**

也就是说，上面的模板并不是只存在一份代码在运行时处理所有类型。  
而是编译器会根据实际使用情况，生成类似这样的函数：

```cpp
void Print(int value) { ... }
void Print(float value) { ... }
void Print(const char* value) { ... }
```

所以模板更像是：

> **代码生成的蓝图（Blueprint）**

而不是传统意义上的“一个接受任意类型的运行时函数”。

---

## 5. `typename` 和 `class`

在模板参数列表里，下面两种写法通常是等价的：

```cpp
template<typename T>
```

```cpp
template<class T>
```

例如：

```cpp
template<class T>
void Print(T value)
{
    std::cout << value << std::endl;
}
```

和前面的 `typename T` 效果一样。

### 说明

- 在模板参数这里，`typename` 和 `class` 基本等价
- 现代 C++ 代码中，很多人更喜欢 `typename`
- 因为它更明确地表达了“这里是一个类型”

---

## 6. 显式指定模板类型 

大多数时候，编译器可以自动推导模板参数类型。

例如：

```cpp
Print(10);
```

编译器会自动推导 `T` 为 `int`。

但你也可以手动显式指定：

```cpp
Print<int>(10);
Print<float>(10.5f);
```

### 示例代码

```cpp
#include <iostream>

template<typename T>
void Print(T value)
{
    std::cout << value << std::endl;
}

int main()
{
    Print<int>(5);
    Print<float>(5.5f);

    return 0;
}
```

### 说明

- `Print(5)` 是自动推导
- `Print<int>(5)` 是显式指定
- 一般自动推导就够用了
- 只有在推导不明确或者你想强制指定类型时，才手动写出来

---

## 7. 类模板

模板不仅能用于函数，也能用于类。

类模板的意义是：

> **让一个类可以适配多种数据类型，而不用重复写多个版本。**

### 示例代码

```cpp
#include <iostream>
#include <string>

template<typename T>
class Printer
{
private:
    T m_Value;

public:
    Printer(T value)
        : m_Value(value)
    {
    }

    void Print()
    {
        std::cout << m_Value << std::endl;
    }
};

int main()
{
    Printer<int> intPrinter(5);
    intPrinter.Print();

    Printer<std::string> stringPrinter("Hello");
    stringPrinter.Print();

    return 0;
}
```

### 说明

这里：

```cpp
template<typename T>
class Printer
```

表示 `Printer` 是一个类模板。

当你写：

```cpp
Printer<int> intPrinter(5);
```

时，编译器会生成一个“存储 `int` 的 Printer 类”。

当你写：

```cpp
Printer<std::string> stringPrinter("Hello");
```

时，编译器会生成一个“存储 `std::string` 的 Printer 类”。

---

## 8. 为什么类模板很有用

如果不用类模板，你可能要写：

```cpp
class IntPrinter { ... };
class FloatPrinter { ... };
class StringPrinter { ... };
```

这些类的结构和逻辑可能几乎一样，  
差别只是成员变量类型不同。

类模板可以把这种重复消掉，  
让代码更通用、更易维护。

---

## 9. 模板的本质：泛型编程

模板是 C++ 泛型编程的核心工具之一。

它的思想不是：

> “针对某一种类型写代码”

而是：

> “针对某一类行为写代码，只要某个类型满足要求，就能使用这段代码”

例如前面的 `Print(T value)`，  
只要这个 `T` 能被 `std::cout` 输出，这个模板就能工作。

这也是模板强大的地方：

- 可复用
- 高性能
- 类型安全
- 编译期展开

---

## 10. 模板的优点 (Advantages)

### 1）减少重复代码

同样逻辑不需要为不同类型重复写很多遍。

### 2）提高可复用性

一份模板代码可以服务多种类型。

### 3）保留类型安全

和 `void*` 这种做法不同，模板是在编译期进行类型检查的。

### 4）通常没有额外运行时开销

因为模板在编译期就会被展开成具体代码，  
不需要运行时去判断类型。

---

## 11. 模板的缺点 (Disadvantages)

模板虽然强大，但也有代价。

### 1）报错信息可能很复杂

尤其当模板嵌套很多层时，编译错误会非常长。

### 2）代码可读性可能下降

过度使用模板，会让代码难以理解。

### 3）可能导致代码膨胀

如果同一个模板被很多不同类型实例化，  
编译器会生成多个版本的代码。

---

## 12. 核心思想

### 重点 1：模板就是让“类型”参数化

普通函数参数化的是“值”，  
模板参数化的是“类型”。

---

### 重点 2：模板是编译期机制，不是运行时机制

模板不是运行时再决定用什么类型，  
而是在编译时就生成对应代码。

---

### 重点 3：函数模板和类模板都很重要

- 函数模板：解决“同一逻辑，不同参数类型”
- 类模板：解决“同一类结构，不同成员类型”

---

### 重点 4：模板的目标是代码复用，而不是炫技

模板最核心的价值，是减少重复代码，  
让程序更通用、更整洁。

---

## 13. 实战建议

### 什么时候适合用模板？

当你发现：

- 代码逻辑完全一样
- 只是类型在变化

这时候就应该考虑模板。

例如：

- 打印函数
- 数学工具函数
- 容器类
- 通用包装类

---

### 什么时候不要滥用模板？

如果：

- 逻辑本身并不通用
- 只是看起来有一点相似
- 使用模板后代码反而更难读

那就不要为了“高级”而强行模板化。

