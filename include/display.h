#pragma once

#include <player.h>
#include <ball.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <string>
#include <vector>

class Display
{
    private:
        int WIDTH;
        int HEIGHT;
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;
        SDL_Surface* gScreenSurface;
        SDL_Surface* gPlayer;
        
        bool init();
    public:
        Display(int width, int height);
        bool loadMedia();
        SDL_Texture* loadTexture(std::string path);
        void close();
        void draw_field();
        void draw_player(Player player);
        void draw_ball(Ball ball);
        void draw(std::vector<Player> players, Ball ball);
};
