//
// Created by Notesdepth on 28/08/2019.
//

#include <frac.h>

Sphere::Sphere()
{
    initSphere(this);
}

Sphere::Sphere(float x, float y, float z, int rayon, int r, int g, int b, float ka, float kd, float ks, float n)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
    this->r = rayon;
    this->c.r = r;
    this->c.g = g;
    this->c.b = b;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n = n;
}