//
// Created by Notesdepth on 29/08/2019.
//

#include <frac.h>

Light::Light(float x, float y, float z, float i, int r, int g, int b)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
    this->i = i;
    this->c.r = r;
    this->c.g = g;
    this->c.b = b;
}