//
//  program.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef __TestOpenCL__program__
#define __TestOpenCL__program__

#include <iostream>
#include <map>

#include <OpenCL/OpenCL.h>

namespace ParallelCompute
{
    class Context;
    class CommandQueue;
    
    class Memory;
    
    class Program
    {
    private:
        Context*   _context;
        
        cl_program _program;
        cl_kernel  _kernel;
        
    public:
        Program(Context* context);
        ~Program(void);
        
    public:
        cl_program program(void) const;
        cl_kernel  kernel(void) const;
        
    public:
        bool initialization(char* kernel_name, const char** source_str, const size_t* source_size);
        void release(void);
        
        int setMemoryObject(uint index, Memory* memory);
    };
}

#endif /* defined(__TestOpenCL__program__) */
