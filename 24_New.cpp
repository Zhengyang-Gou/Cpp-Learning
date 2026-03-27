/*
new 的时候，底层主要发生了两件事
寻找内存,返回这块内存的地址
调用构造函数,找到内存后，如果你的类型是一个类或结构体，new 会自动调用该类型的构造函数
*/

class Entity { 
public:
    float X, Y;

    void Move(float xa, float ya) {
        X += xa;
        Y += ya;
    }
};

int main() {
    // 例子 1：基本类型
    int* b = new int; // 在堆上分配了 4 个字节，返回地址给 b

    // 例子 2：数组
    int* array = new int[50]; // 在堆上分配了 200 个字节（50 * 4），用来装数组

    // 例子 3：类对象
    Entity* e = new Entity(); // 分配了 Entity 大小的内存，并且调用了 Entity() 构造函数！
    
    // delete 防止内存泄漏
    delete b;
    delete e;
    delete[] array;
}

// new 和 C 语言的 malloc 有什么区别
// 它只是一个函数，它只管给你指定字节数的Raw Memory，不管要在这块内存上放什么，也不会调用构造函数
