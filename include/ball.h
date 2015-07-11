#pragma once

#include <string>

class Ball
{
    private:
        int x;
        int y;
        double x_vel;
        double y_vel;
        int size;
        double max_vel;
    public:
        Ball(int x, int y);
        Ball(int x, int y, int x_vel, int y_vel, int size, double max_vel);
        void move();
        void set_x(int x);
        void set_y(int y);
        void set_velocity(double x_vel, double y_vel);
        int get_x();
        int get_y();
        double get_x_vel();
        double get_y_vel();
        int get_size();
        void toggle_x_vel();
        void toggle_y_vel();
        void reset(int x, int y);
        std::string toString();
};
