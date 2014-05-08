

#include "ParallelCompute-Prefix.pch"
#include <OpenCL/OpenCL.h>

#include <vector>
#include <utility>
#include <map>

#include "util.h"

#define MAX_SOURCE_SIZE (0x100000)

cl_platform_id  platform_id;
cl_device_id    device_id;

cl_uint         ret_num_platforms;
cl_int          ret;

std::map<unsigned int, cl_context>       ContextArray;
std::map<unsigned int, cl_command_queue> CommendArray;
std::map<unsigned int, cl_mem>           MemoryArray;
std::map<unsigned int, cl_program>       ProgramArray;
std::map<unsigned int, cl_kernel>        KernelArray;

bool Init(void)
{
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_platforms);
    
    return true;
}

void Release(void)
{

}

unsigned int CreateContext(void)
{
    cl_context   context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    unsigned int key     = GenerateKey();
    
    ContextArray.insert(std::make_pair(key, context));
    
    return key;
}

unsigned int CreateCommendQueue(unsigned int context_key)
{
    auto it = ContextArray.find(context_key);
    
    cl_command_queue commend = clCreateCommandQueue(it->second, device_id, 0, &ret);
    unsigned int     key     = GenerateKey();
    
    CommendArray.insert(std::make_pair(key, commend));
    
    return key;
}

unsigned int CreateBufferObject(unsigned int context_key, int rw_mode, int mem_count, int type_size)
{
    auto it = ContextArray.find(context_key);
    
    cl_mem       mobj = clCreateBuffer(it->second, CL_MEM_READ_WRITE, type_size*mem_count, NULL, &ret);
    unsigned int key  = GenerateKey();
    
    MemoryArray.insert(std::make_pair(key, mobj));
    
    return key;
}

unsigned int CreateProgramWithSource(unsigned int context_key, char* source_path)
{
    auto it = ContextArray.find(context_key);
    
    FILE* pf = fopen(source_path, "r");
    if ( pf != NULL ) {
        char*  source      = (char*)malloc(MAX_SOURCE_SIZE);
        size_t source_size = fread(source, 1, MAX_SOURCE_SIZE, pf);

        unsigned int key     = GenerateKey();
        cl_program   program = clCreateProgramWithSource(it->second,
                                                       1,
                                                       (const char**)&source,
                                                       (const size_t*)&source_size,
                                                       &ret);
        
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        
        ProgramArray.insert(std::make_pair(key, program));
        
        free(source);
        fclose(pf);
        
        return key;
    }
    
    return 0;
}

unsigned int CreateKernel(unsigned int program_key, char* entry_function)
{
    auto it = ProgramArray.find(program_key);
    
    cl_kernel    kernel = clCreateKernel(it->second, (const char*)entry_function, &ret);
    unsigned int key    = GenerateKey();

    KernelArray.insert(std::make_pair(key, kernel));
    
    return key;
}

void SetKernelArg(unsigned int kernel_key, int arg_index, int mem_size, void* data)
{
    auto it = KernelArray.find(kernel_key);
    
    ret = clSetKernelArg(it->second, arg_index, mem_size, data);
}

void SetKernelArgFormObject(unsigned int kernel_key, int arg_index, unsigned int memory_key)
{
    auto it = MemoryArray.find(memory_key);
    
    SetKernelArg(kernel_key, arg_index, sizeof(cl_mem), (void*)&it->second);
}

void EnqueueWriteBuffer(unsigned int commend_key, unsigned int memory_key, int data_size, void* data)
{
    auto commend = CommendArray.find(commend_key);
    auto memory  = MemoryArray.find(memory_key);
    
    ret = clEnqueueWriteBuffer(commend->second, memory->second, CL_TRUE, 0, data_size, data, 0, NULL, NULL);
}

void EnqueueReadBuffer(unsigned int commend_key, unsigned int memory_key, int data_size, void* data)
{
    auto commend = CommendArray.find(commend_key);
    auto memory  = MemoryArray.find(memory_key);
    
    ret = clEnqueueReadBuffer(commend->second, memory->second, CL_TRUE, 0, data_size, data, 0, NULL, NULL);
}

void EnqueueTask(unsigned int commend_key, unsigned int kernel_key)
{
    auto commend = CommendArray.find(commend_key);
    auto kernel  = KernelArray.find(kernel_key);
    
    ret = clEnqueueTask(commend->second, kernel->second, 0, NULL, NULL);
}

void EnqueueNDRangeKernel(unsigned int commend_key, unsigned int kernel_key, int globle_size, int local_size)
{
    auto commend = CommendArray.find(commend_key);
    auto kernel  = KernelArray.find(kernel_key);
    
    ret = clEnqueueNDRangeKernel(commend->second,
                                 kernel->second,
                                 1,
                                 NULL,
                                 (size_t*)&globle_size,
                                 (size_t*)&local_size,
                                 0,
                                 NULL,
                                 NULL);
}

void Flush(unsigned int commend_key)
{
    auto it = CommendArray.find(commend_key);
    
    clFlush(it->second);
}

void Finish(unsigned int commend_key)
{
    auto it = CommendArray.find(commend_key);
    
    clFinish(it->second);
}

void ReleaseKernel(unsigned int kernel_key)
{
    auto it = KernelArray.find(kernel_key);
    
    clReleaseKernel(it->second);
    KernelArray.erase(it);
}

void ReleaseProgram(unsigned int program_key)
{
    auto it = ProgramArray.find(program_key);
    
    clReleaseProgram(it->second);
    ProgramArray.erase(it);
}

void ReleaseMemObject(unsigned int memory_key)
{
    auto it = MemoryArray.find(memory_key);
    
    clReleaseMemObject(it->second);
    MemoryArray.erase(it);
}

void ReleaseCommandQueue(unsigned int commend_key)
{
    auto it = CommendArray.find(commend_key);
    
    clReleaseCommandQueue(it->second);
    CommendArray.erase(it);
}

void ReleaseContext(unsigned int context_key)
{
    auto it = ContextArray.find(context_key);
    
    clReleaseContext(it->second);
    ContextArray.erase(it);
}
