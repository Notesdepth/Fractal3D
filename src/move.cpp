//
// Created by Notesdepth on 28/08/2019.
//

#include <frac.h>

void move(Camera *cam, ViewPoint *vp, Sphere *s, SDL_Event event)
{
    float sensibilite = 1.0f;
    switch (event.key.keysym.sym)
    {
        case SDLK_d:
            cam->pos.x += sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_q:
            cam->pos.x -= sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_c:
            cam->pos.y += sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_SPACE:
            cam->pos.y -= sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_z:
            cam->pos.z += sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_s:
            cam->pos.z -= sensibilite;
            initVp(vp, *cam, WIDTH, HEIGHT);
            break;
        case SDLK_u:
            s->ka += 0.1f;
            break;
        case SDLK_j:
            s->ka -= 0.1f;
            break;
        case SDLK_i:
            s->kd += 0.1f;
            break;
        case SDLK_k:
            s->kd -= 0.1f;
            break;
        case SDLK_o:
            s->ks += 0.1f;
            break;
        case SDLK_l:
            s->ks -= 0.1f;
            break;
        case SDLK_p:
            s->n += 0.1f;
            break;
        case SDLK_m:
            s->n -= 0.1f;
            break;
    }
}