//
// Created by Notesdepth on 19/12/2018.
//

#include <frac.h>

void move(SDL_Event event, float *x1, float *x2, float *y1, float *y2, float zoom)
{
    zoom *= zoom * zoom;
    if (event.key.keysym.sym == SDLK_UP)
    {
        *y1 -= 1.0f / zoom;
        *y2 -= 1.0f / zoom;
    }
    else if (event.key.keysym.sym == SDLK_DOWN)
    {
        *y1 += 1.0f / zoom;
        *y2 += 1.0f / zoom;
    }
    else if (event.key.keysym.sym == SDLK_LEFT)
    {
        *x1 -= 1.0f / zoom;
        *x2 -= 1.0f / zoom;
    }
    else if (event.key.keysym.sym == SDLK_RIGHT)
    {
        *x1 += 1.0f / zoom;
        *x2 += 1.0f / zoom;
    }
}

void resetZoom(float *x1, float *x2, float *y1, float *y2)
{
    *x1 = -2.1f;
    *x2 = 2.1f;
    *y1 = -1.2f;
    *y2 = 1.2f;
}
void zoom(float *x1, float *x2, float *y1, float *y2, float gros, float *oldScale)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    /*x1 = x + (*x1 / WIDTH - 0.5f) * gros;
    *x2 = x + (*x2 / WIDTH + 0.5f) * gros;
    *y1 = x + (*y1 / HEIGHT - 0.5f) * gros;
    *y2 = x + (*y2 / HEIGHT + 0.5f) * gros;*/

    float d_x = *x2 - *x1;
    float d_y = *y2 - *y1;

    float center_x = (*x1 + *x2) / 2.0f;
    float center_y = (*y1 + *y2) / 2.0f;

    *oldScale = gros * *oldScale;

    float new_x = x / ((float)WIDTH / d_x) + *x1;
    float new_y = y / ((float)HEIGHT / d_y) + *y1;

    float delta_x = abs(center_x - new_x);
    float delta_y = abs(center_y - new_y);

    if (center_x > new_x)
    {
        *x1 = (*x1) / *oldScale - delta_x;
        *x2 = (*x2) / *oldScale - delta_x;
    }
    else
    {
        *x1 = (*x1) / *oldScale + delta_x;
        *x2 = (*x2) / *oldScale + delta_x;
    }

    if (center_y > new_y)
    {
        *y1 = (*y1) / *oldScale - delta_y;
        *y2 = (*y2) / *oldScale - delta_y;
    }
    else
    {
        *y1 = (*y1) / *oldScale + delta_y;
        *y2 = (*y2) / *oldScale + delta_y;
    }
    printf("mouse_x = %d\nd_x = %f\ncenter_x = %f\n new_x = %f\ndelta_x = %f\nx1 = %f\n", x, d_x, center_x, new_x, delta_x, *x1);
}


void loopFrac(Window *win, int *screen, int width, int height, float *x1, float *x2, float *y1, float *y2)
{
    ocl ocl;
    char s[] = "openCL/frac.cl";
    //Affichage
    SDL_Event event;
    float scale = 1.0f;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            SDL_DestroyRenderer(win->render);
            SDL_DestroyWindow(win->w);
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
            zoom(x1, x2, y1, y2, 1.1f, &scale);
        else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
            resetZoom(x1, x2, y1, y2);
        else if (event.type == SDL_KEYDOWN)
            move(event, x1, x2, y1, y2, 1.1f);
    }



    int i = -1;
    int j = -1;
    int count = 0;
    update(win->render);

   /* while (++i != width)
    {
        while (++j != height)
        {
            calcRay(r, vp, cam, i, j);
            mandelbrot(win->render, i, j);
            //julia(win->render, i, j);
        }
        j = -1;
    }*/

    //execCL(&ocl, "mandelbrot", s, screen, width, height, IT_MAX, *x1, *x2, *y1, *y2);
    juliaCL(&ocl, "julia", s, screen, width, height, IT_MAX, -1.5f, 1.5f, -1.0f, 1.0f);
    while (++i != WIDTH * HEIGHT)
    {
            /*if (screen[++count] == 0)
                SDL_SetRenderDrawColor(win->render, 0, 0, 0, 0);
            else if (screen[count] == 1)
                SDL_SetRenderDrawColor(win->render, 255, 255, 255, 0);*/
            SDL_SetRenderDrawColor(win->render, screen[count], screen[count + 1], screen[count + 2], 255);
            SDL_RenderDrawPoint(win->render, i % WIDTH, i / WIDTH);
            count += 3;
    }
    SDL_UpdateWindowSurface(win->w);
}

void loopRT(Window *win, Camera *cam, ViewPoint *vp, Ray *r, Sphere *s, Plan *p)
{
    //Affichage
    SDL_Event event;
    float scale = 1.0f;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            SDL_DestroyRenderer(win->render);
            SDL_DestroyWindow(win->w);
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN)
            move(cam, vp, s, event);
    }
    Light l(3.0f, 5.0f, 8.0f, 0.5f, 0, 255, 0);
    int i = -1;
    int j = -1;
    while (++i != WIDTH)
    {
        while (++j != HEIGHT)
        {
            initRay(r, *vp, *cam, i, j);
            //traceSphere(r, s, win, l);
            tracePlan(r, p, win, l);
            SDL_RenderDrawPoint(win->render, i, j);
        }
        j = -1;
    }
    SDL_UpdateWindowSurface(win->w);
}