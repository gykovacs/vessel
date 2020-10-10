/**
 * @file Transform2.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Transform2 is the base class of all image to image transforms.
 */

#ifndef TRANSFORM2_H
#define	TRANSFORM2_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/VectorTransform2.h>

#include <openipDS/clFunctions.h>

#include <string>
#include <iostream>

#include <CL/cl.h>

namespace openip
{
    /**
     * Transform2 is the base class of all image to image transforms.
     */
    template<typename INPUT, typename OUTPUT>
    class Transform2: public VectorTransform2<INPUT, OUTPUT>
    {
    public:
        using VectorTransform2<INPUT, OUTPUT>::descriptor;
        using VectorTransform2<INPUT, OUTPUT>::apply;
        using VectorTransform2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * default constructor
         */
        Transform2();

        /**
         * copy constructor
         * @param t instance to copy
         */
        Transform2(const Transform2& t);

        /**
         * destructor
         */
        ~Transform2();

        /**
         * applies the transform to the input image, in the foreground (non-zero) region of the mask, using the pixels covered
         * by the foreground (non-zero) region of roi
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         * @param roi the operation is performed using the pixels covered by the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

#ifdef USE_OPENCL        
        virtual std::string getOpenCLKernel();
        
        virtual int applyGPU(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif
        /**
         * using the proposed border, the operation can be performed in each pixel of the original image
         * @return the proposed border
         */
        


        /**
         * transform descriptor string
         */
        //std::string descriptor;
    };
    
#ifdef USE_OPENCL
    
#endif
    
    std::string type(const std::type_info& ti);    

    /**
     * prints the description of the parameter Transform2 object to the parameter output stream
     * @param out parameter output stream
     * @param t2 Transform2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& out, Transform2<INPUT, OUTPUT>& t2)
    {
        out << t2.descriptor;
        return out;
    }

    template<typename INPUT, typename OUTPUT>
    Transform2<INPUT, OUTPUT>::Transform2()
    : VectorTransform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2<INPUT, OUTPUT>::Transform2(const Transform2& t)
    : VectorTransform2<INPUT, OUTPUT>(t)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2<INPUT, OUTPUT>::~Transform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Transform2<INPUT, OUTPUT>::apply(Image<INPUT>&, Image<OUTPUT>&, Image<unsigned char>*, Image<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Transform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->apply(input, output(n), roi, support);
    }

    #ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT>
    std::string Transform2<INPUT, OUTPUT>::getOpenCLKernel()
    {
        return std::string("");
    }
    #endif

    void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data);
    
    #ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT>
    int Transform2<INPUT, OUTPUT>::applyGPU(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        printf("transform2 apply gpu\n"); fflush(stdout);
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
            clGetPlatformInfo(clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL);
            if ( strstr(chBuffer, "NVIDIA") != NULL )
            {
                clSelectedPlatformID= clPlatformIDs[i];
                break;
            }
        }

        printf("get device\n"); fflush(stdout);
        ciErrNum= clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, 0, NULL, &uiNumAllDevs);
        printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        cl_device_id cdDevices[uiNumAllDevs];

        ciErrNum= clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, uiNumAllDevs, cdDevices, NULL);
        printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        cl_context cxGPUContext= clCreateContext(0, 1, cdDevices, &pfn_notify, NULL, &ciErrNum);

        cl_program cpProgram;               // OpenCL program
        string program= this->getOpenCLKernel();
        //cout << program << endl;
        const char* programPointer= program.c_str();
        size_t program_size= program.size();

        
        std::ifstream infile;
        infile.open("kernel.bin", ios::binary | ios::out);
        if ( ! infile.is_open() )
        {
        
            cpProgram = clCreateProgramWithSource(cxGPUContext, 1, &(programPointer), &program_size, &ciErrNum);

            char flags[32];
            strcpy(flags,"-cl-fast-relaxed-math -w");

            printf("build program\n"); fflush(stdout);
            ciErrNum = clBuildProgram(cpProgram, 0, NULL, flags, NULL, NULL);
            printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
            if ( ciErrNum != CL_SUCCESS ) return 1;

            size_t size[1];
            clGetProgramInfo(cpProgram, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &size, NULL);
            printf("program binary size: %d\n", int(size[0]));

            char* buffer[1];
            buffer[0]= (char*)malloc(sizeof(char)*(size[0] + 1));
            clGetProgramInfo(cpProgram, CL_PROGRAM_BINARIES, size[0]+1, &buffer, NULL);
            std::ofstream file;
            file.open("kernel.bin", ios::binary | ios::out);
            file.write((char*)(buffer[0]), size[0]);
            file.close();
        }
        else
        {
            char* buffer[1];
            buffer[0]= (char*)malloc(sizeof(char)*(1000000));
            infile.read(buffer[0], 1000000);
            buffer[0][infile.gcount()]= '\0';
            size_t lengths[1];
            lengths[0]= infile.gcount();
            cl_int status;
            //printf("input program: %d\n%s\n", lengths[0], buffer[0]);
            cpProgram= clCreateProgramWithBinary(cxGPUContext, 1, cdDevices, lengths, reinterpret_cast<const unsigned char**>(&buffer), &status, &ciErrNum);
            printf("binary: %d %d\n", status, ciErrNum);
            
            char flags[32];
            strcpy(flags,"-cl-fast-relaxed-math -w");

            printf("build program\n"); fflush(stdout);
            ciErrNum = clBuildProgram(cpProgram, 0, NULL, flags, NULL, NULL);
            printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
            if ( ciErrNum != CL_SUCCESS ) return 1;
        }
        
        
        printf("create kernel\n"); fflush(stdout);
        cl_kernel k_conv= clCreateKernel(cpProgram, "convolution", &ciErrNum);
        printf("%s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        cl_command_queue cqCommandQueue;
        cl_mem cmPinnedBufIn;               // OpenCL host memory input buffer object:  pinned
        cl_mem cmPinnedBufOut;              // OpenCL host memory output buffer object:  pinned
        cl_mem cmPinnedBufRoi;

        cqCommandQueue= clCreateCommandQueue(cxGPUContext, cdDevices[0], 0, &ciErrNum) ;

        cl_image_format cif;
        cif.image_channel_order= CL_R;
        cif.image_channel_data_type= CL_FLOAT;
        printf("%d %d\n", input.columns, input.rows); fflush(stdout);
        cmPinnedBufIn = clCreateImage2D(cxGPUContext, CL_MEM_READ_ONLY, &cif, input.columns, input.rows, 0, NULL, &ciErrNum);
        printf("createImage1: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        cmPinnedBufOut = clCreateBuffer(cxGPUContext, CL_MEM_WRITE_ONLY, input.n * sizeof(OUTPUT), NULL, &ciErrNum);
        //cmPinnedBufRoi = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, input.n * sizeof(unsigned char), NULL, &ciErrNum);
        cl_image_format cif2;
        cif2.image_channel_order= CL_R;
        cif2.image_channel_data_type= CL_UNSIGNED_INT8;
        cmPinnedBufRoi = clCreateImage2D(cxGPUContext, CL_MEM_READ_ONLY, &cif2, input.columns, input.rows, 0, NULL, &ciErrNum);
        printf("createImage2: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);

        printf("setting up kernel...\n");

        size_t origin[3];
        size_t region[3];
        origin[0]= origin[1]= origin[2]= 0;
        region[0]= input.columns;
        region[1]= input.rows;
        region[2]= 1;
        
        size_t origin2[3];
        size_t region2[3];
        origin2[0]= origin2[1]= origin2[2]= 0;
        region2[0]= input.columns;
        region2[1]= input.rows;
        region2[2]= 1;
        
        //clEnqueueWriteBuffer(cqCommandQueue, cmPinnedBufIn, CL_FALSE, 0, input.n * sizeof(INPUT), &(input(0)), 0, NULL, NULL);
        ciErrNum= clEnqueueWriteImage(cqCommandQueue, cmPinnedBufIn, CL_FALSE, origin, region, sizeof(INPUT)*input.columns, 0, &(input(0)), 0, NULL, NULL);
        printf("readImage1: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        //clEnqueueWriteBuffer(cqCommandQueue, cmPinnedBufRoi, CL_FALSE, 0, input.n * sizeof(unsigned char), &((*roi)(0)), 0, NULL, NULL);
        ciErrNum= clEnqueueWriteImage(cqCommandQueue, cmPinnedBufRoi, CL_FALSE, origin2, region2, sizeof(unsigned char)*input.columns, 0, &((*roi)(0)), 0, NULL, NULL);
        printf("readImage2: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        clFinish(cqCommandQueue);

        enqueueKernel(&cxGPUContext, &cqCommandQueue, &k_conv, (void*)&cmPinnedBufIn, (void*)&cmPinnedBufOut, (void*)&cmPinnedBufRoi, input);
        
        ciErrNum= clFlush(cqCommandQueue);
        printf("flush: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        ciErrNum= clFinish(cqCommandQueue);
        printf("finish: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        ciErrNum= clEnqueueReadBuffer(cqCommandQueue, cmPinnedBufOut, CL_FALSE, 0, input.n * sizeof(OUTPUT), (void*)&(output(0)), 0, NULL, NULL);
        printf("read buffer: %s %d %d\n", print_cl_errstring(ciErrNum), input.n, int(sizeof(OUTPUT))); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;

        //ciErrNum= clFinish(cqCommandQueue);
        //ciErrNum= clFlush(cqCommandQueue);
        printf("flush: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        ciErrNum= clFinish(cqCommandQueue);
        printf("finish: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseMemObject(cmPinnedBufIn);
        printf("release in: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseMemObject(cmPinnedBufOut);
        printf("release out: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseProgram(cpProgram);
        printf("release program: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseKernel(k_conv);
        printf("release kernel: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseCommandQueue(cqCommandQueue);
        printf("release cq: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        ciErrNum= clReleaseContext(cxGPUContext);
        printf("release cx: %s\n", print_cl_errstring(ciErrNum)); fflush(stdout);
        if ( ciErrNum != CL_SUCCESS ) return 1;
        
        return 0;
    }
    #endif
    
    #ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT>
    void Transform2<INPUT, OUTPUT>::enqueueKernel(cl_context* , cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* , Image<INPUT>& input)
    {
        printf("transform2 enqueueKernel\n"); fflush(stdout);
        cl_int ciErrNum;
        ciErrNum= clSetKernelArg(*k, 0, sizeof(cl_mem), bufin);
        ciErrNum= clSetKernelArg(*k, 1, sizeof(cl_mem), bufout);

        size_t szLocalWorkSize;
        size_t szGlobalWorkSize;

        szLocalWorkSize= 1;
        szGlobalWorkSize= input.n;
        printf("global work size %d %d\n", int(szGlobalWorkSize), input.n); fflush(stdout);

        printf("enqueue kernel\n"); fflush(stdout);
        ciErrNum= clEnqueueNDRangeKernel(*cq, *k, 1, NULL, &szGlobalWorkSize, &szLocalWorkSize, 0, NULL, NULL);
        printf("ndrangekernel %d\n", ciErrNum);
    }
    #endif
}

#endif	/* TRANSFORM_H */

