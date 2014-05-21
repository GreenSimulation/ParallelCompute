//
//  memory.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//
#include <string>

#include "device.h"

#include "memory.h"
#include "context.h"
#include "command_queue.h"

using namespace ParallelCompute;

Memory::Memory(Context* context, CommandQueue* queue) : _context(context), _command_queue(queue), _mem_object(0) {}
Memory::~Memory(void) { this->release(); }

cl_mem Memory::memory(void) const
{
    return _mem_object;
}

bool Memory::initialization(const ACCESS_MODE&& mode, const ulong& array_count, const ulong& memory_count)
{
    char         buf[20];
    cl_context   context = _context->context();
    cl_mem_flags flags   = this->convert(std::move(mode));
    cl_int       result  = 0;
    
    _mem_object = clCreateBuffer(context, flags, array_count*memory_count, NULL, &result);

    sprintf(buf, "%ld", array_count*memory_count);
    if( !SUCCESS(result, std::string("create buffer : ")+buf) ) {
        return false;
    }
    
    return true;
}

void Memory::release(void)
{
    if( _mem_object != 0 ) {
        clReleaseMemObject(_mem_object);
        _mem_object = 0;
    }
}

void Memory::copy_in(ulong array_count, ulong object_size, void* array)
{
    cl_command_queue queue = _command_queue->queue();
    
    clEnqueueWriteBuffer(queue, _mem_object, CL_TRUE, 0, array_count*object_size, array, 0, NULL, NULL);
}

int Memory::copy_out(ulong array_count, ulong object_size, void* array)
{
    cl_command_queue queue = _command_queue->queue();
    
    return clEnqueueReadBuffer(queue, _mem_object, CL_TRUE, 0, array_count*object_size, array, 0, NULL, NULL);
}

cl_mem_flags Memory::convert(const ACCESS_MODE& mode)
{
    cl_mem_flags flags = 0;
    
    switch (mode) {
        case ACCESS_MODE::READ_WRITE: flags = CL_MEM_READ_WRITE; break;
        case ACCESS_MODE::READ_ONLY:  flags = CL_MEM_READ_ONLY;  break;
        case ACCESS_MODE::WRITE_ONLY: flags = CL_MEM_WRITE_ONLY; break;
    };
    
    return flags;
}