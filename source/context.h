//
//  Context.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef __TestOpenCL__Context__
#define __TestOpenCL__Context__

#include <iostream>
#include <vector>

#include <OpenCL/OpenCL.h>

namespace ParallelCompute
{
    class Device;
    
    class CommandQueue;
    class Memory;
    
    class Context
    {
    private:
        Device*      _device;
        cl_context   _context;
        
        int          _device_index;
        
    public:
        Context(Device* device, const int& device_index);
        ~Context(void);
        
    public:
        cl_context   context(void) const;
        cl_device_id device(void) const;
        
    public:
        bool initialization(void);
        void release(void);
        
    public:
        CommandQueue* create_commend_queue(void);
    };
}

#endif /* defined(__TestOpenCL__Context__) */
