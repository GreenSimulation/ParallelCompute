//
//  device.cpp
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include "device.h"
#include "context.h"

using namespace ParallelCompute;

Device::Device(void) : _platform(0) {}
Device::~Device(void) {}

PLATFORM Device::platform(void) const
{
    return _platform;
}

DEVICE Device::device(const int& index) const
{
    return _devices[index];
}

std::string Device::device_name(const int& index) const
{
    char buffer[128];
    clGetDeviceInfo(_devices[index], CL_DEVICE_NAME, 128, buffer, NULL);
    
    return buffer;
}

ulong Device::count(void) const
{
    return _devices.size();
}

bool Device::initialization(void)
{
    cl_uint platform_count = 0;
    cl_uint device_count   = 0;

    // Init OpenCL Platform.
    if( !SUCCESS(clGetPlatformIDs(1, &_platform, &platform_count), "getting platform id") ) { return false; };
    if( !SUCCESS(clGetDeviceIDs(_platform, CL_DEVICE_TYPE_ALL, 0, NULL, &device_count), "getting device count")) { return false; }

    _devices.resize(device_count);
    
    if( !SUCCESS(clGetDeviceIDs(_platform, CL_DEVICE_TYPE_ALL, device_count, &_devices[0], NULL), "getting device ids") ) { return false; }
    
    return true;
}

void Device::release(void)
{
    DEVICE_ARRAY empty;
    
    for( int n = 0; n < _devices.size(); ++n ) {
        //clReleaseDevice(_devices[n]);
    }
    
    _devices.swap(empty);
}

Context* Device::create_context(const COMPUTE_MODE&& mode)
{
    int          device_index = 0;
    cl_device_id device_id    = this->selectDeviceType(std::move(mode), &device_index);
    
    if (device_id != 0) {
        return new Context(this, device_index);
    }
    
    return nullptr;
}

cl_device_id Device::selectDeviceType(const COMPUTE_MODE&& mode, int* index)
{
    cl_device_type device_type = 0;
    cl_device_type object_type = 0;
    
    switch (mode) {
        case COMPUTE_MODE::DEFAULT:     device_type = CL_DEVICE_TYPE_DEFAULT;      break;
        case COMPUTE_MODE::CPU:         device_type = CL_DEVICE_TYPE_CPU;          break;
        case COMPUTE_MODE::GPU:         device_type = CL_DEVICE_TYPE_GPU;          break;
        case COMPUTE_MODE::ACCELERATOR: device_type = CL_DEVICE_TYPE_ACCELERATOR;  break;
        case COMPUTE_MODE::CUSTOM:      device_type = CL_DEVICE_TYPE_CUSTOM;       break;
        case COMPUTE_MODE::ALL:         device_type = CL_DEVICE_TYPE_ALL;          break;
    };
    
    for( int n = 0; n < _devices.size(); ++n ) {
        if( !SUCCESS(clGetDeviceInfo(_devices[n], CL_DEVICE_TYPE, sizeof(cl_device_type), &object_type, NULL), "getting device info")) { break; }
        
        if( object_type == device_type ) {
            *index = n;
            return _devices[n];
        }
    }
    
    return 0;
}

bool ParallelCompute::SUCCESS(cl_int result, std::string message)
{
    if( result != CL_SUCCESS ) {
        std::cout << "Error " << message << ": " << result << std::endl;
        return false;
    }
    
    return true;
}