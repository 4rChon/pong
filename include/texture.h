#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <string>

class Texture
{
    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        int width;
        int height;
    public:
        Texture();
        Texture(SDL_Renderer* gRenderer);
        Texture(std::string path, SDL_Renderer* gRenderer);
        ~Texture();
        bool loadFromFile(std::string path);
        void free();
        void render(int x, int y);
        int get_width() const;
        int get_height() const;
        std::string to_string() const;
};
