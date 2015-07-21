#pragma once

#include <ball.h>
#include <player.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class Display
{
    private:
        int WIDTH;
        int HEIGHT;
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;
        TTF_Font *gFont;
        SDL_Surface* gScreenSurface;
        Texture* gBackgroundTexture;
        Texture* gTextTexture;
        
        bool init();
    public:
        Display(int width, int height);
        SDL_Surface* loadSurface(std::string path);
        SDL_Texture* loadTexture(std::string path);
        bool loadMedia();
        void close();
        void draw(std::vector<Player> players, Ball ball);
};
