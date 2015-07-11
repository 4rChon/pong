#include <display.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <stdio.h>

Display::Display(int WIDTH, int HEIGHT)
{
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    gRenderer = NULL;
    gWindow = NULL;
    SDL_Surface* gScreenSurface = NULL;
    SDL_Surface* gPlayer = NULL;
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
    
    //gScreenSurface = SDL_GetWindowSurface(gWindow);
    //
    //int imgFlags = IMG_INIT_PNG;
    //if(!(IMG_Init(imgFlags) & imgFlags))
    //{
    //    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    //    return false;
    //}
    
    return true;    
}

bool Display::loadMedia()
{
    gPlayer = SDL_LoadBMP("../res/player/player.bmp");
    if(!gPlayer)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "player.bmp", SDL_GetError());
        return false;
    }
    return true;
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

    newTexture= SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

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
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    
    SDL_Quit();
}

void Display::draw_player(Player player)
{
    SDL_Rect fillRect = {player.get_x(), player.get_y(), player.get_width(), player.get_height()};
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

void Display::draw_ball(Ball ball)
{
    SDL_Rect fillRect = {ball.get_x(), ball.get_y(), ball.get_size(), ball.get_size()};
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &fillRect);
}

void Display::draw_field()
{
    SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
    SDL_RenderDrawLine(gRenderer, 0, int(HEIGHT/2), WIDTH, int(HEIGHT/2));
}

void Display::draw(std::vector<Player> players, Ball ball)
{    
    if(!init())
    {
        printf("Failed to initialize!\n");
        return;
    }
    
    /*if(!loadMedia())
    {
        printf("Failed to load media!\n");
        return;        
    }
    
    SDL_BlitSurface(gPlayer, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(gWindow);*/
    
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
        
        for(std::size_t i = 0; i < players.size(); i++)
        {
            if(players[i].get_x() + players[i].get_velocity() <= (this->WIDTH-players[i].get_width()) && players[i].get_x() + players[i].get_velocity() >= 0)
                players[i].move();
            else
                if(players[i].get_velocity() > 0)
                    players[i].set_x(WIDTH-players[i].get_width());
                else
                    players[i].set_x(0);
            
            int x_dist = (ball.get_x() + (ball.get_size()/2)) - (players[i].get_x() + (players[i].get_width()/2));
            int y_dist = (ball.get_y() + (ball.get_size()/2)) - (players[i].get_y() + (players[i].get_height()/2));
            
            if(std::abs(x_dist) < ball.get_size()/2 + players[i].get_width()/2 && std::abs(y_dist) < ball.get_size()/2 + players[i].get_height()/2)
            {
                ball.toggle_y_vel();
                ball.set_velocity(ball.get_x_vel() + (0.05 * (players[i].get_velocity() + x_dist)), ball.get_y_vel() * 1.05);
                std::cout << ball.toString() << std::endl;
            }
            draw_player(players[i]);
        }
        
        if(ball.get_x() + ball.get_x_vel() <= (this->WIDTH-ball.get_size()) && ball.get_x() + ball.get_x_vel() >= 0)
                ball.move();
            else
                ball.toggle_x_vel();
        if(ball.get_y() + ball.get_y_vel() <= (this->HEIGHT-ball.get_size()) && ball.get_y() + ball.get_y_vel() >= 0)
                ball.move();
            else
                ball.reset(WIDTH/2, HEIGHT/2);
            
        draw_ball(ball);
        
        SDL_RenderPresent(gRenderer);
        SDL_UpdateWindowSurface(gWindow);        
    }    
}

