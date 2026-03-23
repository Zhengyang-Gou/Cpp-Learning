/*
const 某种意义上是一个伪概念
它在最终编译生成的机器码中其实什么都不做
它完全是给编译器和写代码的人看的
*/
#include <iostream>

int main() {
    // 声明一个不能改变的变量
    const int MAX_AGE = 90;

    // 数据本身不能改，但指针的指向可以改
    const int* a = new int(5);
    a = (int*)&MAX_AGE;

    // 指针的指向不能改，但数据本身可以改
    int* const b = new int(5);
    *b = 2;

    // 都不能改
    const int* const c = new int(5);

}

class Entity {
// 在写类的时候，一定要把只读方法标记为 const
private:
    int m_X, m_Y;
public:
    // 这是一个 getter，它只读取数据，不修改数据，所以加上 const
    int GetX() const {
        // 如果你在这里尝试修改，编译器会直接报错
        return m_X;
    }

    // 这是一个 setter，它需要修改数据，所以绝对不能加 const
    void SetX(int x) {
        m_X = x;
    }
};