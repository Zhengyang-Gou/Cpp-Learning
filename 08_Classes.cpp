#include <iostream>

class Player0
{
public:
    int x, y, speed;
};

struct Player1 // 和 class 的区别是默认为 public
{
    int x, y, speed;
};

void Move(Player0& player, int xa, int ya)
{
    player.x += xa * player.speed;
    player.y += ya * player.speed;
}

int main()
{
    Player0 Kyue; // 实例化
    Kyue.x = 30;
    Kyue.y = 163;
    Kyue.speed = 10;
    Move(Kyue, 10, -10);
}