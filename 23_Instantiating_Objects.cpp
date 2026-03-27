#include <string>
// 如何在内存中创建你的类

// 栈分配
// 极致的性能,自动清理
class Entity {
private:
    std::string m_Name;
public:
    Entity() : m_Name("Unknown") {}
    Entity(const std::string& name) : m_Name(name) {}
};

void CreateObject() {
    // 方式一：调用默认构造函数（注意不要加空括号）
    Entity entity; 
    
    // 方式二：调用带参数的构造函数
    Entity entity2("Cherno"); 
}

// 堆分配
// 当你使用 new 关键字时，就是在堆上创建对象
// 对象太大了,需要控制对象的生命周期时用 new
void CreateObjectOnHeap() {
    // 使用 new 关键字，它会返回一个指向该内存地址的指针
    Entity* entity = new Entity("Cherno");
    
    // 极其重要：你必须手动释放它！
    delete entity; 
}
