__kernel void mandelbrot(__global int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2)
{
    int id = get_global_id(0);
    if (id % 3 != 0)
        return;

    int i = (id / 3) % (int)width;
    int j = (id / 3) / (int)width;

    float2 z1;
    float tmp_r = 0.0f;
    float tmp_i = (float)i / (width / (x2 - x1)) + x1;
    float tmp_j = (float)j / (height / (y2 - y1)) + y1;
    
    int it = -1;
    while (++it < it_max && z1.x * z1.x + z1.y * z1.y < 4)
    {
        tmp_r = z1.x;
        z1.x = z1.x * z1.x - z1.y * z1.y + tmp_i;
        z1.y = 2 * tmp_r * z1.y + tmp_j;
    }

    /*ptr_screen[id] = (float)sin((float)0.2 * it + 4) * 128 + 127;
    ptr_screen[id + 1] = (float)sin((float)0.2 * it + 2) * 128 + 127;
    ptr_screen[id + 2] = (float)sin((float)0.2 * it + 2) * 128 + 127;*/

    float sn = it - log2(log2(z1.x * z1.x + z1.y * z1.y)) + 4.0;
    float a = smoothstep(-0.1, 0, sin(0.5f*6.2831f*it));
    float l = mix(it, sn, a);
    ptr_screen[id] = sin(l * 0.15f + 0) * 128 + 127;
    ptr_screen[id + 1] = sin(l * 0.15f + 0.6f) * 128 + 127;
    ptr_screen[id + 2] = sin(l * 0.15f + 1) * 128 + 127;

    /*ptr_screen[id] = 255 * (1 - cos(3.3219f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
    ptr_screen[id] = 255 * (1 - cos(0.78297f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
    ptr_screen[id] = 255 * (1 - cos(0.41357f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
    à retravailler*/


}

__kernel void julia(__global int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2, int x, int y)
{
    int id = get_global_id(0);
    if (id % 3 != 0)
        return;
    int i = (id / 3) % (int)width;
    int j = (id / 3) / (int)width;

    float2 z1;
    z1.x = (float)i / (width / (x2 - x1)) + x1;
    z1.y = (float)j / (height / (y2 - y1)) + y1;

    float tmp_r = 0.0f;
    float tmp_i = (float)x / (width / (x2 - x1)) + x1;
    float tmp_j = (float)y / (height / (y2 - y1)) + y1;

    int it = -1;
    while (++it < it_max && z1.x * z1.x + z1.y * z1.y < 4)
    {
        tmp_r = z1.x;
        z1.x = z1.x * z1.x - z1.y * z1.y + tmp_i;
        z1.y = 2 * tmp_r * z1.y + tmp_j;
    }

    /*ptr_screen[id] = (float)sin((float)0.2f * it) * 128 + 127;
    ptr_screen[id + 1] = (float)sin((float)0.2f * it + 2) * 128 + 127;
    ptr_screen[id + 2] = (float)sin((float)0.2f * it + 4) * 128 + 127;*/
    float sn = it - log2(log2(z1.x * z1.x + z1.y * z1.y)) + 4.0;
    float a = smoothstep(-0.1, 0, sin(0.5f * 6.2831f * it));
    float l = mix(it, sn, a);

    ptr_screen[id] = sin(l * 0.15f + 0) * 128 + 127;
    ptr_screen[id + 1] = sin(l * 0.15f + 1.2f) * 128 + 127;
    ptr_screen[id + 2] = sin(l * 0.15f + 1) * 128 + 127;
}

__kernel void tricorn(__global int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2)
{
    int id = get_global_id(0);
    if (id % 3 != 0)
        return;

    int i = (id / 3) % (int)width;
    int j = (id / 3) / (int)width;

    float2 z1;
    float tmp_r = 0.0f;

    float tmp_i = (float)i / (width / (x2 - x1)) + x1;
    float tmp_j = (float)j / (height / (y2 - y1)) + y1;

    int it = -1;
    while (++it < it_max && z1.x * z1.x + z1.y * z1.y < 4)
    {
        tmp_r = z1.x;
        z1.x = z1.x * z1.x - z1.y * z1.y + tmp_i;
        z1.y = -2 * tmp_r * z1.y + tmp_j;
    }

    /*ptr_screen[id] = (float)sin((float)0.2 * it + 4) * 128 + 127;
    ptr_screen[id + 1] = (float)sin((float)0.2 * it + 2) * 128 + 127;
    ptr_screen[id + 2] = (float)sin((float)0.2 * it + 2) * 128 + 127;*/

    float sn = it - log2(log2(z1.x * z1.x + z1.y * z1.y)) + 4.0;
    float a = smoothstep(-0.1, 0, sin(0.5f*6.2831f*it));
    float l = mix(it, sn, a);
    ptr_screen[id] = sin(l * 0.15f + 0) * 128 + 127;
    ptr_screen[id + 1] = sin(l * 0.15f + 1) * 128 + 127;
    ptr_screen[id + 2] = sin(l * 0.15f + 2) * 128 + 127;
}

__kernel void burningShip(__global int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2)
{
    int id = get_global_id(0);
    if (id % 3 != 0)
        return;

    int i = (id / 3) % (int)width;
    int j = (id / 3) / (int)width;

    float2 z1;
    float tmp_r = 0.0f;

    float tmp_i = (float)i / (width / (x2 - x1)) + x1;
    float tmp_j = (float)j / (height / (y2 - y1)) + y1;

    int it = -1;
    while (++it < it_max && z1.x * z1.x + z1.y * z1.y < 4)
    {
        tmp_r = z1.x;
        z1.x = z1.x * z1.x - z1.y * z1.y + tmp_i;
        z1.y = fabs(2 * tmp_r * z1.y) + tmp_j;
    }

    /*ptr_screen[id] = (float)sin((float)0.2 * it + 4) * 128 + 127;
    ptr_screen[id + 1] = (float)sin((float)0.2 * it + 2) * 128 + 127;
    ptr_screen[id + 2] = (float)sin((float)0.2 * it + 2) * 128 + 127;*/

    float sn = it - log2(log2(z1.x * z1.x + z1.y * z1.y)) + 4.0;
    float a = smoothstep(-0.1, 0, sin(0.5f*6.2831f*it));
    float l = mix(it, sn, a);
    ptr_screen[id] = sin(l * 0.15f + 0) * 128 + 127;
    ptr_screen[id + 1] = sin(l * 0.15f + 1) * 128 + 127;
    ptr_screen[id + 2] = sin(l * 0.15f + 2) * 128 + 127;
}

//__kernel void buddhabrot()
//{
//    int id = get_global_id(0);
//    if (id % 3 != 0)
//        return;
//
//    int i = (id / 3) % (int)width;
//    int j = (id / 3) / (int)width;
//
//    float2 z1;
//    float tmp_r = 0.0f;
//    float tmp_i = (float)i / (width / (x2 - x1)) + x1;
//    float tmp_j = (float)j / (height / (y2 - y1)) + y1;
//
//    int it = -1;
//    while (++it < it_max && z1.x * z1.x + z1.y * z1.y < 4)
//    {
//        tmp_r = z1.x;
//        z1.x = z1.x * z1.x - z1.y * z1.y + tmp_i;
//        z1.y = 2 * tmp_r * z1.y + tmp_j;
//    }
//
//    /*ptr_screen[id] = (float)sin((float)0.2 * it + 4) * 128 + 127;
//    ptr_screen[id + 1] = (float)sin((float)0.2 * it + 2) * 128 + 127;
//    ptr_screen[id + 2] = (float)sin((float)0.2 * it + 2) * 128 + 127;*/
//
//    float sn = it - log2(log2(z1.x * z1.x + z1.y * z1.y)) + 4.0;
//    float a = smoothstep(-0.1, 0, sin(0.5f*6.2831f*it));
//    float l = mix(it, sn, a);
//    ptr_screen[id] = sin(l * 0.15f + 0) * 128 + 127;
//    ptr_screen[id + 1] = sin(l * 0.15f + 0.6f) * 128 + 127;
//    ptr_screen[id + 2] = sin(l * 0.15f + 1) * 128 + 127;
//
//    /*ptr_screen[id] = 255 * (1 - cos(3.3219f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
//    ptr_screen[id] = 255 * (1 - cos(0.78297f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
//    ptr_screen[id] = 255 * (1 - cos(0.41357f * log(sqrt(z1.x * z1.x + z1.y * z1.y)) / 2.0f)) / 2.0f;
//    à retravailler*/
//
//}