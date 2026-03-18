// 继承的核心目的就一个：消除重复代码，并在类与类之间建立一种包含关系
#include <iostream>

class Entity { // 父类
public:
    float X, Y;

    void Move(float xa, float ya) {
        X += xa;
        Y += ya;
    }
};

// 冒号加上 public Entity，表示 Player 继承了 Entity 的所有公开内容
// 子类的内存占用 = 父类的内存占用 + 子类独有的内存占用
class Player : public Entity {
public:
    const char* Name;

    void PrintName() {
        std::cout << Name << std::endl;
    }
};

int main() {
    std::cout << sizeof(Entity) << " bytes" << std::endl; // 输出 8 (两个 float)
    std::cout << sizeof(Player) << " bytes" << std::endl; // 输出 16 (两个 float + 一个指针)

    Player player;
    // player 虽然没有自己写 Move 方法和 X, Y 变量，但它可以直接使用
    player.Move(5.0f, 5.0f); 
    player.X = 2.0f;

    std::cin.get();
}