# 45 - C++ 中的计时 (Timing in C++)

## 1. 基本概念 

在 C++ 中，**timing** 指的是测量一段代码执行所花费的时间。

这在实际开发中非常重要，因为我们经常需要知道：

- 某个函数到底运行了多久；
- 某段代码是否足够快；
- 优化前后性能有没有变化；
- 程序中的瓶颈在哪里；
- 某个操作是否阻塞了 main thread；
- 游戏、图形程序或工具程序中的一帧耗时是多少。

核心是使用 C++ 标准库中的 `<chrono>` 来进行时间测量。

`<chrono>` 是 C++ 标准库提供的时间工具库，它可以用来表示：

- 时间点，例如当前时间；
- 时间间隔，例如两个时间点之间相差多少秒；
- 不同时间单位，例如 seconds、milliseconds、microseconds、nanoseconds。

最常见的计时方式是：

1. 在代码开始前记录一个 start time；
2. 在代码结束后记录一个 end time；
3. 用 `end - start` 得到 duration；
4. 把 duration 转换成想要的单位，例如 milliseconds。

---

## 2. 为什么需要它

写 C++ 时，很多时候我们不能只凭感觉判断代码快不快。

例如下面这两段代码：

```cpp
for (int i = 0; i < 1000000; i++)
{
    // do something
}
```

和：

```cpp
for (int i = 0; i < 1000000; i++)
{
    // do something else
}
```

哪一段更快？差多少？是否值得优化？

仅靠肉眼看代码很难判断。我们需要实际测量。

计时可以帮助我们回答这些问题：

- 一个算法是否真的比另一个算法快；
- 某个函数是不是性能瓶颈；
- 一次资源加载用了多久；
- 一帧渲染时间是否超过预算；
- 程序卡顿是不是由某段代码造成的。

例如在游戏开发中，如果目标是 60 FPS，那么一帧大约只有：

```text
1000 ms / 60 ≈ 16.67 ms
```

如果某个函数单独就花了 20 ms，那么它已经超过了一帧的时间预算，程序就可能掉帧。

所以 timing 的核心意义是：**用实际数据判断代码性能，而不是靠猜。**

---

## 3. 核心语法 

### 3.1 引入头文件

使用 C++ 标准库计时功能，需要包含：

```cpp
#include <chrono>
```

如果示例中需要暂停当前 thread，也会用到：

```cpp
#include <thread>
```

---

### 3.2 获取当前时间点

可以使用 `std::chrono::high_resolution_clock::now()` 获取当前时间点：

```cpp
auto start = std::chrono::high_resolution_clock::now();
```

它返回的是一个 time point，表示当前时刻。

常见 clock 有：

```cpp
std::chrono::high_resolution_clock
std::chrono::steady_clock
std::chrono::system_clock
```

含义大致如下：

| clock | 说明 |
| :--- | :--- |
| `system_clock` | 系统真实时间，可能受到系统时间调整影响 |
| `steady_clock` | 单调递增的时钟，适合测量时间间隔 |
| `high_resolution_clock` | 标准库提供的高精度时钟，具体实现依赖编译器和平台 |

对于性能计时，实际工程中通常更推荐 `std::chrono::steady_clock`，因为它不会因为系统时间被修改而倒退。

演示中常用 `high_resolution_clock`，因为它直观、精度高，适合讲解 timing 的基本用法。

---

### 3.3 计算时间差

记录两个时间点：

```cpp
auto start = std::chrono::high_resolution_clock::now();

// 要计时的代码

auto end = std::chrono::high_resolution_clock::now();
```

然后计算 duration：

```cpp
std::chrono::duration<float> duration = end - start;
```

这里的 `duration` 表示 `start` 和 `end` 之间的时间间隔。

默认情况下：

```cpp
duration.count()
```

通常可以得到以秒为单位的浮点数。

---

### 3.4 转换为 milliseconds

如果想把秒转换成 milliseconds，可以这样写：

```cpp
float ms = duration.count() * 1000.0f;
```

也可以使用 `std::chrono::duration` 的单位模板：

```cpp
std::chrono::duration<float, std::milli> ms = end - start;
```

然后：

```cpp
std::cout << ms.count() << " ms" << std::endl;
```

其中：

```cpp
std::milli
```

表示 milliseconds，也就是千分之一秒。

---

### 3.5 使用 `sleep_for()` 模拟耗时操作

为了演示计时，可以让当前 thread 暂停一段时间：

```cpp
std::this_thread::sleep_for(std::chrono::seconds(1));
```

也可以使用 chrono literals：

```cpp
using namespace std::literals::chrono_literals;

std::this_thread::sleep_for(1s);
```

这里的 `1s` 表示 1 秒。

常见 chrono literals 包括：

```cpp
1h    // 1 hour
1min  // 1 minute
1s    // 1 second
1ms   // 1 millisecond
1us   // 1 microsecond
1ns   // 1 nanosecond
```

---

## 4. 示例代码

```cpp
#include <iostream>
#include <chrono>
#include <thread>

class Timer
{
public:
    Timer()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(
            m_StartTimepoint
        ).time_since_epoch().count();

        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(
            endTimepoint
        ).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << "Duration: " << duration << " us ("
                  << ms << " ms)" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

void Function()
{
    Timer timer;

    for (int i = 0; i < 100; i++)
    {
        std::cout << "Hello\n";
    }
}

int main()
{
    Function();

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `Timer` class 的作用

```cpp
class Timer
{
public:
    Timer()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }
};
```

这个 `Timer` class 的作用是自动测量一段作用域内代码的执行时间。

创建 `Timer` 对象时，constructor 会记录开始时间：

```cpp
m_StartTimepoint = std::chrono::high_resolution_clock::now();
```

当 `Timer` 对象离开作用域时，destructor 会自动调用：

```cpp
~Timer()
{
    Stop();
}
```

这利用了 C++ 的 **RAII** 思想。

也就是说，我们不需要手动写：

```cpp
StartTimer();
DoSomething();
StopTimer();
```

只需要在作用域开头创建一个对象：

```cpp
Timer timer;
```

当作用域结束时，它会自动输出耗时。

---

### 5.2 为什么 destructor 里调用 `Stop()`

```cpp
~Timer()
{
    Stop();
}
```

这样做的好处是不会忘记结束计时。

例如：

```cpp
void Function()
{
    Timer timer;

    // 要计时的代码
}
```

当 `Function()` 结束时，`timer` 是局部变量，会自动析构。

析构时调用 `Stop()`，于是计时自动结束。

这是一种非常实用的 profiling 写法，尤其适合临时测量函数或代码块的耗时。

---

### 5.3 `time_point_cast`

```cpp
auto start = std::chrono::time_point_cast<std::chrono::microseconds>(
    m_StartTimepoint
).time_since_epoch().count();
```

这段代码的作用是把 time point 转换成 microseconds 级别的时间数值。

拆开理解：

```cpp
std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint)
```

表示把时间点转换成 microseconds 精度。

```cpp
.time_since_epoch()
```

表示这个时间点距离 epoch 的时间长度。

```cpp
.count()
```

表示取得这个时间长度的数值。

这里的 epoch 可以理解为这个 clock 的起点。我们不关心起点具体是什么，只关心两个时间点相减后的差值。

---

### 5.4 `duration` 的单位

```cpp
auto duration = end - start;
```

因为 `start` 和 `end` 都已经被转换成 microseconds 的数值，所以这里的 `duration` 单位就是 microseconds。

接着：

```cpp
double ms = duration * 0.001;
```

因为：

```text
1 millisecond = 1000 microseconds
```

所以 microseconds 转 milliseconds 需要乘以：

```text
0.001
```

---

### 5.5 为什么示例里打印 `Hello`

```cpp
for (int i = 0; i < 100; i++)
{
    std::cout << "Hello\n";
}
```

这段代码只是为了制造一个可以被测量的任务。

不过要注意，`std::cout` 本身是比较慢的操作，因为它涉及输出流和控制台 IO。计时结果可能更多反映的是输出成本，而不是普通 CPU 计算成本。

所以如果你想测量算法性能，不应该把大量 `std::cout` 放在被测量区域里。

例如下面这种计时结果通常不适合作为算法性能参考：

```cpp
for (int i = 0; i < 100000; i++)
{
    std::cout << i << std::endl;
}
```

因为这里主要慢在 console output。

---

### 5.6 更简单的计时写法

如果只是临时测量一小段代码，也可以不写 `Timer` class：

```cpp
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;

    std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
    std::cout << "Duration: " << duration.count() * 1000.0f << " ms" << std::endl;

    return 0;
}
```

这段代码会测量 `sleep_for(500ms)` 大约花了多久。

---

## 6. 常见用法

### 6.1 测量函数耗时

最常见的用法是测量某个函数执行多久：

```cpp
void LoadAssets()
{
    Timer timer;

    // 加载资源
}
```

这样可以知道资源加载是否太慢。

如果一个资源加载函数花了几秒，就需要考虑：

- 是否可以异步加载；
- 是否可以缓存；
- 是否可以减少文件 IO；
- 是否需要压缩或优化数据格式。

---

### 6.2 测量算法性能

例如比较两个排序、查找或数据处理方案：

```cpp
void TestAlgorithm()
{
    Timer timer;

    // 执行算法
}
```

通过 timing 可以判断优化是否真的有效。

不过要注意：一次测量可能不准确。真实性能测试通常需要多次运行，并取平均值、中位数或最小值。

---

### 6.3 测量一帧耗时

在游戏或图形程序中，可以测量每一帧花了多久：

```cpp
auto frameStart = std::chrono::high_resolution_clock::now();

// Update
// Render

auto frameEnd = std::chrono::high_resolution_clock::now();
std::chrono::duration<float, std::milli> frameTime = frameEnd - frameStart;

std::cout << "Frame time: " << frameTime.count() << " ms" << std::endl;
```

如果目标是 60 FPS，那么 frame time 最好低于 16.67 ms。

如果目标是 144 FPS，那么 frame time 最好低于约 6.94 ms。

---

### 6.4 简单 profiling

`Timer` class 可以作为一个简单 profiler。

例如：

```cpp
void Render()
{
    Timer timer;

    DrawMeshes();
    DrawUI();
    PresentFrame();
}
```

这样可以快速判断 `Render()` 总共花了多少时间。

还可以给 `Timer` 增加 name：

```cpp
Timer timer("Render");
```

输出：

```text
Render: 3.5 ms
```

这在调试性能问题时很有用。

---

### 6.5 测量作用域耗时

使用 RAII 的 `Timer` 特别适合测量一个作用域：

```cpp
{
    Timer timer;

    // 只测量这个 block 中的代码
}
```

当代码离开 `{}` 作用域时，`timer` 自动析构，计时自动结束。

这比手动调用 `Start()` 和 `Stop()` 更不容易出错。

---

### 6.6 模拟延迟

虽然 timing 的重点是测量时间，但 `<chrono>` 也常和 `std::this_thread::sleep_for()` 一起使用：

```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(100));
```

这可以用于：

- 模拟耗时任务；
- 降低循环频率；
- 简单测试异步行为；
- 临时让 thread 等待。

不过不要用 `sleep_for()` 当作精确同步工具。它不能保证绝对精确，只能保证当前 thread 至少等待大约这么久。

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 使用 `<chrono>` 是标准 C++ 写法，跨平台 | 不同平台和编译器的 clock 实现可能不同 |
| 可以精确测量代码耗时 | 单次测量可能受到系统调度影响 |
| `Timer` class 配合 RAII 使用非常方便 | 简单 Timer 只能做粗略 profiling |
| 可以帮助定位性能瓶颈 | Debug 模式下的结果通常不能代表 Release 性能 |
| 代码简单，适合日常开发中快速验证 | 被测代码太短时，测量误差可能很明显 |
| 可以测量函数、作用域、帧时间等 | `std::cout`、IO、系统调度会干扰结果 |

---

## 8. 和其他相关概念的区别

### 8.1 `high_resolution_clock` vs `steady_clock`

`high_resolution_clock` 通常表示当前实现中精度最高的 clock。

```cpp
std::chrono::high_resolution_clock::now();
```

它适合教学和简单计时。

`steady_clock` 是单调递增的 clock：

```cpp
std::chrono::steady_clock::now();
```

它的特点是不会因为系统时间被修改而倒退。

对于测量一段代码的耗时，工程中通常更推荐：

```cpp
std::chrono::steady_clock
```

因为我们关心的是时间间隔，而不是现实世界的日期和时间。

---

### 8.2 `system_clock` vs performance timing

`system_clock` 表示系统时间，也就是现实世界中的时间。

它适合做：

- 获取当前日期；
- 记录日志时间；
- 显示真实时间；
- 和日历时间相关的操作。

但它不适合做严肃的性能计时，因为系统时间可能被用户或操作系统调整。

性能计时更应该使用：

```cpp
std::chrono::steady_clock
```

或者在简单场景下使用：

```cpp
std::chrono::high_resolution_clock
```

---

### 8.3 profiling vs benchmarking

**profiling** 更偏向于找性能瓶颈。

例如：

- 哪个函数最慢；
- 哪个系统占用最多时间；
- 一帧中哪个阶段最耗时。

**benchmarking** 更偏向于严格比较性能。

例如：

- 算法 A 和算法 B 谁更快；
- 优化前后提升了多少；
- 某个函数在大量重复运行下平均耗时多少。

The Cherno 这一集展示的 `Timer` class 更接近简单 profiling。

如果要做严肃 benchmarking，需要考虑更多因素：

- 多次运行；
- Release 模式；
- 编译器优化；
- 预热；
- 避免 IO 干扰；
- 防止编译器把无用代码优化掉；
- 统计平均值、中位数、最小值等。

---

### 8.4 `Timer` class vs 手动计时

手动计时写法：

```cpp
auto start = std::chrono::high_resolution_clock::now();

// code

auto end = std::chrono::high_resolution_clock::now();
```

优点是直观。

缺点是代码重复，而且容易忘记结束计时。

`Timer` class 写法：

```cpp
{
    Timer timer;

    // code
}
```

优点是：

- 更简洁；
- 作用域结束自动计时；
- 不容易忘记 Stop；
- 适合临时 profiling。

缺点是：

- 需要先写一个 Timer 工具类；
- 简单 Timer 的功能有限；
- 复杂项目中可能需要更完整的 profiler。

---

### 8.5 Debug timing vs Release timing

Debug 模式下，编译器通常不会做大量优化，并且可能插入额外的调试信息。

Release 模式下，编译器会进行优化，代码执行速度通常更接近真实发布环境。

所以性能测试时，不应该只看 Debug 模式的结果。

如果你想判断代码性能，应该优先在 Release 模式下测量。

---

## 9. 核心思想

### 重点 1：计时的本质是记录两个 time point 的差值

最核心的模式是：

```cpp
auto start = std::chrono::high_resolution_clock::now();

// code

auto end = std::chrono::high_resolution_clock::now();
auto duration = end - start;
```

这就是 timing 的基本结构。

无论写得多复杂，本质都是记录开始时间和结束时间，然后计算 duration。

---

### 重点 2：`<chrono>` 是现代 C++ 的标准时间工具

不要使用平台相关的旧式计时 API 来写普通 C++ 代码。

现代 C++ 中更推荐使用：

```cpp
#include <chrono>
```

它类型安全、跨平台，并且能明确表达时间单位。

例如：

```cpp
std::chrono::milliseconds(100)
```

比单纯写：

```cpp
100
```

更清楚，因为读代码的人知道这里的单位是 milliseconds。

---

### 重点 3：RAII 可以让计时更自动化

The Cherno 这一集非常重要的思想是：用一个 `Timer` class 自动计时。

```cpp
void Function()
{
    Timer timer;

    // code
}
```

当函数结束时，`timer` 自动析构，计时自动完成。

这是 C++ 中非常典型的 RAII 思维：对象的生命周期管理资源或行为。

在这里，`Timer` 管理的是“计时开始”和“计时结束”。

---

### 重点 4：计时结果要结合上下文理解

计时结果不是绝对真理。

例如：

- Debug 模式会更慢；
- `std::cout` 会严重影响计时；
- 操作系统调度会带来波动；
- CPU 状态、后台程序也会影响结果；
- 第一次运行可能和后续运行不同。

所以不要因为一次测量结果就下结论。

更可靠的做法是多次测量，并尽量减少无关因素影响。

---

### 重点 5：测量是优化的前提

不要盲目优化。

正确顺序应该是：

1. 先写出清晰正确的代码；
2. 用 timing 或 profiler 找出真正慢的地方；
3. 针对瓶颈优化；
4. 再次测量确认优化有效。

没有测量的优化很容易浪费时间，甚至可能让代码更复杂但并没有变快。