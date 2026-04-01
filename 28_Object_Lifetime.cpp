// C++对象主要存在堆和栈两个地方
// 栈上的对象会在离开作用域时自动销毁，而堆上的对象需要手动管理内存
// 堆对象是手动管理生命周期的

#include <iostream>

class Entity
{
public:
    Entity() { std::cout << "Created\n"; }
    ~Entity() { std::cout << "Destroyed\n"; }
};

// 堆对象是手动管理生命周期的

int main()
{
    Entity e;

    Entity* a = new Entity();
    delete a; // 需要手动调用 delete 来销毁堆对象
}