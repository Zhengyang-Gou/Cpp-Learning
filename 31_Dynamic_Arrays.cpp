// C++ 原生的标准数组（比如 int arr[5]; 或者 new int[5]）有一个致命的缺陷：它的大小是固定的
// std::vector 就是全自动完成的智能数组

#include <vector>
#include <iostream>

struct Vertex {
    float x, y, z;
};

int main() {
    // 创建一个只装 Vertex 类型的动态数组
    std::vector<Vertex> vertices; 

    // 使用 push_back 向数组尾部塞入数据
    vertices.push_back({ 1.0f, 2.0f, 3.0f });
    vertices.push_back({ 4.0f, 5.0f, 6.0f });
    vertices.push_back({ 7.0f, 8.0f, 9.0f });

    // 传统 for 循环遍历
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << vertices[i].x << std::endl; // 像普通数组一样用 [] 访问
    }

    // 现代 C++ 的 Range-based for 循环
    // 注意这里的引用 &，为了避免每次循环都复制一遍 Vertex 对象
    for (Vertex& v : vertices) { 
        std::cout << v.y << std::endl;
    }

    // 清空数组
    vertices.clear();
}