//
// Created by Notesdepth on 9/17/2019 ap. J.-C..
//

#include <frac.h>

Plan::Plan()
{

}

Plan::Plan(float w, float x, float y, float z, int r, int g, int b, float ka, float kd, float ks, float n)
{
    this->w = w; //coeff devant x, idem pour b, c et d
    this->x = x;
    this->y = y;
    this->z = z;
    this->c.r = r;
    this->c.g = g;
    this->c.b = b;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n = n;
}