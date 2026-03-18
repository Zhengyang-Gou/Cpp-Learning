#include <iostream>

class Entity0 {
public:
    float X, Y;

    void Print() {
        std::cout << X << ", " << Y << std::endl;
    }
};

// 构造函数
class Entity {
public:
    float X, Y;

    // 函数名必须与类名完全一致，没有返回值类型
    Entity() {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Entity Created!" << std::endl;
    }

    // 带参数的构造函数
    Entity(float x, float y) {
        X = x;
        Y = y;
    }

    void Print() {
        std::cout << X << ", " << Y << std::endl;
    }
};

// 禁用实例化
class Log {
public:
    // 明确告诉编译器：不要给我生成默认构造函数，我不允许别人实例化这个类！
    Log() = delete; 

    static void Write(const char* message) {
        std::cout << message << std::endl;
    }
};

int main() {
    Entity0 a; // 在栈上分配内存来存放两个 float 变量
    a.Print(); // 产生垃圾数据

    Entity b; 
    b.Print(); // 现在会稳定打印出 0, 0

    Entity c(10.5f, 5.0f);
    c.Print();

    std::cin.get();
}