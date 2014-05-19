//
//  export.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include <OpenCL/OpenCL.h>
#include <map>

#include "device.h"
#include "context.h"
#include "command_queue.h"
#include "memory.h"
#include "program.h"

#include "export.h"

using namespace ParallelCompute;

Device* device = nullptr;

std::map<uint, Context*>        ContextArray;
std::map<uint, CommandQueue*>   CommandQueueArray;
std::map<uint, Program*>        ProgramArray;
std::map<uint, Memory*>         MemoryArray;

unsigned int GenerateKey(void)
{
    static unsigned int NextID = 0;
    
    NextID = NextID + 1;
    return NextID;
}

bool InitOpenCL(void)
{
    if( device == nullptr ) {
        device = new Device();
        return device->initialization();
    }
    else {
        return true;
    }
}

void ReleaseOpenCL(void)
{
    for (auto it = ContextArray.begin(); it != ContextArray.end(); ++it ) {
        it->second->release();
        delete it->second;
    }
    
    std::map<uint, Context*> empty1;
    ContextArray.swap(empty1);
    
    for (auto it = CommandQueueArray.begin(); it != CommandQueueArray.end(); ++it ) {
        it->second->release();
        delete it->second;
    }
    
    std::map<uint, CommandQueue*> empty2;
    CommandQueueArray.swap(empty2);
    
    for (auto it = ProgramArray.begin(); it != ProgramArray.end(); ++it ) {
        it->second->release();
        delete it->second;
    }
    
    std::map<uint, Program*> empty3;
    ProgramArray.swap(empty3);
    
    for (auto it = MemoryArray.begin(); it != MemoryArray.end(); ++it ) {
        it->second->release();
        delete it->second;
    }
    
    std::map<uint, Memory*> empty4;
    MemoryArray.swap(empty4);
    
    device->release();
    delete device;
    
    device = nullptr;
}

uint CreateContext(int device_type)
{
    Context* context = device->create_context((Device::COMPUTE_MODE)device_type);
    uint     key     = GenerateKey();
    
    context->initialization();
    
    ContextArray.insert(std::make_pair(key, context));
    
    return key;
}

uint CreateCommandQueue(uint context, uint queue_size)
{
    auto it = ContextArray.find(context);
    
    if( it != ContextArray.end() ) {
        CommandQueue* command_queue = it->second->create_commend_queue();
        uint          key           = GenerateKey();
        
        command_queue->initialization();
        command_queue->program_count(std::move(queue_size));
        
        CommandQueueArray.insert(std::make_pair(key, command_queue));
        
        return key;
    }
    
    return 0;
}

uint CreateProgramWithSource(uint context, const char* kernel_name, const char* source, ulong length)
{
    auto it = CommandQueueArray.find(context);
    
    if( it != CommandQueueArray.end() ) {
        Program* program = it->second->create_program();
        uint     key     = GenerateKey();
        
        program->initialization((char*)kernel_name, (const char**)&source, &length);
        
        ProgramArray.insert(std::make_pair(key, program));
        
        return key;
    }
    
    return 0;
}

uint CreateProgramFile(uint context, char* file_name)
{
    return 0;
}

uint CreateMemoryObject(uint command_queue, int mode, ulong array_count, ulong memory_count)
{
    auto it = CommandQueueArray.find(command_queue);
    
    if( it != CommandQueueArray.end() ) {
        Memory* memory = it->second->create_memory_object();
        uint    key    = GenerateKey();
        
        memory->initialization((Memory::ACCESS_MODE)mode, array_count, memory_count);
        
        MemoryArray.insert(std::make_pair(key, memory));
        
        return key;
    }
    
    return 0;
}

void DeleteContext(uint context_key)
{
    auto it = ContextArray.find(context_key);
    
    if( it != ContextArray.end() ) {
        delete it->second;
        ContextArray.erase(it);
    }
}

void DeleteCommandQueue(uint command_queue_key)
{
    auto it = CommandQueueArray.find(command_queue_key);
    
    if( it != CommandQueueArray.end() ) {
        delete it->second;
        CommandQueueArray.erase(it);
    }
}

void DeleteProgram(uint program)
{
    auto it = ProgramArray.find(program);
    
    if( it != ProgramArray.end() ) {
        delete it->second;
        ProgramArray.erase(it);
    }
}

void DeleteMemoryObject(uint memory)
{
    auto it = MemoryArray.find(memory);
    
    if( it != MemoryArray.end() ) {
        delete it->second;
        MemoryArray.erase(it);
    }
}

void SetProgramArg(uint program, uint index, uint mem)
{
    auto it_program = ProgramArray.find(program);
    auto it_mem     = MemoryArray.find(mem);
    
    if( it_program != ProgramArray.end() && it_mem != MemoryArray.end() ) {
        it_program->second->setMemoryObject(index, it_mem->second);
    }
}

void AddCommandQueue(uint command_queue, uint order_index, uint program)
{
    auto it_command = CommandQueueArray.find(command_queue);
    auto it_program = ProgramArray.find(program);
    
    if ( it_command != CommandQueueArray.end() && it_program != ProgramArray.end() ) {
        it_command->second->program(std::move(order_index), it_program->second);
    }
}

void WriteMemory(uint mem, ulong array_size, ulong object_size, void* array)
{
    auto it = MemoryArray.find(mem);
    
    if ( it != MemoryArray.end() ) {
        it->second->copy_in(array_size, object_size, array);
    }
}

void ReadMemory(uint mem, ulong array_size, ulong object_size, void* array)
{
    auto it = MemoryArray.find(mem);
    
    if ( it != MemoryArray.end() ) {
        it->second->copy_out(array_size, object_size, array);
    }
}

void Execute(uint command_queue, int dim_count, ulong* global_size, ulong* local_size)
{
    auto it = CommandQueueArray.find(command_queue);
    
    if( it != CommandQueueArray.end() ) {
        it->second->execute(dim_count, global_size, local_size);
    }
}
