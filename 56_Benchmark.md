# 56 - C++ 中的基准测试：如何测量性能 (BENCHMARKING in C++ (how to measure performance))

## 1. 基本概念

**benchmarking** 是指用可重复、可对比的方式测量代码性能。

它和简单的 timing 不完全一样：

- timing 更像是“这段代码运行了多久”；
- benchmarking 更强调“在相同条件下比较不同写法的性能”。

例如你想知道：

```cpp
std::vector<int> values;
```

在没有 `reserve()` 和使用 `reserve()` 的情况下，哪个更快、差多少，这就是 benchmark 的典型场景。

C++ 中常见的 benchmark 做法是：

1. 准备测试数据；
2. 记录开始时间；
3. 执行被测代码；
4. 记录结束时间；
5. 输出耗时；
6. 多次运行并比较结果。

核心工具仍然是：

```cpp
#include <chrono>
```

但 benchmark 的重点不只是“会计时”，而是要避免测出错误结论。

---

## 2. 为什么需要它 

性能优化不能靠猜。

例如下面两种写法：

```cpp
std::vector<int> values;

for (int i = 0; i < 100000; i++)
{
    values.push_back(i);
}
```

和：

```cpp
std::vector<int> values;
values.reserve(100000);

for (int i = 0; i < 100000; i++)
{
    values.push_back(i);
}
```

哪一个更快？

我们大概知道第二种可能更快，因为 `reserve()` 可以减少重新分配内存。但到底快多少，必须测量。

benchmark 的价值是：

- 验证优化是否真的有效；
- 找出真正慢的代码；
- 比较不同实现方式；
- 避免“看起来更快”的错觉；
- 用数据指导代码选择。

没有 benchmark 的优化，经常只是主观判断。

---

## 3. 核心语法 

### 3.1 使用 `<chrono>` 测量耗时

```cpp
auto start = std::chrono::high_resolution_clock::now();

// code to benchmark

auto end = std::chrono::high_resolution_clock::now();

std::chrono::duration<double, std::milli> duration = end - start;
std::cout << duration.count() << " ms\n";
```

这里使用 milliseconds 作为输出单位。

---

### 3.2 使用 RAII 封装 Timer

```cpp
class Timer
{
public:
    Timer()
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - m_Start;

        std::cout << duration.count() << " ms\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
```

这样在作用域开始时创建 `Timer`，作用域结束时自动输出耗时。

---

### 3.3 用作用域控制 benchmark 范围

```cpp
{
    Timer timer;

    // only this code is measured
}
```

这可以精确控制要测量的代码范围。

---

### 3.4 多次运行

单次 benchmark 结果可能不稳定。更好的做法是多次运行：

```cpp
for (int i = 0; i < 10; i++)
{
    RunBenchmark();
}
```

然后观察平均值、最小值或大致趋势。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <vector>
#include <chrono>

class Timer
{
public:
    Timer(const char* name)
        : m_Name(name)
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - m_Start;

        std::cout << m_Name << ": " << duration.count() << " ms\n";
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

int main()
{
    const int count = 1000000;

    {
        Timer timer("Without reserve");

        std::vector<int> values;

        for (int i = 0; i < count; i++)
        {
            values.push_back(i);
        }
    }

    {
        Timer timer("With reserve");

        std::vector<int> values;
        values.reserve(count);

        for (int i = 0; i < count; i++)
        {
            values.push_back(i);
        }
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 两个作用域分别测量两段代码

```cpp
{
    Timer timer("Without reserve");

    std::vector<int> values;

    for (int i = 0; i < count; i++)
    {
        values.push_back(i);
    }
}
```

这个作用域测量没有 `reserve()` 的情况。

```cpp
{
    Timer timer("With reserve");

    std::vector<int> values;
    values.reserve(count);

    for (int i = 0; i < count; i++)
    {
        values.push_back(i);
    }
}
```

这个作用域测量使用 `reserve()` 的情况。

作用域结束时，`Timer` 自动析构并输出耗时。

---

### 5.2 为什么 `reserve()` 通常更快

`std::vector` 底层是一块连续内存。

如果不断 `push_back()`，当容量不够时，`vector` 需要：

1. 分配一块更大的内存；
2. 把旧元素移动或复制过去；
3. 释放旧内存；
4. 再插入新元素。

这会带来额外成本。

提前调用：

```cpp
values.reserve(count);
```

可以一次性预留足够容量，减少中途扩容。

---

### 5.3 benchmark 要测你真正关心的代码

如果你想比较 `push_back()` 的成本，就不要把无关操作放进测试范围。

例如不要在循环里大量输出：

```cpp
std::cout << i << std::endl;
```

因为这样测到的主要是 console output，而不是容器操作。

---

### 5.4 Debug 模式结果不可靠

Debug build 通常没有开启优化，而且可能带有额外检查。

性能测试应该尽量在 Release build 下进行。

否则你测到的可能是 Debug 开销，而不是代码真实性能。

---

### 5.5 编译器可能优化掉无用代码

如果 benchmark 的结果没有被使用，编译器可能认为这段代码没有意义，然后优化掉。

例如：

```cpp
int value = 0;

for (int i = 0; i < 1000000; i++)
{
    value += i;
}
```

如果 `value` 后面完全没被使用，Release 模式下编译器可能优化这段代码。

所以 benchmark 中要确保被测代码确实产生了可观察结果。