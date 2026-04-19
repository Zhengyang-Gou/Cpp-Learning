# 05 - C++ 循环结构 (Loops)
## 1. 常见循环类型 (Loop Types)

循环结构用于重复执行某段代码，直到满足特定条件为止。C++ 中最常用的循环包括 `for` 循环和 `while` 循环。

| 类型名称 | 关键字 | 使用场景 |
| :--- | :--- | :--- |
| **For 循环** | `for` | 通常用于已知循环次数的情况（如遍历数组） |
| **While 循环** | `while` | 适用于当满足某个逻辑条件时持续执行，循环次数不一定固定 |

## 2. 循环控制语句 (Loop Control)

在循环执行过程中，我们可以使用特定的关键字来精确控制循环的走向。

| 关键字 | 含义 | 作用说明 |
| :--- | :--- | :--- |
| `break` | **打破/退出** | 立即终止当前循环，程序跳出循环体继续执行后面的代码 |
| `continue` | **跳过/继续** | 结束本次循环的剩余语句，直接开始下一次循环判定 |

---

## 示例代码

```cpp
#include <iostream>

int main()
{
    // 1. for 循环示例
    // 语法：for (初始化; 条件判断; 更新)
    for (int i = 0; i < 5; i++)
    {
        if (i > 2)
        {
            // 当 i 等于 3 或 4 时，跳过后面的打印语句
            continue; 
        }
        std::cout << "hello" << std::endl;
        std::cout << i << std::endl;
    }

    // 2. while 循环示例
    // 语法：while (条件判断)
    int i = 0;
    while (i < 5)
    {
        std::cout << "hello world" << std::endl;
        i++;
        
        if (i == 2)
        {
            // 当 i 等于 2 时，直接强制退出整个 while 循环
            break; 
        }
    }
    
    return 0;
}