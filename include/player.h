#pragma once

#include <texture.h>
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
        Texture* texture;
    public:
        Player(int id, int x, int y);
        Player(int id, int x, int y, int width, int height);
        void move();
        void draw();
        void set_x(int x);
        void set_y(int y);
        void set_velocity(double velocity);
        void set_texture(Texture* texture);
        int get_x() const;
        int get_y() const;
        int get_width() const;
        int get_height() const;
        double get_velocity() const;        
        std::string to_string() const;        
};
