//
// Created by Notesdepth on 20/05/2019.
//

#include <frac.h>

#include <errno.h>  /* errno */
#include <string.h>

void release(ocl *ocl, cl_mem screen_buffer)
{
    clReleaseKernel(ocl->kernel);
    clReleaseProgram(ocl->program);
    clReleaseCommandQueue(ocl->queue);
    clReleaseMemObject(screen_buffer);
    clReleaseContext(ocl->context);
}

void CLReadBuffer(ocl *ocl, cl_mem screen_buffer, size_t size, int *ptr_screen)
{
    ocl->err_code = clEnqueueReadBuffer(ocl->queue, screen_buffer, CL_TRUE, 0, size, ptr_screen, 0, NULL, NULL);
}

void CLRangeKernel(ocl *ocl, int size, int nb_boucle)
{
    size_t dim[] = {size, 0, 0}; //nombre de dimensions = nb de boucles, et taille de la boucle
    ocl->err_code = clEnqueueNDRangeKernel(ocl->queue, ocl->kernel, nb_boucle, NULL, dim, NULL, 0, NULL, NULL);
}

void CLCreateKernel(ocl *ocl, const char *source)
{
    ocl->kernel = clCreateKernel(ocl->program, source, &ocl->err_code);
}

cl_mem CLCreateBuffer(ocl *ocl, cl_mem_flags flags, size_t size, void *ptr_var)
{
    return clCreateBuffer(ocl->context, flags, size, ptr_var, &ocl->err_code);
}
void err(ocl *ocl)
{
    if (ocl->err_code != CL_SUCCESS)
    {
        printf("ERREUR :\n");
        char logErreurs[4096];
        size_t longueur_Log;

        clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, sizeof(logErreurs), logErreurs, &longueur_Log);
        printf("%s\n", logErreurs);
        return;
    }
}

void CLBuild(ocl *ocl)
{
    ocl->err_code = clBuildProgram(ocl->program, 1/*nb_device*/, &ocl->device, NULL, NULL, NULL);
}

void CLCreateProgramm(ocl *ocl, const char *prog_name)
{
    int file = open(prog_name, O_RDONLY);
    if (file == -1)
    {
        printf("le fichier n'existe pas\n");
        printf("%s\n",strerror(errno));
        return;
    }

    size_t max_size = 0x1000000;
    char *source = (char*)malloc(max_size);
    size_t src_size = read(file, source, max_size);
    close(file);

    ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char **)&source, (const size_t *)&src_size, &ocl->err_code);
    free(source);
}

void CLCreateCommandQueue(ocl *ocl, cl_command_queue_properties properties)
{
    ocl->queue = clCreateCommandQueue(ocl->context, ocl->device, properties, &ocl->err_code);
}

void CLCreateContext(ocl *ocl, cl_context_properties context)
{
    cl_context_properties properties[3];
    properties[0] = (cl_context_properties) CL_CONTEXT_PLATFORM;
    properties[1] = (cl_context_properties) ocl->platform;
    properties[2] = 0;
    ocl->context = clCreateContext(properties, 1/*nb de devices, peut possiblement varier*/, &ocl->device, NULL, NULL, &ocl->err_code);
}

void CLInitPlateform(ocl *ocl, int nb_entries)
{
    ocl->nb_entries = nb_entries;
    ocl->err_code = clGetPlatformIDs(nb_entries, &ocl->platform, &ocl->nb_entries); //création de la plateforme
    ocl->err_code = clGetDeviceIDs(ocl->platform, CL_DEVICE_TYPE_GPU, nb_entries, &ocl->device, &ocl->nb_entries);
}

void execCL(ocl *ocl, const char *s, char *prog_name, int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2)
{
    CLInitPlateform(ocl, 1);
    CLCreateContext(ocl, (cl_context_properties)CL_CONTEXT_PLATFORM);
    CLCreateCommandQueue(ocl, (cl_command_queue_properties) CL_QUEUE_PROFILING_ENABLE);
    CLCreateProgramm(ocl, prog_name);
    CLBuild(ocl);
    err(ocl);
    cl_mem screen_buffer = CLCreateBuffer(ocl, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * WIDTH * HEIGHT * 3, ptr_screen);
    CLCreateKernel(ocl, s);

    ocl->err_code = clSetKernelArg(ocl->kernel, 0, sizeof(cl_mem), &screen_buffer);
    ocl->err_code = clSetKernelArg(ocl->kernel, 1, sizeof(float), &width);
    ocl->err_code = clSetKernelArg(ocl->kernel, 2, sizeof(float), &height);
    ocl->err_code = clSetKernelArg(ocl->kernel, 3, sizeof(int), &it_max);
    ocl->err_code = clSetKernelArg(ocl->kernel, 4, sizeof(float), &x1);
    ocl->err_code = clSetKernelArg(ocl->kernel, 5, sizeof(float), &x2);
    ocl->err_code = clSetKernelArg(ocl->kernel, 6, sizeof(float), &y1);
    ocl->err_code = clSetKernelArg(ocl->kernel, 7, sizeof(float), &y2);
    err(ocl);

    CLRangeKernel(ocl, WIDTH * HEIGHT * 3, 1);
    CLReadBuffer(ocl, screen_buffer, sizeof(int) * WIDTH * HEIGHT * 3, ptr_screen);
    err(ocl);
    release(ocl, screen_buffer);
}

void juliaCL(ocl *ocl, const char *s, char *prog_name, int *ptr_screen, float width, float height, int it_max, float x1, float x2, float y1, float y2)
{
    CLInitPlateform(ocl, 1);
    CLCreateContext(ocl, (cl_context_properties)CL_CONTEXT_PLATFORM);
    CLCreateCommandQueue(ocl, (cl_command_queue_properties) CL_QUEUE_PROFILING_ENABLE);
    CLCreateProgramm(ocl, prog_name);
    CLBuild(ocl);
    err(ocl);
    cl_mem screen_buffer = CLCreateBuffer(ocl, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * WIDTH * HEIGHT * 3, ptr_screen);
    CLCreateKernel(ocl, s);

    int x, y;
    SDL_GetMouseState(&x, &y);

    ocl->err_code = clSetKernelArg(ocl->kernel, 0, sizeof(cl_mem), &screen_buffer);
    ocl->err_code = clSetKernelArg(ocl->kernel, 1, sizeof(float), &width);
    ocl->err_code = clSetKernelArg(ocl->kernel, 2, sizeof(float), &height);
    ocl->err_code = clSetKernelArg(ocl->kernel, 3, sizeof(int), &it_max);
    ocl->err_code = clSetKernelArg(ocl->kernel, 4, sizeof(float), &x1);
    ocl->err_code = clSetKernelArg(ocl->kernel, 5, sizeof(float), &x2);
    ocl->err_code = clSetKernelArg(ocl->kernel, 6, sizeof(float), &y1);
    ocl->err_code = clSetKernelArg(ocl->kernel, 7, sizeof(float), &y2);
    ocl->err_code = clSetKernelArg(ocl->kernel, 8, sizeof(int), &x);
    ocl->err_code = clSetKernelArg(ocl->kernel, 9, sizeof(int), &y);

    err(ocl);

    CLRangeKernel(ocl, WIDTH * HEIGHT * 3, 1);
    CLReadBuffer(ocl, screen_buffer, sizeof(int) * WIDTH * HEIGHT * 3, ptr_screen);
    err(ocl);
    release(ocl, screen_buffer);
}