# 58 - C++ 中如何处理可选数据 (How to Deal with Optional Data in C++)

## 1. 基本概念

**optional data** 指的是：一个值可能存在，也可能不存在。

在 C++17 中，可以用：

```cpp
std::optional<T>
```

来表达这种情况。

例如：

```cpp
std::optional<std::string>
```

表示：

```text
这里可能有一个 std::string，也可能没有任何值。
```

它定义在：

```cpp
#include <optional>
```

`std::optional` 的核心作用是：**把“值不存在”这件事明确写进 type 里。**

例如一个函数读取文件内容：

```cpp
std::optional<std::string> ReadFileAsString(const std::string& path);
```

这个返回值直接告诉调用者：

```text
读取成功：返回 string
读取失败：返回 empty optional
```

这比返回空字符串、返回 `nullptr` 或额外传出一个 `bool` 更清楚。

---

## 2. 为什么需要它

很多函数并不总是能返回有效值。

例如：

- 文件可能不存在；
- 配置项可能没有设置；
- map 中可能找不到 key；
- 用户输入可能无效；
- 网络请求可能失败；
- 查询结果可能为空。

如果不用 `std::optional`，常见写法可能是返回一个特殊值：

```cpp
std::string ReadFile()
{
    if (failed)
        return "";

    return contents;
}
```

问题是：空字符串本身也可能是合法结果。

例如一个文件存在，但内容本来就是空的。这时你无法区分：

```text
文件读取失败
文件读取成功，但内容为空
```

`std::optional` 可以明确区分这两种情况：

```cpp
return std::nullopt;       // 没有值
return std::string("");    // 有值，只是字符串为空
```

这就是它的价值。

---

## 3. 核心语法

### 3.1 创建 optional

```cpp
std::optional<int> value;
```

此时 `value` 没有保存任何 `int`。

也可以直接给值：

```cpp
std::optional<int> value = 10;
```

---

### 3.2 表示没有值

```cpp
std::nullopt
```

例如：

```cpp
return std::nullopt;
```

也可以写：

```cpp
return {};
```

但 `std::nullopt` 更明确。

---

### 3.3 判断是否有值

```cpp
if (value.has_value())
{
}
```

更常见的简洁写法：

```cpp
if (value)
{
}
```

`std::optional` 可以在 `if` 中被当作 bool 使用，表示它是否包含值。

---

### 3.4 取出值

```cpp
value.value()
```

或者：

```cpp
*value
```

例如：

```cpp
std::cout << value.value() << std::endl;
```

注意：如果 optional 没有值，直接调用 `.value()` 会抛出异常；直接 `*value` 则是不安全行为。

---

### 3.5 提供默认值

```cpp
value.value_or(defaultValue)
```

例如：

```cpp
int result = value.value_or(0);
```

如果 `value` 有值，返回其中的值；如果没有值，返回 `0`。

---

## 4. 示例代码

```cpp
#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string> ReadFileAsString(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file)
    {
        return std::nullopt;
    }

    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}

int main()
{
    std::optional<std::string> contents = ReadFileAsString("data.txt");

    if (contents)
    {
        std::cout << "File contents:\n";
        std::cout << contents.value() << std::endl;
    }
    else
    {
        std::cout << "Could not read file." << std::endl;
    }

    std::string text = contents.value_or("Default content");

    std::cout << "Text used by program:\n";
    std::cout << text << std::endl;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 返回 `std::optional<std::string>`

```cpp
std::optional<std::string> ReadFileAsString(const std::string& filepath)
```

这个函数不再单纯返回 `std::string`，而是返回一个“可能存在的 string”。

它表达了两种结果：

```text
有值：文件读取成功
无值：文件读取失败
```

调用者必须面对失败情况，而不是假装函数一定成功。

---

### 5.2 `return std::nullopt;`

```cpp
if (!file)
{
    return std::nullopt;
}
```

如果文件打开失败，就返回 `std::nullopt`。

这比返回空字符串更准确，因为空字符串可能是合法文件内容。

---

### 5.3 `if (contents)`

```cpp
if (contents)
{
    std::cout << contents.value() << std::endl;
}
```

这里判断 `contents` 是否包含值。

如果有值，才读取它。

这是使用 `std::optional` 时最重要的习惯：**先检查，再取值。**

---

### 5.4 `.value()`

```cpp
contents.value()
```

返回 optional 中真正保存的 `std::string`。

但只有在确定它有值时才应该调用。

如果 optional 没有值，`.value()` 会抛出：

```cpp
std::bad_optional_access
```

所以不要随手写：

```cpp
contents.value()
```

而不做检查。

---

### 5.5 `value_or`

```cpp
std::string text = contents.value_or("Default content");
```

如果 `contents` 有值，`text` 使用文件内容。

如果没有值，`text` 使用默认字符串。

这适合配置读取、默认参数、可选用户输入等场景。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 明确表达“可能没有值” | 需要 C++17 |
| 比特殊返回值更安全 | 调用者必须处理空值情况 |
| 避免空字符串、`-1`、`nullptr` 这类模糊约定 | 不适合表示复杂错误原因 |
| 可读性好 | `std::optional<T&>` 不被标准支持 |
| 适合返回可选结果 | 过度使用会让简单逻辑变复杂 |

