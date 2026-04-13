// . 和 -> 的区别
// ptr->x = (*ptr).x

// Arrow Operator 的真实含义
// 编译器会把 ptr->x 转换成 (*ptr).x
// 这就是为什么我们可以通过指针访问对象成员的原因
#include <memory>
class Entity
{
public:
    int x = 10;
};

int main()
{
    Entity e;
    Entity* ptr = &e;

    e.x = 5;        // 对象
    ptr->x = 20;    // 指针

    // 下面这行代码会创建一个 Entity 对象，并返回一个 unique_ptr 来管理它的生命周期
    std::unique_ptr<Entity> e1 = std::make_unique<Entity>();
    
    e1->x = 5;
}