//
// Created by Notesdepth on 19/05/2019.
//

#include <frac.h>
void update(SDL_Renderer *render)
{
    SDL_Rect rect;
    rect.x = 0.0f;
    rect.y = 0.0f;
    rect.w = WIDTH;
    rect.h = HEIGHT;
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderFillRect(render, &rect);
}

void calcRay(Ray *r, ViewPoint vp, Camera cam, int x, int y)
{
    r->pos.x = vp.init_point.x + x * vp.xvect.x + y * vp.yvect.x; //initiale donc sur le viewplane
    r->pos.y = vp.init_point.y + x * vp.xvect.y + y * vp.yvect.y;
    r->pos.z = vp.init_point.z + x * vp.xvect.z + y * vp.yvect.z;
    r->dirV.x = r->pos.x - cam.pos.x;
    r->dirV.y = r->pos.y - cam.pos.y;
    r->dirV.z = r->pos.z - cam.pos.z;
}

void mandelbrot(SDL_Renderer *render, float i, float j)
{
    //initialisation du nombre complexe
    complex z1;
    z1.r = 0.0f;
    z1.i = 0.0f;
    float tmp_r = 0;

    float tmp_i = i / ((float)WIDTH / 2.7) - 2.1;
    float tmp_j = j / ((float)HEIGHT / 2.4) - 1.2;

    int it = -1;
    while (++it < IT_MAX && pow(z1.r, 2) + pow(z1.i, 2) < 4)
    {
        tmp_r = z1.r;
        z1.r = pow(z1.r, 2) - pow(z1.i, 2) + tmp_i;
        z1.i = 2 * tmp_r * z1.i + tmp_j;
    }

    if (it == IT_MAX)
    {
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderPresent(render);
        SDL_RenderDrawPoint(render, i, j);
    }
}

void julia(SDL_Renderer *render, float i, float j)
{
    //initialisation du nombre complexe
    complex z1;
    int x, y;
    SDL_GetMouseState(&x, &y);
    z1.r = x / ((float)WIDTH / 2.7) - 2.1;
    z1.i = y / ((float)HEIGHT / 2.4) - 1.2;
    float tmp_r = 0;

    float tmp_i = i / ((float)WIDTH / 2.7) - 2.1;
    float tmp_j = j / ((float)HEIGHT / 2.4) - 1.2;

    int it = -1;
    while (++it < IT_MAX && pow(z1.r, 2) + pow(z1.i, 2) < 4)
    {
        tmp_r = z1.r;
        z1.r = pow(z1.r, 2) - pow(z1.i, 2) + tmp_i;
        z1.i = 2 * tmp_r * z1.i + tmp_j;
    }

    if (it == IT_MAX)
    {
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderPresent(render);
        SDL_RenderDrawPoint(render, i, j);
    }
}