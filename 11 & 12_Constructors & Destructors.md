# 11 & 12 - C++ 构造函数与析构函数 (Constructors & Destructors)

## 1. 构造函数 (Constructors)

构造函数是在实例化对象时运行的一种特殊方法。它主要用于初始化类中的成员变量，防止产生随机的“垃圾数据”。

| 特性 | 描述 |
| :--- | :--- |
| **命名规则** | 方法名必须与类名完全一致 |
| **返回类型** | 没有任何返回类型（连 `void` 也不写） |
| **自动调用** | 在使用 `new` 或直接声明对象时自动触发 |
| **重载** | 可以拥有多个参数不同的构造函数（如默认构造与带参构造） |

## 2. 析构函数 (Destructors)

析构函数在对象销毁（生命周期结束）时自动调用。它通常用于清理内存、关闭文件或释放系统资源。

| 特性 | 描述 |
| :--- | :--- |
| **命名规则** | 名字为 `~`（波浪号）加上类名 |
| **参数限制** | 不接受任何参数，也不支持重载 |
| **调用时机** | 对象超出作用域（Scope）或被手动 `delete` 时 |

## 3. 生命周期与特殊控制

* **作用域控制**：在 C++ 中，大括号 `{ }` 定义了一个作用域。当对象在作用域内创建，执行到右大括号 `}` 时，该对象会被销毁并触发析构函数。
* **禁用实例化**：如果你只想把类当作工具集（全是静态成员），可以使用 `ClassName() = delete;` 明确告诉编译器不要生成构造函数，从而禁止他人实例化该类。

---

## 示例代码

```cpp
#include <iostream>

class Entity {
public:
    float X, Y;

    // 1. 默认构造函数：对象诞生时初始化
    Entity() {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Created Entity!" << std::endl;
    }

    // 2. 带参数的构造函数（重载）
    Entity(float x, float y) {
        X = x;
        Y = y;
        std::cout << "Created Entity with parameters!" << std::endl;
    }

    // 3. 析构函数：对象死亡前调用
    ~Entity() {
        std::cout << "Destroyed Entity!" << std::endl;
    }

    void Print() {
        std::cout << X << ", " << Y << std::endl;
    }
};

// 禁用实例化的例子
class Log {
public:
    // 显式删除构造函数，防止被实例化
    Log() = delete; 

    static void Write(const char* message) {
        std::cout << message << std::endl;
    }
};

int main() {
    // 测试作用域与生命周期
    {
        Entity e; // 调用默认构造函数
        e.Print();
        
        Entity e_param(10.5f, 5.0f); // 调用带参构造函数
        e_param.Print();
        
        std::cout << "--- End of Scope ---" << std::endl;
    } // 执行到此处时，e 和 e_param 的析构函数会被自动调用

    std::cout << "Back to main..." << std::endl;
    
    // Log::Write("Hello"); // 静态调用，不需要实例化
    
    std::cin.get();
}