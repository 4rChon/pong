#pragma once

#include <string>
#include <SDL2/SDL.h>

class Ball
{
    private:
        double x;
        double y;
        double x_vel;
        double y_vel;
        int size;
        double max_vel;
        SDL_Texture *sprite;
    public:
        Ball(double x, double y);
        Ball(double x, double y, double x_vel, double y_vel, int size, double max_vel);
        void move();
        void set_x(double x);
        void set_y(double y);
        void set_velocity(double x_vel, double y_vel);
        void set_sprite(SDL_Texture *sprite);
        double get_x();
        double get_y();
        double get_x_vel();
        double get_y_vel();
        int get_size();
        void toggle_x_vel();
        void toggle_y_vel();
        void reset(double x, double y);
        SDL_Texture* get_sprite();
        std::string toString();
};
