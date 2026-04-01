// 在 C++ 中，像 +、-、*、/ 甚至 == 这样的运算符，本质上只不过是函数而已
// 如果你重载的运算符，不能让人第一眼就凭常识猜出它是干什么的，那就绝对不要用重载，去写一个普通的函数
#include <iostream>

struct Vector2 {
    float x, y;
    Vector2(float x, float y) : x(x), y(y) {}
};

// 没有重载运算符
struct Vector2_no_operator {
    float x, y;
    Vector2_no_operator(float x, float y) : x(x), y(y) {}

    Vector2_no_operator Add(const Vector2_no_operator& other) const {
        return Vector2_no_operator(x + other.x, y + other.y);
    }

    Vector2_no_operator Multiply(const Vector2_no_operator& other) const {
        return Vector2_no_operator(x * other.x, y * other.y);
    }
};

// 重载 + 运算符
struct Vector2_operator {
    float x, y;
    Vector2_operator(float x, float y) : x(x), y(y) {}

    Vector2_operator operator+(const Vector2_operator& other) const {
        return Vector2_operator(x + other.x, y + other.y);
    }

    Vector2_operator operator*(const Vector2_operator& other) const {
        return Vector2_operator(x * other.x, y * other.y);
    }
};

int main() {
    Vector2_no_operator a(1.0f, 2.0f);
    Vector2_no_operator b(3.0f, 4.0f);
    Vector2_no_operator c = a.Add(b); // 使用成员函数进行加法
    std::cout << "Without operator overloading: " << c.x << ", " << c.y << std::endl;

    Vector2_operator d(1.0f, 2.0f);
    Vector2_operator e(3.0f, 4.0f);
    Vector2_operator f = d + e; // 使用重载的 + 运算符进行加法
    std::cout << "With operator overloading: " << f.x << ", " << f.y << std::endl;

    return 0;
}