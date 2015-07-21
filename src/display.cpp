#include <display.h>
#include <texture.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

constexpr double PI = acos(-1);

Display::Display(int WIDTH, int HEIGHT)
{
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    this->gRenderer = NULL;
    this->gWindow = NULL;
    this->gBackgroundTexture = NULL;
    this->gScreenSurface = NULL;
    this->gFont = NULL;
    this->gTextTexture = NULL;
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
    
    if(TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    
    return true;    
}

bool Display::loadMedia()
{
    this->gFont = TTF_OpenFont("../res/fonts/consolab.ttf", 50);
    if(!this->gFont)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void Display::close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_FreeSurface(gScreenSurface);
    
    TTF_CloseFont(this->gFont);
    
    this->gRenderer = NULL;
    this->gWindow = NULL;
    this->gBackgroundTexture = NULL;
    this->gScreenSurface = NULL;
    this->gFont = NULL;
    this->gTextTexture = NULL;
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Display::draw(std::vector<Player> players, Ball ball)
{    
    if(!init())
    {
        printf("Failed to initialize!\n");
        return;
    }
    
    int player1 = 0;
    int player2 = 0;
    
    /* load textures */
    Texture textTexture = Texture(gRenderer);
    this->gTextTexture = &textTexture;
    
    Texture playerTexture = Texture("../res/player/player.png", gRenderer);
    Texture playerLeftTexture = Texture("../res/player/player_left.png", gRenderer);
    Texture playerRightTexture = Texture("../res/player/player_right.png", gRenderer);
    Texture ballTexture = Texture("../res/ball/ball.png", gRenderer);
    Texture backgroundTexture = Texture("../res/background/field.png", gRenderer);
    for(std::size_t i = 0; i < players.size(); i++)
    {
        players[i].set_texture(&playerTexture);
    }
    ball.set_texture(&ballTexture);
    this->gBackgroundTexture = &backgroundTexture;
    
    if(!loadMedia())
        printf("Failed to load media!\n");
    else
        printf("Media Loaded\n");
    
    /* physics */    
    double t = 0.0;
    const double dt = 1.0/60;
    
    Uint32 currentTime = SDL_GetTicks();
    double accumulator = 0.0;    
    double velocity = 0.0;
    
    /* init teams */
    //Team team1 = Team(players[0]);
    //Team team2 = Team(players[1]);
    
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
                        case SDLK_LEFT: 
                        {                            
                            players[0].set_texture(&playerLeftTexture);
                            players[0].set_velocity(-1 * velocity);
                        }
                        break;
                        case SDLK_RIGHT: 
                        {
                            players[0].set_texture(&playerRightTexture);
                            players[0].set_velocity(velocity);
                        }
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
                            {
                                players[0].set_texture(&playerTexture);
                                players[0].set_velocity(0);                                                            
                            }
                            break;
                        case SDLK_RIGHT:
                            if(players[0].get_velocity() > 0)
                            {
                                players[0].set_texture(&playerTexture);
                                players[0].set_velocity(0);
                            }
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
        
        gBackgroundTexture->render(0, 0);
        
        /* ball movement detection */
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        
        double ball_center_x = ball.get_x() + (ball.get_size()/2);
        double ball_center_y = ball.get_y() + (ball.get_size()/2);
        double tanTheta = ball.get_x_vel()/ball.get_y_vel();
        //double height_mod = 0;
        double width_mod = 0;
        double dist = 0;

        if(ball.get_y_vel() > 0)
        {
            dist = (HEIGHT - ball_center_y) * tanTheta;
            //height_mod = HEIGHT;            
        }
        else
            dist = -1 * ball_center_y * tanTheta;
        
        //double gradient = (ball_center_y - height_mod) / (ball_center_x - (ball_center_x + dist));
        //double intercept = ball_center_y - (gradient * ball_center_x);
        //double y2 = intercept;                    
        
//        SDL_RenderDrawLine(gRenderer,
//                           round(ball_center_x),
//                           round(ball_center_y),
//                           round(ball_center_x + dist),
//                           round(height_mod));
        
        
        if(ball.get_x() + (ball.get_size()/2) + dist > WIDTH)
            width_mod = WIDTH;
        
        //y2 = (gradient * width_mod) + intercept;
        
//        SDL_RenderDrawLine(gRenderer,
//                           width_mod,
//                           round(y2),
//                           round((2 * width_mod) - (ball_center_x + dist)),
//                           round(height_mod));
        
        double final_x = 0;
        if(ball_center_x + dist <= WIDTH && ball_center_x + dist >= 0)
            final_x = ball_center_x + dist;
        else final_x = (2 * width_mod) - (ball_center_x + dist);
        
        /* artificial intelligence */
        if(final_x < players[1].get_x() + players[1].get_width()/10)
        {
            players[1].set_texture(&playerLeftTexture);
            players[1].set_velocity(-1 * velocity);
        }
        else if(final_x > players[1].get_x() + players[1].get_width() - players[1].get_width()/10)
        {
            players[1].set_texture(&playerRightTexture);
            players[1].set_velocity(velocity);
        }
        else
        {
            players[1].set_texture(&playerTexture);
            players[1].set_velocity(0);
        }
        
        /* collision detection */        
        for(std::size_t i = 0; i < players.size(); i++)
        {
            /* restrict x-axis movement range */
            if(players[i].get_x() + players[i].get_velocity() <= (this->WIDTH-players[i].get_width()) && players[i].get_x() + players[i].get_velocity() >= 0)
                players[i].move();
            else
                if(players[i].get_velocity() > 0)
                    players[i].set_x(WIDTH-players[i].get_width()-2);
                else
                    players[i].set_x(2);
            
            /* assign variables to reduce code repetition */        
            
            double player_center_x = players[i].get_x() + (players[i].get_width()/2);
            double player_center_y = players[i].get_y() + (players[i].get_height()/2);
            
            double x_dist = round(ball_center_x - player_center_x);
            double y_dist = round(ball_center_y - player_center_y);
            
            //if distance between the centers of two rectangles is less than the sum of their sizes then:
            if(
                std::abs(round(x_dist)) < (ball.get_size()/2) + (players[i].get_width()/2) && 
                std::abs(round(y_dist)) < (ball.get_size()/2) + (players[i].get_height()/2)
            )
            {
                /* get angle from center to corner*/
                double angle_i = std::abs(atan(double(players[i].get_height()/2) / double(players[i].get_width()/2)) * 180 / PI);
                double angle_o;                
                
                //printf("Angle_i: %.2f | %.2f/%.2f\n", angle_i, double(players[i].get_height()/2), double(players[i].get_width()/2));
                /* y direction of ball */
                if(ball.get_y_vel() < 0)
                    angle_o = std::abs(atan((y_dist + (ball.get_size()/2)) / x_dist) * 180 / PI);
                else
                    angle_o = std::abs(atan((y_dist - (ball.get_size()/2)) / x_dist) * 180 / PI);
                
                //printf("Angle_o: %.2f | %.2f/%.2f\n", angle_o, y_dist + (ball.get_size()/2), x_dist);
                //printf("Angle_o: %.2f | %.2f/%.2f\n", angle_o, y_dist - (ball.get_size()/2), x_dist);
            
                //printf("x_dist = %.1f, %.2f\n", round(x_dist), x_dist);
                //printf("y_dist = %.1f, %.2f\n", round(y_dist), y_dist);
                
                /* angle of incidence of collision */
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
        
        /* bump off the sides */
        if(ball.get_x() + ball.get_x_vel() <= this->WIDTH-ball.get_size() && 
           ball.get_x() + ball.get_x_vel() >= 0)
            ball.move();
        else
            ball.toggle_x_vel();
        
        /* check score */
        if(ball.get_y() + ball.get_y_vel() > this->HEIGHT-ball.get_size())
        {
            player1++;
            ball.reset(WIDTH/2, HEIGHT/2);
        }
        else if(ball.get_y() + ball.get_y_vel() < 0) 
        {
            player2++;
            ball.reset(WIDTH/2, HEIGHT/2);
        }
        else
            ball.move();
            
        ball.draw();
        
        
        SDL_Color textColor = {255, 255, 255, 0};
        
        std::ostringstream player1stream;        
        player1stream << player1;
        this->gTextTexture->loadFromRenderedText(this->gFont, player1stream.str(), textColor);
        gTextTexture->render(WIDTH - gTextTexture->get_width() - 10, (HEIGHT - gTextTexture->get_height())/2 - 25);
        
        std::ostringstream player2stream;
        player2stream << player2;
        this->gTextTexture->loadFromRenderedText(this->gFont, player2stream.str(), textColor);
        gTextTexture->render(10, (HEIGHT - gTextTexture->get_height())/2 + 30);        
        
        SDL_RenderPresent(gRenderer);
    }   
    playerTexture.free(); 
    playerLeftTexture.free();
    playerRightTexture.free();
    backgroundTexture.free();
    textTexture.free();    
    ballTexture.free();
}

