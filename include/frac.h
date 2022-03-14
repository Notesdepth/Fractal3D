//
// Created by Notesdepth on 18/09/2018.
//

#ifndef FRAC
#define FRAC

#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <unistd.h>
#define CL_SILENCE_DEPRECATION

#if defined (__APPLE__) ||defined(MACOSX)
#include <OpenCL/opencl.h>

#else
#include <CL/opencl.h>
#endif

//dimensions de l'écran
#define WIDTH 1440
#define HEIGHT 900


//nombre d'itérations
#define IT_MAX 50

typedef struct vect //can be a point too
{
    float x;
    float y;
    float z;
}              vect;

typedef struct color
{
    int r;
    int g;
    int b;
}               color;

typedef struct pix //pixel de l'image finale
{
    int x;
    int y;
    color c;
}               pix;

class Window
{
public :
    Window(int width, int weight);
    SDL_Window *w;
    SDL_Renderer *render;
};

typedef struct ocl
{
    cl_int err_code;
    cl_uint nb_entries;
    cl_platform_id platform;

    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_mem inputBuffer;
    cl_mem outputBuffer;
    cl_kernel kernel;
}               ocl;


class Camera
        {
            public :
            Camera();

            vect pos; //position de la caméra
            vect vp; //position du viewpoint (écran)
            vect upV; //définit le haut de la caméra
            vect dirV; //vecteur directeur de la droite caméra-viewpoint
            vect normV; //vector normal au plan formé par upV et dirV
            float fov;
        };

class ViewPoint
        {
        public :
            ViewPoint(Camera cam, int width, int height);

            float demi_longueur;
            float demi_largeur;
            vect init_point;
            vect xvect;
            vect yvect;
        };
class Ray
        {
        public :
            Ray();

            vect pos; //position sur le viewplane
            float dist;//distance parcouru par le rayon
            float tmp_dist; //distance temporaire à stocker
            vect dirV; //vecteur directeur du rayon
            vect intersect; //point d'intersection
            float it_max; //nb max de recalcul pour la refraction et mirroir
};

typedef struct complex //nombre complexe
{
    float r; //partie réelle
    float i; // partie imaginaire
}               complex;

//obj
class Sphere
{
public :
    Sphere();
    Sphere(float x, float y, float z, int rayon, int r, int g, int b, float ka, float kd, float ks, float n);
    vect pos;
    float r;
    color c;
    float ka;
    float kd;
    float ks;
    float n;
};

class Plan
{
public :
    Plan();
    Plan(float w, float x, float y, float z, int r, int g, int b, float ka, float kd, float ks, float n);
    float w;
    float x;
    float y;
    float z;
    color c;
    float ka;
    float kd;
    float ks;
    float n;
};

class Light
{
public:
    Light(float x, float y, float z, float i, int r, int g, int b);
    vect pos;
    float i; //intensité de la source lumineuse (entre 0 et 1)
    vect dirV; //direction de la lumière
    color c;
};


//init
void initCam(Camera *cam);
void initVp(ViewPoint *vp, Camera cam, int longueur, int largeur);
void initFileRay(Ray *r);
void initRay(Ray *r, ViewPoint vp, Camera cam, int i, int j);
void initSphere(Sphere *s);
void loopFrac(Window *win, int *screen, int width, int height, float *x1, float *x2, float *y1, float *y2);
void loopRT(Window *win, Camera *cam, ViewPoint *vp, Ray *r, Sphere *s, Plan *p);

//calc
void calcRay(Ray *r, ViewPoint vp, Camera cam, int x, int y);
void update(SDL_Renderer *render);
void traceSphere(Ray *r, Sphere *s, Window *w, Light l);
void tracePlan(Ray *r, Plan *p, Window *w, Light l);

//fractals
void mandelbrot(SDL_Renderer *render, float i, float j);
void juliaCL(ocl *ocl, const char *s, char *prog_name, int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2);

//openCL
void juliaCL(ocl *ocl, char *prog_name, int *ptr_screen);
void execCL(ocl *ocl, const char *s, char *prog_name, int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2);

//move
void move(Camera *cam, ViewPoint *vp, Sphere *s, SDL_Event event);


#endif //FRAC
