#include <iostream>

class Entity {
public:
    float X, Y;

    // 构造函数：对象诞生时调用
    Entity() {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Created Entity!" << std::endl;
    }

    // 析构函数：对象死亡前调用
    // 名字必须是 ~ 加上类名
    // 没有返没有任何参数
    ~Entity() {
        std::cout << "Destroyed Entity!" << std::endl;
    }
};

int main() {
    // 这是一个空的作用域 (Scope)
    {
        Entity e; 
        std::cout << "Doing something with Entity..." << std::endl;
    } // <-- 注意这里！当代码执行到这个右括号时，对象 e 就死了

    std::cout << "Entity is gone, back to main." << std::endl;
    std::cin.get();
}