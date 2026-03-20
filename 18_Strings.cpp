#include <iostream>
#include <string>
using namespace std::string_literals;

int main() {
    // C 风格字符串
    // 它本质上是一个指针，指向一块连续的内存，里面存着 'C', 'h', 'e', 'r', 'n', 'o'
    // 不在堆上，也不在栈上，编译器会把它直接塞进你最终生成的可执行文件的只读数据段
    const char* name = "Cherno";

    // 当编译器看到 "Cherno" 时，内存里分配了 7 个字节，最后一个字节是一个数值为 0 的字符
    // std::cout 会一直向后打印，直到它一头撞上这个 0 为止 (Null Terminator)
    std::cout << name << std::endl;

    // 如果要修改一个用双引号写出来的字符串，应该把它定义成一个数组
    char nameArray[] = "Cherno"; 
    nameArray[0] = 'A'; 
    std::cout << nameArray << std::endl;

    // 1个字节 (UTF-8, C++20 引入 u8)
    const char* name1 = u8"Cherno"; 

    // 宽字符 (Windows 上通常是 2 字节，Linux 上是 4 字节)
    const wchar_t* name2 = L"Cherno"; 

    // 明确的 2 字节 (UTF-16，常用于 C# 和 Java 互操作)
    const char16_t* name3 = u"Cherno"; 

    // 明确的 4 字节 (UTF-32)
    const char32_t* name4 = U"Cherno";

    // std::string
    std::string StringName = "m0NESY";
    std::string greeting = "Hello" + StringName; // 直接拼接

    bool contains = greeting.find("no") != std::string::npos;
    std::cout << "Length:" << greeting.size() << std::endl;

    // s 后缀
    auto Sname = "Cherno"s; // 不再是原始的字符数组了，编译器会直接把它变成一个 std::string 对象

    // Raw String Literals
    const char* goodText = R"(
    Line 1
    Line 2
    She said: "Hello!"
    C:\User\Desktop\Code // 连文件路径都不需要写双斜杠 \\ 了！
    )";

    std::cout << goodText << std::endl;
}

// 不要按值传递 void PrintString(std::string text)
// 加了 &：别复制！直接用我原来的那个字符串
// 加了 const：我向你保证，这个函数只是读取它，绝对不会修改它。
void PrintString(const std::string& text) {
    std::cout << text << std::endl;
}