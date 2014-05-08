//
//  main.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 8..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "ParallelCompute-Prefix.pch"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

typedef unsigned int uint;

int main()
{
    char buffer[128];
	//char fileName[] = "/Users/Macintosh/Project/Xcode/TestOpenCL/TestOpenCL/hello.cl";
	
    Init();
    
    uint context = CreateContext();
    uint commend = CreateCommendQueue(context);
    uint memory  = CreateBufferObject(context, 0, 128, sizeof(char));
    uint program = CreateProgramWithSource(context, "/Users/Macintosh/Project/GitHub/ParallelCompute/test/hello.cl");
    uint kernel  = CreateKernel(program, "hello");
    
    SetKernelArgFormObject(kernel, 0, memory);
    EnqueueTask(commend, kernel);
    
    EnqueueReadBuffer(commend, memory, 128, buffer);
    
    ReleaseKernel(kernel);
    ReleaseProgram(program);
    ReleaseMemObject(memory);
    ReleaseCommandQueue(commend);
    ReleaseContext(context);
    
    std::cout << std::string(buffer) << std::endl;
    
	return 0;
}
