#include <texture.h>
#include <stdio.h>
#include <sstream>

Texture::Texture()
{
    this->texture = NULL;
    this->renderer = NULL;
    this->width = 0;
    this->height = 0;
}

Texture::Texture(SDL_Renderer* gRenderer)
{
    this->texture = NULL;
    this->renderer = gRenderer;
    this->width = 0;
    this->height = 0;
}

Texture::Texture(std::string path, SDL_Renderer* gRenderer)
{
    this->texture = NULL;
    this->renderer = gRenderer;
    this->width = 0;
    this->height = 0;
    loadFromFile(path);
}

Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile(std::string path)
{
    free();
    
    SDL_Texture* newTexture = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(!loadedSurface)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    
    newTexture = SDL_CreateTextureFromSurface(this->renderer, loadedSurface);
    
    if(!newTexture)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    
    this->width = loadedSurface->w;
    this->height = loadedSurface->h;
    
    SDL_FreeSurface(loadedSurface);
    
    this->texture = newTexture;
    
    return this->texture != NULL;
}

bool Texture::loadFromRenderedText(TTF_Font* gFont, std::string textureText, SDL_Color textColor)
{
    free();
    
    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
    if(!textSurface)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;        
    }
    this->texture = SDL_CreateTextureFromSurface(this->renderer, textSurface);
    if(!this->texture)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    this->width = textSurface->w;
    this->height = textSurface->h;
    
    SDL_FreeSurface(textSurface);
    
    return this->texture != NULL;
}

void Texture::free()
{
    if(this->texture)
    {
        SDL_DestroyTexture(texture);
        this->texture = NULL;
        this->width = 0;
        this->height = 0;
    }
}

void Texture::render(int x, int y)
{
    SDL_Rect renderQuad = {x, y, this->width, this->height};    
    SDL_RenderCopy(this->renderer, this->texture, NULL, &renderQuad);
}

int Texture::get_width() const
{
    return this->width;
}

int Texture::get_height() const
{
    return this->height;
}

std::string Texture::to_string() const
{
    std::ostringstream s;
    s << "Texture " << this->width << ": " << this->height << " | renderer: " << this->renderer;
    return s.str();
}

