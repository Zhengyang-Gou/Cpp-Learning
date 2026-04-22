# 55 - C++ 宏 (Macros)

## 1. 基本概念 (Basic Concepts)

宏（Macro）是通过 **预处理器（Preprocessor）** 工作的。  
它不是 C++ 语言层面的函数、类或者模板，而是在**正式编译之前**，先对源码做一次**文本替换**。

也就是说：

> **宏的本质不是“执行代码”，而是“改写代码文本”。**

例如：

```cpp
#define WAIT std::cin.get()
```

当预处理器看到 `WAIT` 时，会直接把它替换成：

```cpp
std::cin.get()
```

然后编译器才去编译替换后的代码。

---

## 2. 预处理阶段 (Preprocessing Stage)

在 C++ 编译流程里，带 `#` 的指令属于**预处理指令**，例如：

- `#define`
- `#include`
- `#if`
- `#ifdef`
- `#ifndef`
- `#endif`

这些内容会在真正编译开始前先处理。  
所以宏比模板更早生效，因为模板属于后面的 C++ 编译阶段，而宏属于更早的预处理阶段。

### 关键理解

- **宏是预处理器处理的**
- **模板是编译器处理的**
- 宏更像“搜索替换”
- 模板更像“类型级别的代码生成”

---

## 3. 宏的本质：文本替换 (Text Substitution)

### 示例代码

```cpp
#include <iostream>

#define WAIT std::cin.get()

int main()
{
    std::cout << "Press Enter..." << std::endl;
    WAIT;
    return 0;
}
```

### 预处理后大致会变成

```cpp
#include <iostream>

int main()
{
    std::cout << "Press Enter..." << std::endl;
    std::cin.get();
    return 0;
}
```

### 说明

这里没有什么“调用 WAIT 函数”的过程。  
`WAIT` 根本不是函数，它只是一个被替换掉的标记。

这也是 Cherno 这集反复想强调的点：

> **编译器根本不知道你原来写过 `WAIT`，它看到的已经是替换后的代码。**

---

## 4. 对象宏与函数宏 (Object-like vs Function-like Macros)

宏常见有两种形式。

| 类型 | 写法 | 本质 |
| :--- | :--- | :--- |
| **对象宏** | `#define NAME value` | 用一段文本替换一个名字 |
| **函数宏** | `#define LOG(x) ...` | 带参数的文本替换 |

---

### 4.1 对象宏 (Object-like Macro)

```cpp
#define WAIT std::cin.get()
```

这里的 `WAIT` 就是对象宏。

---

### 4.2 函数宏 (Function-like Macro)

```cpp
#define LOG(x) std::cout << x << std::endl;
```

使用时：

```cpp
LOG("Hello");
```

预处理后大致变成：

```cpp
std::cout << "Hello" << std::endl;
```

### 示例代码

```cpp
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

int main()
{
    LOG("Hello");
    LOG(10);
    return 0;
}
```

---

## 5. 宏不是函数 (Macros Are Not Functions)

这集非常容易考到或者问到的一个点就是：

> **宏看起来像函数，但它不是函数。**

### 宏和函数的区别

| 特性 | 宏 (Macro) | 函数 (Function) |
| :--- | :--- | :--- |
| **处理阶段** | 预处理阶段 | 编译后的程序执行阶段 |
| **本质** | 文本替换 | 真正的函数调用 |
| **类型检查** | 没有类型检查 | 有类型检查 |
| **作用域规则** | 很弱，接近纯替换 | 遵守 C++ 作用域规则 |
| **调试友好性** | 一般较差 | 更好 |
| **可读性** | 容易变差 | 通常更清晰 |

### 核心区别

宏只是把代码展开，  
它没有正常函数那种语义约束。

所以：

- 宏不安全
- 宏不够直观
- 宏容易制造奇怪错误

---

## 6. 条件编译 (Conditional Compilation)

Cherno 这集里一个很典型的实际用途，就是：

> **根据 Debug / Release 配置决定某段代码要不要存在。**

这不是运行时 `if`，  
而是在**编译之前**，直接决定某些代码是否送进编译器。

### 示例代码

```cpp
#include <iostream>

#define PR_DEBUG 1

#if PR_DEBUG == 1
    #define LOG(x) std::cout << x << std::endl;
#else
    #define LOG(x)
#endif

int main()
{
    LOG("Hello Debug");
    return 0;
}
```

### 说明

如果 `PR_DEBUG == 1`，那么：

```cpp
LOG("Hello Debug");
```

会展开为：

```cpp
std::cout << "Hello Debug" << std::endl;
```

如果不是，则可能展开成空内容：

```cpp
;
```

这样就实现了：

- Debug 版本输出日志
- Release 版本去掉日志

### 为什么这类用法合理

因为日志系统、断言系统、性能分析系统，  
往往需要根据编译配置决定是否保留代码。

这正是宏比较常见、也比较合理的场景。

---

## 7. `#ifdef`、`#if`、`defined` 的区别

### `#ifdef`

表示：

> **如果某个宏被定义过，就编译这部分代码。**

```cpp
#ifdef PR_DEBUG
    #define LOG(x) std::cout << x << std::endl;
#else
    #define LOG(x)
#endif
```

---

### `#if`

表示：

> **如果某个条件表达式成立，就编译这部分代码。**

```cpp
#define PR_DEBUG 1

#if PR_DEBUG == 1
    #define LOG(x) std::cout << x << std::endl;
#else
    #define LOG(x)
#endif
```

---

### `defined(...)`

用于在条件表达式里判断宏是否存在。

```cpp
#if defined(PR_DEBUG)
    #define LOG(x) std::cout << x << std::endl;
#elif defined(PR_RELEASE)
    #define LOG(x)
#endif
```

### 实战理解

强调“**让条件更清楚**”。  
很多时候直接定义：

```cpp
#define PR_DEBUG 0
```

然后用：

```cpp
#if PR_DEBUG == 1
```

会比“定义了没有”更直观。

---

## 8. 多行宏 (Multi-line Macros)

宏可以写成多行，但每一行结尾需要用反斜杠 `\` 连接。

### 示例代码

```cpp
#include <iostream>

#define MAIN int main() \
{ \
    std::cout << "Hello" << std::endl; \
    return 0; \
}

MAIN
```

### 说明

这里的 `\` 表示：

- 当前宏定义还没有结束
- 下一行仍然是宏的一部分

### 注意事项

- `\` 后面不要乱加空格
- 多行宏可读性通常比较差
- 只有在确实需要时才使用

---

## 9. 预定义宏 (Predefined Macros)

C++ 里还有一些编译器/标准预定义的宏，比较常见的是：

- `__FILE__`
- `__LINE__`
- `__DATE__`
- `__TIME__`

它们可以帮助你在编译时拿到当前文件名、行号、日期和时间。

### 示例代码

```cpp
#include <iostream>

#define LOG(x) std::cout << x << " | " << __FILE__ << ":" << __LINE__ << std::endl;

int main()
{
    LOG("Something happened");
    return 0;
}
```

### 可能输出类似

```cpp
Something happened | main.cpp:7
```

### 用途

这类宏经常用于：

- 日志系统
- 调试输出
- 错误定位
- 内存追踪

Cherno 后面讲内存分配跟踪时，也会延伸到这类思路：  
通过宏把“代码位置”自动带进去。

---

## 10. 宏的优点 (Advantages)

### 1）能在编译前改写代码

适合做条件编译、平台切换、开关控制。

### 2）适合某些全局性配置

例如：

- Debug / Release
- 日志开关
- 平台差异
- 编译期常量替换

### 3）在特定基础设施代码里很有用

例如：

- 引擎日志系统
- 断言系统
- Profiling 工具
- 资源追踪

---

## 11. 宏的缺点 (Disadvantages)

### 1）没有类型检查

宏只是文本替换，  
不会像函数或模板那样做正常的类型检查。

### 2）调试困难

展开后代码可能和你写的源代码看起来不一样，  
定位问题更麻烦。

### 3）容易破坏可读性

一旦宏用得太多，代码会变得很难理解。

### 4）容易产生副作用和优先级问题

例如这种宏就很危险：

```cpp
#define SQUARE(x) x * x
```

如果你写：

```cpp
SQUARE(1 + 2)
```

展开后其实是：

```cpp
1 + 2 * 1 + 2
```

这不是你想要的结果。

更安全一点的写法通常会加括号：

```cpp
#define SQUARE(x) ((x) * (x))
```

但即使这样，宏依然不如真正的函数或模板安全。

---

## 12. 为什么现代 C++ 不鼓励滥用宏

> **宏能用，但不要滥用。**

很多初学者一学宏，就喜欢把任何东西都写成宏：

- 把普通代码包装成奇怪名字
- 用宏代替函数
- 用宏代替常量
- 用宏制造“看起来很酷”的 DSL

这通常都不是好习惯。

在现代 C++ 里，很多宏过去承担的工作，  
现在更适合用下面这些方式替代：

- `constexpr`
- `const`
- `inline`
- 模板
- 普通函数
- `enum class`

---

## 13. 什么时候适合用宏？

### 比较合适的情况

- 条件编译
- 日志系统
- 断言系统
- 平台适配
- 编译配置切换
- 自动带入 `__FILE__` / `__LINE__`

### 不适合的情况

- 只是想少写几个字
- 代替普通函数
- 代替模板
- 代替常量定义
- 让代码“看起来更高级”

---

## 14. 核心思想

### 重点 1：宏是预处理器做的文本替换

它不是函数，不是模板，也不是运行时机制。

---

### 重点 2：编译器看到的是展开后的代码

你写的宏名字，最终不会以那种形式进入真正的编译阶段。

---

### 重点 3：条件编译是宏最实用的场景之一

例如：

- Debug 日志
- Release 去日志
- 不同平台切换代码路径

---

### 重点 4：宏可以很强，但滥用会让代码变糟

宏越多，代码越容易失控。  
能不用就不用，必须用时再用。

---

## 15. 实战建议 

### 1）先问自己：这个东西能不能用普通 C++ 语法解决？

很多时候答案是：

- 能，用函数
- 能，用模板
- 能，用 `constexpr`
- 能，用 `const`

如果能，就先别用宏。

---

### 2）宏优先用于“编译前开关”

例如：

```cpp
#define PR_DEBUG 1
```

配合：

```cpp
#if PR_DEBUG == 1
```

这种模式通常是合理的。

---

### 3）函数宏要非常小心

尤其是带参数的宏：

- 注意括号
- 注意副作用
- 注意可读性

---

### 4）日志和断言系统是宏的经典场景

例如：

```cpp
#define LOG(x) std::cout << x << " @ " << __FILE__ << ":" << __LINE__ << std::endl;
```

这种写法在工程里很常见，也比较有实际价值。
