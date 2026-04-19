# 04 - C++ 条件分支 (Conditions)

## 1. 基础条件判断 (Basic Conditions)

C++ 使用 `if` 和 `else` 关键字来根据表达式的真假（bool 值）控制程序的执行流。当 `if` 后面的条件为 `true` 时，执行紧随其后的语句；否则，程序将跳过该语句或进入 `else` 分支。

## 2. 关系运算符 (Relational Operators)

在 `if` 语句的括号内，我们通常使用关系运算符来比较数值。

| 运算符 | 含义 | 示例 | 结果 |
| :--- | :--- | :--- | :--- |
| `==` | **等于** | `num == 5` | 若 num 为 5 则为 true |
| `!=` | **不等于** | `num != 5` | 若 num 不为 5 则为 true |
| `>` | **大于** | `num > 5` | 若 num 大于 5 则为 true |
| `<` | **小于** | `num < 5` | 若 num 小于 5 则为 true |
| `>=` | **大于等于** | `num >= 5` | 若 num 大于或等于 5 则为 true |
| `<=` | **小于等于** | `num <= 5` | 若 num 小于或等于 5 则为 true |

## 3. 嵌套 if-else 结构 (Nested Structure)

当存在两个以上的逻辑分支时，可以在 `else` 语句中嵌套另一个 `if` 语句，或者直接使用 `else if`。

| 结构类型 | 描述 |
| :--- | :--- |
| **单分支** | 仅使用 `if`，条件不满足则什么都不做 |
| **双分支** | 使用 `if...else`，处理两种对立的情况 |
| **多分支** | 使用嵌套或 `else if`，处理多种并列的情况 |

---

## 示例代码

```cpp
#include <iostream>

int main()
{
    int num;
    // 从标准输入读取一个整数
    std::cin >> num;
    
    // 1. 第一层判断：检查是否等于 5
    if (num == 5)
    {
        std::cout << "hello world" << std::endl;
    }
    // 2. 第二层嵌套：如果不等于 5，则进入 else
    else
    {
        // 再次判断是否等于 6
        if (num == 6)
        {
            std::cout << "hello" << std::endl;
        }
        // 3. 兜底逻辑：既不等于 5 也不等于 6
        else
        {
            std::cout << "nothing" << std::endl;
        }
    }
    
    return 0;
}