//
//  program.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include "program.h"

#include "device.h"
#include "context.h"
#include "memory.h"

using namespace ParallelCompute;

Program::Program(Context* context) : _context(context), _program(0), _kernel(0) {}
Program::~Program(void) { this->release(); }

cl_program Program::program(void) const
{
    return _program;
}

cl_kernel Program::kernel(void) const
{
    return _kernel;
}

bool Program::initialization(char* kernel_name, const char** source_str, const size_t* source_size)
{
    cl_context      context   = _context->context();
    cl_device_id    device_id = _context->device();
    cl_int          result    = 0;
    
    char buf[2048];
    
    _program = clCreateProgramWithSource(context, 1, source_str, source_size, &result);
    
    if ( SUCCESS(result, "create program with source") ) {
        result = clBuildProgram(_program, 1, &device_id, NULL, NULL, NULL);
        
        if ( SUCCESS(result, "build program") ) {
            _kernel = clCreateKernel(_program, kernel_name, &result);
            
            return SUCCESS(result, "create kerenl");
        }
        
        cl_build_status status;
        size_t          log_size;
        
        clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), (void*)&status, NULL);
        printf("Build Status: %d\n", status);
        
        clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_OPTIONS, 2048, buf, NULL);
        printf("Build Options: %s\n", buf);
        
        
        clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *) malloc(log_size);
        
        clGetProgramBuildInfo(_program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build Log: %s\n", log);
        
        free(log);
    }
    
    return false;
}

void Program::release(void)
{
    if ( _program != 0) {
        clReleaseKernel(_kernel);
        clReleaseProgram(_program);
        
        _kernel = 0;
        _program = 0;
    }
}

int Program::setMemoryObject(uint index, Memory* memory)
{
    cl_mem memory_object = memory->memory();
    
    int result = clSetKernelArg(_kernel, index, sizeof(cl_mem), &memory_object);
    SUCCESS(result, "set kernel arg");
    
    return result;
}