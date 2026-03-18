#include <iostream>

// 普通结构体
struct Entity {
    int x, y;

    void print() {
        std::cout << "Entity (Normal) -> x: " << x << ", y: " << y << std::endl;
    }
};

// 带有静态成员的结构体
struct EntityStatic {
    static int x, y; 

    void print() {
        std::cout << "Entity (Static) -> x: " << x << ", y: " << y << std::endl;
    }
};

// 必须在类外定义静态变量
int EntityStatic::x;
int EntityStatic::y;

int main() {
    std::cout << "普通变量测试" << std::endl;
    Entity e1;
    e1.x = 5; e1.y = 8;

    Entity e2;
    e2.x = 10; e2.y = 16;

    e1.print(); // 输出 5, 8
    e2.print(); // 输出 10, 16（互不干扰）

    std::cout << "\n静态变量测试" << std::endl;
    EntityStatic s1;
    s1.x = 5; s1.y = 8; // 通过 s1 修改

    EntityStatic s2;
    s2.x = 10; s2.y = 16; // 通过 s2 修改，这实际上覆盖了之前 s1 设置的值

    s1.print(); // 输出 10, 16 
    s2.print(); // 输出 10, 16

    // 推荐写法：静态变量不依赖于对象，直接用类名访问
    EntityStatic::x = 100;
    std::cout << "Using Class Name: " << EntityStatic::x << std::endl;

    return 0;
}