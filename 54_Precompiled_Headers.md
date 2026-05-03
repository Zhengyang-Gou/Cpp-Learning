# 54 - C++ 中的预编译头文件 (Precompiled Headers in C++)

## 1. 基本概念

**precompiled header**，简称 **PCH**，中文通常叫“预编译头文件”。

它的作用是：把一些很少变化、但被大量 `.cpp` 文件反复包含的 header 提前编译好，之后编译其他文件时直接复用这份结果，从而减少编译时间。

在 C++ 中，`#include` 本质上是文本替换。比如：

```cpp
#include <iostream>
#include <vector>
#include <string>
```

编译器会把这些 header 的内容展开到当前 `.cpp` 文件里。标准库 header 往往非常大，如果每个 `.cpp` 都重复解析它们，编译会很慢。

precompiled header 就是为了解决这个问题。

常见命名：

```cpp
pch.h
```

或者 Visual Studio 中常见：

```cpp
stdafx.h
```

核心是：

- PCH 不改变程序运行性能；
- PCH 主要优化 compile time；
- 适合放稳定、常用、不经常改的 header；
- 不应该把项目中频繁变化的业务 header 放进去。

---

## 2. 为什么需要它

C++ 编译速度慢的一个重要原因是 header 机制。

假设项目中有很多 `.cpp` 文件：

```text
Renderer.cpp
Application.cpp
Layer.cpp
Input.cpp
Window.cpp
```

每个文件都包含：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
```

这些标准库 header 会被每个 translation unit 重复解析。

如果项目越来越大，编译器会花大量时间处理同样的 header 内容。

precompiled header 的目标是：

```text
把重复、稳定、昂贵的 header 编译一次，然后复用。
```

它不会让程序运行更快，但可以让项目编译更快，尤其是大型 C++ 项目。

---

## 3. 核心语法 

### 3.1 创建 `pch.h`

通常会创建一个专门的 header：

```cpp
// pch.h
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <unordered_map>
```

这里放的是常用且稳定的 header。

---

### 3.2 创建 `pch.cpp`

很多构建系统或 IDE 需要一个 `.cpp` 文件来生成 PCH：

```cpp
// pch.cpp
#include "pch.h"
```

这个文件通常只包含 `pch.h`。

---

### 3.3 在其他 `.cpp` 中包含 PCH

使用 PCH 的 `.cpp` 文件通常要先包含：

```cpp
#include "pch.h"
```

例如：

```cpp
#include "pch.h"
#include "Application.h"

void RunApplication()
{
    std::cout << "Running...\n";
}
```

在一些编译器或 IDE 设置中，PCH 必须是 `.cpp` 文件里第一个 include。

---

### 3.4 Visual Studio 中的常见写法

Visual Studio 早期项目中经常使用：

```cpp
#include "stdafx.h"
```

现代项目中更常见：

```cpp
#include "pch.h"
```

名字不重要，本质都是 precompiled header。

---

## 4. 示例代码

```cpp
// pch.h
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
```

```cpp
// pch.cpp
#include "pch.h"
```

```cpp
// Entity.h
#pragma once

#include <string>

class Entity
{
public:
    Entity(const std::string& name)
        : m_Name(name)
    {
    }

    const std::string& GetName() const
    {
        return m_Name;
    }

private:
    std::string m_Name;
};
```

```cpp
// main.cpp
#include "pch.h"
#include "Entity.h"

int main()
{
    std::vector<Entity> entities;

    entities.emplace_back("Player");
    entities.emplace_back("Enemy");
    entities.emplace_back("NPC");

    for (const Entity& entity : entities)
    {
        std::cout << entity.GetName() << std::endl;
    }

    return 0;
}
```

---

## 5. 重点解释

### 5.1 `pch.h` 放什么

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
```

这些 header 很常用，也很少修改，适合放进 PCH。

常见适合放入 PCH 的内容：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <functional>
```

也可以放一些稳定的第三方库 header，例如大型引擎中经常用到的基础库。

---

### 5.2 `pch.h` 不该放什么

不推荐把频繁变化的项目 header 放进 PCH：

```cpp
#include "Player.h"
#include "Renderer.h"
#include "Scene.h"
```

原因是：PCH 一旦依赖这些经常变化的文件，就会频繁重新生成，反而降低编译效率。

PCH 应该稳定。如果你每改一个普通 class 都导致 PCH 重新编译，那就失去了它的意义。

---

### 5.3 PCH 优化的是编译时间，不是运行时间

precompiled header 只影响 build process。

它不会让下面代码运行更快：

```cpp
std::vector<int> values;
```

它只是让编译器在处理 `std::vector` 相关 header 时少做重复工作。

所以：

```text
PCH improves compile time, not runtime performance.
```

---

### 5.4 为什么要有 `pch.cpp`

```cpp
#include "pch.h"
```

`pch.cpp` 通常用来告诉构建系统：

```text
请用这个 translation unit 生成 precompiled header。
```

具体行为取决于编译器和构建系统。

在 Visual Studio 中，可以设置某个 `.cpp` 用于创建 PCH，其他 `.cpp` 使用 PCH。

---

### 5.5 PCH 和 include 的关系

使用 PCH 并不意味着不需要正确 include。

如果某个 header 文件本身依赖 `std::string`，它最好自己 include：

```cpp
#include <string>
```

而不是偷偷依赖 `pch.h`。

原因是 header 应该自洽。别人单独 include 这个 header 时，也应该能正常编译。

---

## 6. 优点和缺点

| 优点 | 缺点 |
| :--- | :--- |
| 减少重复解析 header 的时间 | 配置比普通 include 更复杂 |
| 大型项目中能明显加快编译 | PCH 改动会导致大量重新编译 |
| 适合标准库和稳定第三方库 | 可能掩盖不完整 include 问题 |
| 对开发迭代速度有帮助 | 不同编译器配置方式不同 |
| 不影响运行逻辑 | 滥用会让项目依赖关系变混乱 |
