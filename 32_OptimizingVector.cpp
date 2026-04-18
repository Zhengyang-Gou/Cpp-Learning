// 优化的核心思想是：减少内存重新分配和减少不必要的拷贝
// std::vector 的工作机制是在堆内存中维护一个连续的数组
// 当你添加元素且当前容量不足时，它会执行以下三步（分配、搬迁、释放）

#include <iostream>
#include <vector>

struct Vertex {
    float x, y, z;

    // 默认构造函数
    Vertex(float x, float y, float z)
        : x(x), y(y), z(z) {
    }

    // 拷贝构造函数（只要发生拷贝，就会打印）
    Vertex(const Vertex& vertex)
        : x(vertex.x), y(vertex.y), z(vertex.z) {
        std::cout << "Copied!" << std::endl;
    }
};

int main() {
    std::vector<Vertex> vertices;

    // 连续放入 3 个元素
    vertices.push_back(Vertex(1, 2, 3));
    vertices.push_back(Vertex(4, 5, 6));
    vertices.push_back(Vertex(7, 8, 9));

    std::cout << "----------------------------" << std::endl;

    std::vector<Vertex> vertices1;
    
    // 提前分配能容纳 3 个元素的内存
    vertices1.reserve(3); 

    vertices1.push_back(Vertex(1, 2, 3));
    vertices1.push_back(Vertex(4, 5, 6));
    vertices1.push_back(Vertex(7, 8, 9));

    std::cout << "----------------------------" << std::endl;
    std::vector<Vertex> vertices2;
    vertices2.reserve(3); 

    // 不再传入 Vertex 对象，直接传入构造参数（推荐）
    vertices2.emplace_back(1, 2, 3);
    vertices2.emplace_back(4, 5, 6);
    vertices2.emplace_back(7, 8, 9);


    std::cin.get();
}