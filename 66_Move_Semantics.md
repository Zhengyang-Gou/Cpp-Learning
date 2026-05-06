# 66 - C++ 中的移动语义 (Move Semantics in C++)

## 1. 基本概念 

在 C++ 中，对象传递数据时，传统方式是 **copy**。  
例如：

```cpp
std::string a = "Hello";
std::string b = a;
```

这会复制字符串内容。

但如果对象内部管理的是 heap memory，比如字符串、数组、buffer，复制可能很贵。  
**move semantics** 的作用是：不复制资源，而是把资源的所有权从一个对象转移到另一个对象。

简单理解：

```text
copy：复制一份资源
move：把已有资源转移过去
```

---

## 2. 为什么需要它

假设一个 `String` class 内部有一块 heap memory：

```cpp
char* m_Data;
```

如果 copy 一个对象，就需要：

1. 重新分配内存
2. 复制原来的字符
3. 两个对象各自管理一份数据

但有些对象是临时对象：

```cpp
String name = CreateString();
```

`CreateString()` 返回的临时对象马上就不用了。  
这时如果还复制它的内容，就很浪费。

更好的做法是：

```text
直接拿走临时对象内部的指针
```

这就是 move semantics 的意义。

---

## 3. 核心语法 

### 3.1 Move constructor

```cpp
String(String&& other);
```

`String&&` 是 **rvalue reference**，通常用来接收临时对象。

---

### 3.2 Move assignment operator

```cpp
String& operator=(String&& other);
```

用于已有对象接收另一个对象的资源。

---

### 3.3 `std::move`

```cpp
String b = std::move(a);
```

注意：

```cpp
std::move
```

本身不移动任何数据。  
它只是把 `a` 转成 rvalue，让 compiler 可以选择 move constructor。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <cstring>
#include <utility>

class String
{
public:
    String() = default;

    String(const char* text)
    {
        m_Size = std::strlen(text);
        m_Data = new char[m_Size + 1];
        std::memcpy(m_Data, text, m_Size + 1);

        std::cout << "Created\n";
    }

    String(const String& other)
    {
        m_Size = other.m_Size;
        m_Data = new char[m_Size + 1];
        std::memcpy(m_Data, other.m_Data, m_Size + 1);

        std::cout << "Copied\n";
    }

    String(String&& other) noexcept
    {
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        other.m_Size = 0;
        other.m_Data = nullptr;

        std::cout << "Moved\n";
    }

    ~String()
    {
        delete[] m_Data;
    }

    void Print() const
    {
        if (m_Data)
            std::cout << m_Data << std::endl;
    }

private:
    char* m_Data = nullptr;
    unsigned int m_Size = 0;
};

int main()
{
    String a = "Hello";

    String b = std::move(a);

    b.Print();

    return 0;
}
```

---

## 5. 重点解释

### 5.1 Copy constructor 会复制资源

```cpp
String(const String& other)
```

这里会重新申请一块 heap memory：

```cpp
m_Data = new char[m_Size + 1];
```

然后把原来的字符复制过去。

这很安全，但成本较高。

---

### 5.2 Move constructor 会转移资源

```cpp
String(String&& other) noexcept
```

这里没有重新分配内存，而是直接拿走原对象的 pointer：

```cpp
m_Data = other.m_Data;
```

然后把原对象清空：

```cpp
other.m_Data = nullptr;
other.m_Size = 0;
```

这样可以避免两个对象同时释放同一块内存。

---

### 5.3 `std::move` 只是转换

```cpp
String b = std::move(a);
```

这句话不会直接移动数据。  
它只是告诉 compiler：

```text
可以把 a 当成 rvalue 使用
```

真正执行移动的是：

```cpp
String(String&& other)
```

---

### 5.4 被 move 后的对象仍然存在

执行：

```cpp
String b = std::move(a);
```

之后，`a` 仍然是一个有效对象。

但它的原始资源已经被转移走了。  
所以不要再依赖 `a` 原来的内容。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 避免昂贵的 copy | 需要正确处理原对象状态 |
| 减少 heap allocation | 写错可能 double delete |
| 提高临时对象传递效率 | 初学者容易误解 `std::move` |
| 对资源管理类很重要 | 被 move 后对象不能继续当原值使用 |

---

## 7. 核心思想

### 重点 1：move 是资源转移

move semantics 的本质不是复制数据，而是转移资源所有权。

```text
旧对象失去资源
新对象获得资源
```

---

### 重点 2：移动后必须让原对象安全

移动 pointer 后要写：

```cpp
other.m_Data = nullptr;
```

否则两个对象会指向同一块内存，析构时可能发生 double free。

---

### 重点 3：`std::move` 不是真的移动

```cpp
std::move(a)
```

只是一个类型转换。

它让 `a` 可以匹配：

```cpp
String(String&& other)
```

真正的移动逻辑需要你在 move constructor 里写。

---

### 重点 4：move semantics 是性能优化基础

对于管理 heap memory、文件句柄、socket、GPU buffer 等资源的 class，move semantics 非常重要。

它可以让对象传递更便宜。