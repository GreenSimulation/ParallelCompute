//
//  command_queue.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef __TestOpenCL__command_queue__
#define __TestOpenCL__command_queue__

#include <iostream>
#include <vector>

#include <OpenCL/OpenCL.h>

namespace ParallelCompute
{
    class Context;
    
    class Program;
    class Memory;
    
    class CommandQueue
    {
    private:
        typedef std::vector<Program*> PROGRAM_ARRAY;
        
    private:
        Context*         _context; // 약한 참조, weak
        cl_command_queue _command_queue;
        
        PROGRAM_ARRAY    _programs;
        
    public:
        CommandQueue(Context* context);
        ~CommandQueue(void);
        
    public:
        cl_command_queue queue(void) const;
        ulong            program_count(void) const;
        void             program_count(const ulong&& count);
        
    public:
        bool initialization(void);
        void release(void);
        
        void program(const int&& index, Program* program);
        
        void execute(int dim_count, uint* global_size, uint* local_size);
        
    public:
        Program* create_program(void);
        Memory*  create_memory_object(void);
    };
}

#endif /* defined(__TestOpenCL__command_queue__) */
