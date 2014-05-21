//
//  export.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef TestOpenCL_export_h
#define TestOpenCL_export_h

typedef unsigned int  uint;
typedef unsigned long ulong;

extern "C"
{
    bool InitOpenCL(bool debug_output, char* file_path);
    void ReleaseOpenCL(void);
    
    uint CreateContext(int device_type);
    uint CreateCommandQueue(uint context, uint queue_size);
    uint CreateProgramWithSource(uint command_queue, const char* kernel_name, const char* source, uint length);
    uint CreateProgramFile(uint command_queue, char* file_name); // 미구현.
    uint CreateMemoryObject(uint command_queue, int mode, uint array_count, uint memory_count);
    
    void DeleteContext(uint context_key);
    void DeleteCommandQueue(uint command_queue_key);
    void DeleteProgram(uint program);
    void DeleteMemoryObject(uint memory);
    
    void SetProgramArg(uint program, uint index, uint mem);
    
    void AddCommandQueue(uint command_queue, uint order_index, uint program);
    
    void WriteMemory(uint mem, uint array_size, uint object_size, void* array);
    void ReadMemory(uint mem, uint array_size, uint object_size, void* array);
    
    void Execute(uint command_queue, int dim_count, uint* global_size, uint* local_size);
}

#endif
