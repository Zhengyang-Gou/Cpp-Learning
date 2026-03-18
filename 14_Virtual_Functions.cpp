#include <iostream>
#include <string>

// class Entity {
// public:
//     std::string GetName() { return "Entity"; }
// };

// class Player : public Entity {
// private:
//     std::string m_Name;
// public:
//     Player(const std::string& name) { m_Name = name; }
    
//     std::string GetName() { return m_Name; }
// };

// // 这是一个全局函数，接收一个 Entity 的指针并打印它的名字
// void PrintName(Entity* entity) { // 指针类型是 Entity*，绑定了 Entity 类里的 GetName() 方法
//     std::cout << entity->GetName() << std::endl;
// }

class Entity {
public:
    // 加上 virtual，开启多态
    virtual std::string GetName() { return "Entity"; }
};

class Player : public Entity {
private:
    std::string m_Name;
public:
    Player(const std::string& name) { m_Name = name; }
    
    // 在子类重写的方法后面加上 override 关键字
    // 这不是必须的，但如果你的函数名拼写错了，编译器会报错提醒你
    std::string GetName() override { return m_Name; } 
};


void PrintName(Entity* entity) { 
    // 因为父类里加了 virtual，这里会在运行时动态决定调用谁的 GetName()
    std::cout << entity->GetName() << std::endl;
}

int main() {
    Entity* e = new Entity();
    PrintName(e); // 打印 Entity

    Player* p = new Player("Cherno");
    PrintName(p); // 打印 Cherno

    std::cin.get();
}