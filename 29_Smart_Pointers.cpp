// Smart Pointers 自动管理 heap 对象的生命周期，避免 memory leak 和野指针
#include <iostream>
#include <memory> // 包含智能指针的头文件

class Entity {
public:
    Entity() { std::cout << "Created\n"; }
    ~Entity() { std::cout << "Destroyed\n"; }
    void Print() { std::cout << "Hello from Entity!\n"; }
};

int main() {
    // 创建一个 unique_ptr，自动管理 Entity 对象的生命周期
    std::unique_ptr<Entity> entity = std::make_unique<Entity>(); 
    // 当 unique_ptr 离开作用域时，Entity 对象会自动销毁

    // unique_ptr 只能 move，不能 copy
    std::unique_ptr<Entity> e1 = std::make_unique<Entity>();
    std::unique_ptr<Entity> e2 = std::move(e1);

    // unique_ptr 访问对象
    entity->Print();

    // shared_ptr 允许多个指针共享同一个对象
    std::shared_ptr<Entity> sharedEntity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> sharedEntity2 = sharedEntity1; // 共享同一个对象

    // weak_ptr 不拥有对象，只是观察者，避免循环引用
    std::weak_ptr<Entity> weakEntity = sharedEntity1;
}