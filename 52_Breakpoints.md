# 52 - C++ 中的条件断点和动作断点 (Conditional and Action Breakpoints in C++)

## 1. 基本概念

**breakpoint** 是调试器中的断点。程序运行到断点所在的代码行时，会暂停执行，让我们检查当前状态，例如：

- 变量的值；
- call stack；
- memory；
- object 状态；
- 当前代码执行路径。

普通 breakpoint 每次执行到这一行都会暂停。

而这一集讲的重点是两种更高级的 breakpoint：

1. **conditional breakpoint**
   - 只有满足某个条件时才暂停。
2. **action breakpoint**
   - 到达断点时执行某个动作，例如打印日志，而不一定暂停程序。

它们不是 C++ 语言本身的语法，而是 debugger / IDE 提供的功能，例如 Visual Studio、CLion、Xcode、gdb、lldb 等。

---

## 2. 为什么需要它

普通 breakpoint 很有用，但在循环、频繁调用函数或大量对象更新时会很麻烦。

例如：

```cpp
for (int i = 0; i < 1000; i++)
{
    Process(i);
}
```

如果你只关心：

```cpp
i == 500
```

普通 breakpoint 会从 `i == 0` 开始每次都停，非常低效。

这时可以使用 conditional breakpoint：

```text
i == 500
```

程序只有在 `i` 等于 500 时才会暂停。

再比如，你只是想知道某个变量运行时的值，不一定想暂停程序。以前可能会写：

```cpp
std::cout << value << std::endl;
```

但这样需要修改代码、重新编译，还可能污染代码。使用 action breakpoint 可以直接让 debugger 输出信息，而不修改源代码。

---

## 3. 核心语法 

内容主要是调试器功能，不是 C++ 语法。

### 3.1 普通 breakpoint

在 IDE 中点击代码行左侧，设置断点：

```cpp
Process(i);
```

程序运行到这一行时暂停。

---

### 3.2 conditional breakpoint

给断点添加条件，例如：

```text
i == 500
```

或者：

```text
value > 100
```

或者：

```text
entity != nullptr
```

只有条件为 `true` 时，程序才会暂停。

---

### 3.3 hit count breakpoint

有些 debugger 支持 **hit count**，也就是断点命中指定次数后才暂停。

例如：

```text
Break when hit count == 100
```

适合调试循环或频繁调用的函数。

---

### 3.4 action breakpoint / tracepoint

动作断点可以在命中时输出信息，例如：

```text
i = {i}, value = {value}
```

通常可以选择：

```text
Continue execution
```

这样程序不会暂停，只是在 output/debug console 中打印信息。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <vector>

void ProcessValue(int index, int value)
{
    int result = value * 2;

    if (result > 1000)
    {
        std::cout << "Large result\n";
    }
}

int main()
{
    std::vector<int> values;

    for (int i = 0; i < 1000; i++)
    {
        values.push_back(i);
    }

    for (int i = 0; i < values.size(); i++)
    {
        ProcessValue(i, values[i]);
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 普通断点的问题

如果在这里设置普通 breakpoint：

```cpp
ProcessValue(i, values[i]);
```

程序会从 `i == 0` 开始暂停。

但如果我们真正关心的是：

```cpp
i == 500
```

那么前 500 次暂停都是浪费时间。

---

### 5.2 使用 conditional breakpoint

可以在这一行设置断点：

```cpp
ProcessValue(i, values[i]);
```

然后给断点添加条件：

```text
i == 500
```

这样程序只有在第 500 次循环时才会暂停。

暂停后可以检查：

```cpp
i
values[i]
```

也可以进入 `ProcessValue()`，观察函数内部逻辑。

---

### 5.3 使用条件检查异常状态

如果怀疑某个值过大：

```cpp
int result = value * 2;
```

可以在这一行设置条件：

```text
value > 500
```

或：

```text
result > 1000
```

这样可以直接定位异常数据出现的时刻。

---

### 5.4 使用 action breakpoint 输出信息

如果不想暂停程序，只想观察运行过程，可以设置 action breakpoint。

例如在这行：

```cpp
ProcessValue(i, values[i]);
```

设置输出信息：

```text
i = {i}, value = {values[i]}
```

并勾选：

```text
Continue execution
```

这样 debugger 会在输出窗口打印信息，但程序继续运行。

这相当于临时插入了一个 debug log，但不需要修改 C++ 源码。

---

### 5.5 action breakpoint 和 `std::cout` 的区别

`std::cout` 需要改代码：

```cpp
std::cout << i << std::endl;
```

改完通常还要重新编译。

action breakpoint 不需要修改源文件，只在调试器中配置。

适合临时观察变量，调完以后直接删除断点即可。

---

## 6. 常见用法

### 6.1 调试循环中的某一次迭代

例如只想在 `i == 750` 时暂停：

```text
i == 750
```

适合调试数组、vector、批量数据处理。

---

### 6.2 调试某个特定对象

例如只想在某个 `entity` 的 ID 为 42 时暂停：

```text
entity->GetID() == 42
```

注意：条件表达式太复杂时可能影响调试速度，也可能因为 pointer 无效导致调试器求值失败。

---

### 6.3 检查 pointer 是否为空

```text
ptr == nullptr
```

或：

```text
ptr != nullptr
```

适合定位 null pointer 相关问题。

---

### 6.4 临时打印变量

使用 action breakpoint 打印：

```text
x = {x}, y = {y}
```

适合观察坐标、状态值、循环变量、函数参数。

---

### 6.5 替代临时 debug log

以前可能会临时写：

```cpp
std::cout << "Health: " << health << std::endl;
```

使用 action breakpoint 可以避免污染代码。

---

## 7. 优点和缺点

| 功能 | 优点 | 缺点 |
| :--- | :--- | :--- |
| 普通 breakpoint | 简单直接 | 高频代码中会反复暂停 |
| conditional breakpoint | 精准定位特定状态 | 条件复杂时可能较慢 |
| hit count breakpoint | 适合循环和重复调用 | 需要知道大概命中次数 |
| action breakpoint | 不改代码即可输出信息 | 依赖 debugger 功能 |
| continue execution | 可观察程序而不打断流程 | 输出过多会影响性能和可读性 |

---

## 8. 和其他相关概念的区别

### 8.1 breakpoint vs `std::cout`

`std::cout` 是代码的一部分：

```cpp
std::cout << value << std::endl;
```

breakpoint 是调试器功能，不属于最终程序逻辑。

临时调试时，优先考虑 breakpoint；需要长期记录时，应该使用 logging system。

---

### 8.2 conditional breakpoint vs `if`

条件断点不需要改源码。

```text
i == 500
```

等价于调试器帮你做了一次临时判断。

而 `if` 是程序逻辑的一部分：

```cpp
if (i == 500)
{
    // debug
}
```

调试结束后，如果这个 `if` 没有业务意义，就应该删除。

---

### 8.3 action breakpoint vs logging

action breakpoint 适合临时调试。

logging 适合长期保留，例如：

- 错误日志；
- 线上诊断；
- 性能记录；
- 用户行为追踪。

不要把 action breakpoint 当作正式日志系统。

---

### 8.4 breakpoint vs watch window

breakpoint 用来控制程序何时暂停。

watch window 用来查看表达式或变量值。

通常搭配使用：

1. breakpoint 暂停程序；
2. watch window 查看变量；
3. call stack 查看调用路径。

---

## 9. 核心思想

### 重点 1：断点不只是“暂停程序”

breakpoint 还可以带条件、带动作、统计命中次数。

这能大幅提高调试效率。

---

### 重点 2：conditional breakpoint 用来定位特定状态

如果 bug 只在某个值、某次循环、某个对象上出现，不要一遍遍手动继续执行。

直接加条件：

```text
i == 500
```

---

### 重点 3：action breakpoint 可以少写临时代码

临时调试输出不一定要写进源文件。

用 action breakpoint 可以让 debugger 输出变量，同时保持代码干净。

---

### 重点 4：调试器是 C++ 开发的重要工具

很多初学者只会用 `std::cout` 调试。

但实际 C++ 开发中，debugger 能提供更多信息，包括：

- 当前变量；
- call stack；
- memory；
- object 内容；
- pointer 地址；
- 条件断点；
- 动作断点。