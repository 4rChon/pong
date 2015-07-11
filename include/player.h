#pragma once

#include <string>

class Player
{
    private:
        int id;
        int x;
        int y;
        int width;
        int height;
        double velocity;
    public:
        Player(int id, int x, int y);
        Player(int id, int x, int y, int width, int height);
        void move();
        void set_x(int x);
        void set_y(int y);
        void set_velocity(double velocity);
        int get_x() const;
        int get_y() const;
        int get_width() const;
        int get_height() const;
        double get_velocity() const;
        std::string toString() const;
};
