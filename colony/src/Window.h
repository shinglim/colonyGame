#pragma once
#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window();
    ~Window();

    bool create(const std::string& title, int width, int height, bool fullscreen);
    void clear();
    void present();

    SDL_Renderer* getRenderer() const { return renderer; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void destroy();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0, height = 0;
};
