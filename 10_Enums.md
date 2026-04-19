# 10 - C++ 枚举 (Enums)

## 1. 基础概念 (Basic Concepts)

在 C++ 中，枚举（Enums）用于表示一组相关的状态或选项，例如控制台日志的级别（Error, Warning, Info）。使用枚举可以替代散乱的整数常量，使代码逻辑更清晰。

## 2. 数值递增规则 (Value Rules)

枚举成员本质上是整数常量。如果不显式指定，它们从 0 开始递增；如果指定了起始值，后续成员会自动在此基础上加 1。

| 特性 | 描述 | 示例 |
| :--- | :--- | :--- |
| **自动赋值** | 默认从 0 开始递增 | `LevelError = 0` |
| **显式赋值** | 可以手动指定某个成员的值 | `A = 5` |
| **后续递增** | 未赋值成员会在前一个成员基础上递增 | `B` 自动变为 `6` |

## 3. 内存优化 (Memory Optimization)

默认情况下枚举使用 `int` 类型，但 C++ 允许显式指定枚举的底层数据类型，以便在处理大量数据或嵌入式开发时节省内存。

* **语法**：`enum Name : type { ... };`
* **示例**：使用 `unsigned char`（1字节）替代默认的 `int`（通常为4字节）。

---

## 4. 类中的枚举 (Enums in Classes)

将枚举定义在类（Class）内部可以有效管理命名空间。在类外引用这些枚举值时，需要使用类名作为前缀。

* **作用域限制**：例如使用 `Log::LevelWarning` 来确保类型安全。
* **状态控制**：可以作为类的成员变量来控制程序逻辑（如 `m_LogLevel`）。

---

## 示例代码

```cpp
#include <iostream>

/*
在 C++ 中，如果你有一组相关的状态或选项，比如控制台日志的级别（Error, Warning, Info）
你可以用枚举来表示它们
*/

enum Example0 {
    A = 5, B, C // B 自动变成 6
};

// 可以显式地指定枚举的底层数据类型，以节省内存
enum Example1 : unsigned char {
    D = 0, E, F
};

// Log 类优化
class Log {
public:
    // 将日志级别定义为枚举
    enum Level {
        LevelError = 0, LevelWarning, LevelInfo
    };

private:
    Level m_LogLevel = LevelInfo; // 默认级别设为 Info

public:
    void SetLevel(Level level) {
        m_LogLevel = level;
    }

    void Error(const char* message) {
        if (m_LogLevel >= LevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }

    void Warn(const char* message) {
        if (m_LogLevel >= LevelWarning)
            std::cout << "[WARNING]: " << message << std::endl;
    }
};

int main() {
    Log log;
    log.SetLevel(Log::LevelWarning); // 只能传入 Log::Level 中的枚举值
    
    log.Warn("Hello!");
    log.Error("Something went wrong!");
     
    std::cin.get();
}