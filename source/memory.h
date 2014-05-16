//
//  memory.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef __TestOpenCL__memory__
#define __TestOpenCL__memory__

#include <iostream>

#include <OpenCL/OpenCL.h>

namespace ParallelCompute
{
    class Context;
    class CommandQueue;
    
    class Memory
    {
    public:
        enum class ACCESS_MODE : int
        {
            READ_WRITE,
            WRITE_ONLY,
            READ_ONLY
        };
        
    private:
        Context*      _context;
        CommandQueue* _command_queue;
        
        cl_mem        _mem_object;
        
    public:
        Memory(Context* context, CommandQueue* queue);
        ~Memory(void);
        
    public:
        cl_mem memory(void) const;
        
    public:
        bool initialization(const ACCESS_MODE&& mode, const ulong& array_count, const ulong& memory_count);
        void release(void);
        
    public:
        void copy_in(ulong array_count, ulong object_size, void* array);
        void copy_out(ulong arrya_count, ulong object_size, void* array);
        
    private:
        cl_mem_flags convert(const ACCESS_MODE& mode);
    };
}

#endif /* defined(__TestOpenCL__memory__) */
