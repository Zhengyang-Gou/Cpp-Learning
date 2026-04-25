# 44 - C++ 中的线程 

## 1. 基本概念

**thread** 是程序执行的一条独立路径。通常情况下，一个 C++ 程序从 `main()` 开始执行，代码是一行一行顺序运行的，这可以理解为只有一个主要执行流，也就是 **main thread**。

当我们创建一个新的 `std::thread` 时，程序就可以让另一个函数在新的 thread 中同时运行。这样，主线程可以继续处理自己的事情，而另一个 thread 可以在后台执行耗时任务。

在 C++ 中，标准库提供了 `std::thread` 来创建和管理 thread：

```cpp
#include <thread>
```

核心重点是：

- thread 可以让代码并发执行；
- `std::thread` 接收一个函数，并在新的 thread 中运行它；
- 主线程和工作线程会同时运行；
- 使用 `join()` 等待一个 thread 执行结束；
- 可以通过共享变量控制后台 thread 何时停止；
- `std::this_thread::sleep_for()` 可以让当前 thread 暂停一段时间；
- `std::this_thread::get_id()` 可以查看当前代码运行在哪个 thread 上。

---

## 2. 为什么需要它 

如果程序只有一个 thread，那么所有任务都必须按顺序执行。

例如，一个游戏或图形程序中可能有这些任务：

- 渲染画面；
- 加载资源；
- 处理输入；
- 播放音频；
- 从磁盘读取文件；
- 和服务器通信；
- 做复杂计算。

如果所有事情都放在 main thread 上执行，那么一旦某个任务很慢，整个程序就会卡住。

例如：

```cpp
LoadLargeFile();
RenderFrame();
HandleInput();
```

如果 `LoadLargeFile()` 需要 5 秒，程序在这 5 秒内就无法继续渲染画面，也无法响应用户输入。

使用 thread 后，可以把耗时任务放到后台执行：

```cpp
std::thread worker(LoadLargeFile);

RenderFrame();
HandleInput();

worker.join();
```

这样 main thread 可以继续处理主要逻辑，而后台 thread 负责执行耗时任务。

在实际项目中，thread 常用于：

- 后台加载资源；
- 网络请求；
- 文件读写；
- 数据处理；
- 游戏引擎中的任务系统；
- 工具程序中的长时间计算；
- 不希望阻塞主界面的任务。

thread 的核心价值是：**让多个任务在时间上重叠执行，避免一个耗时任务阻塞整个程序。**

---

## 3. 核心语法

### 3.1 引入头文件

使用 `std::thread` 需要包含：

```cpp
#include <thread>
```

如果要使用时间相关功能，例如 `sleep_for()`，通常还会包含：

```cpp
#include <chrono>
```

---

### 3.2 创建 thread

最基本的写法是把一个函数传给 `std::thread`：

```cpp
void DoWork()
{
    // 在新 thread 中运行的代码
}

int main()
{
    std::thread worker(DoWork);

    worker.join();

    return 0;
}
```

这里：

```cpp
std::thread worker(DoWork);
```

表示创建一个名为 `worker` 的 thread，并让它执行 `DoWork()` 函数。

注意：

```cpp
DoWork
```

传的是函数本身。

不要写成：

```cpp
std::thread worker(DoWork());
```

因为 `DoWork()` 会立即在当前 thread 中调用函数，而不是把函数交给新的 thread。

---

### 3.3 等待 thread 结束：`join()`

```cpp
worker.join();
```

`join()` 的意思是：当前 thread 等待 `worker` 这个 thread 执行完成。

如果不调用 `join()`，当 `std::thread` 对象析构时，如果 thread 仍然是 joinable 的，程序会直接调用 `std::terminate()`，导致异常终止。

所以只要创建了一个 `std::thread`，通常就要明确处理它：

```cpp
worker.join();
```

或者：

```cpp
worker.detach();
```

但初学者更应该优先使用 `join()`。

---

### 3.4 获取当前 thread 的 ID

```cpp
std::this_thread::get_id()
```

可以获取当前正在执行代码的 thread ID。

例如：

```cpp
std::cout << std::this_thread::get_id() << std::endl;
```

这可以帮助我们观察：某段代码到底是在 main thread 里运行，还是在新创建的 worker thread 里运行。

---

### 3.5 让当前 thread 暂停一段时间

```cpp
std::this_thread::sleep_for(std::chrono::seconds(1));
```

表示让当前 thread 暂停 1 秒。

示例中使用了 chrono literals：

```cpp
using namespace std::literals::chrono_literals;

std::this_thread::sleep_for(1s);
```

这里的 `1s` 表示 1 秒。

也可以写成更完整的形式：

```cpp
std::this_thread::sleep_for(std::chrono::seconds(1));
```

---

### 3.6 使用共享变量控制 thread 退出

可以用一个变量控制后台 thread 是否继续运行：

```cpp
static bool s_Finished = false;
```

然后在 worker thread 中循环检查：

```cpp
while (!s_Finished)
{
    // 继续工作
}
```

main thread 中可以修改它：

```cpp
s_Finished = true;
```

这样 worker thread 就会退出循环。

不过在真实工程中，这种共享变量会涉及 **data race** 问题，通常应该使用 `std::atomic<bool>` 或其他同步机制。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

static std::atomic<bool> s_Finished = false;

void DoWork()
{
    using namespace std::literals::chrono_literals;

    std::cout << "Worker thread started. Thread ID: "
              << std::this_thread::get_id() << std::endl;

    while (!s_Finished)
    {
        std::cout << "Working..." << std::endl;
        std::this_thread::sleep_for(1s);
    }

    std::cout << "Worker thread finished." << std::endl;
}

int main()
{
    std::cout << "Main thread ID: "
              << std::this_thread::get_id() << std::endl;

    std::thread worker(DoWork);

    std::cin.get();

    s_Finished = true;

    worker.join();

    std::cout << "Program finished." << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::thread worker(DoWork);`

这一行创建了一个新的 thread。

```cpp
std::thread worker(DoWork);
```

它的意思不是立即在 main thread 里调用 `DoWork()`，而是让 `DoWork()` 在新的 thread 中运行。

从这一行开始，程序中至少有两条执行路径：

1. main thread 继续往下执行；
2. worker thread 开始执行 `DoWork()`。

这就是并发执行的核心。

---

### 5.2 `DoWork()` 是后台执行的函数

```cpp
void DoWork()
{
    using namespace std::literals::chrono_literals;

    std::cout << "Worker thread started. Thread ID: "
              << std::this_thread::get_id() << std::endl;

    while (!s_Finished)
    {
        std::cout << "Working..." << std::endl;
        std::this_thread::sleep_for(1s);
    }

    std::cout << "Worker thread finished." << std::endl;
}
```

这个函数会在新的 worker thread 中执行。

它会不断打印：

```cpp
Working...
```

然后睡眠 1 秒。

如果没有 `sleep_for()`，这个循环会非常快地运行，占用大量 CPU。加上 `sleep_for(1s)` 后，worker thread 每秒执行一次循环，更适合演示。

---

### 5.3 `std::this_thread::get_id()`

```cpp
std::this_thread::get_id()
```

用于获取当前执行代码的 thread ID。

在 `main()` 中调用它，得到的是 main thread 的 ID。

在 `DoWork()` 中调用它，得到的是 worker thread 的 ID。

这可以证明：`DoWork()` 确实不是在 main thread 中执行的，而是在另一个 thread 中执行的。

---

### 5.4 `std::cin.get();`

```cpp
std::cin.get();
```

这行代码会让 main thread 等待用户输入。

在 main thread 等待输入时，worker thread 仍然会继续运行，并不断打印：

```cpp
Working...
```

这正好说明了 thread 的作用：**一个 thread 阻塞时，另一个 thread 仍然可以继续运行。**

---

### 5.5 `s_Finished = true;`

```cpp
s_Finished = true;
```

用户按下 Enter 后，main thread 会执行这一行，把 `s_Finished` 改为 `true`。

worker thread 的循环条件是：

```cpp
while (!s_Finished)
```

当 `s_Finished` 变成 `true` 后，`!s_Finished` 就是 `false`，循环结束，`DoWork()` 函数执行完毕。

---

### 5.6 为什么这里使用 `std::atomic<bool>`

演示里常见写法是用一个简单的 `static bool` 控制 thread 停止。

为了让示例更接近现代 C++ 的安全写法，这里使用：

```cpp
static std::atomic<bool> s_Finished = false;
```

原因是：`s_Finished` 会被两个 thread 同时访问。

- worker thread 会读取它；
- main thread 会修改它。

普通 `bool` 在多线程环境下可能产生 **data race**，属于未定义行为。

`std::atomic<bool>` 可以保证这个布尔值的读写是原子的，适合这种简单的跨 thread 状态标记。

---

### 5.7 `worker.join();`

```cpp
worker.join();
```

这行代码非常重要。

它让 main thread 等待 worker thread 执行结束。

如果不写 `join()`，程序可能在 worker thread 还没结束时就退出，这会导致严重问题。对于 `std::thread` 对象来说，如果它析构时仍然代表一个正在运行且没有被 join 或 detach 的 thread，程序会终止。

所以创建 thread 后，必须明确决定：

```cpp
worker.join();
```

或者：

```cpp
worker.detach();
```

初学阶段建议优先使用 `join()`，因为它更容易理解，也更安全。

---

## 6. 常见用法

### 6.1 后台执行耗时任务

如果某个任务很慢，例如加载大文件，就可以放到 worker thread 中：

```cpp
void LoadAssets()
{
    // 加载贴图、模型、声音等资源
}
```

然后：

```cpp
std::thread loader(LoadAssets);
```

这样 main thread 不需要一直等待资源加载完成，可以继续显示加载界面或处理输入。

---

### 6.2 避免主界面卡死

在桌面程序或工具程序中，如果主线程负责 UI，而你在主线程中做大量计算，界面就可能无法响应。

把计算任务放到另一个 thread 中，可以让 UI thread 保持响应。

例如：

```cpp
void HeavyCalculation()
{
    // 执行复杂计算
}
```

```cpp
std::thread worker(HeavyCalculation);
```

---

### 6.3 定时执行后台任务

类似 The Cherno 示例中的写法：

```cpp
while (!finished)
{
    DoSomething();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

可以用于周期性任务，例如：

- 每隔一段时间保存一次数据；
- 定期检查网络状态；
- 后台刷新缓存；
- 简单的监控任务。

---

### 6.4 网络请求

网络请求通常比较慢。如果在 main thread 中直接等待服务器响应，程序可能会卡住。

可以用 thread 把网络请求放到后台：

```cpp
void DownloadData()
{
    // 模拟网络下载
}
```

```cpp
std::thread networkThread(DownloadData);
```

不过真实工程中，现代 C++ 项目通常会使用更高级的异步系统、thread pool、任务队列或网络库，而不是手动为每个请求创建一个 thread。

---

## 7. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 可以让耗时任务在后台运行 | 多线程代码更难调试 |
| 避免 main thread 被长时间阻塞 | 容易出现 data race |
| 能更好地利用多核 CPU | 需要考虑同步问题 |
| 适合文件 IO、网络、资源加载等任务 | thread 创建和切换本身也有成本 |
| 可以提升程序响应性 | 输出顺序和执行顺序可能不稳定 |
| 适合构建异步系统的基础 | 错误使用可能导致 deadlock、崩溃或未定义行为 |

thread 很强大，但并不是越多越好。

创建太多 thread 会带来额外开销，甚至降低性能。真实项目中经常使用 **thread pool**，而不是随意创建大量 `std::thread`。

---

## 8. 和其他相关概念的区别

### 8.1 thread vs process

**process** 是操作系统中的进程。一个程序运行起来后，通常就是一个 process。

**thread** 是 process 内部的一条执行路径。

简单理解：

- process 是程序运行的容器；
- thread 是容器内部真正执行代码的路径。

一个 process 可以包含多个 thread。

多个 thread 共享同一个 process 的内存空间，因此它们可以访问同一份全局变量、heap 内存和静态数据。

这既方便，也危险。

方便在于：thread 之间共享数据比较容易。

危险在于：多个 thread 同时访问同一份数据时，可能产生 data race。

---

### 8.2 main thread vs worker thread

**main thread** 是程序从 `main()` 开始运行的 thread。

**worker thread** 通常是我们额外创建出来，用于执行后台任务的 thread。

例如：

```cpp
int main()
{
    std::thread worker(DoWork);
    worker.join();
}
```

这里：

- `main()` 运行在 main thread；
- `DoWork()` 运行在 worker thread。

main thread 通常负责程序主要流程，worker thread 通常负责辅助任务。

---

### 8.3 `join()` vs `detach()`

`join()` 表示等待 thread 完成：

```cpp
worker.join();
```

调用 `join()` 后，当前 thread 会阻塞，直到 `worker` 执行结束。

`detach()` 表示把 thread 分离出去，让它独立运行：

```cpp
worker.detach();
```

分离后，当前 `std::thread` 对象不再管理这个 thread。

| 对比 | `join()` | `detach()` |
| :--- | :--- | :--- |
| 是否等待 thread 结束 | 是 | 否 |
| 是否容易控制生命周期 | 容易 | 较难 |
| 初学者是否推荐 | 推荐 | 不推荐随意使用 |
| 常见风险 | 当前 thread 会等待 | detached thread 可能访问已经销毁的数据 |

初学者应该优先使用 `join()`。只有在非常清楚 thread 生命周期的情况下，才考虑使用 `detach()`。

---

### 8.4 concurrency vs parallelism

**concurrency** 通常强调任务在时间上交错进行。

**parallelism** 强调任务真的在多个 CPU 核心上同时执行。

使用 `std::thread` 可以实现 concurrency，也可能实现 parallelism。

例如在单核 CPU 上，多个 thread 可能是轮流执行的，看起来像同时进行。

在多核 CPU 上，多个 thread 可能真的在不同核心上同时运行。

---

### 8.5 `std::thread` vs `std::async`

`std::thread` 是比较底层的 thread 管理工具。

使用它时，你需要自己管理：

- thread 的创建；
- `join()`；
- `detach()`；
- 数据同步；
- 生命周期。

`std::async` 是更高层的异步工具，可以返回 `std::future`，更适合获取异步任务的返回值。

简单理解：

| 对比 | `std::thread` | `std::async` |
| :--- | :--- | :--- |
| 控制程度 | 更底层，控制更多 | 更高层，更方便 |
| 是否需要手动 `join()` | 通常需要 | 通常通过 `future` 管理 |
| 是否适合教学理解 thread | 很适合 | 更偏异步任务 |
| 是否适合返回结果 | 需要自己处理 | 更方便 |

---

### 8.6 shared variable vs message passing

示例中使用共享变量控制 thread：

```cpp
s_Finished = true;
```

worker thread 不断检查这个变量：

```cpp
while (!s_Finished)
```

这是最直观的方式，但在复杂系统中，过多共享变量会让程序难以维护。

另一种思路是 **message passing**：thread 之间不直接共享大量数据，而是通过队列发送任务或消息。

真实工程里，经常会使用：

- mutex；
- condition variable；
- atomic；
- thread-safe queue；
- task system；
- job system。

这些都是为了更安全地协调多个 thread。

---

## 9. 核心思想

### 重点 1：thread 让函数可以和 main thread 同时运行

创建一个 `std::thread` 后，传入的函数会在新的 thread 中执行。

例如：

```cpp
std::thread worker(DoWork);
```

这意味着 `DoWork()` 不再阻塞 main thread，而是可以和 main thread 并发运行。

这是 thread 最基本、最重要的作用。

---

### 重点 2：创建 thread 后必须管理它的生命周期

`std::thread` 不是创建完就不用管了。

你必须明确决定如何处理它：

```cpp
worker.join();
```

或者：

```cpp
worker.detach();
```

如果一个 joinable 的 `std::thread` 对象在析构时没有被 join 或 detach，程序会终止。

所以初学者要养成习惯：**创建了 thread，就必须考虑它什么时候结束，以及谁负责等待它结束。**

---

### 重点 3：多个 thread 访问同一份数据时要小心

示例用一个共享变量控制 worker thread 停止。

这个思路很直观：

```cpp
while (!s_Finished)
{
    // 工作
}
```

main thread 修改：

```cpp
s_Finished = true;
```

但真实 C++ 多线程中，共享数据是最容易出问题的地方。

如果多个 thread 同时读写同一变量，而没有使用 atomic、mutex 或其他同步机制，就可能产生 data race。

所以实际写代码时，不要随意让多个 thread 同时访问同一份数据。

---

### 重点 4：thread 不一定让程序更快，但可以让程序更流畅

thread 并不是万能的性能优化工具。

如果任务很小，创建 thread 的开销可能比任务本身还大。

但 thread 很适合解决“阻塞”问题。

例如：

- 用户界面不能卡死；
- 游戏主循环不能因为加载资源停住；
- 网络请求不能阻塞整个程序；
- 后台任务不应该影响主流程。

所以 thread 的意义不仅是“更快”，更重要的是“不要让一个任务卡住整个程序”。

---

### 重点 5：输出顺序可能不可预测

当多个 thread 同时执行时，执行顺序不一定固定。

例如 main thread 和 worker thread 都在 `std::cout` 输出内容时，谁先输出、谁后输出，不一定每次都一样。

这是多线程程序的基本特点。

不要写依赖“某个 thread 一定先执行”的代码，除非你使用了明确的同步机制。
