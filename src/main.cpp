#include <display.h>
#include <player.h>
#include <ball.h>
#include <vector>
#include <iostream>

int main(int, char**)
{
    std::vector<Player> players;    
    players.push_back(Player(0, 225, 476, 50, 8));
    players.push_back(Player(1, 225, 16, 50, 8));
    Display display = Display(500, 500);        
    display.draw(players, Ball(240, 240, -1, -2, 10, 5));
    return 0;
}
