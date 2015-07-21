#pragma once

#include <texture.h>
#include <string>

class Player
{
    private:
        int id;
        double x;
        double y;
        int width;
        int height;
        double velocity;
        Texture* texture;
    public:
        Player(int id, double x, double y);
        Player(int id, double x, double y, int width, int height);
        void move();
        void draw();
        void set_x(double x);
        void set_y(double y);
        void set_velocity(double velocity);
        void set_texture(Texture* texture);
        double get_x() const;
        double get_y() const;
        int get_width() const;
        int get_height() const;
        double get_velocity() const;        
        std::string to_string() const;        
};
