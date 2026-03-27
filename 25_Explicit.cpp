#include <iostream>
#include <string>

class Entity {
private:
    std::string m_Name;
    int m_Age;
public:
    // 接受一个 int 的构造函数
    Entity(int age) : m_Name("Unknown"), m_Age(age) {}
    
    // 接受一个 string 的构造函数
    Entity(const std::string& name) : m_Name(name), m_Age(-1) {}
};

void PrintEntity(const Entity& entity) {
    // 打印实体的逻辑...
}

// explicit 关闭隐式转换
class Entity_explicit {
private:
    std::string m_Name;
    int m_Age;
public:
    // 加上 explicit，禁止隐式转换
    explicit Entity_explicit(int age) : m_Name("Unknown"), m_Age(age) {}
    
    explicit Entity_explicit(const std::string& name) : m_Name(name), m_Age(-1) {}
};

int main() {
    // 隐式转换发生在这里
    // 找到一个 Entity(int age) 构造函数，把 22 塞进这个构造函数，把 22 塞进这个构造函数
    Entity a = 22; 
    // 或者用在函数传参里：
    PrintEntity(22); 

    // 编译器报错
    // Entity b = "Cherno";
    // C++ 编译器最多只允许发生一次隐式转换
    Entity b = std::string("Cherno");

    // Entity_explicit a = 22; // 编译器报错！因为构造函数是 explicit 的
    // PrintEntity(22); // 编译器报错！
    
    // 必须显式地调用构造函数，代码意图非常清晰：
    Entity a(22); 
    Entity b = Entity(22);
    PrintEntity(Entity(22));
}