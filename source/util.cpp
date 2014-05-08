//
//  util.cpp
//  ParallelCompute
//
//  Created by Macintosh on 14. 5. 7..
//  Copyright (c) 2014년 Macintosh. All rights reserved.
//

#include "ParallelCompute-Prefix.pch"
#include "util.h"

unsigned int GenerateKey(void)
{
    static unsigned int NextID = 0;
    
    NextID = NextID + 1;
    return NextID;
}