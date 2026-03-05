#include <iostream>

void Plus1(int Value) // var 的值赋值给 Value，不改变 var 的值 
{
    Value++;
}

void Plus(int& value) // value 等价于 var，会改变 var 的值
{
    value++;
}

int main()
{
    int var = 8;
    Plus(var); 
    Plus1(var);

    int& ref = var; // ref 是引用，不创建新的变量
    std::cout << var << std::endl;
}