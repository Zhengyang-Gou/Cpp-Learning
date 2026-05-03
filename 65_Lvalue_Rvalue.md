# 65 - 左值和右值 (lvalues and rvalues in C++)

## 1. 基本概念 

这一集的核心主题是 **lvalue** 和 **rvalue**。

简单理解：

- **lvalue**：有明确内存位置、可以被再次访问的值
- **rvalue**：临时值，通常没有持久身份，表达式结束后就消失

例如：

```cpp
int x = 10;
```

这里：

```cpp
x
```

是 lvalue，因为它有名字、有地址，可以继续使用。

```cpp
10
```

是 rvalue，因为它只是一个临时字面量。

---

## 2. 为什么需要它 

C++ 需要区分 lvalue 和 rvalue，是因为它们的使用方式不同。

例如：

```cpp
int x = 10;
x = 20;
```

`x` 可以放在赋值号左边，因为它是 lvalue。

但下面这样不行：

```cpp
10 = x;
```

因为 `10` 是 rvalue，它不是一个可以被修改的对象。

更重要的是，lvalue / rvalue 是后面理解 **move semantics**、**rvalue reference**、性能优化的基础。

---

## 3. 核心语法 

### 3.1 lvalue reference

```cpp
int x = 10;
int& ref = x;
```

`int&` 只能绑定到 lvalue。

错误：

```cpp
int& ref = 10;
```

因为 `10` 是 rvalue。

---

### 3.2 const lvalue reference

```cpp
const int& ref = 10;
```

`const lvalue reference` 可以绑定到 rvalue。

这是 C++ 中非常常见的写法：

```cpp
void Print(const std::string& text);
```

它既能接收 lvalue，也能接收临时对象。

---

### 3.3 rvalue reference

```cpp
int&& ref = 10;
```

`T&&` 是 rvalue reference，主要用于绑定临时对象。

它是 move semantics 的基础。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <string>

void Print(int& value)
{
    std::cout << "lvalue reference: " << value << std::endl;
}

void Print(int&& value)
{
    std::cout << "rvalue reference: " << value << std::endl;
}

int GetValue()
{
    return 42;
}

int main()
{
    int x = 10;

    Print(x);
    Print(20);
    Print(GetValue());
    Print(x + 5);

    const int& constRef = 100;
    std::cout << "const reference: " << constRef << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `x` 是 lvalue

```cpp
int x = 10;
Print(x);
```

`x` 有名字，也有稳定的内存地址。

所以它会调用：

```cpp
void Print(int& value)
```

---

### 5.2 `20` 是 rvalue

```cpp
Print(20);
```

`20` 是临时值，没有名字，不能被普通 `int&` 绑定。

所以它会调用：

```cpp
void Print(int&& value)
```

---

### 5.3 函数返回值通常是 rvalue

```cpp
Print(GetValue());
```

`GetValue()` 返回的是一个临时 `int`。

它不是一个可以长期访问的变量，所以它是 rvalue。

---

### 5.4 表达式结果通常是 rvalue

```cpp
Print(x + 5);
```

`x + 5` 会产生一个临时计算结果。

这个结果是 rvalue。

---

### 5.5 `const int&` 可以绑定临时值

```cpp
const int& constRef = 100;
```

普通 `int&` 不能绑定到 `100`，但 `const int&` 可以。

这是因为 `const reference` 保证不会修改临时值。

C++ 还会延长这个临时值的生命周期，使它和 `constRef` 一样久。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 帮助理解对象生命周期 | 初学者容易混淆 |
| 是 move semantics 的基础 | `T&`、`const T&`、`T&&` 规则较多 |
| 能区分临时对象和持久对象 | 单看表达式有时不直观 |
| 有助于写高性能代码 | 需要结合 reference 一起理解 |

---

## 7. 核心思想

### 重点 1：有名字的变量通常是 lvalue

```cpp
int x = 10;
```

`x` 是 lvalue，因为它可以被取地址、被修改、被多次访问。

---

### 重点 2：临时值通常是 rvalue

```cpp
10
x + 5
GetValue()
```

这些通常都是 rvalue，因为它们只是临时结果。

---

### 重点 3：`T&` 绑定 lvalue，`T&&` 绑定 rvalue

```cpp
int& a = x;
int&& b = 10;
```

这是理解 reference 类型的关键。

---

### 重点 4：`const T&` 很特殊

```cpp
const int& value = 10;
```

`const lvalue reference` 可以绑定到 rvalue。

这也是为什么很多函数参数会写成：

```cpp
void Print(const std::string& text);
```

它既避免复制，又能接收临时对象。

---

### 重点 5：这是 move semantics 的前置知识

后面学习：

```cpp
std::move
move constructor
move assignment operator
```

都会依赖 lvalue 和 rvalue。

其中最重要的一点是：

> rvalue 通常代表“临时对象”，所以它的资源可以被移动，而不是复制。

