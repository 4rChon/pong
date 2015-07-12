#pragma once

#include <string>
#include <SDL2/SDL.h>

class Player
{
    private:
        int id;
        int x;
        int y;
        int width;
        int height;
        double velocity;
        SDL_Texture *sprite;
    public:
        Player(int id, int x, int y);
        Player(int id, int x, int y, int width, int height);
        void move();
        void set_x(int x);
        void set_y(int y);
        void set_velocity(double velocity);
        void set_sprite(SDL_Texture *sprite);
        int get_x() const;
        int get_y() const;
        int get_width() const;
        int get_height() const;
        double get_velocity() const;
        SDL_Texture* get_sprite();
        std::string toString() const;
};
