/* 
 * File:   clFunctions.h
 * Author: gykovacs
 *
 * Created on 2011. július 7., 19:04
 */

#ifndef CLFUNCTIONS_H
#define	CLFUNCTIONS_H

#ifdef USE_OPENCL

#include <CL/cl.h>

namespace openip
{
    char *print_cl_errstring(cl_int err);
    
    int printCLInfo();
}

#endif

#endif	/* CLFUNCTIONS_H */

