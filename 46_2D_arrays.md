# 46 - C++ 中的多维数组（2D arrays）

## 1. 基本概念 

**multidimensional array** 指的是“多维数组”。最常见的是二维数组，也就是 **2D array**。

一维数组可以理解成一排数据：

```text
[0] [1] [2] [3] [4]
```

二维数组可以理解成表格、网格或矩阵：

```text
[0][0] [1][0] [2][0]
[0][1] [1][1] [2][1]
[0][2] [1][2] [2][2]
```

在 C++ 里，二维数组经常用来表示：

- 棋盘；
- 地图 tile；
- 图像像素；
- 矩阵；
- 表格数据；
- 游戏中的 grid；
- 二维坐标系统中的数据。

重点是：

> 多维数组本质上仍然是一块内存，只是我们用多个索引去理解它。

也就是说，计算机内存本质上是线性的：

```text
[0] [1] [2] [3] [4] [5] [6] [7] [8]
```

二维数组只是我们人为地把它解释成：

```text
[0][0] [1][0] [2][0]
[0][1] [1][1] [2][1]
[0][2] [1][2] [2][2]
```

C++ 中可以用不同方式表示二维数组：

1. 固定大小的 stack array；
2. `int**` 形式的 pointer-to-pointer；
3. 手动映射的一维动态数组；
4. `std::array` 或 `std::vector` 等现代 C++ 容器。

底层内存角度：二维数组如何在内存中组织，以及为什么很多时候用一维数组模拟二维数组会更好。

---

## 2. 为什么需要它

很多数据天然就不是一维的。

例如一个 5x5 的地图：

```text
0 0 0 0 0
0 1 1 1 0
0 1 2 1 0
0 1 1 1 0
0 0 0 0 0
```

如果用一维数组表示，代码可能是：

```cpp
int map[25];
```

这样虽然可以存储 25 个元素，但访问时不直观：

```cpp
map[12] = 2;
```

`12` 到底代表地图上的哪个位置？不明显。

如果使用二维概念，就更自然：

```cpp
map[x][y] = 2;
```

这样可以直接表达：

```text
第 x 列，第 y 行
```

二维数组解决的问题是：**让我们更自然地表达二维结构的数据。**

不过，C++ 的底层内存仍然是一维的。二维数组的核心问题是：

> 如何把二维坐标 `(x, y)` 映射到线性内存中的一个位置？

例如一个宽度为 `width` 的二维数组，坐标 `(x, y)` 对应的一维索引通常是：

```cpp
index = x + y * width;
```
---

## 3. 核心语法

### 3.1 固定大小的二维数组

如果数组大小在编译期已知，可以直接写：

```cpp
int array[5][5];
```

访问元素：

```cpp
array[0][0] = 1;
array[2][3] = 10;
```

这种写法简单直接，但大小必须在编译期确定。

---

### 3.2 使用 `new` 创建 pointer-to-pointer 二维数组

The Cherno 这一集中会展示类似这种写法：

```cpp
int** array = new int*[5];

for (int i = 0; i < 5; i++)
{
    array[i] = new int[5];
}
```

这里：

```cpp
int** array
```

可以理解为：

- `array` 是一个 pointer；
- 它指向一组 `int*`；
- 每个 `int*` 又指向一组 `int`。

结构类似这样：

```text
array
 |
 +--> row 0: [int][int][int][int][int]
 +--> row 1: [int][int][int][int][int]
 +--> row 2: [int][int][int][int][int]
 +--> row 3: [int][int][int][int][int]
 +--> row 4: [int][int][int][int][int]
```

访问方式：

```cpp
array[2][3] = 7;
```

释放内存时，必须先释放每一行，再释放最外层 pointer array：

```cpp
for (int i = 0; i < 5; i++)
{
    delete[] array[i];
}

delete[] array;
```

---

### 3.3 用一维数组模拟二维数组

更推荐的底层写法是：

```cpp
int* array = new int[5 * 5];
```

访问二维坐标时手动计算 index：

```cpp
int x = 2;
int y = 3;
int width = 5;

array[x + y * width] = 7;
```

这里：

```cpp
x + y * width
```

就是把二维坐标转换成一维索引。

释放内存：

```cpp
delete[] array;
```

这种写法内存连续，cache locality 更好，管理起来也更简单。

---

### 3.4 三维数组的索引计算

如果是三维数组，例如：

```text
width  = 5
height = 5
depth  = 5
```

可以用一维数组：

```cpp
int* array = new int[width * height * depth];
```

三维坐标 `(x, y, z)` 对应的 index：

```cpp
index = x + y * width + z * width * height;
```

访问：

```cpp
array[x + y * width + z * width * height] = 10;
```

这说明多维数组的本质仍然是线性内存映射。

---

## 4. 示例代码

```cpp
#include <iostream>

int main()
{
    const int width = 5;
    const int height = 5;

    int* grid = new int[width * height];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            grid[x + y * width] = x + y * width;
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            std::cout << grid[x + y * width] << "\t";
        }

        std::cout << std::endl;
    }

    delete[] grid;

    return 0;
}
```

---

## 5. 重点解释

### 5.1 为什么只分配一次内存

代码中使用：

```cpp
int* grid = new int[width * height];
```

这里并没有创建真正的 `int**`，而是创建了一整块连续内存。

如果：

```cpp
width = 5;
height = 5;
```

那么：

```cpp
width * height
```

就是：

```cpp
25
```

所以这行代码等价于创建 25 个 `int`：

```cpp
int* grid = new int[25];
```

只是我们把这 25 个元素当成 5x5 的二维表格来使用。

---

### 5.2 `x + y * width` 的含义

二维坐标要转换成一维索引。

假设宽度是 5：

```text
row 0: index 0  1  2  3  4
row 1: index 5  6  7  8  9
row 2: index 10 11 12 13 14
row 3: index 15 16 17 18 19
row 4: index 20 21 22 23 24
```

如果访问 `(x = 2, y = 3)`：

```cpp
index = x + y * width;
```

代入：

```cpp
index = 2 + 3 * 5;
```

结果：

```cpp
index = 17;
```

所以：

```cpp
grid[2 + 3 * 5]
```

也就是：

```cpp
grid[17]
```

它表示第 3 行、第 2 列的元素。

---

### 5.3 为什么外层循环通常是 `y`

示例中打印二维网格时使用：

```cpp
for (int y = 0; y < height; y++)
{
    for (int x = 0; x < width; x++)
    {
        std::cout << grid[x + y * width] << "\t";
    }

    std::cout << std::endl;
}
```

外层循环是 `y`，表示逐行输出。

内层循环是 `x`，表示一行里面从左到右输出。

这和二维表格的阅读方式一致：

```text
先处理第 0 行的所有列
再处理第 1 行的所有列
再处理第 2 行的所有列
```

---

### 5.4 为什么一维数组模拟二维数组更高效

如果使用 `int**`：

```cpp
int** array = new int*[height];

for (int y = 0; y < height; y++)
{
    array[y] = new int[width];
}
```

这里会发生多次内存分配：

1. 分配一组 `int*`；
2. 每一行再单独分配一组 `int`。

这些行不一定连续存在于内存中。

而使用一维数组：

```cpp
int* grid = new int[width * height];
```

只分配一次，并且所有元素连续排列在内存中。

连续内存通常有更好的 **cache locality**。

CPU 访问内存时，不只是读取一个元素，通常会把附近的一段内存也加载到 cache 中。如果数组连续，访问下一个元素时很可能已经在 cache 里，速度会更快。

---

### 5.5 `delete[] grid;` 为什么只需要一次

因为我们只调用了一次：

```cpp
new int[width * height]
```

所以只需要一次：

```cpp
delete[] grid;
```

这和 `int**` 写法不同。

如果使用 `int**`，每一行都单独 `new`，就必须每一行都 `delete[]`：

```cpp
for (int y = 0; y < height; y++)
{
    delete[] array[y];
}

delete[] array;
```

一维数组写法不仅更高效，也更不容易写错释放逻辑。

---

### 5.6 固定二维数组和动态二维数组的区别

固定二维数组：

```cpp
int array[5][5];
```

这种数组通常在 stack 上分配，大小在编译期确定。

动态一维数组：

```cpp
int* array = new int[width * height];
```

这种数组在 heap 上分配，大小可以在运行时决定。

例如：

```cpp
int width;
int height;

std::cin >> width >> height;

int* grid = new int[width * height];
```

这就是动态分配的优势：数组大小可以在程序运行时才确定。

不过现代 C++ 中，通常更推荐使用 `std::vector` 管理动态数组，而不是直接手写 `new` 和 `delete[]`。

---

## 7. 优点和缺点

### 7.1 `int**` 多维数组写法

| 优点 | 缺点 |
| :--- | :--- |
| 语法上可以使用 `array[y][x]` 或类似形式访问 | 需要多次内存分配 |
| 看起来像真正的二维数组 | 每一行内存不一定连续 |
| 每一行可以有不同长度 | cache locality 较差 |
| 容易理解成“数组的数组” | 释放内存麻烦，容易 memory leak |
| 可以表达 jagged array | 多了一层 pointer indirection |

---

### 7.2 一维数组模拟二维数组

| 优点 | 缺点 |
| :--- | :--- |
| 只需要一次内存分配 | 访问时需要手动计算 index |
| 内存连续，cache locality 更好 | `x + y * width` 初学者容易写错 |
| 释放内存简单 | 代码可读性不如 `array[y][x]` 直观 |
| 更接近底层内存模型 | 需要保存 width、height 信息 |
| 性能通常更好 | 越界访问时不容易被发现 |

---

### 7.3 固定二维数组

| 优点 | 缺点 |
| :--- | :--- |
| 语法简单 | 大小必须在编译期确定 |
| 可以直接写 `array[y][x]` | 不适合运行时动态尺寸 |
| 内存通常连续 | 大数组放 stack 上可能导致 stack overflow |
| 不需要手动 `delete[]` | 不能方便地改变大小 |

---

## 8. 和其他相关概念的区别

### 8.1 一维数组 vs 二维数组

一维数组：

```cpp
int array[10];
```

适合表示线性数据，例如：

- 分数列表；
- ID 列表；
- 一组对象；
- 连续 buffer。

二维数组：

```cpp
int array[5][5];
```

适合表示二维结构，例如：

- 地图；
- 表格；
- 棋盘；
- 图像；
- 矩阵。

不过在内存中，二维数组也会以线性方式存储。区别主要在于我们如何解释和访问它。

---

### 8.2 `int*` vs `int**`

`int*` 是 pointer to int：

```cpp
int* data = new int[25];
```

它指向一块连续的 `int` 数组。

`int**` 是 pointer to pointer to int：

```cpp
int** data = new int*[5];
```

它指向一组 `int*`，每个 `int*` 又可以指向一行 `int`。

区别如下：

| 对比 | `int*` | `int**` |
| :--- | :--- | :--- |
| 含义 | 指向 `int` | 指向 `int*` |
| 分配次数 | 通常一次 | 通常多次 |
| 内存是否连续 | 是 | 不一定 |
| 访问方式 | `data[x + y * width]` | `data[y][x]` |
| 释放方式 | 一次 `delete[]` | 多次 `delete[]` |
| 性能 | 通常更好 | 可能更差 |

---

### 8.3 stack array vs heap array

stack array：

```cpp
int array[5][5];
```

特点：

- 自动管理生命周期；
- 离开作用域自动销毁；
- 分配速度快；
- 大小通常必须编译期确定；
- 不适合特别大的数组。

heap array：

```cpp
int* array = new int[width * height];
```

特点：

- 大小可以运行时决定；
- 适合较大的数据；
- 需要手动释放；
- 容易 memory leak；
- 现代 C++ 更推荐用 `std::vector` 替代裸 `new`。

---

### 8.4 raw array vs `std::vector`

raw array：

```cpp
int* array = new int[width * height];
```

需要手动释放：

```cpp
delete[] array;
```

`std::vector`：

```cpp
std::vector<int> array(width * height);
```

不需要手动释放，离开作用域会自动清理。

访问方式：

```cpp
array[x + y * width] = 10;
```

现代 C++ 中，如果不是为了学习底层内存或写非常底层的系统代码，通常更推荐：

```cpp
std::vector<int>
```

而不是：

```cpp
new int[]
```

---

### 8.5 row-major vs column-major

二维数据在线性内存中可以按不同顺序排列。

C++ 的二维数组通常是 **row-major**，也就是一行一行连续存储。

例如：

```text
[0][0] [0][1] [0][2]
[1][0] [1][1] [1][2]
```

内存中类似：

```text
[0][0] [0][1] [0][2] [1][0] [1][1] [1][2]
```

如果我们使用 `(x, y)` 坐标，并且按行存储，常见 index 是：

```cpp
x + y * width
```

有些数学库、图形 API 或语言可能使用不同布局。写矩阵、图像、渲染相关代码时，要特别注意数据布局。


---

## 9. 核心思想

### 重点 1：多维数组本质上仍然是线性内存

计算机内存不是天然二维或三维的。

内存更像是一条长长的线：

```text
0, 1, 2, 3, 4, 5, 6, ...
```

所谓二维数组，只是我们用坐标 `(x, y)` 去解释这段线性内存。

理解这一点后，`x + y * width` 就变得非常自然。

---

### 重点 2：`int**` 不等于一整块连续的二维数组

很多初学者会以为：

```cpp
int** array;
```

就是“二维数组”。

更准确地说，它是 pointer-to-pointer。

如果你这样分配：

```cpp
int** array = new int*[height];

for (int y = 0; y < height; y++)
{
    array[y] = new int[width];
}
```

每一行都是单独分配的，它们不一定在内存中连续。

这和一块连续的二维数组不是同一回事。

---

### 重点 3：连续内存通常更适合性能敏感代码

一维数组模拟二维数组：

```cpp
int* grid = new int[width * height];
```

所有元素连续排列。

这对 CPU cache 更友好，尤其是在大量遍历数据时。

例如：

```cpp
for (int y = 0; y < height; y++)
{
    for (int x = 0; x < width; x++)
    {
        Process(grid[x + y * width]);
    }
}
```

这种按顺序访问连续内存的方式通常性能更好。

---

### 重点 4：多维索引的核心是公式

二维：

```cpp
index = x + y * width;
```

三维：

```cpp
index = x + y * width + z * width * height;
```

这个思想可以扩展到更多维度。

多维数组并不神秘，本质就是把多个坐标转换成一个线性 index。

---

### 重点 5：现代 C++ 中优先让容器管理内存

讲 `new` 和 `delete[]`，主要是为了理解底层原理。

但实际写现代 C++ 时，很多情况下更推荐：

```cpp
std::vector<int> grid(width * height);
```

而不是：

```cpp
int* grid = new int[width * height];
```

因为 `std::vector` 可以自动管理 heap 内存，减少 memory leak 和手动释放错误。