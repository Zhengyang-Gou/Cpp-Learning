/*
只提供接口，强制子类实现
*/
 #include <iostream>
#include <string>

// 1. 定义一个接口（本质上是包含纯虚函数的类）
class Printable {
public:
    virtual std::string GetClassName() = 0; // 纯虚函数，类变成抽象类
};

// 2. Entity 继承了 Printable 接口
// 继承抽象类的子类必须实现所有的纯虚函数
class Entity : public Printable {
public:
    virtual std::string GetName() { return "Entity"; }

    // 如果不写这行，Entity 也会变成抽象类，无法创建实例！
    std::string GetClassName() override { return "Entity"; }
};

// 3. Player 继承了 Entity（间接继承了 Printable）
class Player : public Entity {
private:
    std::string m_Name;
public:
    Player(const std::string& name) { m_Name = name; }
    
    std::string GetName() override { return m_Name; } 

    // 【必须实现】覆盖父类的实现
    std::string GetClassName() override { return "Player"; }
};

// 4. 终极多态：这个函数现在可以接收任何“实现了 Printable 接口”的对象！
void PrintClassName(Printable* obj) { 
    std::cout << obj->GetClassName() << std::endl;
}

int main() {
    Entity* e = new Entity();
    Player* p = new Player("Cherno");

    // 只要是实现了 Printable 的对象，都可以安全地传进来
    PrintClassName(e); // 打印 Entity
    PrintClassName(p); // 打印 Player

    std::cin.get();
}