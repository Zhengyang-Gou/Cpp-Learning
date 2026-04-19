# 01 - C++ 变量与数据类型 (Variables & Data Types)

## 1. 整数类型 (Integer Types)

整数类型用于存储没有小数部分的数字。在 C++ 中，可以通过 `unsigned` 关键字来声明无符号类型（只存储正数和 0），从而使正数的存储范围翻倍。

| 类型名称 | 关键字 | 内存大小 | 取值范围 |
| :--- | :--- | :--- | :--- |
| **字符型** | `char` | 1 byte | -128 ~ 127 |
| **无符号字符型** | `unsigned char` | 1 byte | 0 ~ 255 |
| **短整型** | `short` | 2 bytes | -32,768 ~ 32,767 |
| **无符号短整型** | `unsigned short` | 2 bytes | 0 ~ 65,535 |
| **整型** | `int` | 4 bytes | -2³¹ ~ 2³¹-1 |
| **无符号整型** | `unsigned int` | 4 bytes | 0 ~ 2³²-1 |
| **长长整型** | `long long` | 8 bytes | -2⁶³ ~ 2⁶³-1 |
| **无符号长长整型**| `unsigned long long` | 8 bytes | 0 ~ 2⁶⁴-1 |

## 2. 浮点类型 (Floating Point Types)

浮点类型用于存储带有小数部分的数字。后缀 `f` 用于明确告诉编译器这是一个 `float` 类型，否则默认带有小数的数字为 `double` 类型。

| 类型名称 | 关键字 | 内存大小 | 取值范围 (数量级) |
| :--- | :--- | :--- | :--- |
| **单精度浮点型** | `float` | 4 bytes | 1.2E-38 ~ 3.4E+38 |
| **双精度浮点型** | `double` | 8 bytes | 2.3E-308 ~ 1.7E+308 |

## 3. 其他类型 (Other Types)

| 类型名称 | 关键字 | 内存大小 | 含义 / 取值范围 |
| :--- | :--- | :--- | :--- |
| **布尔型** | `bool` | 1 byte | `true` (1) / `false` (0) |

---

## 示例代码

```cpp
int main() 
{
    // 1. 整数类型 (Integer Types) 
    char c = 'A';                    
    unsigned char uc = 255;          
    
    short s = 8;                     
    unsigned short us = 8;           
    
    int i = 8;                       
    unsigned int ui = 8;             
    
    long long ll = 8;                
    unsigned long long ull = 8;      

    // 2. 浮点类型 (Floating Point Types) 
    float f = 5.5f;                  
    double d = 5.5;                  

    // 3. 其他类型 (Other Types) 
    bool b = true;                   
    
    return 0;
}
```
