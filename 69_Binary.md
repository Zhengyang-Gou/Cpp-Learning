# 69 - 二进制和位运算符 (Binary and Bitwise Operators in C++)

## 1. 基本概念

计算机底层用二进制存储数据，也就是只使用：

```text
0 和 1
```

一个二进制位叫做 **bit**。  
8 个 bit 通常组成 1 个 **byte**。

例如十进制数字 `5`，用二进制表示是：

```text
00000101
```

C++ 中的 bitwise operators 可以直接操作整数的每一个 bit。

常见 bitwise operators：

| 运算符 | 名称 | 作用 |
| :--- | :--- | :--- |
| `&` | bitwise AND | 两个位都为 1，结果才为 1 |
| `|` | bitwise OR | 有一个位为 1，结果就是 1 |
| `^` | bitwise XOR | 两个位不同，结果为 1 |
| `~` | bitwise NOT | 每个位取反 |
| `<<` | left shift | bit 向左移动 |
| `>>` | right shift | bit 向右移动 |

---

## 2. 为什么需要它 

普通代码通常按“数字”理解数据：

```cpp
int value = 5;
```

但在底层，`5` 是一串 bit。

如果我们直接操作 bit，就可以做很多高效的事情：

- 保存多个开关状态
- 设置或清除某个 flag
- 做权限系统
- 做图形、网络、压缩、加密相关操作
- 用更少内存表达更多状态

例如一个角色可以有多个状态：

```text
isVisible
isAlive
isMoving
isAttacking
```

不用创建多个 `bool`，也可以用一个整数的不同 bit 表示这些状态。

---

## 3. 核心语法

### 3.1 二进制字面量

C++14 以后可以写二进制字面量：

```cpp
int value = 0b00000101;
```

这就是十进制的 `5`。

---

### 3.2 左移 `<<`

```cpp
int value = 1 << 3;
```

含义是把 `1` 的 bit 向左移动 3 位。

结果：

```text
00000001 -> 00001000
```

也就是十进制 `8`。

左移一位通常相当于乘以 2：

```cpp
1 << 1 // 2
1 << 2 // 4
1 << 3 // 8
```

---

### 3.3 右移 `>>`

```cpp
int value = 8 >> 1;
```

结果是：

```text
00001000 -> 00000100
```

也就是十进制 `4`。

右移一位通常相当于除以 2。

---

### 3.4 用 bit 表示 flag

```cpp
const int Visible = 1 << 0;
const int Alive   = 1 << 1;
const int Moving  = 1 << 2;
```

它们分别表示不同 bit：

```text
Visible = 00000001
Alive   = 00000010
Moving  = 00000100
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <bitset>

int main()
{
    const unsigned int Visible = 1 << 0;
    const unsigned int Alive   = 1 << 1;
    const unsigned int Moving  = 1 << 2;

    unsigned int flags = 0;

    flags |= Visible;
    flags |= Alive;

    std::cout << "flags: " << std::bitset<8>(flags) << std::endl;

    if (flags & Visible)
    {
        std::cout << "Entity is visible" << std::endl;
    }

    if (flags & Alive)
    {
        std::cout << "Entity is alive" << std::endl;
    }

    if (!(flags & Moving))
    {
        std::cout << "Entity is not moving" << std::endl;
    }

    flags &= ~Visible;

    std::cout << "after removing Visible: "
              << std::bitset<8>(flags)
              << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `1 << 0`、`1 << 1`、`1 << 2`

```cpp
const unsigned int Visible = 1 << 0;
const unsigned int Alive   = 1 << 1;
const unsigned int Moving  = 1 << 2;
```

这三行代码创建了三个不同的 flag。

它们的二进制形式分别是：

```text
Visible = 00000001
Alive   = 00000010
Moving  = 00000100
```

每个 flag 只占用一个 bit。

---

### 5.2 `flags |= Visible` 设置某个 bit

```cpp
flags |= Visible;
```

等价于：

```cpp
flags = flags | Visible;
```

它会把 `Visible` 对应的 bit 设置为 `1`。

如果原来其他 bit 已经是 `1`，它们不会被清掉。

---

### 5.3 `flags & Visible` 检查某个 bit

```cpp
if (flags & Visible)
```

这里使用 bitwise AND。

如果 `Visible` 对应的 bit 是 `1`，结果就不是 `0`，条件成立。

这是一种常见的 flag 检查方式。

---

### 5.4 `flags &= ~Visible` 清除某个 bit

```cpp
flags &= ~Visible;
```

含义是：

1. `~Visible` 把 `Visible` 的 bit 取反
2. 再和 `flags` 做 AND
3. 最终把 `Visible` 对应 bit 清成 `0`

这是清除 flag 的经典写法。

---

### 5.5 `std::bitset` 只是为了显示

```cpp
std::bitset<8>(flags)
```

这不是必须的，只是方便把整数按二进制形式打印出来。

实际 bitwise operation 操作的仍然是整数。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 非常高效 | 初学者不直观 |
| 节省内存 | 可读性可能下降 |
| 适合 flag 和底层数据处理 | 写错 bit 位置容易出 bug |
| 接近硬件表示方式 | 需要理解二进制 |

---

## 7. 核心思想

### 重点 1：整数本质上是一串 bit

```cpp
int value = 5;
```

从数学角度看是数字 `5`。  
从计算机角度看，是一串二进制位。

理解这一点，才能理解 bitwise operators。

---

### 重点 2：bitwise operators 操作的是每一位

普通加减乘除操作的是数值。

bitwise operators 操作的是 bit：

```cpp
&
|
^
~
<<
>>
```

它们不会把数字当作整体处理，而是逐位处理。

---

### 重点 3：flag 是 bitwise operation 的常见用途

多个状态可以放进一个整数里：

```cpp
flags |= Visible;
flags |= Alive;
```

检查状态：

```cpp
if (flags & Alive)
```

清除状态：

```cpp
flags &= ~Alive;
```

这在引擎、系统编程、图形 API 中很常见。

---

### 重点 4：位运算很快，但不要为了炫技滥用

bitwise operation 很底层、很快，但不代表所有代码都应该用它。

如果普通 `bool` 更清楚，就用 `bool`。

只有在需要 flag、压缩状态、底层控制或性能敏感时，bitwise operators 才特别有价值。

---

## 8. 常见错误

### 错误 1：把 `&` 和 `&&` 混淆

```cpp
a & b   // bitwise AND
a && b  // logical AND
```

`&` 操作 bit。  
`&&` 操作 true / false 逻辑条件。

---

### 错误 2：把 `|` 和 `||` 混淆

```cpp
a | b   // bitwise OR
a || b  // logical OR
```

它们不是一回事。

---

### 错误 3：清除 flag 时写错

错误：

```cpp
flags &= Visible;
```

这会只保留 `Visible`，其他 bit 都可能被清掉。

清除某个 flag 应该写：

```cpp
flags &= ~Visible;
```