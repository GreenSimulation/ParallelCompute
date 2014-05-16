//
//  device.h
//  TestOpenCL
//
//  Created by Macintosh on 14. 5. 15..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#ifndef __TestOpenCL__device__
#define __TestOpenCL__device__

#include <iostream>
#include <vector>

#include <OpenCL/OpenCL.h>

namespace ParallelCompute
{
    typedef cl_platform_id      PLATFORM_ID;
    typedef const PLATFORM_ID&  PLATFORM;
    
    typedef cl_device_id        DEVICE_ID;
    typedef const DEVICE_ID&    DEVICE;
    
    typedef unsigned int  uint;
    typedef unsigned long ulong;
    
    class Context;
    
    class Device
    {
    public:
        enum class COMPUTE_MODE : int
        {
            DEFAULT     = 1,
            CPU         = 2,
            GPU         = 3,
            ACCELERATOR = 4,
            CUSTOM      = 5,
            ALL         = 6
        };
        
    private:
        typedef std::vector<cl_device_id> DEVICE_ARRAY;

        
    private:
        PLATFORM_ID    _platform;
        DEVICE_ARRAY   _devices;
        
    public:
        Device(void);
        ~Device(void);
        
    public:
        PLATFORM    platform(void) const;
        DEVICE      device(const int& index) const;
        std::string device_name(const int& index) const;
        ulong       count(void) const;
        
    public:
        bool initialization(void);
        void release(void);
        
    public:
        Context* create_context(const COMPUTE_MODE&& mode);
        
    private:
        cl_device_id selectDeviceType(const COMPUTE_MODE&& mode, int* index);
    };
    
    bool SUCCESS(cl_int result, std::string message);
}

#endif /* defined(__TestOpenCL__device__) */
