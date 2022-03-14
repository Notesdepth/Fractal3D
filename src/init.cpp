//
// Created by Notesdepth on 19/09/2018.
//

#include <frac.h>

void put_value(float *coord, int *pos, char *tab) //trouve et initialise les valeurs pour cam et vérifie si on a un ou deux chiffres ou plus
{
    *coord = (float)tab[*pos] - 48;
    while (tab[++*pos] != ' ' && (tab[*pos] >= 48 && tab[*pos] <= 57))
    {
        *coord *= 10;
        *coord += ((float)tab[*pos] - 48);
    }
}
void put_value(int *coord, int *pos, char *tab) //idem pour les int*
{
    *coord = (float)tab[*pos] - 48;
    while (tab[++*pos] != ' ' && (tab[*pos] >= 48 && tab[*pos] <= 57))
    {
        *coord *= 10;
        *coord += ((float)tab[*pos] - 48);
    }
}

int find_word(char *str, int count, const char *keyword)
{
    int i = -1;
    int j = 1;
    while (++i != count)
    {
        if (str[i] == keyword[0])
        {
            while (str[i + j] == keyword[j] && str[i + j] != '\0' && keyword[j] != '\0')
                j++;
            if (keyword[j] == '\0')
                return (i + j + 2); //+3 correspondant à " : " dans le fichier
            j = 1;
        }
    }
    return (-1);
}


void initCam(Camera *cam)
{

    FILE *file = fopen("input/input.txt", "r");
    if (file == NULL)
    {
        printf("errno = %d\n",errno);
        return;
    }

    fseek(file, 0, SEEK_END); //on se place à la fin du fichier
    size_t count = ftell(file); //on récupère la position pour le nombre de caractère
    char *tab = (char *)malloc(sizeof(char) * count);
    fseek(file, 0, SEEK_SET); //on se remet au début du fichier (UTILE ??)
    fread(tab, count, sizeof(char),file);

    int i;
    i = find_word(tab, count, "pos");
    i++;
    put_value(&cam->pos.x, &i, tab);
    i++;
    put_value(&cam->pos.y, &i, tab);
    i++;
    put_value(&cam->pos.z, &i, tab);

    i = find_word(tab, count, "vp");
    i++;
    put_value(&cam->vp.x, &i, tab);
    i++;
    put_value(&cam->vp.y, &i, tab);
    i++;
    put_value(&cam->vp.z, &i, tab);
    i++;

    i = find_word(tab, count, "dirV");
    i++;
    put_value(&cam->dirV.x, &i, tab);
    i++;
    put_value(&cam->dirV.y, &i, tab);
    i++;
    put_value(&cam->dirV.z, &i, tab);

    i = find_word(tab, count, "fov");
    i++;
    put_value(&cam->fov, &i, tab);

    vect up;
    i = find_word(tab, count, "up");
    i++;
    put_value(&up.x, &i, tab);
    i++;
    put_value(&up.y, &i, tab);
    i++;
    put_value(&up.z, &i, tab);

    cam->dirV.x = cam->vp.x - cam->pos.x;
    cam->dirV.y = cam->vp.y - cam->pos.y;
    cam->dirV.z = cam->vp.z - cam->pos.z;

    cam->normV.x = cam->dirV.y * up.z - cam->dirV.z * up.y;
    cam->normV.y = cam->dirV.z * up.x - cam->dirV.y * up.z;
    cam->normV.z = cam->dirV.x * up.y - cam->dirV.x * up.x;

    cam->upV.x = cam->normV.y * cam->dirV.z - cam->normV.z * cam->dirV.y;
    cam->upV.y = cam->normV.z * cam->dirV.x - cam->normV.x * cam->dirV.z;
    cam->upV.z = cam->normV.x * cam->dirV.y - cam->normV.y * cam->dirV.x;

    float longueur = sqrt(pow(cam->normV.x,2) + pow(cam->normV.y,2) + pow(cam->normV.z,2));
    //ajouter condition si longueur = 0
    cam->normV.x /= longueur;
    cam->normV.y /= longueur;
    cam->normV.z /= longueur;

    longueur = sqrt(pow(cam->upV.x,2) + pow(cam->upV.y,2) + pow(cam->upV.z,2));
    //ajouter condition si longueur = 0
    cam->upV.x /= longueur;
    cam->upV.y /= longueur;
    cam->upV.z /= longueur;

    free(tab);
    fclose(file);
}

void initVp(ViewPoint *vp, Camera cam, int longueur, int largeur)
{
    float ratio = (float)largeur / (float)longueur;

    vp->demi_longueur = tan(cam.fov / 2.0f); //demi dimensions du viewplane
    vp->demi_largeur = ratio * vp->demi_longueur;
    vp->init_point.x = cam.vp.x - cam.upV.x * vp->demi_largeur - cam.normV.x * vp->demi_longueur; //position point en bas à gauche du viewplane
    vp->init_point.y = cam.vp.y - cam.upV.y * vp->demi_largeur - cam.normV.y * vp->demi_longueur;
    vp->init_point.z = cam.vp.z - cam.upV.z * vp->demi_largeur - cam.normV.z * vp->demi_longueur;

    vp->xvect.x = (cam.normV.x * 2 * vp->demi_longueur) / longueur; //vecteurs déplacement sur le viewplane
    vp->xvect.y = (cam.normV.y * 2 * vp->demi_longueur) / longueur;
    vp->xvect.z = (cam.normV.z * 2 * vp->demi_longueur) / longueur;

    vp->yvect.x = (cam.upV.x * 2 * vp->demi_largeur) / largeur;
    vp->yvect.y = (cam.upV.y * 2 * vp->demi_largeur) / largeur;
    vp->yvect.z = (cam.upV.z * 2 * vp->demi_largeur) / largeur;
}

void initFileRay(Ray *r)
{
    FILE *file = fopen("input/input.txt", "r");
    if (file == NULL) {
        printf("errno = %d\n", errno);
        return;
    }

    fseek(file, 0, SEEK_END); //on se place à la fin du fichier
    size_t count = ftell(file); //on récupère la position pour le nombre de caractère
    char *tab = (char *)malloc(sizeof(char) * count);
    fseek(file, 0, SEEK_SET); //on se remet au début du fichier (UTILE ??)
    fread(tab, count, sizeof(char),file);

    int i;
    i = find_word(tab, count, "dist");
    i++;
    put_value(&r->dist, &i, tab);

    i = find_word(tab, count, "it_max");
    i++;
    put_value(&r->it_max, &i, tab);
    free(tab);
    i = fclose(file);
    if (i != 0)
        printf("err = %d\n", i);
}
void initRay(Ray *r, ViewPoint vp, Camera cam, int x, int y)
{
    r->pos.x = vp.init_point.x + x * vp.xvect.x + y * vp.yvect.x; //initial donc sur le vp
    r->pos.y = vp.init_point.y + x * vp.xvect.y + y * vp.yvect.y;
    r->pos.z = vp.init_point.z + x * vp.xvect.z + y * vp.yvect.z;
    r->dirV.x = r->pos.x - cam.pos.x;
    r->dirV.y = r->pos.y - cam.pos.y;
    r->dirV.z = r->pos.z - cam.pos.z;
}

void initSphere(Sphere *s)
{
    FILE *file = fopen("input/input.txt", "r");
    if (file == NULL)
    {
        printf("errno = %d\n",errno);
        return;
    }

    fseek(file, 0, SEEK_END); //on se place à la fin du fichier
    size_t count = ftell(file); //on récupère la position pour le nombre de caractère
    char *tab = (char *)malloc(sizeof(char) * count);
    fseek(file, 0, SEEK_SET); //on se remet au début du fichier (UTILE ??)
    fread(tab, count, sizeof(char),file);

    int i;
    i = find_word(tab, count, "s_pos");
    i++;
    put_value(&s->pos.x, &i, tab);
    printf("s->pos.x = %f\n", s->pos.x);
    i++;
    put_value(&s->pos.y, &i, tab);
    i++;
    put_value(&s->pos.z, &i, tab);

    i = find_word(tab, count, "s_rgb");
    i++;
    put_value(&s->c.r, &i, tab);
    i++;
    printf("s->c.r = %d\n", s->c.r);

    put_value(&s->c.g, &i, tab);
    i++;
    put_value(&s->c.b, &i, tab);

    i = find_word(tab, count, "s_ka");
    i++;
    put_value(&s->ka, &i, tab);
    printf("s->ka = %f\n", s->ka);
    i = find_word(tab, count, "s_kd");
    i++;
    put_value(&s->kd, &i, tab);
    i = find_word(tab, count, "s_ks");
    i++;
    put_value(&s->kd, &i, tab);
    i = find_word(tab, count, "s_n");
    i++;
    put_value(&s->n, &i, tab);

    free(tab);
    fclose(file);
}