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
    cl_context   context   = _context->context();
    cl_device_id device_id = _context->device();
    cl_int       result    = 0;
    
    _program = clCreateProgramWithSource(context, 1, source_str, source_size, &result);
    
    if ( SUCCESS(result, "create program with source") ) {
        result = clBuildProgram(_program, 1, &device_id, NULL, NULL, NULL);
        
        if ( SUCCESS(result, "build program") ) {
            _kernel = clCreateKernel(_program, kernel_name, &result);
            
            return SUCCESS(result, "create kerenl");
        }
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

bool Program::setMemoryObject(uint index, Memory* memory)
{
    cl_mem memory_object = memory->memory();
    
    return SUCCESS(clSetKernelArg(_kernel, index, sizeof(cl_mem), &memory_object), "set kernel arg");
}