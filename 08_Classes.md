# 08 - C++ 类与结构体 (Classes & Structs)

## 1. 类与结构体的基础 (Basics)

类（Class）和结构体（Struct）都是将数据（变量）和行为（函数）组合在一起的容器。在 C++ 中，它们唯一的区别在于**默认访问权限**。

| 特性 | 类 (`class`) | 结构体 (`struct`) |
| :--- | :--- | :--- |
| **默认访问权限** | `private` (私有) | `public` (公有) |
| **主要用途** | 复杂的对象，需要保护内部数据 | 纯粹的数据集合 (POD) |

## 2. 访问修饰符 (Access Modifiers)

为了实现**封装 (Encapsulation)**，我们使用修饰符来控制外部对类成员的访问权限。

| 修饰符 | 描述 |
| :--- | :--- |
| **`public`** | 外部代码可以随意读取和修改。 |
| **`private`** | 只有类内部的函数可以访问，外部不可见。 |
| **`protected`** | 只有类内部及子类可以访问（涉及继承时使用）。 |


## 3. 封装原则 (Encapsulation)

在 `Log` 类示例中，我们将 `m_LogLevel` 设为 `private`。这样做的好处是：
* **防止误操作**：外部不能随意绕过 `SetLevel` 函数去修改级别。
* **逻辑控制**：可以在 `SetLevel` 中加入判断逻辑（例如限制级别必须在 0-2 之间）。

---

## 示例代码

```cpp
#include <iostream>

/* 1. Player 类与结构体对比 */
class Player0
{
public:
    int x, y, speed;
};

struct Player1 // 和 class 的区别是默认为 public
{
    int x, y, speed;
};

// 外部函数修改对象属性（需使用引用以确保修改的是原对象）
void Move(Player0& player, int xa, int ya)
{
    player.x += xa * player.speed;
    player.y += ya * player.speed;
}

/* 2. Log 类：展示封装与逻辑处理 */
class Log
{
public:
    // 定义常量：用于外部调用设定级别
    const int LogLevelError = 0;
    const int LogLevelWarning = 1;
    const int LogLevelInfo = 2;

private:
    // 成员变量通常以 m_ 开头（编码规范），防止外部随意篡改
    int m_LogLevel = LogLevelInfo; 

public:
    //  setter 方法：修改私有成员的唯一入口
    void SetLevel(int level)
    {
        m_LogLevel = level;
    }

    void Error(const char* message)
    {
        if (m_LogLevel >= LogLevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }

    void Warn(const char* message)
    {
        if (m_LogLevel >= LogLevelWarning)
            std::cout << "[WARNING]: " << message << std::endl;
    }

    void Info(const char* message)
    {
        if (m_LogLevel >= LogLevelInfo)
            std::cout << "[INFO]: " << message << std::endl;
    }
};

int main()
{
    /* Player 实例化 */
    Player0 m0NESY; 
    m0NESY.x = 30;
    m0NESY.y = 163;
    m0NESY.speed = 10;
    Move(m0NESY, 10, -10);

    /* Log 类的使用 */
    Log log;
    log.SetLevel(log.LogLevelWarning); // 设定为只显示 Warning 及以上
    
    log.Warn("Hello!");   // 满足条件，打印
    log.Error("Fatal!");  // 满足条件，打印
    log.Info("Test");     // 不满足条件，被过滤掉
    
    return 0;
}