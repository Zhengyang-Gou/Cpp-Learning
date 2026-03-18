#include <iostream>

/* Player 类*/
class Player0
{
public:
    int x, y, speed;
};

struct Player1 // 和 class 的区别是默认为 public
{
    int x, y, speed;
};

void Move(Player0& player, int xa, int ya)
{
    player.x += xa * player.speed;
    player.y += ya * player.speed;
}


/* log 类*/
class Log
{
public:
    // 定义日志级别常量，用于过滤输出
    const int LogLevelError = 0;
    const int LogLevelWarning = 1;
    const int LogLevelInfo = 2;

private:
    // m_LogLevel 是私有的，防止外部直接修改，遵循封装原则
    int m_LogLevel = LogLevelInfo; 

public:
    // 设置当前的日志显示级别
    void SetLevel(int level)
    {
        m_LogLevel = level;
    }

    // 错误信息输出
    void Error(const char* message)
    {
        if (m_LogLevel >= LogLevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }

    // 警告信息输出
    void Warn(const char* message)
    {
        if (m_LogLevel >= LogLevelWarning)
            std::cout << "[WARNING]: " << message << std::endl;
    }

    // 普通信息输出
    void Info(const char* message)
    {
        if (m_LogLevel >= LogLevelInfo)
            std::cout << "[INFO]: " << message << std::endl;
    }
};

int main()
{
    /* Player 类*/
    Player0 m0NESY; // 实例化
    m0NESY.x = 30;
    m0NESY.y = 163;
    m0NESY.speed = 10;
    Move(m0NESY, 10, -10);

    /* log 类*/
    Log log;
    log.SetLevel(log.LogLevelWarning); // 只显示警告和错误
    
    log.Warn("Hello!");   // 会打印
    log.Error("Fatal!");  // 会打印
    log.Info("Test");     // 不会打印，因为级别不够

}