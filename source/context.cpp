//
//  Context.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include "context.h"
#include "device.h"
#include "command_queue.h"

using namespace ParallelCompute;

Context::Context(Device* device, const int& device_index) : _device(device), _context(0), _device_index(device_index) {}
Context::~Context(void) { this->release(); }

cl_context Context::context(void) const
{
    return _context;
}

cl_device_id Context::device(void) const
{
    return _device->device(_device_index);
}

bool Context::initialization(void)
{
    cl_device_id device_id = this->device();
    cl_int       result    = 0;
    
    _context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &result);
    
    if( !SUCCESS(result, "create context") ) {
        return false;
    }
    
    return true;
}

void Context::release(void)
{
    if( _context != 0 ) {
        clReleaseContext(_context);
        _context = 0;
    }
}

CommandQueue* Context::create_commend_queue(void)
{
    return new CommandQueue(this);
}