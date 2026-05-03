# 61 - 如何让 C++ 运行更快：std::async (How to make C++ run FASTER with std::async)

## 1. 基本概念 

这一集的核心主题是 **`std::async`**。

`std::async` 是 C++11 提供的标准库工具，用来把一个函数异步执行。简单说，就是让某些任务可以和主线程同时进行，而不是一个接一个等待。

例如：

```cpp
auto result = std::async(std::launch::async, SomeFunction);
```

它会返回一个 `std::future`，表示“未来会得到的结果”。

---

## 2. 为什么需要它 

如果程序里有多个互不依赖的耗时任务，顺序执行会浪费时间。

例如：

```cpp
LoadTexture();
LoadModel();
LoadAudio();
```

如果这三个函数互不依赖，就没必要一个等一个。

可以让它们并行执行：

```cpp
auto texture = std::async(std::launch::async, LoadTexture);
auto model   = std::async(std::launch::async, LoadModel);
auto audio   = std::async(std::launch::async, LoadAudio);
```

这样可以减少总等待时间。

但要注意：

> `std::async` 不是让所有代码自动变快。只有任务彼此独立、耗时明显时，异步才有意义。

---

## 3. 核心语法

### 3.1 基本用法

```cpp
std::future<int> result = std::async(std::launch::async, Function);
```

常用组件：

| 组件 | 作用 |
| :--- | :--- |
| `std::async` | 启动异步任务 |
| `std::future` | 保存未来的结果 |
| `get()` | 等待任务完成并取回结果 |
| `std::launch::async` | 要求任务异步执行 |
| `std::launch::deferred` | 延迟到 `get()` 时才执行 |

---

### 3.2 获取返回值

```cpp
int value = result.get();
```

`get()` 会阻塞当前线程，直到异步任务完成。

---

### 3.3 没有返回值的任务

```cpp
std::future<void> task = std::async(std::launch::async, DoWork);
task.get();
```

即使函数返回 `void`，也可以用 `std::future<void>` 等待它完成。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <future>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

std::string LoadAsset(const std::string& name)
{
    std::cout << "开始加载: " << name << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "加载完成: " << name << std::endl;
    return name + " data";
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::future<std::string> textureFuture =
        std::async(std::launch::async, LoadAsset, "Texture");

    std::future<std::string> modelFuture =
        std::async(std::launch::async, LoadAsset, "Model");

    std::future<std::string> audioFuture =
        std::async(std::launch::async, LoadAsset, "Audio");

    std::string texture = textureFuture.get();
    std::string model = modelFuture.get();
    std::string audio = audioFuture.get();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "全部资源加载完成" << std::endl;
    std::cout << texture << std::endl;
    std::cout << model << std::endl;
    std::cout << audio << std::endl;
    std::cout << "耗时: " << duration.count() << " 秒" << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `std::async` 会返回 `std::future`

```cpp
std::future<std::string> textureFuture =
    std::async(std::launch::async, LoadAsset, "Texture");
```

这行代码的意思是：

- 启动一个异步任务
- 执行 `LoadAsset("Texture")`
- 将未来的返回值保存在 `textureFuture` 里

`future` 不是结果本身，而是一个“结果的占位对象”。

---

### 5.2 `get()` 会等待任务完成

```cpp
std::string texture = textureFuture.get();
```

如果任务已经完成，`get()` 直接返回结果。

如果任务还没完成，当前线程会等待。

所以异步不是完全不等待，而是把等待推迟到真正需要结果的时候。

---

### 5.3 必须保存 `future`

不要这样写：

```cpp
std::async(std::launch::async, LoadAsset, "Texture");
```

这样可能会导致临时的 `future` 立刻析构，使当前线程等待任务完成，异步效果就没了。

更好的写法是：

```cpp
auto future = std::async(std::launch::async, LoadAsset, "Texture");
```

---

### 5.4 使用 `std::launch::async`

推荐明确写：

```cpp
std::launch::async
```

如果不写：

```cpp
std::async(LoadAsset, "Texture");
```

标准库可以选择：

- 立即异步执行
- 或者延迟到 `get()` 时执行

这可能导致程序并没有真正并行。

---

### 5.5 异步任务必须尽量独立

适合异步的任务：

```cpp
LoadTexture();
LoadModel();
LoadAudio();
```

因为它们互不依赖。

不适合异步的任务：

```cpp
int a = Step1();
int b = Step2(a);
int c = Step3(b);
```

因为后一步依赖前一步，强行异步没有意义。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 写法比手动创建 `thread` 简单 | 不适合很小的任务 |
| 可以获取返回值 | `get()` 会阻塞 |
| 自动管理一部分线程细节 | 线程调度有开销 |
| 适合并行处理独立任务 | 共享数据容易产生 data race |
| 代码比裸 `std::thread` 更安全 | 不等于自动性能优化 |

---

## 7. 核心思想

### 重点 1：异步适合独立耗时任务

`std::async` 最适合处理互不依赖的任务，例如资源加载、文件读取、计算任务等。

如果任务之间有强依赖，异步通常不会让程序明显变快。

---

### 重点 2：`future` 表示未来的结果

`std::async` 不直接返回函数结果，而是返回：

```cpp
std::future<T>
```

真正取结果时使用：

```cpp
future.get();
```

---

### 重点 3：`get()` 是同步点

程序执行到：

```cpp
future.get();
```

时，如果任务还没完成，就会等待。

所以性能优化的关键是：

> 在调用 `get()` 之前，让异步任务和其他工作并行进行。

---

### 重点 4：不要滥用 `std::async`

不是所有任务都应该异步。

如果任务很小，创建线程和调度线程的成本可能比任务本身还高。

适合异步的任务通常是：

- 耗时明显
- 彼此独立
- 不频繁共享数据
- 不需要严格顺序执行

---

## 8. 常见错误

### 错误 1：忘记保存 `future`

```cpp
std::async(std::launch::async, DoWork);
```

应该写成：

```cpp
auto task = std::async(std::launch::async, DoWork);
```

---

### 错误 2：以为 `std::async` 一定更快

如果任务很轻：

```cpp
auto task = std::async(std::launch::async, Add, 1, 2);
```

可能反而更慢，因为线程调度也有成本。

---

### 错误 3：多个线程同时修改同一个变量

```cpp
int counter = 0;

auto task1 = std::async(std::launch::async, [&]()
{
    counter++;
});

auto task2 = std::async(std::launch::async, [&]()
{
    counter++;
});
```

这可能产生 data race。

如果多个线程共享数据，需要使用：

```cpp
std::mutex
std::atomic
```

或者重新设计代码，避免共享可变状态。

