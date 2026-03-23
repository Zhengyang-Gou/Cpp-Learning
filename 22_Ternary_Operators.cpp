#include <iostream>

int main() {
    // 传统写法
    int level = 6;
    int speed;

    if (level > 5) {
        speed = 10;
    }
    else {
        speed = 5;
    }

    // 三元运算符
    const int speed = level > 5 ? 10 : 5;
}