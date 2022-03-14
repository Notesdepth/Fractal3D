//
// Created by Notesdepth on 19/09/2018.
//

#include <frac.h>

void draw(SDL_Renderer *render)
{
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    int i = -1;
    int j = -1;
    while (++i < WIDTH)
    {
        while (++j < HEIGHT)
        {
            if (i == j)
                SDL_RenderDrawPoint(render, i, j);
        }
        j = -1;
    }
}

void test(Window *w)
{
    SDL_Event event;
    SDL_SetRenderDrawColor(w->render, 0, 0, 255, 255);
    SDL_RenderClear(w->render);
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            exit(0);
    }
    draw(w->render);
    SDL_UpdateWindowSurface(w->w);

}

int main(int argc, char **argv)
{
    Window win(WIDTH, HEIGHT);
    SDL_SetRenderDrawColor(win.render, 0, 0, 0, 255);
    SDL_RenderClear(win.render);

    Camera cam;
    ViewPoint vp(cam, WIDTH, HEIGHT);
    Ray r;
    initFileRay(&r);

    Sphere s = Sphere(0.0f, 0.0f, -2.0f, 3.0f, 125, 255, 255, 0.2f, 0.9f, 0.5f, 1.0f);
    Plan p = Plan(1.0f, 0.0f, 0.0f, 10.0f, 125, 255, 255, 0.2f, 0.9f, 0.5f, 1.0f);
    //Sphere s;
    int *screen = (int *)malloc(sizeof(int) * WIDTH * HEIGHT * 3);
    int i;
    for (i = 0; i < WIDTH * HEIGHT * 3; i++)
        screen[i] = 0;

    float x1 = -2.1;
    float x2 = 2.1f;
    float y1 = -1.2f;
    float y2 = 1.2f;
    while (TRUE)
    {
        //loopFrac(&win, screen, WIDTH, HEIGHT, &x1, &x2, &y1, &y2);
        loopRT(&win, &cam, &vp, &r, &s, &p);
    }

    free(screen);
    return (0);
}