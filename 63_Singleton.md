# 63 - C++ 中的 Singleton (SINGLETONS in C++)

## 1. 基本概念

Singleton 的意思是：  
一个 `class` 在整个程序中只允许存在一个实例，并且提供一个全局访问点。

常见形式：

```cpp
class Singleton
{
public:
    static Singleton& Get()
    {
        static Singleton instance;
        return instance;
    }

private:
    Singleton() {}
};
```

这里的重点是：

- 构造函数是 `private`
- 外部不能随便创建对象
- 只能通过 `Get()` 获取唯一实例
- `static` 局部变量保证实例只创建一次

---

## 2. 为什么需要它

有些系统天然只需要一个实例，例如：

- 日志系统 `Logger`
- 配置系统 `Config`
- 随机数生成器 `Random`
- 渲染器 `Renderer`
- 资源管理器 `AssetManager`

如果这些对象到处都被创建多个实例，可能会导致状态混乱。

例如：

```cpp
Random random1;
Random random2;
```

如果每个 `Random` 都有自己的状态，那么程序里不同地方生成随机数时可能不一致。

Singleton 的目的就是：

> 把“全局唯一对象”这个概念封装进 `class` 里。

---

## 3. 核心语法 

### 3.1 基本 Singleton

```cpp
class Singleton
{
public:
    static Singleton& Get()
    {
        static Singleton instance;
        return instance;
    }

private:
    Singleton() {}
};
```

---

### 3.2 禁止复制

更完整的写法通常会禁止 copy：

```cpp
Singleton(const Singleton&) = delete;
Singleton& operator=(const Singleton&) = delete;
```

否则用户可能通过复制创建出新的对象。

---

### 3.3 通过实例调用成员函数

```cpp
Singleton::Get().DoSomething();
```

---

### 3.4 提供 static 包装函数

有时为了写得更简洁，也可以这样：

```cpp
class Random
{
public:
    static float Float()
    {
        return Get().InternalFloat();
    }

private:
    static Random& Get()
    {
        static Random instance;
        return instance;
    }

    float InternalFloat()
    {
        return 0.5f;
    }
};
```

调用时：

```cpp
float value = Random::Float();
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <random>

class Random
{
public:
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    static Random& Get()
    {
        static Random instance;
        return instance;
    }

    static float Float()
    {
        return Get().FloatInternal();
    }

private:
    Random()
        : m_RandomEngine(std::random_device{}()),
          m_Distribution(0.0f, 1.0f)
    {
    }

    float FloatInternal()
    {
        return m_Distribution(m_RandomEngine);
    }

private:
    std::mt19937 m_RandomEngine;
    std::uniform_real_distribution<float> m_Distribution;
};

int main()
{
    float value1 = Random::Float();
    float value2 = Random::Float();

    std::cout << value1 << std::endl;
    std::cout << value2 << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `static Random& Get()`

```cpp
static Random& Get()
{
    static Random instance;
    return instance;
}
```

这是 Singleton 的核心。

`instance` 是一个局部 `static` 变量，它只会被创建一次。

以后每次调用：

```cpp
Random::Get()
```

都会返回同一个对象。

---

### 5.2 构造函数是 `private`

```cpp
private:
    Random()
```

这样外部不能写：

```cpp
Random random;
```

也就是说，用户不能自己创建新的 `Random` 实例。

对象的创建权被控制在 `class` 内部。

---

### 5.3 删除 copy constructor 和 copy assignment

```cpp
Random(const Random&) = delete;
Random& operator=(const Random&) = delete;
```

这样可以防止：

```cpp
Random copy = Random::Get();
```

否则 Singleton 可能被复制，破坏“唯一实例”的规则。

---

### 5.4 static 包装函数让调用更简单

示例中使用了：

```cpp
static float Float()
{
    return Get().FloatInternal();
}
```

这样外部可以直接写：

```cpp
Random::Float();
```

而不用写：

```cpp
Random::Get().FloatInternal();
```

这是 The Cherno 常用的写法：  
外部看起来像调用普通 `static` 函数，内部仍然使用 Singleton 实例保存状态。

---

### 5.5 Singleton 本质上接近全局变量

Singleton 虽然写在 `class` 里，但它仍然具有全局访问的特点：

```cpp
Random::Float();
```

程序任何地方都能访问它。

所以 Singleton 不是万能设计。  
它解决了“唯一实例”的问题，但也可能带来全局状态的问题。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 保证对象只有一个实例 | 本质上是一种 global state |
| 使用方便，访问简单 | 可能让代码依赖关系不清晰 |
| 适合管理全局系统 | 单元测试不方便 |
| 延迟初始化，第一次使用时创建 | 容易被滥用 |
| 可以封装状态和行为 | 多线程共享状态时仍需小心 |

---

## 7. 核心思想

### 重点 1：Singleton 控制实例数量

Singleton 的目标不是让函数变成 `static`，而是控制一个 `class` 只能创建一个对象。

核心写法是：

```cpp
static Singleton instance;
```

---

### 重点 2：构造函数必须隐藏

如果构造函数是 `public`，外部就可以随便创建对象：

```cpp
Singleton a;
Singleton b;
```

这就不是 Singleton 了。

所以构造函数通常要放在 `private` 中。

---

### 重点 3：注意 global state 的问题

Singleton 很方便，但也容易让程序结构变差。

如果很多代码都直接访问：

```cpp
Renderer::Get()
AssetManager::Get()
Logger::Get()
```

那么代码之间的依赖关系会变得不明显。

使用 Singleton 前要先想清楚：

> 这个对象真的应该全局唯一吗？

