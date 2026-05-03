# 55 - C++ 中的动态类型转换 (Dynamic Casting in C++)

## 1. 基本概念 

**dynamic casting** 指的是使用 `dynamic_cast` 在 inheritance 体系中进行运行时类型转换。

它最常见的用途是：把一个 base class pointer 或 reference 安全地转换成 derived class pointer 或 reference。

例如：

```cpp
Entity* entity = new Player();

Player* player = dynamic_cast<Player*>(entity);
```

这里 `entity` 的静态类型是 `Entity*`，但它实际指向的是 `Player` 对象。`dynamic_cast` 会在运行时检查这个对象到底是不是 `Player`。

如果转换成功，返回有效 pointer。

如果转换失败，返回：

```cpp
nullptr
```

所以 `dynamic_cast` 的核心作用是：

```text
在运行时安全检查对象的真实类型。
```

---

## 2. 为什么需要它

在 C++ 的 polymorphism 中，我们经常用 base class pointer 指向不同的 derived class 对象。

例如游戏里可能有：

```cpp
Entity
Player
Enemy
NPC
```

然后统一放进一个容器：

```cpp
std::vector<Entity*> entities;
```

这样可以统一调用：

```cpp
entity->Update();
```

但有时我们需要判断某个 `Entity*` 实际是不是 `Player*`。

例如：

```cpp
Entity* entity = GetEntity();

Player* player = dynamic_cast<Player*>(entity);

if (player)
{
    player->Jump();
}
```

如果不用 `dynamic_cast`，直接强转：

```cpp
Player* player = (Player*)entity;
```

或者：

```cpp
Player* player = static_cast<Player*>(entity);
```

编译器可能允许，但如果 `entity` 实际不是 `Player`，后续访问就很危险。

`dynamic_cast` 解决的问题就是：**转换前检查真实类型，避免把对象当成错误的 derived type 使用。**

---

## 3. 核心语法

### 3.1 pointer 的 dynamic_cast

```cpp
Derived* derived = dynamic_cast<Derived*>(base);
```

如果转换成功，返回有效 pointer。

如果失败，返回：

```cpp
nullptr
```

所以使用后要检查：

```cpp
if (derived)
{
    // 转换成功
}
```

---

### 3.2 reference 的 dynamic_cast

```cpp
Derived& derived = dynamic_cast<Derived&>(base);
```

reference 不能是 `nullptr`，所以如果转换失败，会抛出异常：

```cpp
std::bad_cast
```

因此日常使用中，pointer 形式更常见。

---

### 3.3 使用 dynamic_cast 的前提

`dynamic_cast` 要求 base class 是 polymorphic type。

也就是说，base class 至少要有一个 `virtual function`：

```cpp
class Entity
{
public:
    virtual ~Entity() = default;
};
```

通常 base class 会有 virtual destructor，或者其他 virtual function：

```cpp
virtual void Update()
{
}
```

如果没有 virtual function，`dynamic_cast` 无法进行运行时类型识别。

---

### 3.4 downcast

从 base class 转成 derived class，叫 **downcast**：

```cpp
Entity* entity = new Player();
Player* player = dynamic_cast<Player*>(entity);
```

这是 `dynamic_cast` 最常见的使用场景。

---

### 3.5 upcast

从 derived class 转成 base class，叫 **upcast**：

```cpp
Player* player = new Player();
Entity* entity = dynamic_cast<Entity*>(player);
```

但 upcast 通常不需要 `dynamic_cast`，因为它是安全且隐式支持的：

```cpp
Entity* entity = player;
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <vector>

class Entity
{
public:
    virtual ~Entity() = default;
};

class Player : public Entity
{
public:
    void Jump()
    {
        std::cout << "Player jumps\n";
    }
};

class Enemy : public Entity
{
public:
    void Attack()
    {
        std::cout << "Enemy attacks\n";
    }
};

int main()
{
    std::vector<Entity*> entities;

    entities.push_back(new Player());
    entities.push_back(new Enemy());
    entities.push_back(new Player());

    for (Entity* entity : entities)
    {
        Player* player = dynamic_cast<Player*>(entity);

        if (player)
        {
            player->Jump();
        }
        else
        {
            std::cout << "This entity is not a Player\n";
        }
    }

    for (Entity* entity : entities)
    {
        delete entity;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `Entity` 必须有 virtual function

```cpp
class Entity
{
public:
    virtual ~Entity() = default;
};
```

这里的 virtual destructor 不只是为了正确释放 derived object，也让 `Entity` 成为 polymorphic type。

`dynamic_cast` 需要运行时类型信息，也就是 RTTI，才能判断对象真实类型。

---

### 5.2 `Entity*` 可以指向不同 derived object

```cpp
entities.push_back(new Player());
entities.push_back(new Enemy());
entities.push_back(new Player());
```

容器里存的是：

```cpp
Entity*
```

但实际对象可能是：

```cpp
Player
Enemy
Player
```

这就是 polymorphism 的常见使用方式。

---

### 5.3 `dynamic_cast<Player*>(entity)`

```cpp
Player* player = dynamic_cast<Player*>(entity);
```

这行代码会检查：

```text
entity 实际指向的对象是不是 Player？
```

如果是，`player` 有效。

如果不是，`player` 是：

```cpp
nullptr
```

---

### 5.4 为什么要判断 `if (player)`

```cpp
if (player)
{
    player->Jump();
}
```

`dynamic_cast` 失败时会返回 `nullptr`。

如果不检查就调用：

```cpp
player->Jump();
```

当 `player == nullptr` 时，程序会出错。

所以 pointer 形式的 `dynamic_cast` 后面通常要立刻检查。

---

### 5.5 `delete entity` 为什么安全

```cpp
for (Entity* entity : entities)
{
    delete entity;
}
```

因为 `Entity` 有 virtual destructor：

```cpp
virtual ~Entity() = default;
```

所以通过 `Entity*` 删除 `Player` 或 `Enemy` 时，可以正确调用 derived destructor。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 可以在运行时检查真实类型 | 有运行时开销 |
| 转换失败时 pointer 返回 `nullptr` | 需要开启 RTTI |
| 比强制 cast 更安全 | 频繁使用可能说明设计有问题 |
| 适合不确定对象类型的 downcast | 依赖 inheritance 和 virtual function |
| 调试和工具代码中很方便 | 不适合替代良好的 polymorphic 设计 |

