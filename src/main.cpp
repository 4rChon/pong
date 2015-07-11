#include <display.h>
#include <player.h>
#include <ball.h>
#include <vector>
#include <iostream>

int main(int, char**)
{
    std::vector<Player> players;    
    players.push_back(Player(0, 240, 476, 50, 8));
    players.push_back(Player(1, 240, 16, 500, 8));
    Display display = Display(500, 500);        
    display.draw(players, Ball(250, 250, -1, -2, 10));
    return 0;
}
