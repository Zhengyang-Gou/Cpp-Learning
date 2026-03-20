/*
连续内存：
如果需要存 5 个整数，用 int 这 5 个 变量在内存中是散落的
如果使用数组，会在栈上划分连续的内存区域
*/

#include <iostream>

int main()
{
    int example[5]; // 创建包含 5 个整数的数组

    // example 是指向这块连续内存首地址的指针，指向 example[0] 的内存地址
    // example[2] = 5; 这种写法，会被翻译成指针运算：*(example + 2) = 5

    int count = sizeof(example) / sizeof(int);
    std::cout << count << std::endl;
}

// 当你把数组传进函数时，它不再是数组了，它退化成了一个普通的 int* 指针
void PrintArray(int* arr) {
    // 这里的 sizeof(arr) 只是一个指针的大小（通常是 8 字节）
    // 8 / 4 = 2，得不到正确的数组长度
    int count = sizeof(arr) / sizeof(int); 
}