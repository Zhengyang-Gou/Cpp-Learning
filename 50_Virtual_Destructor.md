# 68 - C++ 中的虚析构函数 (Virtual Destructors in C++)

## 1. 基本概念 

**destructor** 是对象被销毁时自动调用的函数，主要负责清理资源，例如：

- 释放 heap 内存；
- 关闭文件；
- 释放句柄；
- 断开连接；
- 清理动态分配的对象。

在 C++ 中，如果一个 class 被设计成 **base class**，并且你可能通过 base class pointer 删除 derived object，那么 base class 的 destructor 通常应该声明为 `virtual`。

例如：

```cpp
class Base
{
public:
    virtual ~Base()
    {
    }
};
```

核心问题是：

```cpp
Base* object = new Derived();
delete object;
```

如果 `Base` 的 destructor 不是 `virtual`，那么通过 `Base*` 删除 `Derived` 对象时，`Derived` 的 destructor 可能不会被正确调用。

如果 `Base` 的 destructor 是 `virtual`，C++ 会先调用 `Derived` 的 destructor，再调用 `Base` 的 destructor。

---

## 2. 为什么需要它 

在面向对象代码中，我们经常会用 base class pointer 指向 derived class 对象：

```cpp
Base* entity = new Player();
```

这样做的目的通常是利用 polymorphism：

```cpp
entity->Update();
```

如果 `Update()` 是 `virtual function`，实际调用的是 `Player::Update()`。

但对象销毁时也有类似问题：

```cpp
delete entity;
```

此时 `entity` 的静态类型是 `Base*`，但它实际指向的是 `Player` 对象。

如果 `Base` 的 destructor 不是 `virtual`，编译器可能只调用：

```cpp
Base::~Base()
```

而不调用：

```cpp
Player::~Player()
```

这会导致 `Player` 自己管理的资源没有被释放。

所以 virtual destructor 解决的问题是：

> 当通过 base class pointer 删除 derived object 时，确保 derived class 的 destructor 也会被正确调用。

---

## 3. 核心语法 
### 3.1 普通 destructor

```cpp
class Base
{
public:
    ~Base()
    {
    }
};
```

这是非 virtual destructor。

如果没有 inheritance 和 polymorphism，这样写完全可以。

---

### 3.2 virtual destructor

```cpp
class Base
{
public:
    virtual ~Base()
    {
    }
};
```

这表示 destructor 可以通过 virtual dispatch 正确调用 derived class 的 destructor。

---

### 3.3 derived class destructor

```cpp
class Derived : public Base
{
public:
    ~Derived()
    {
    }
};
```

如果 base class 的 destructor 是 `virtual`，derived class 的 destructor 也会自动具有 virtual 行为。

现代 C++ 中也可以显式写：

```cpp
~Derived() override
{
}
```

不过 `override` 用在 destructor 上不是必须的，但可以表达这是在覆盖 base class 的 virtual destructor。

---

### 3.4 virtual destructor 的调用顺序

如果执行：

```cpp
Base* object = new Derived();
delete object;
```

并且 `Base` 的 destructor 是 `virtual`，调用顺序是：

```text
Derived destructor
Base destructor
```

销毁顺序总是从最 derived 的部分开始，再逐层销毁 base 部分。

---

## 4. 示例代码

```cpp
#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base constructor\n";
    }

    virtual ~Base()
    {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        m_Array = new int[5];
        std::cout << "Derived constructor\n";
    }

    ~Derived() override
    {
        delete[] m_Array;
        std::cout << "Derived destructor\n";
    }

private:
    int* m_Array = nullptr;
};

int main()
{
    Base* object = new Derived();

    delete object;

    return 0;
}
```

输出通常是：

```text
Base constructor
Derived constructor
Derived destructor
Base destructor
```

---

## 5. 重点解释

### 5.1 `Base* object = new Derived();`

```cpp
Base* object = new Derived();
```

这行代码创建的是 `Derived` 对象，但用 `Base*` 保存它。

这是 polymorphism 中很常见的写法。

对象实际类型是：

```cpp
Derived
```

指针类型是：

```cpp
Base*
```

问题出现在销毁时：

```cpp
delete object;
```

编译器必须知道应该调用哪个 destructor。

---

### 5.2 没有 virtual destructor 会怎样

如果 `Base` 写成这样：

```cpp
class Base
{
public:
    ~Base()
    {
        std::cout << "Base destructor\n";
    }
};
```

然后执行：

```cpp
Base* object = new Derived();
delete object;
```

可能只调用：

```text
Base destructor
```

`Derived` 的 destructor 没有被调用。

如果 `Derived` 中有资源：

```cpp
m_Array = new int[5];
```

那么：

```cpp
delete[] m_Array;
```

就不会执行，造成资源泄漏。

严格来说，通过没有 virtual destructor 的 base pointer 删除 derived object，属于 undefined behavior，不只是“少调用一个 destructor”这么简单。

---

### 5.3 virtual destructor 如何解决问题

当 `Base` 的 destructor 是 virtual：

```cpp
virtual ~Base()
{
}
```

执行：

```cpp
delete object;
```

时，C++ 会根据对象的真实类型调用正确的 destructor。

对于：

```cpp
Base* object = new Derived();
```

真实对象是 `Derived`，所以会先调用：

```cpp
Derived::~Derived()
```

然后自动调用：

```cpp
Base::~Base()
```

这保证 derived class 自己的资源可以被正确清理。

---

### 5.4 destructor 的调用顺序

构造时顺序是：

```text
Base constructor
Derived constructor
```

销毁时顺序相反：

```text
Derived destructor
Base destructor
```

原因很直接：derived object 是建立在 base object 之上的。

销毁时要先清理 derived 部分，再清理 base 部分。

---

### 5.5 virtual destructor 和普通 virtual function 的关系

如果一个 class 已经有 virtual function，例如：

```cpp
class Entity
{
public:
    virtual void Update()
    {
    }
};
```

这通常说明它会被当作 polymorphic base class 使用。

这时 destructor 也应该写成 virtual：

```cpp
virtual ~Entity()
{
}
```

常见经验规则：

> 如果一个 class 里有 virtual function，通常也应该有 virtual destructor。

---

## 6. 常见用法

### 6.1 polymorphic base class

只要一个 class 是为了被继承并通过 base pointer 使用，就应该考虑 virtual destructor。

```cpp
class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update() = 0;
};
```

例如：

```cpp
Entity* entity = new Player();
delete entity;
```

这样 `Player` 的 destructor 才能正确调用。

---

### 6.2 interface class

C++ 中常用纯 virtual function 模拟 interface：

```cpp
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Draw() = 0;
};
```

interface 通常会被这样使用：

```cpp
IRenderer* renderer = CreateRenderer();
delete renderer;
```

所以 interface 的 destructor 应该是 virtual。

---

### 6.3 base class 管理 derived object

例如一个容器保存 base pointer：

```cpp
std::vector<Entity*> entities;
```

里面可能实际放的是：

```cpp
Player
Enemy
NPC
```

清理时：

```cpp
for (Entity* entity : entities)
{
    delete entity;
}
```

如果 `Entity` 没有 virtual destructor，derived class 的资源就可能无法正确释放。

---

### 6.4 配合 smart pointer 使用

即使用 `std::unique_ptr`，virtual destructor 仍然重要。

```cpp
std::unique_ptr<Entity> entity = std::make_unique<Player>();
```

当 `entity` 被销毁时，它内部仍然会通过 `Entity*` 删除对象。

所以 `Entity` 的 destructor 依然应该是 virtual：

```cpp
class Entity
{
public:
    virtual ~Entity() = default;
};
```

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 能通过 base pointer 正确销毁 derived object | 会引入 virtual dispatch |
| 避免 derived class 资源泄漏 | class 通常会有 vtable 相关开销 |
| 是 polymorphic base class 的常规写法 | 不需要 polymorphism 的 class 没必要使用 |
| 代码行为更安全、更符合预期 | 滥用 virtual 会让设计变复杂 |

virtual destructor 的开销通常很小。如果 class 本来就有 virtual function，那么基本已经有 vtable 了，再加 virtual destructor 通常不是主要成本。

---

## 8. 和其他相关概念的区别

### 8.1 virtual destructor vs 普通 destructor

普通 destructor：

```cpp
~Base()
{
}
```

适合不作为 polymorphic base class 的普通 class。

virtual destructor：

```cpp
virtual ~Base()
{
}
```

适合会被继承，并且可能通过 base pointer 删除 derived object 的 base class。

---

### 8.2 constructor vs destructor

constructor 不能是 virtual。

```cpp
virtual Base(); // 错误
```

因为构造对象时，derived 部分还没有完全建立，virtual dispatch 没有意义。

destructor 可以是 virtual，因为销毁时对象已经存在，需要根据真实类型决定从哪个 destructor 开始销毁。

---

### 8.3 `delete Derived*` vs `delete Base*`

如果指针类型是 `Derived*`：

```cpp
Derived* object = new Derived();
delete object;
```

会正常调用 `Derived` destructor 和 `Base` destructor。

问题主要出现在：

```cpp
Base* object = new Derived();
delete object;
```

这里必须依赖 base class 的 virtual destructor 才能安全销毁。

---

### 8.4 virtual destructor vs pure virtual destructor

destructor 也可以是 pure virtual：

```cpp
class Base
{
public:
    virtual ~Base() = 0;
};

Base::~Base()
{
}
```

注意：pure virtual destructor 仍然必须提供定义。

原因是 derived object 销毁时，base destructor 一定会被调用。

这种写法可以让 class 变成 abstract class，但实际项目中更常见的是：

```cpp
virtual ~Base() = default;
```

---

## 9. 核心思想

### 重点 1：通过 base pointer 删除对象时，base destructor 要 virtual

关键代码是：

```cpp
Base* object = new Derived();
delete object;
```

只要你的设计允许这种写法，`Base` 就应该有 virtual destructor。

---

### 重点 2：destructor 负责资源清理

如果 derived class 中有：

```cpp
new
malloc
file handle
socket
GPU resource
```

那么 derived destructor 必须被调用。

否则资源可能泄漏。

---

### 重点 3：有 virtual function 的 base class 通常也要有 virtual destructor

如果一个 class 已经这样写：

```cpp
virtual void Update();
```

它大概率是用于 polymorphism。

那就应该补上：

```cpp
virtual ~Base() = default;
```

这是非常重要的 C++ 习惯。

---

### 重点 4：virtual destructor 不是给所有 class 都加

如果一个 class 不打算被继承，或者不会通过 base pointer 删除 derived object，就不需要 virtual destructor。

不要机械地给所有 class 都加 `virtual`。