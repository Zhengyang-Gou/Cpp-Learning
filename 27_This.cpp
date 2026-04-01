// this 就是一个实实在在的指针, 它包含了当前这个对象实例在内存中的具体地址

#include <iostream>

// 解决成员变量与参数同名问题
class Person {
private:
    int age;

public:
    void setAge(int age) {
        this->age = age;   // 左边是成员变量，右边是参数
    }

    void printAge() {
        std::cout << age << std::endl;
    }
};

// 在成员函数中返回当前对象
class Counter {
private:
    int value;

public:
    Counter(int v) {
        value = v;
    }

    Counter& add(int x) {
        value += x;
        return *this;  // 返回当前对象
    }

    void print() {
        std::cout << value << std::endl;
    }
};

// 显式访问当前对象地址
class Test {
public:
    void show() {
        std::cout << "this address: " << this << std::endl;
    }
};

int main() {
    Person p;
    p.setAge(20);
    p.printAge();

    Counter c(10);
    c.add(5).add(3).print();  // 链式调用

    Test t;
    t.show();
}