#pragma once

#include <player.h>
#include <ball.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <vector>

class Display
{
    private:
        int WIDTH;
        int HEIGHT;
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;
        
        bool init();
    public:
        Display(int width, int height);
        void close();
        void draw_field();
        void draw_player(Player player);
        void draw_ball(Ball ball);
        void draw(std::vector<Player> players, Ball ball);
};
