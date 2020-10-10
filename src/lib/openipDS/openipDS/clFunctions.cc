#include <openipDS/clFunctions.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_OPENCL

namespace openip
{
    char* print_cl_errstring(cl_int err) {
    switch (err) {
        case CL_SUCCESS:                          return strdup("Success!");
        case CL_DEVICE_NOT_FOUND:                 return strdup("Device not found.");
        case CL_DEVICE_NOT_AVAILABLE:             return strdup("Device not available");
        case CL_COMPILER_NOT_AVAILABLE:           return strdup("Compiler not available");
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return strdup("Memory object allocation failure");
        case CL_OUT_OF_RESOURCES:                 return strdup("Out of resources");
        case CL_OUT_OF_HOST_MEMORY:               return strdup("Out of host memory");
        case CL_PROFILING_INFO_NOT_AVAILABLE:     return strdup("Profiling information not available");
        case CL_MEM_COPY_OVERLAP:                 return strdup("Memory copy overlap");
        case CL_IMAGE_FORMAT_MISMATCH:            return strdup("Image format mismatch");
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return strdup("Image format not supported");
        case CL_BUILD_PROGRAM_FAILURE:            return strdup("Program build failure");
        case CL_MAP_FAILURE:                      return strdup("Map failure");
        case CL_INVALID_VALUE:                    return strdup("Invalid value");
        case CL_INVALID_DEVICE_TYPE:              return strdup("Invalid device type");
        case CL_INVALID_PLATFORM:                 return strdup("Invalid platform");
        case CL_INVALID_DEVICE:                   return strdup("Invalid device");
        case CL_INVALID_CONTEXT:                  return strdup("Invalid context");
        case CL_INVALID_QUEUE_PROPERTIES:         return strdup("Invalid queue properties");
        case CL_INVALID_COMMAND_QUEUE:            return strdup("Invalid command queue");
        case CL_INVALID_HOST_PTR:                 return strdup("Invalid host pointer");
        case CL_INVALID_MEM_OBJECT:               return strdup("Invalid memory object");
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return strdup("Invalid image format descriptor");
        case CL_INVALID_IMAGE_SIZE:               return strdup("Invalid image size");
        case CL_INVALID_SAMPLER:                  return strdup("Invalid sampler");
        case CL_INVALID_BINARY:                   return strdup("Invalid binary");
        case CL_INVALID_BUILD_OPTIONS:            return strdup("Invalid build options");
        case CL_INVALID_PROGRAM:                  return strdup("Invalid program");
        case CL_INVALID_PROGRAM_EXECUTABLE:       return strdup("Invalid program executable");
        case CL_INVALID_KERNEL_NAME:              return strdup("Invalid kernel name");
        case CL_INVALID_KERNEL_DEFINITION:        return strdup("Invalid kernel definition");
        case CL_INVALID_KERNEL:                   return strdup("Invalid kernel");
        case CL_INVALID_ARG_INDEX:                return strdup("Invalid argument index");
        case CL_INVALID_ARG_VALUE:                return strdup("Invalid argument value");
        case CL_INVALID_ARG_SIZE:                 return strdup("Invalid argument size");
        case CL_INVALID_KERNEL_ARGS:              return strdup("Invalid kernel arguments");
        case CL_INVALID_WORK_DIMENSION:           return strdup("Invalid work dimension");
        case CL_INVALID_WORK_GROUP_SIZE:          return strdup("Invalid work group size");
        case CL_INVALID_WORK_ITEM_SIZE:           return strdup("Invalid work item size");
        case CL_INVALID_GLOBAL_OFFSET:            return strdup("Invalid global offset");
        case CL_INVALID_EVENT_WAIT_LIST:          return strdup("Invalid event wait list");
        case CL_INVALID_EVENT:                    return strdup("Invalid event");
        case CL_INVALID_OPERATION:                return strdup("Invalid operation");
        case CL_INVALID_GL_OBJECT:                return strdup("Invalid OpenGL object");
        case CL_INVALID_BUFFER_SIZE:              return strdup("Invalid buffer size");
        case CL_INVALID_MIP_LEVEL:                return strdup("Invalid mip-map level");
        default:                                  return strdup("Unknown");
        }
    }    
    
    int printCLInfo()
    {
        cl_int ciErrNum;
        char chBuffer[1024];
        cl_uint num_platforms;
        cl_platform_id* clPlatformIDs;
        cl_platform_id clSelectedPlatformID = NULL;
        cl_uint uiNumAllDevs;

        printf("setting up environment...\n"); fflush(stdout);
        ciErrNum = clGetPlatformIDs (0, NULL, &num_platforms);
        printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        clPlatformIDs= (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));

        printf("get platform\n"); fflush(stdout);
        ciErrNum= clGetPlatformIDs(num_platforms, clPlatformIDs, NULL);
        printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        for ( cl_uint i= 0; i < num_platforms; ++i )
        {
            clSelectedPlatformID= clPlatformIDs[i];

            clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_NAME, 1024, &chBuffer, NULL);
            printf("=============================================================\n");
            printf("CL_PLATFORM_NAME \t\t\t%s\n", chBuffer);
            
            clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_VERSION, 1024, &chBuffer, NULL);
            printf("CL_PLATFORM_VERSION \t\t\t%s\n", chBuffer);
            
            clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_VENDOR, 1024, &chBuffer, NULL);
            printf("CL_PLATFORM_VENDOR \t\t\t%s\n", chBuffer);
            
            ciErrNum= clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, 0, NULL, &uiNumAllDevs);
            if ( ciErrNum != CL_SUCCESS ) return 1;

            cl_device_id cdDevices[uiNumAllDevs];

            ciErrNum= clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, uiNumAllDevs, cdDevices, NULL);

            size_t size;
            for ( unsigned int j= 0; j < uiNumAllDevs; ++j )
            {
                cl_device_id tmp= cdDevices[j];
                
                printf("-------------------------------------------------------------\n");
                cl_device_type cdt;
                clGetDeviceInfo(tmp, CL_DEVICE_TYPE, sizeof(cl_device_type), &cdt, &size);
                if ( cdt == CL_DEVICE_TYPE_CPU )
                    printf("CL_DEVICE_TYPE \t\t\t\tCL_DEVICE_TYPE_CPU\n");
                if ( cdt == CL_DEVICE_TYPE_GPU )
                    printf("CL_DEVICE_TYPE \t\t\t\tCL_DEVICE_TYPE_GPU\n");
                if ( cdt == CL_DEVICE_TYPE_ACCELERATOR )
                    printf("CL_DEVICE_TYPE \t\t\t\tCL_DEVICE_TYPE_ACCELERATOR\n");
                
                clGetDeviceInfo(tmp, CL_DEVICE_NAME, 1024, &chBuffer, NULL);
                printf("CL_DEVICE_NAME \t\t\t\t%s\n", chBuffer);
                
                clGetDeviceInfo(tmp, CL_DEVICE_VENDOR, 1024, &chBuffer, NULL);
                printf("CL_DEVICE_VENDOR \t\t\t%s\n", chBuffer);
                
                cl_uint vid;
                clGetDeviceInfo(tmp, CL_DEVICE_VENDOR_ID, sizeof(cl_uint), &vid, &size);
                printf("CL_DEVICE_VENDOR_ID \t\t\t%d\n", vid);
                
                clGetDeviceInfo(tmp, CL_DEVICE_VERSION, 1024, &chBuffer, NULL);
                printf("CL_DEVICE_VERSION \t\t\t%s\n", chBuffer);
                
                clGetDeviceInfo(tmp, CL_DRIVER_VERSION, 1024, &chBuffer, NULL);
                printf("CL_DRIVER_VERSION \t\t\t%s\n", chBuffer);
                
                cl_ulong glob_mem_cache;
                clGetDeviceInfo(tmp, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &glob_mem_cache, &size);
                printf("CL_DEVICE_GLOBAL_MEM_CACHE_SIZE \t%ld\n", glob_mem_cache);

                cl_ulong glob_mem_cacheline;
                clGetDeviceInfo(tmp, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_ulong), &glob_mem_cacheline, &size);
                printf("CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE \t%ld\n", glob_mem_cacheline);

                cl_ulong glob_mem;
                clGetDeviceInfo(tmp, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &glob_mem, &size);
                printf("CL_DEVICE_GLOBAL_MEM_SIZE \t\t%ld\n", glob_mem);

                cl_ulong local_mem;
                clGetDeviceInfo(tmp, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &local_mem, &size);
                printf("CL_DEVICE_LOCAL_MEM_SIZE \t\t%ld\n", local_mem);

                cl_device_local_mem_type lmt;
                clGetDeviceInfo(tmp, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(cl_device_local_mem_type), &lmt, &size);
                if ( lmt == CL_LOCAL )
                    printf("CL_DEVICE_LOCAL_MEM_TYPE \t\tCL_LOCAL\n");
                else
                    printf("CL_DEVICE_LOCAL_MEM_TYPE \t\tCL_GLOBAL\n");

                cl_uint max_clock_frequency;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &max_clock_frequency, &size);
                printf("CL_DEVICE_MAX_CLOCK_FREQUENCY \t\t%d\n", max_clock_frequency);

                cl_uint max_compute_units;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &max_compute_units, &size);
                printf("CL_DEVICE_MAX_COMPUTE_UNITS \t\t%d\n", max_compute_units);

                cl_uint max_constant_args;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint), &max_constant_args, &size);
                printf("CL_DEVICE_MAX_CONSTANT_ARGS \t\t%d\n", max_constant_args);

                cl_ulong max_constant_buffer_size;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &max_constant_buffer_size, &size);
                printf("CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE \t%ld\n", max_constant_buffer_size);

                cl_ulong max_mem_alloc_size;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &max_mem_alloc_size, &size);
                printf("CL_DEVICE_MAX_MEM_ALLOC_SIZE \t\t%ld\n", max_mem_alloc_size);

                size_t max_parameter_size;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &max_parameter_size, &size);
                printf("CL_DEVICE_MAX_PARAMETER_SIZE \t\t%d\n", int(max_parameter_size));

                size_t max_workgroup_size;
                clGetDeviceInfo(tmp, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_workgroup_size, &size);
                printf("CL_DEVICE_MAX_WORK_GROUP_SIZE \t\t%d\n", int(max_workgroup_size));
                
                size_t max_width;
                clGetDeviceInfo(tmp, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &max_width, &size);
                printf("CL_DEVICE_IMAGE2D_MAX_WIDTH \t\t%d\n", int(max_width));
                
                size_t max_height;
                clGetDeviceInfo(tmp, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &max_height, &size);
                printf("CL_DEVICE_IMAGE2D_MAX_HEIGHT \t\t%d\n", int(max_height));
            }
        
        }
        
        return 0;
    }
}

#endif