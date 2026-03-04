#include <iostream>

int main()
{
    int num;
    std::cin >> num;
    
    if (num == 5)
        std::cout << "hello world" << std::endl;
    else
        if (num == 6)
        std::cout << "hello" << std::endl;
    else
        std::cout << "nothing" << std::endl;
}