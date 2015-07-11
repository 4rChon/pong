#include <ball.h>
#include <sstream>

Ball::Ball(int x, int y)
{
    this->x = x;
    this->y = y;
    this->x_vel = 0;
    this->y_vel = 0;
    this->size = 3;    
    this->max_vel = 8;
}

Ball::Ball(int x, int y, int x_vel, int y_vel, int size, double max_vel)
{
    this->x = x;
    this->y = y;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
    this->size = size;
    this->max_vel = max_vel;   
}

void Ball::move()
{
    this->x += int(this->x_vel);
    this->y += int(this->y_vel);
}

void Ball::set_x(int x)
{
    this->x = x;
}

void Ball::set_y(int y)
{
    this->y = y;
}

void Ball::set_velocity(double x_vel, double y_vel)
{
    if(x_vel > max_vel) x_vel = max_vel;
    else if(x_vel < -max_vel) x_vel = -max_vel;
    if(y_vel > max_vel) y_vel = max_vel;
    else if(y_vel < -max_vel) y_vel = -max_vel;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
}

int Ball::get_x()
{
    return this->x;
}

int Ball::get_y()
{
    return this->y;
}

double Ball::get_x_vel()
{
    return this->x_vel;
}
double Ball::get_y_vel()
{
    return this->y_vel;
}

int Ball::get_size()
{
    return this->size;
}

void Ball::toggle_x_vel()
{
    this->x_vel *= -1;
}

void Ball::toggle_y_vel()
{
    this->y_vel *= -1;
}

void Ball::reset(int x, int y)
{
    this->x = x;
    this->y = y;
    this->x_vel = -1;    
    this->y_vel = -2;
}

std::string Ball::toString()
{
    std::ostringstream s;
    s << "Ball: " << this->x << ", " << this->y << " | Speed: " << this->x_vel << ", " << this->y_vel;
    return s.str();
}
