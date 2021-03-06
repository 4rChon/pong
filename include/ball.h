#pragma once

#include <texture.h>
#include <string>

class Ball
{
    private:
        double x;
        double y;
        double x_vel;
        double y_vel;
        int size;
        double max_vel;
        Texture* texture;
    public:
        Ball(double x, double y);
        Ball(double x, double y, double x_vel, double y_vel, int size, double max_vel);
        void move();
        void draw();
        void set_x(double x);
        void set_y(double y);
        void set_velocity(double x_vel, double y_vel);
        void set_texture(Texture* texture);
        double get_x();
        double get_y();
        double get_x_vel();
        double get_y_vel();
        int get_size();
        void toggle_x_vel();
        void toggle_y_vel();
        void reset(double x, double y);
        std::string toString();
};
