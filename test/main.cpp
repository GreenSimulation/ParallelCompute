//
//  main.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 8..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include "export.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

const char* buffer = "\
int index(int x, int y, int z)\
{\
    int size_x = get_global_size(0);\
	int size_y = get_global_size(1);\
	\
	return z*size_x*size_y + y*size_x + x;\
}\
\
float value(float* array, int x, int y, int z)\
{\
	return array[index(x, y, z)];\
}\
\
__kernel void Demo(__global float* out_array)\
{\
	int x = get_global_id(0);\
	int y = get_global_id(1);\
	int z = get_global_id(2);\
    \
	out_array[index(x, y, z)] = 10.0f;\
}\
";

const char* kernel_name = "Demo";

void showReuslt(float* array, int x, int y, int z, int max_x, int max_y)
{
    z = (z < 1)?1:z;
    printf("Pos[%3d,%3d,%3d]:%f\n", x, y, z, array[(z - 1)*max_x*max_y + (y - 1)*max_x + (x - 1)]);
}

int main()
{
    std::string code = buffer;
    clock_t start_time, end_time;
    
    int size_x     = 1100;
    int size_y     = 833;
    int size_z     = 30;
    int array_size = size_x*size_y*size_z;
    int test_run   = 1;
    
    float* test1  = (float*)malloc(size_x*size_y*size_z*sizeof(float));
    float* test2  = (float*)malloc(size_x*size_y*size_z*sizeof(float));
    uint  global[] = {static_cast<uint>(size_x), static_cast<uint>(size_y), static_cast<uint>(size_z)};
    uint  local[]  = {1, 1, 1};
    
    clock_t run_time = 0;
    double  time     = 0.0;
    
    InitOpenCL(false, nullptr);
    
    uint context = CreateContext(3);
    uint queue   = CreateCommandQueue(context, 1);
    
    uint program = CreateProgramWithSource(queue, kernel_name, buffer, (uint)code.length());
    uint in_mem  = CreateMemoryObject(queue, 0, sizeof(float), size_x*size_y*size_z);
    uint out_mem = CreateMemoryObject(queue, 0, sizeof(float), size_x*size_y*size_z);
    
    SetProgramArg(program, 0, out_mem);
    //SetProgramArg(program, 1, in_mem);
    
    AddCommandQueue(queue, 0, program);
    
    //WriteMemory(in_mem, size_x*size_y*size_z, sizeof(float), (void*)test1);
    //WriteMemory(out_mem, size_x*size_y*size_z, sizeof(float), (void*)test2);
    
    printf("Test Run [%d count]\n", test_run);
    printf("Memory Size      : %d, (%ld byte)\n", array_size, array_size*sizeof(float));
    
    run_time = 0.0;
    for (int n = 0; n < test_run; ++n ) {
        start_time = clock();
        Execute(queue, 3, global, local);
        end_time = clock();
        run_time = run_time + (end_time - start_time);
    }
    
    time = (double)(run_time)/CLOCKS_PER_SEC;
    printf("GPU Running Time : [run:%2f sec] [avg:%2f sec]\n", time, time/(double)test_run);
    

    run_time = 0.0;
    for( int m = 0; m < test_run; ++m ) {
        start_time = clock();
        //for( int n = 0; n < array_size; ++n ) {
        //    test1[n] = 2.0f;
        //}
        end_time = clock();
        run_time = run_time + (end_time - start_time);
    }
    
    time = (double)(run_time)/CLOCKS_PER_SEC;
    printf("CPU Running Time : [run:%2f sec] [avg:%2f sec]\n", time, time/(double)test_run);

    run_time = 0.0;
    for( int n = 0; n < test_run; ++ n ) {
        start_time = clock();
        WriteMemory(in_mem, size_x*size_y*size_z, sizeof(float), (void*)test1);
        end_time = clock();
        run_time = run_time + (end_time - start_time);
    }
    
    time = (double)(run_time)/CLOCKS_PER_SEC;
    printf("Write Time       : [run:%2f sec] [avg:%2f sec]\n", time, time/(double)test_run);
    
    run_time = 0.0;
    for( int n = 0; n < test_run; ++n ) {
        start_time = clock();
        ReadMemory(out_mem, size_x*size_y*size_z, sizeof(float), (void*)test2);
        end_time = clock();
        run_time = run_time + (end_time - start_time);
    }
    
    time = (double)(run_time)/CLOCKS_PER_SEC;
    printf("Read Time        : [run:%2f sec] [avg:%2f sec]\n", time, time/(double)test_run);

    printf("\nOutput\n");
    showReuslt(test2, size_x/512, size_y/512, size_z/512, size_x, size_y);
    showReuslt(test2, size_x/256, size_y/256, size_z/256, size_x, size_y);
    showReuslt(test2, size_x/128, size_y/128, size_z/128, size_x, size_y);
    showReuslt(test2, size_x/64, size_y/64, size_z/64, size_x, size_y);
    showReuslt(test2, size_x/32, size_y/32, size_z/32, size_x, size_y);
    showReuslt(test2, size_x/16, size_y/16, size_z/16, size_x, size_y);
    showReuslt(test2, size_x/8, size_y/8, size_z/8, size_x, size_y);
    showReuslt(test2, size_x/4, size_y/4, size_z/4, size_x, size_y);
    showReuslt(test2, size_x/2, size_y/2, size_z/2, size_x, size_y);
    showReuslt(test2, size_x, size_y, size_z, size_x, size_y);

    
    DeleteMemoryObject(out_mem);
    DeleteMemoryObject(in_mem);
    DeleteProgram(program);
    DeleteContext(context);
    
    ReleaseOpenCL();
    
    free(test1);
    free(test2);
}
