# 18 - C++ 字符串 (Strings)

## 1. C 风格字符串 (C-Style Strings)

在 C++ 中，如果直接使用双引号声明一个文本，它本质上是一个 C 风格字符串。它通过一个隐藏的**空字符 (Null Terminator, `\0`)** 来告诉程序字符串在哪里结束。

| 存储方式 | 特点 / 机制 | 代码示例 |
| :--- | :--- | :--- |
| **字符串常量指针** | 存储在最终生成的可执行文件的**只读数据段**，无法修改其内容。 | `const char* name = "Cherno";` |
| **字符数组** | 在栈上分配内存，允许修改其中的具体字符。 | `char nameArray[] = "Cherno";`<br>`nameArray[0] = 'A';` |

## 2. 字符类型与编码前缀 (Character Types & Encodings)

由于不同平台和语言对字符编码的需求不同，C++ 提供了多种字符类型及其对应的字面量前缀：

| 编码格式 | 前缀标志 | 数据类型 | 内存大小 | 适用场景 / 备注 |
| :--- | :--- | :--- | :--- | :--- |
| **UTF-8** | `u8` | `char` | 1 byte | C++20 引入，标准的单字节字符。 |
| **宽字符** | `L` | `wchar_t` | 2 字节或 4 字节 | Windows 上通常是 2 字节，Linux 上通常是 4 字节。 |
| **UTF-16** | `u` | `char16_t` | 2 bytes | 明确的 2 字节，常用于 C# 和 Java 互操作。 |
| **UTF-32** | `U` | `char32_t` | 4 bytes | 明确的 4 字节。 |

## 3. C++ 标准字符串 (`std::string`)

`std::string` 是 C++ 标准库提供的类，它封装了底层的字符数组，提供了更加安全和便捷的字符串操作。

| 核心功能 | 说明 | 代码示例 |
| :--- | :--- | :--- |
| **动态操作** | 支持直接使用 `+` 拼接，内置 `.size()`、`.find()` 等实用方法。 | `std::string g = "Hello" + name;` |
| **字面量后缀 (`s`)** | 引入 `std::string_literals` 后，加上 `s` 后缀，编译器会直接将其处理为 `std::string` 对象，而非原始数组。 | `auto Sname = "Cherno"s;` |
| **原始字符串 (Raw)** | 使用 `R"(...)"` 格式，完全无视转义字符，支持直接写包含斜杠的路径或多行文本。 | `const char* text = R"(C:\User)";` |

> **性能优化准则 (传参规范)**：
> 在向函数传递 `std::string` 时，**永远不要按值传递**（会触发极其昂贵的内存分配和字符串拷贝操作）。
> 应当使用 `const std::string&`（常量引用），这表示：“别复制！直接读取原始字符串，并且我保证绝对不会修改它”。

---

## 示例代码

```cpp
#include <iostream>
#include <string>
using namespace std::string_literals;

int main() {
    // 1. C 风格字符串
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

    // 2. 字符类型与编码
    // 1个字节 (UTF-8, C++20 引入 u8)
    const char* name1 = u8"Cherno"; 

    // 宽字符 (Windows 上通常是 2 字节，Linux 上是 4 字节)
    const wchar_t* name2 = L"Cherno"; 

    // 明确的 2 字节 (UTF-16，常用于 C# 和 Java 互操作)
    const char16_t* name3 = u"Cherno"; 

    // 明确的 4 字节 (UTF-32)
    const char32_t* name4 = U"Cherno";

    // 3. std::string
    std::string StringName = "m0NESY";
    std::string greeting = "Hello " + StringName; // 直接拼接

    bool contains = greeting.find("no") != std::string::npos;
    std::cout << "Length:" << greeting.size() << std::endl;

    // s 后缀
    // 不再是原始的字符数组了，编译器会直接把它变成一个 std::string 对象
    auto Sname = "Cherno"s; 

    // 4. Raw String Literals (原始字符串字面量)
    const char* goodText = R"(
    Line 1
    Line 2
    She said: "Hello!"
    C:\User\Desktop\Code // 连文件路径都不需要写双斜杠 \\ 了！
    )";

    std::cout << goodText << std::endl;
}

// 5. 参数传递最佳实践
// 不要按值传递 void PrintString(std::string text)
// 加了 &：别复制！直接用我原来的那个字符串
// 加了 const：我向你保证，这个函数只是读取它，绝对不会修改它。
void PrintString(const std::string& text) {
    std::cout << text << std::endl;
}
```