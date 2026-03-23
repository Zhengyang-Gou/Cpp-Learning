/*
Mutable 允许变量在 const 方法被修改
*/
#include <iostream>

class Entity {
private:
    int m_X;
    mutable int m_DebugCount;
public:
    int GetX() const {
        m_DebugCount++; // 可以修改
        return m_X;
    }
};

int main() {
    int x = 8;
    auto f = [=]() mutable{
        x++;
        std::cout << x << std::endl; // 输出 9
    };

f();
std::cout << x << std::endl; // 输出 8，外部的 x 并未改变
}