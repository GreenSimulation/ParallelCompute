//
//  export.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef TestOpenCL_export_h
#define TestOpenCL_export_h

//typedef unsigned int  uint;
typedef unsigned long ulong;

extern "C"
{
    bool InitOpenCL(void);
    void ReleaseOpenCL(void);
    
    uint CreateContext(int device_type);
    uint CreateCommandQueue(uint context, uint queue_size);
    uint CreateProgramWithSource(uint command_queue, const char* kernel_name, const char* source, ulong length);
    uint CreateProgramFile(uint command_queue, char* file_name); // 미구현.
    uint CreateMemoryObject(uint command_queue, int mode, ulong array_count, ulong memory_count);
    
    void DeleteContext(uint context_key);
    void DeleteCommandQueue(uint command_queue_key);
    void DeleteProgram(uint program);
    void DeleteMemoryObject(uint memory);
    


    void SetProgramArg(uint program, uint index, uint mem);
    
    void AddCommandQueue(uint command_queue, uint order_index, uint program);
    
    void WriteMemory(uint mem, ulong array_size, ulong object_size, void* array);
    void ReadMemory(uint mem, ulong array_size, ulong object_size, void* array);
    
    void Execute(uint command_queue, int dim_count, ulong* global_size, ulong* local_size);
}

#endif
