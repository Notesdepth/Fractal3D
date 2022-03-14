//
// Created by Notesdepth on 20/12/2018.
//

#include <frac.h>

Window::Window(int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);
    this->w = SDL_CreateWindow("Fractale 3D", 0, 0, width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Surface *surface = SDL_GetWindowSurface(this->w);
    this->render = SDL_CreateSoftwareRenderer(surface);
    //SDL_FreeSurface(surface);
}