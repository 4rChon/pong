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
    this->renderer = gRenderer;
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
    
    return true;
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

