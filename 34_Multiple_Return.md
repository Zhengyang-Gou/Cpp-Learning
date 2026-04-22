# 52 - C++ 多返回值 (Multiple Return Values)

## 1. 基本概念 (Basic Concepts)

C++ 的函数在语法上一次只能 `return` 一个对象。  
但这个对象本身可以是一个“打包后的结果”，所以我们仍然可以实现“返回多个值”的效果。

常见做法有三种：

1. **通过引用参数输出多个结果**
2. **返回标准库打包类型**
   - `std::pair`
   - `std::tuple`
3. **返回自定义结构体**
   - `struct`
   - `class`

这一集的核心不是“怎么硬返回多个值”，而是：

> **如何用更清晰、更可维护的方式表达函数结果。**

---

## 2. 常见方案对比

| 方案 | 写法 | 优点 | 缺点 | 适用场景 |
| :--- | :--- | :--- | :--- | :--- |
| **引用输出参数** | `void Func(int& a, int& b)` | 不需要额外包装类型，直接修改外部变量 | 调用点不直观，输入输出不易区分 | 旧代码、底层接口、性能敏感场景 |
| **`std::pair`** | `return {x, y};` | 简单，适合返回两个值 | 只有 `first` / `second`，语义弱 | 两个简单值 |
| **`std::tuple`** | `return std::make_tuple(x, y, z);` | 可返回多个不同类型 | `std::get<>()` 可读性一般 | 临时打包、多类型结果 |
| **自定义 `struct`** | `return Result{...};` | 字段有名字，可读性最好 | 需要先定义类型 | **最推荐，工程里最清晰** |

---

## 3. 方案一：引用参数 (Output Parameters)

通过引用参数，可以让函数直接修改调用者传入的变量。

### 示例代码

```cpp
#include <iostream>
#include <string>

void GetNameAndAge(std::string& name, int& age)
{
    name = "Cherno";
    age = 24;
}

int main()
{
    std::string name;
    int age;

    GetNameAndAge(name, age);

    std::cout << name << ", " << age << std::endl;
    return 0;
}
```

### 说明

- `name` 和 `age` 不是函数返回值
- 它们是通过**引用**传进去的
- 函数内部直接修改了外部变量

### 特点

**优点：**
- 直接
- 不需要额外定义类型
- 在某些底层代码中常见

**缺点：**
- 函数签名不够直观
- 难以一眼分辨哪些参数是输入，哪些是输出
- 参数多时调用点会变乱

---

## 4. 方案二：`std::pair`

如果只需要返回两个值，可以用 `std::pair`。

### 示例代码

```cpp
#include <iostream>
#include <utility>
#include <string>

std::pair<std::string, int> CreatePerson()
{
    return { "Cherno", 24 };
}

int main()
{
    std::pair<std::string, int> person = CreatePerson();

    std::cout << person.first << ", " << person.second << std::endl;
    return 0;
}
```

### 说明

- `std::pair` 用来打包两个值
- 访问成员使用：
  - `first`
  - `second`

### 特点

**优点：**
- 简单
- 快速
- 适合小型返回值

**缺点：**
- `first` / `second` 没有业务语义
- 代码可读性一般
- 稍复杂时不利于维护

---

## 5. 方案三：`std::tuple`

如果要返回多个不同类型的值，可以用 `std::tuple`。

### 示例代码

```cpp
#include <iostream>
#include <tuple>
#include <string>

std::tuple<std::string, int, float> GetPlayerInfo()
{
    return std::make_tuple("PlayerOne", 100, 92.5f);
}

int main()
{
    auto data = GetPlayerInfo();

    std::cout << std::get<0>(data) << std::endl;
    std::cout << std::get<1>(data) << std::endl;
    std::cout << std::get<2>(data) << std::endl;

    return 0;
}
```

### 说明

- `tuple` 可以打包任意多个值
- 通过 `std::get<索引>()` 访问成员

### 特点

**优点：**
- 灵活
- 支持多个值和不同类型
- 临时组合数据方便

**缺点：**
- `std::get<0>()`、`std::get<1>()` 可读性较差
- 不容易从代码表面看出每个位置代表什么
- 结果稍复杂时不如自定义类型清晰

---

## 6. 方案四：自定义 `struct`（推荐）

如果多个返回值本来就属于同一个结果对象，最清晰的做法是定义一个结构体。

### 示例代码

```cpp
#include <iostream>
#include <string>

struct FilePathInfo
{
    std::string Directory;
    std::string Filename;
    std::string Extension;
};

FilePathInfo ParsePath()
{
    return { "assets/textures", "brick", "png" };
}

int main()
{
    FilePathInfo info = ParsePath();

    std::cout << "Directory: " << info.Directory << std::endl;
    std::cout << "Filename: " << info.Filename << std::endl;
    std::cout << "Extension: " << info.Extension << std::endl;

    return 0;
}
```

### 说明

- 这里函数仍然只返回了**一个对象**
- 这个对象是 `FilePathInfo`
- 但它内部包含多个有意义的字段

### 特点

**优点：**
- 字段名清晰
- 语义明确
- 可维护性最好
- 最适合工程代码

**缺点：**
- 需要多定义一个类型
- 对于特别简单的情况可能显得稍微啰嗦

---

## 7. 核心思想

### 重点 1：函数表面上只能返回一个值，但这个值可以是复合对象

也就是说，  
“多个返回值”本质上是**把多个结果组织成一个对象再返回**。

### 重点 2：引用参数虽然能实现效果，但接口不够优雅

它是可行方案，  
但从表达力和可读性来看，不是最理想的现代写法。

### 重点 3：`pair` / `tuple` 适合临时打包

它们在简单场景下很好用，  
但一旦业务语义增强，就容易变得难读。

### 重点 4：最推荐的是 `struct`

只要多个返回值之间存在关联，  
就应该考虑把它们建模成一个有名字的结果类型。

---

## 8. 实战建议

### 当你只想临时返回两个简单值时

可以用：

```cpp
std::pair<T1, T2>
```

### 当你要快速返回多个不同类型时

可以用：

```cpp
std::tuple<T1, T2, T3>
```

### 当这些数据本来就属于同一种结果时

优先考虑：

```cpp
struct Result
{
    ...
};
```

