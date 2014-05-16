//
//  command_queue.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "device.h"

#include "command_queue.h"
#include "context.h"
#include "program.h"

#include "memory.h"

using namespace ParallelCompute;

CommandQueue::CommandQueue(Context* context) : _context(context), _command_queue(0) {}
CommandQueue::~CommandQueue(void) { this->release(); }

cl_command_queue CommandQueue::queue(void) const
{
    return _command_queue;
}

ulong CommandQueue::program_count(void) const
{
    return _programs.size();
}

void CommandQueue::program_count(const ulong&& count)
{
    _programs.resize(count);
}

bool CommandQueue::initialization(void)
{
    cl_context   context   = _context->context();
    cl_device_id device_id = _context->device();
    cl_int       result    = 0;
    
    _command_queue = clCreateCommandQueue(context, device_id, 0, &result);
    
    cl_uint temp;
    size_t temp_t, temps[3];
    char buf[128];
    
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 128, buf, NULL);
    fprintf(stdout, "Device %s supports ", buf);
    clGetDeviceInfo(device_id, CL_DEVICE_VERSION, 128, buf, NULL);
    fprintf(stdout, "%s\n", buf);
    
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &temp, NULL);
    fprintf(stdout, "Max Clock Frequency      : %d\n", temp);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &temp, NULL);
    fprintf(stdout, "Max Compute Units        : %d\n", temp);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &temp, NULL);
    fprintf(stdout, "Max Work-Item Dimensions : %d\n", temp);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(temps), temps, NULL);
    fprintf(stdout, "Max Work-Item Size       : %zd, %zd, %zd\n", temps[0], temps[1], temps[2]);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &temp_t, NULL);
    fprintf(stdout, "Max Work-Group Size      : %zd\n", temp_t);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &temp_t, NULL);
    fprintf(stdout, "Max Mem Alloc Size       : %zd\n\n", temp_t);
    
    return SUCCESS(result, "create command queue");
}

void CommandQueue::release(void)
{
    if (_command_queue != 0) {
        clReleaseCommandQueue(_command_queue);
        _command_queue = 0;
    }
}

void CommandQueue::program(const int&& index, Program* program)
{
    assert(index < _programs.size());
    _programs[index] = program;
}

void CommandQueue::execute(int dim_count, ulong* global_size, ulong* local_size)
{
    char buf[10];
    
    for ( int n = 0; n < _programs.size(); ++n ) {
        cl_kernel kernel = _programs[n]->kernel();
        cl_int    result = 0;
        
        result = clEnqueueNDRangeKernel(_command_queue,
                                        kernel,
                                        dim_count,
                                        NULL,
                                        global_size,
                                        local_size,
                                        0,
                                        NULL,
                                        NULL);
        
        sprintf(buf, "%d", n);
        SUCCESS(result, std::string("Enqueue ND Ragne Kerenl [")+buf+"]");
        
        result = clFinish(_command_queue);
        SUCCESS(result, "Finish");
    }
}

Program* CommandQueue::create_program(void)
{
    return new Program(_context);
}

Memory* CommandQueue::create_memory_object(void)
{
    return new Memory(_context, this);
}