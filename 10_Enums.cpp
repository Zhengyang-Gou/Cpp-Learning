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