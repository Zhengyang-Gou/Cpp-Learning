# 67 - std::move 和移动赋值运算符 (std::move and the Move Assignment Operator in C++)

## 1. 基本概念 

```cpp
b = std::move(a);
```

这是在一个已经存在的对象上，接收另一个对象的资源。

区别是：

```cpp
String b = std::move(a); // move constructor

b = std::move(a);        // move assignment operator
```

---

## 2. 为什么需要它 

假设一个 `String` class 内部管理一块 heap memory：

```cpp
char* m_Data;
```

如果我们写：

```cpp
String a = "Hello";
String b = "World";

b = a;
```

这通常会调用 copy assignment，把 `a` 的内容复制给 `b`。

但如果写：

```cpp
b = std::move(a);
```

我们希望直接把 `a` 的资源转移给 `b`，避免重新分配和复制。

不过这里有一个问题：

> `b` 原来已经拥有资源，所以在接收新资源之前，必须先释放旧资源。

这就是 move assignment operator 和 move constructor 的重要区别。

---

## 3. 核心语法

### 3.1 Move constructor

```cpp
String(String&& other) noexcept;
```

用于创建新对象：

```cpp
String b = std::move(a);
```

---

### 3.2 Move assignment operator

```cpp
String& operator=(String&& other) noexcept;
```

用于已有对象赋值：

```cpp
b = std::move(a);
```

---

### 3.3 `std::move`

```cpp
std::move(a)
```

`std::move` 本身不移动数据。  
它只是把 `a` 转换成 rvalue，让 compiler 可以选择 move constructor 或 move assignment operator。

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
        MoveFrom(other);

        std::cout << "Moved\n";
    }

    String& operator=(String&& other) noexcept
    {
        std::cout << "Move assigned\n";

        if (this != &other)
        {
            delete[] m_Data;
            MoveFrom(other);
        }

        return *this;
    }

    ~String()
    {
        delete[] m_Data;
    }

    void Print() const
    {
        if (m_Data)
            std::cout << m_Data << std::endl;
        else
            std::cout << "Empty\n";
    }

private:
    void MoveFrom(String& other)
    {
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        other.m_Size = 0;
        other.m_Data = nullptr;
    }

private:
    char* m_Data = nullptr;
    unsigned int m_Size = 0;
};

int main()
{
    String a = "Hello";
    String b = "World";

    b = std::move(a);

    b.Print();
    a.Print();

    return 0;
}
```

---

## 5. 重点解释

### 5.1 move constructor 用于初始化

```cpp
String b = std::move(a);
```

这里 `b` 是新创建的对象。

所以会调用：

```cpp
String(String&& other)
```

因为 `b` 之前没有资源，不需要先释放旧资源。

---

### 5.2 move assignment 用于已有对象

```cpp
b = std::move(a);
```

这里 `b` 已经存在，并且可能已经拥有一块 heap memory。

所以 move assignment 必须先释放旧资源：

```cpp
delete[] m_Data;
```

然后再接收 `a` 的资源：

```cpp
m_Data = other.m_Data;
m_Size = other.m_Size;
```

---

### 5.3 移动后要清空原对象

移动完成后必须写：

```cpp
other.m_Data = nullptr;
other.m_Size = 0;
```

否则 `a` 和 `b` 会指向同一块 memory。

当两个对象析构时，就会发生 double delete。

---

### 5.4 `this != &other` 防止自我赋值

```cpp
if (this != &other)
```

这是为了防止这种情况：

```cpp
a = std::move(a);
```

虽然这种代码不常见，但 move assignment 最好仍然处理。

如果不检查，可能会先释放自己的资源，然后再从自己身上移动，导致错误。

---

### 5.5 返回 `*this`

```cpp
return *this;
```

赋值运算符通常返回当前对象的 reference。

这样可以支持链式赋值：

```cpp
a = b = c;
```

虽然 move assignment 中不一定常用，但这是 operator= 的标准写法。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 避免已有对象赋值时的昂贵 copy | 需要正确释放旧资源 |
| 减少 heap allocation | 写错容易 memory leak 或 double delete |
| 适合管理资源的 class | 被 move 的对象不能继续依赖原值 |
| 提升临时对象赋值效率 | 需要理解 `std::move` 的真实作用 |

---

## 7. 核心思想

### 重点 1：初始化和赋值不是一回事

```cpp
String b = std::move(a);
```

这是初始化，调用 move constructor。

```cpp
b = std::move(a);
```

这是赋值，调用 move assignment operator。

---

### 重点 2：move assignment 要先处理旧资源

因为目标对象已经存在，所以它可能已经拥有资源。

移动新资源之前，必须先释放旧资源，否则会 memory leak。

---

### 重点 3：`std::move` 只是允许移动

```cpp
std::move(a)
```

不会移动任何数据。

它只是把 `a` 转成 rvalue，让 compiler 可以调用：

```cpp
operator=(String&& other)
```

真正的移动逻辑在 move assignment operator 里。

---

### 重点 4：移动后原对象要保持安全状态

被移动的对象仍然会析构。

所以它必须处于安全状态，例如：

```cpp
m_Data = nullptr;
m_Size = 0;
```

---

## 8. 常见错误

### 错误 1：move assignment 中忘记释放旧资源

错误：

```cpp
m_Data = other.m_Data;
```

这样会丢失原来的 `m_Data`，造成 memory leak。

正确：

```cpp
delete[] m_Data;
m_Data = other.m_Data;
```

---

### 错误 2：移动后忘记清空原对象

错误：

```cpp
m_Data = other.m_Data;
```

正确：

```cpp
m_Data = other.m_Data;
other.m_Data = nullptr;
```

否则两个对象会释放同一块 memory。

---

### 错误 3：以为 `std::move` 会自动完成移动

`std::move` 只是 cast。

如果 class 没有 move constructor 或 move assignment，compiler 可能会退回 copy，或者生成不符合预期的默认行为。