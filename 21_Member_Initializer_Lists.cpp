#include <iostream>
/*
对于像 int、float 这样的基本数据类型，两种写法在性能上没有任何区别
但是，当你的成员变量是另一个类时，区别就大
*/
class Entity {
private:
    std::string m_Name;
    int m_Score;
public:
    // 传统写法：在构造函数体内赋值
    Entity() {
        m_Name = "Unknown";
        m_Score = 0;
    }
};

class Entity_Init {
private:
    std::string m_Name;
    int m_Score;
public:
    // 初始化列表写法
    Entity_Init()
        : m_Name("Unknown"), m_Score(0) // 注意这里是用括号()来初始化，且用逗号分隔
    {
        // 构造函数体现在是空的
    }
};

class Example {
public:
    Example() {
        std::cout << "Created Entity!" << std::endl;
    }
    Example(int x) {
        std::cout << "Created Entity with " << x << "!" << std::endl;
    }
};

class Entity_Test {
private:
    Example m_Example;
public:
    Entity_Test() {
        m_Example = Example(8); // 在这里赋值, 浪费性能
    }
};

class Entity_Right {
private:
    Example m_Example;
public:
    Entity_Right()
        : m_Example(Example(8)) // 或者简写为 : m_Example(8)
    {
    }
};

int main() {
    Entity_Test();
    Entity_Right();
}