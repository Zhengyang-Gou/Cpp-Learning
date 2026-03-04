#include <iostream>

int Multiply(int a, int b) 
{
    return a * b;
}

int main()
{
    int result = Multiply(3,4);
    std::cout << result << std::endl;
    // main 函数默认返回 0
}
