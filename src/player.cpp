#include <player.h>
#include <sstream>

Player::Player(int id, int x, int y)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->width = 20;
    this->height = 8;
    this->velocity = 0;
}

Player::Player(int id, int x, int y, int width, int height)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->velocity = 0;
}

void Player::move()
{
    this->x += int(this->velocity);
}

void Player::set_x(int x)
{
    this->x = x;
}

void Player::set_y(int y)
{
    this->y = y;
}

void Player::set_velocity(double velocity)
{
    this->velocity = velocity;    
}

int Player::get_x() const
{
    return this->x;
}

int Player::get_y() const
{
    return this->y;
}

int Player::get_height() const
{
    return this->height;
}

int Player::get_width() const
{
    return this->width;
}

double Player::get_velocity() const
{
    return this->velocity;
}

std::string Player::toString() const
{
    std::ostringstream s;
    s << "Player " << this->id << ": " << this->x << ", " << this->y << " | Speed: " << this->velocity;
    return s.str();
}
