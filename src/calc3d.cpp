//
// Created by Notesdepth on 2019-08-27.
//

#include <frac.h>

static float prodScal(vect v1, vect v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

static bool sameSign(float a, float b)
{
    if (a * b > 0 || (a == 0 && b == 0))
        return true;
    return false;
}

static bool isForward(Ray *r)
{
    vect dir;

    dir.x = r->intersect.x - r->pos.x;
    dir.y = r->intersect.y - r->pos.y;
    dir.z = r->intersect.z - r->pos.z;

    if (sameSign(dir.x, r->dirV.x) && sameSign(dir.y, r->dirV.y) && sameSign(dir.z, r->dirV.z))
        return true;
    return false;
}

static float distance(float x1, float x2, float y1, float y2, float z1, float z2)
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)));
}

static void normalisation(vect *norm, float d)
{
    norm->x /= d;
    norm->y /= d;
    norm->z /= d;
}

static void normalSphere(vect *norm, Ray r, Sphere s)
{
    float d;
    norm->x = r.intersect.x - s.pos.x;
    norm->y = r.intersect.y - s.pos.y;
    norm->z = r.intersect.z - s.pos.z;
    d = distance(norm->x, 0, norm->y, 0, norm->z, 0);
    normalisation(norm, d);
}

static float reflexionDiffuse(Ray *r, Light l, vect norm)
{
    float d = distance(norm.x, 0.0f, norm.y, 0.0f, norm.z, 0.0f);
    normalisation(&norm, d);

    l.dirV.x = l.pos.x - r->intersect.x;
    l.dirV.y = l.pos.y - r->intersect.y;
    l.dirV.z = l.pos.z - r->intersect.z;

    d = distance(l.dirV.x, 0.0f, l.dirV.y, 0.0f, l.dirV.z, 0.0f);
    normalisation(&l.dirV, d);

    d = prodScal(norm, l.dirV);
    if (d > 0.0f)
        return (d);
    return (0.0f);
}

static float reflexionSpeculaire(Ray *r, Light l, vect norm)
{
    float angle1;
    float angle2;
    float d;
    float c;
    vect refl;

    d = distance(r->dirV.x, 0.0f, r->dirV.y, 0.0f, r->dirV.z, 0.0f);
    normalisation(&r->dirV,d);

    l.dirV.x = r->intersect.x - l.pos.x;
    l.dirV.y = r->intersect.y - l.pos.y;
    l.dirV.z = r->intersect.z - l.pos.z;
    d = distance(l.dirV.x, 0.0f, l.dirV.y, 0.0f, l.dirV.z, 0.0f);
    normalisation(&l.dirV,d);

    c = 2.0f * prodScal(norm,l.dirV);
    refl.x = c * norm.x - l.dirV.x;
    refl.y = c * norm.y - l.dirV.y;
    refl.z = c * norm.z - l.dirV.z;
    d = distance(refl.x, 0.0f, refl.y, 0.0f, refl.z, 0.0f);
    normalisation(&refl,d);

    if (prodScal(refl,r->dirV) >= 0.0f)
        return (prodScal(refl,r->dirV));
    else
        return (0.0f);
}

static void reflexionPartielle(Sphere s, Ray *r, Light l, vect norm, color *i_tot)
{
    float i_amb = 0.0f;
    float i_diff = 0.0f;
    float i_spec = 0.0f;

    i_amb += s.ka * l.i;
    i_diff += s.kd * l.i * reflexionDiffuse(r, l, norm);

    i_spec = s.ks * l.i * pow(reflexionSpeculaire(r, l, norm), s.n);
    i_tot->r += i_spec * l.c.r;
    i_tot->g += i_spec * l.c.g;
    i_tot->b += i_spec * l.c.b;

    i_tot->r += (i_amb + i_diff) * s.c.r;
    i_tot->g += (i_amb + i_diff) * s.c.g;
    i_tot->b += (i_amb + i_diff) * s.c.b;
}

static void intersectionSphere(float x1, float x2, Ray *r)
{
    vect tmp_intersect;
    float tmp_dist;

    r->intersect.x = r->pos.x + r->dirV.x * x1;
    r->intersect.y = r->pos.y + r->dirV.y * x1;
    r->intersect.z = r->pos.z + r->dirV.z * x1;
    r->dist = distance(r->pos.x, r->intersect.x, r->pos.y, r->intersect.y, r->pos.z, r->intersect.z);

    tmp_intersect.x = r->pos.x + r->dirV.x * x2;
    tmp_intersect.y = r->pos.y + r->dirV.y * x2;
    tmp_intersect.z = r->pos.z + r->dirV.z * x2;
    tmp_dist = distance(r->pos.x, tmp_intersect.x, r->pos.y, tmp_intersect.y, r->pos.z, tmp_intersect.z);

    if (tmp_dist < r->dist)
    {
        r->intersect.x = tmp_intersect.x;
        r->intersect.y = tmp_intersect.y;
        r->intersect.z = tmp_intersect.z;
        r->dist = tmp_dist;
    }

}

static void resolve_sphere(float a, float b, float c, Window *w, Ray *r, Sphere s, Light l)
{
    float disc;
    float x1;
    float x2;

    disc = b * b - 4.0f * a * c;
    if (disc > 0.0f)
    {
        x1 = - (b - sqrt(disc)) / (2.0f * a);
        x2 = - (b + sqrt(disc)) / (2.0f * a);

        intersectionSphere(x1, x2, r);
        if (!isForward(r))
            return;

        vect norm;
        color i_tot;
        i_tot.r = 0.0f;
        i_tot.g = 0.0f;
        i_tot.b = 0.0f;
        normalSphere(&norm, *r, s);
        reflexionPartielle(s, r, l, norm, &i_tot);

        i_tot.r = i_tot.r < 0 ? 0 : i_tot.r;
        i_tot.g = i_tot.g < 0 ? 0 : i_tot.g;
        i_tot.b = i_tot.b < 0 ? 0 : i_tot.b;

        i_tot.r = i_tot.r > 255 ? 255 : i_tot.r;
        i_tot.g = i_tot.g > 255 ? 255 : i_tot.g;
        i_tot.b = i_tot.b > 255 ? 255 : i_tot.b;

        SDL_SetRenderDrawColor(w->render, i_tot.r, i_tot.g, i_tot.b, 255);
    }
    else
        SDL_SetRenderDrawColor(w->render, 0, 0, 0, 255);
}

void traceSphere(Ray *r, Sphere *s, Window *w, Light l)
{
    float a;
    float b;
    float c;

    a = pow(r->dirV.x,2) + pow(r->dirV.y,2) + pow(r->dirV.z,2);
    b = 2 * (r->pos.x * r->dirV.x + r->pos.y * r->dirV.y + r->pos.z * r->dirV.z - (s->pos.x * r->dirV.x + s->pos.y * r->dirV.y + s->pos.z * r->dirV.z));
    c = pow(r->pos.x,2) + pow(s->pos.x,2) - 2 * r->pos.x * s->pos.x + pow(r->pos.y,2) + pow(s->pos.y,2) - 2 * r->pos.y * s->pos.y + pow(r->pos.z,2) + pow(s->pos.z,2) - 2 * r->pos.z * s->pos.z - pow(s->r,2);
    resolve_sphere(a, b, c, w, r, *s, l);
}










static void reflexionPartiellePlan(Plan p, Ray *r, Light l, vect norm, color *i_tot)
{
    float i_amb = 0.0f;
    float i_diff = 0.0f;
    float i_spec = 0.0f;

    i_amb += p.ka * l.i;
    i_diff += p.kd * l.i * reflexionDiffuse(r, l, norm);

    i_spec = p.ks * l.i * pow(reflexionSpeculaire(r, l, norm), p.n);
    i_tot->r += i_spec * l.c.r;
    i_tot->g += i_spec * l.c.g;
    i_tot->b += i_spec * l.c.b;

    i_tot->r += (i_amb + i_diff) * p.c.r;
    i_tot->g += (i_amb + i_diff) * p.c.g;
    i_tot->b += (i_amb + i_diff) * p.c.b;
}

void normalPlan(vect *norm, Plan p)
{
    float d;
    norm->x = p.w;
    norm->y = p.x;
    norm->z = p.y;
    d = distance(norm->x, 0.0f, norm->y, 0.0f, norm->z, 0.0f);
    normalisation(norm, d);
}

void intersectPlan(Ray *r, float t)
{
    r->intersect.x = r->pos.x + r->dirV.x * t;
    r->intersect.y = r->pos.y + r->dirV.y * t;
    r->intersect.z = r->pos.z + r->dirV.z * t;
}

void tracePlan(Ray *r, Plan *p, Window *w, Light l)
{
    float t;
    t = (p->z - p->w * r->pos.x - p->x * r->pos.y - p->y * r->pos.z) / (p->w * r->dirV.x + p->x * r->dirV.y + p->y * r->dirV.z);
    if (!isForward(r))
        return;
    vect norm;
    color i_tot;
    i_tot.r = 0.0f;
    i_tot.g = 0.0f;
    i_tot.b = 0.0f;

    if (sameSign(r->pos.x, r->dirV.x) && sameSign(r->pos.y, r->dirV.y) && sameSign(r->pos.z, r->dirV.z)) {
        normalPlan(&norm, *p);

        normalPlan(&norm, *p);
        reflexionPartiellePlan(*p, r, l, norm, &i_tot);

        i_tot.r = i_tot.r < 0 ? 0 : i_tot.r;
        i_tot.g = i_tot.g < 0 ? 0 : i_tot.g;
        i_tot.b = i_tot.b < 0 ? 0 : i_tot.b;

        i_tot.r = i_tot.r > 255 ? 255 : i_tot.r;
        i_tot.g = i_tot.g > 255 ? 255 : i_tot.g;
        i_tot.b = i_tot.b > 255 ? 255 : i_tot.b;

        SDL_SetRenderDrawColor(w->render, i_tot.r, i_tot.g, i_tot.b, 255);
    }
}