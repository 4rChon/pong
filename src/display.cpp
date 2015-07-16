#include <display.h>
#include <texture.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <stdio.h>

constexpr double PI = acos(-1);

Display::Display(int WIDTH, int HEIGHT)
{
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    this->gRenderer = NULL;
    this->gWindow = NULL;
    this->gBackgroundTexture = NULL;
    this->gScreenSurface = NULL;
}

bool Display::init()
{
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(!gWindow)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(!gRenderer)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_SetRenderDrawColor( gRenderer, 0x0A, 0x0A, 0x32, 0xFF );
        
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    
    return true;    
}

SDL_Surface* Display::loadSurface(std::string path)
{
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return optimizedSurface;
    }
    
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
    if(optimizedSurface == NULL)
    {
        printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return optimizedSurface;
    }
    
    SDL_FreeSurface(loadedSurface);
    return optimizedSurface;
}

SDL_Texture* Display::loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(!loadedSurface)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());        
        return newTexture;
    }

    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    if(!newTexture)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return newTexture;
    }

    SDL_FreeSurface(loadedSurface);        

    return newTexture;
}

void Display::close()
{
    SDL_DestroyRenderer(gRenderer);
    gBackgroundTexture->free();
    SDL_DestroyWindow(gWindow);
    SDL_FreeSurface(gScreenSurface);
    gRenderer = NULL;
    gBackgroundTexture = NULL;
    gWindow = NULL;
    gScreenSurface = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

/*void Display::draw_ball(Ball ball)
{
    SDL_Rect SrcR = {0, 0, ball.get_size(), ball.get_size()};
    SDL_Rect DestR = {int(ball.get_x()), int(ball.get_y()), ball.get_size(), ball.get_size()};
    SDL_RenderCopy(gRenderer, ball.get_sprite(), &SrcR, &DestR);

    // Remove comment for lasers
    /*SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    if(ball.get_y_vel() > 0)
    {
        int dist = int((HEIGHT - ball.get_y()) * ball.get_x_vel()/ball.get_y_vel());
        SDL_RenderDrawLine(gRenderer, int(ball.get_x()) + ball.get_size()/2, int(ball.get_y()) + ball.get_size()/2, int(ball.get_x()) + ball.get_size()/2 + dist, HEIGHT);
    }
    else
    {
        int dist = int((ball.get_y()) * ball.get_x_vel()/ball.get_y_vel());
        SDL_RenderDrawLine(gRenderer, int(ball.get_x()) + ball.get_size()/2, int(ball.get_y()) + ball.get_size()/2, int(ball.get_x()) + ball.get_size()/2 - dist, 0);
    }
}*/

void Display::draw_field()
{
    gBackgroundTexture->render(0, 0);
}

void Display::draw(std::vector<Player> players, Ball ball)
{    
    if(!init())
    {
        printf("Failed to initialize!\n");
        return;
    }
    
    Texture playerTexture = Texture("../res/player/player.png", gRenderer);
    Texture ballTexture = Texture("../res/ball/ball.png", gRenderer);
    Texture backgroundTexture = Texture("../res/background/field.png", gRenderer);
    for(std::size_t i = 0; i < players.size(); i++)
    {
        players[i].set_texture(&playerTexture);
    }
    ball.set_texture(&ballTexture);
    this->gBackgroundTexture = &backgroundTexture;
    
    double t = 0.0;
    const double dt = 1.0/60;
    
    Uint32 currentTime = SDL_GetTicks();
    double accumulator = 0.0;    
    double velocity = 0.0;
    
    bool quit = false;
    
    SDL_Event e;
    
    while(!quit)
    {
        /* physics */        
        Uint32 newTime = SDL_GetTicks();
        Uint32 frameTime = newTime - currentTime;
        currentTime = newTime;
        
        accumulator += frameTime;
        
        while(accumulator >= dt)
        {
            velocity = (velocity + 550) * dt;
            accumulator -= dt;
            t += dt;            
        }
        
        /* input */        
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT: quit = true;
                break;
                case SDL_KEYDOWN:
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_LEFT: players[0].set_velocity(-1 * velocity);
                        break;
                        case SDLK_RIGHT: players[0].set_velocity(velocity);
                        break;
                        case SDLK_a: players[1].set_velocity(-1 * velocity);
                        break;
                        case SDLK_d: players[1].set_velocity(velocity);
                        break;
                        default:
                            break;
                    }                    
                }
                break;
                case SDL_KEYUP:
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            if(players[0].get_velocity() < 0)
                                players[0].set_velocity(0);
                            break;
                        case SDLK_RIGHT:
                            if(players[0].get_velocity() > 0)
                                players[0].set_velocity(0);
                            break;
                        case SDLK_a:
                            if(players[1].get_velocity() < 0)
                                players[1].set_velocity(0);
                            break;
                        case SDLK_d:
                            if(players[1].get_velocity() > 0)
                                players[1].set_velocity(0);
                            break;
                        default:
                           break;
                    }
                }
                break;
                default:
                    break;
            }
        }
        
        /* draw */
        SDL_SetRenderDrawColor(gRenderer, 0x0A, 0x0A, 0x32, 0xFF);
        SDL_RenderClear(gRenderer);
        
        //printf("FPS: %d\n", int(1000.0/frameTime));
        
        draw_field();
        
        /* collision detection */
        
        for(std::size_t i = 0; i < players.size(); i++)
        {
            if(players[i].get_x() + players[i].get_velocity() <= (this->WIDTH-players[i].get_width()) && players[i].get_x() + players[i].get_velocity() >= 0)
                players[i].move();
            else
                if(players[i].get_velocity() > 0)
                    players[i].set_x(WIDTH-players[i].get_width()-2);
                else
                    players[i].set_x(2);
                
            int ball_center_x = ball.get_x() + (ball.get_size()/2);
            int ball_center_y = ball.get_y() + (ball.get_size()/2);
            
            int player_center_x = players[i].get_x() + (players[i].get_width()/2);
            int player_center_y = players[i].get_y() + (players[i].get_height()/2);
            
            int x_dist = ball_center_x - player_center_x;
            int y_dist = ball_center_y - player_center_y;
            
            //if distance between the centers of two rectangles is less than the sum of their sizes then:
            if(std::abs(x_dist) < (ball.get_size()/2) + (players[i].get_width()/2) && std::abs(y_dist) < (ball.get_size()/2) + (players[i].get_height()/2))
            {
                /* get angle */
                double angle_i = std::abs(atan2(double(players[i].get_height()/2), double(players[i].get_width()/2)) * 180 / PI);
                double angle_o;                
                
                if(ball.get_y_vel() < 0)
                    angle_o = std::abs(atan2(double(ball_center_y - player_center_y + ball.get_size()/2), double(ball_center_x - player_center_x)) * 180 / PI);    
                else
                    angle_o = std::abs(atan2(double(ball_center_y - player_center_y - ball.get_size()/2), double(ball_center_x - player_center_x)) * 180 / PI);    
                
                if(angle_o > angle_i)
                {
                    if(ball.get_y_vel() < 0)
                        ball.set_y(players[i].get_y() + players[i].get_height());
                    else
                        ball.set_y(players[i].get_y() - players[i].get_height());
                    ball.toggle_y_vel();
                }
                else
                {
                    if(ball.get_x_vel() < 0)
                        ball.set_x(players[i].get_x() + players[i].get_width());
                    else
                        ball.set_x(players[i].get_x());
                    ball.toggle_x_vel();
                }
                ball.set_velocity(ball.get_x_vel() + players[i].get_velocity() + (0.05 * x_dist), ball.get_y_vel() * 1.05);
                std::cout << ball.toString() << std::endl;
            }

            players[i].draw();
        }
        
        if(ball.get_x() + ball.get_x_vel() <= (this->WIDTH-ball.get_size()) && ball.get_x() + ball.get_x_vel() >= 0)
                ball.move();
            else
                ball.toggle_x_vel();
        if(ball.get_y() + ball.get_y_vel() <= (this->HEIGHT-ball.get_size()) && ball.get_y() + ball.get_y_vel() >= 0)
                ball.move();
            else
                ball.reset(WIDTH/2, HEIGHT/2);
            
        ball.draw();
        
        SDL_RenderPresent(gRenderer);
    }   
    playerTexture.free(); 
}

