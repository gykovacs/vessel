/**
 * @file Filter2.h
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
 * The Filter2 class represents a filter applicable on matrices or images and has
 * several useful operators and member functions defined.
 */

#ifndef _FILTER2_H_
#define _FILTER2_H_

#include <vector>
#include <set>
#include <stdio.h>
#include <math.h>

#include <openipDS/Image.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/StructuringElement2.h>
#include <openipDS/PositionWeightPair.h>
#include <openipDS/drawing.h>
#include <openipDS/Set.h>
#include <openipDS/Pixel.h>
#include <openipDS/Filter1.h>
#include <openipDS/Feature2.h>
#include <openipDS/Template2.h>

#ifdef USE_OPENCL
#include <CL/cl.h>
#endif

namespace openip
{
    /**
     * Filter represents a filter as a set of PositionWeightPairs
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class Filter2 : public Template2<WEIGHTS>, public Feature2<INPUT, OUTPUT>
    {
    public:
        //using Feature2<INPUT, OUTPUT>::computeMinMax;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Template2<WEIGHTS>::size;
        using Template2<WEIGHTS>::getWeights;
        using Template2<WEIGHTS>::setWeights;
	using Template2<WEIGHTS>::updateStride;
	using Template2<WEIGHTS>::computeMinMax;
	using Template2<WEIGHTS>::getMin;
	using Template2<WEIGHTS>::getMax;
	using Template2<WEIGHTS>::getRows;
	using Template2<WEIGHTS>::getColumns;
	using Template2<WEIGHTS>::getProposedBorder;
        using Template2<WEIGHTS>::getProposedBorder2;
	using Template2<WEIGHTS>::getCoordinate2D;
	using Template2<WEIGHTS>::stride;
	using Template2<WEIGHTS>::add;
	using Template2<WEIGHTS>::normalizer;
	using Template2<WEIGHTS>::normalize;
#ifdef USE_OPENCL
        using Feature2<INPUT, OUTPUT>::getOpenCLKernel;
        using Feature2<INPUT, OUTPUT>::enqueueKernel;
        using Feature2<INPUT, OUTPUT>::applyGPU;
#endif

        /**
         * Filter::iterator typedef
         */
        typedef typename Template2<WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Template2<WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        Filter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        Filter2(const Filter2& f);

        /**
         * destructor
         */
        virtual ~Filter2();
        
        /**
         * adds a new element
         * @param row coordinate of the new element, relative to 0,0
         * @param column coordinate of the new element, relative to 0,0
         * @param weight weight of the element
         */
        void addElement(int row, int column, WEIGHTS weight);

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);

        /**
         * applies the filter
         * @param in input image
         * @param out output image
         * @param mask the filter is applied in the foreground(non 0) region
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
//        virtual int applyGPU(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpFilter(Image<WEIGHTS>& im);

#ifdef USE_OPENCL
	/**
	* returns the equivalent OpenCL kernel code for filtering, as string
	* @return OpenCL kernel of the filter
	*/
        virtual std::string getOpenCLKernel();
#endif
        

        /**
         * rotates the filter with 90 degrees
         * @return this
         */
        virtual Filter2<INPUT, OUTPUT, WEIGHTS>& rotate90();

        /**
         * rotates the filter with 180 degrees
         * @return this
         */
        virtual Filter2<INPUT, OUTPUT, WEIGHTS>& rotate180();

        /**                                  
         * rotates the filter with 270 degrees
         * @return this
         */
        virtual Filter2<INPUT, OUTPUT, WEIGHTS>& rotate270();
        
#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);        
#endif
    };


    /**
    * CorrelationFilter2 represents a filter where the filter response is the correlation with the kernel
    */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class CorrelationFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMin;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMax;

        /**
         * Filter::iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        CorrelationFilter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        CorrelationFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * destructor
         */
        ~CorrelationFilter2();

        /**
	* returns the equivalent OpenCL kernel code for filtering, as string
	* @return OpenCL kernel of the filter
	*/
#ifdef USE_OPENCL        
        virtual std::string getOpenCLKernel();
#endif

#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);        
#endif        
        
        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);

	/**
	* applies the correlation filter to image in into image out in the foreground (non 0) region of mask
	* @param in input image
	* @param out output image
	* @param mask the operation is performed in the foreground of the mask
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class CCorrelationFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMin;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMax;

        /**
         * Filter::iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        CCorrelationFilter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        CCorrelationFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * destructor
         */
        ~CCorrelationFilter2();

        /**
    * returns the equivalent OpenCL kernel code for filtering, as string
    * @return OpenCL kernel of the filter
    */
/*#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
#endif

#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif*/

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);


    /**
    * applies the correlation filter to image in into image out in the foreground (non 0) region of mask
    * @param in input image
    * @param out output image
    * @param mask the operation is performed in the foreground of the mask
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
    */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float meanF;
        float devF;
        int initialized;
    };
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class EuclideanFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMin;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMax;

        /**
         * Filter::iterator typedef
         */
        typedef typename EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        EuclideanFilter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        EuclideanFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * destructor
         */
        ~EuclideanFilter2();

        /**
    * returns the equivalent OpenCL kernel code for filtering, as string
    * @return OpenCL kernel of the filter
    */
/*#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
#endif

#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif*/

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);


    /**
    * applies the correlation filter to image in into image out in the foreground (non 0) region of mask
    * @param in input image
    * @param out output image
    * @param mask the operation is performed in the foreground of the mask
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
    */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float meanF;
        float devF;
        int initialized;
    };
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class PWCMTMPWFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMin;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMax;

        /**
         * Filter::iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        PWCMTMPWFilter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        PWCMTMPWFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * destructor
         */
        ~PWCMTMPWFilter2();

        /**
    * returns the equivalent OpenCL kernel code for filtering, as string
    * @return OpenCL kernel of the filter
    */
/*#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
#endif

#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif*/

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);


    /**
    * applies the correlation filter to image in into image out in the foreground (non 0) region of mask
    * @param in input image
    * @param out output image
    * @param mask the operation is performed in the foreground of the mask
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
    */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float meanF;
        float devF;
        int initialized;
	
    };
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class PWCMTMCCorrFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMin;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::getMax;

        /**
         * Filter::iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Filter2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        PWCMTMCCorrFilter2(int stride= 4000);

        /**
         * copy constructor
         * @param f instance to copy
         */
        PWCMTMCCorrFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * destructor
         */
        ~PWCMTMCCorrFilter2();
	
	virtual int updateStride(int stride);

        /**
    * returns the equivalent OpenCL kernel code for filtering, as string
    * @return OpenCL kernel of the filter
    */
/*#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
#endif

#ifdef USE_OPENCL
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif*/

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
         * @returns filter response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support= NULL);


    /**
    * applies the correlation filter to image in into image out in the foreground (non 0) region of mask
    * @param in input image
    * @param out output image
    * @param mask the operation is performed in the foreground of the mask
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
    */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float meanF;
        float devF;
        int initialized;
	
	Vector<Vector<int> > levels;
	Vector<float> levelValues;
	Vector<float> means;
	Vector<int> nums;
	Vector<int> indicator;
	Vector<float> pattern;
	Vector<float> pmeans;
	Vector<float> originalPattern;
    };
}
namespace openip
{
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>::Filter2(int stride)
    : Template2<WEIGHTS>(), Feature2<INPUT, OUTPUT>()
    {
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>::Filter2(const Filter2& f)
    : Template2<WEIGHTS>(f), Feature2<INPUT, OUTPUT>(f)
    {
        this->stride= f.stride;
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>::~Filter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT Filter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        float f= 0;
        /*float sum= 0.0f;
        float sum2= 0.0f;
        int m= 0;*/

        /*if ( support == NULL )
        {
            for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                sum+= in(n + fit->first);
            }
            //sum/= this->size();
        }
        else
        {
            for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(fit->first + n) > 0 )
                {
                    sum+= in(n + fit->first);
                    ++m;
                }
            //sum/= m;
        }*/

        if ( support == NULL )
            for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                f+= (in(fit->first + n)) * (fit->second);
        else
        {
            for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(fit->first + n) > 0 )
                {
                    f+= (in(fit->first + n)) * (fit->second);
                    //sum2+= (fit->second)*(fit->second);
                }
            /*if ( fabs(sum2) > 0 )
                f= f/sqrt(sum2);
            else
                return 0;*/
        }

        return f;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(in.columns) )
            computeMinMax();

        if ( roi == NULL )
        {
            out= 0;
            int begin= -this->getMin();
            int end= in.size() - this->getMax();
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                out(i)= apply(in, i, roi);
        }
        else
        {
            int begin= -this->getMin();
            int end= in.size() - this->getMax();
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter2<INPUT, OUTPUT, WEIGHTS>::addElement(int row, int columns, WEIGHTS weight)
    {
        //this->push_back(PositionWeightPair<WEIGHTS>(row * stride + columns, weight));
      add(row*stride + columns, weight);
    }

#ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::string Filter2<INPUT, OUTPUT, WEIGHTS>::getOpenCLKernel()
    {
        printf("filter2 get opencl kernel\n"); fflush(stdout);
        std::string kernel;
        std::stringstream ss;
        ss.clear();

        ss << this->getMin();

        kernel= string("__kernel void convolution(__read_only image2d_t input,") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output, __read_only image2d_t roi)\n")
                + string("{\n"
                //"\tconst int idx= get_global_id(0) * 211 % get_global_size(0) - (") + ss.str() + string(");\n"
                "\tconst int idx= get_global_id(0) - (") + ss.str() + string(");\n"
                "\tint i; float") + string(" tmp= 0;\n");

        ss.str("");
        
        ss << "const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;\n";
        ss << "int r, c;\n";
        kernel+= ss.str();
        ss.str("");
        
        int rows, columns;
        
        printf("aaa\n"); fflush(stdout);
        ss << "r= idx / " << this->stride << ";\n";
        ss << "c= idx % " << this->stride << ";\n";
        ss << "if ( read_imagei(roi, smp, (int2)(c, r)).x > 0 )\n{\n";
        
        int x;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            x= (*this)(i).first;
            this->getCoordinate2D(x, rows, columns);
            ss << "if ( read_imagei(roi, smp, (int2)(c + (" << columns << "), r + (" <<rows << "))).x )\n";
            ss << "{\n";
            ss << "\ttmp+= read_imagef(input, smp, (int2)(c + (" << columns << "), r + (" << rows << "))).x*" << (*this)(i).second << ";\n";
            ss << "}\n";
        }
        printf("ccc\n"); fflush(stdout);
        ss << "}\n";
        
        kernel+= ss.str();
        
        ss.str("");
//        ss << string("\ttmp/= ");
//        ss << this->size();
        //ss << string(";\n");
        
        kernel+= ss.str();

        //kernel+= string("\toutput[idx]= read_imagef(input, smp, (int2)(c, r)).x;\n");
        kernel+= string("\toutput[idx]= tmp;\n");
        kernel+= string("}\n");

        printf("end get opencl kernel\n"); fflush(stdout);
        cout << kernel;
        return kernel;
    }
#endif
    
#ifdef USE_OPENCL   
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter2<INPUT, OUTPUT, WEIGHTS>::enqueueKernel(cl_context*, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input)
    {
        printf("filter22 enqueuekernel\n"); fflush(stdout);
        printCLInfo();
        cl_int ciErrNum;
        
        /*cl_mem cmPinnedPositions;              // OpenCL host memory output buffer object:  pinned

        cmPinnedPositions = clCreateBuffer(*cc, CL_MEM_READ_ONLY, this->size() * sizeof(int), NULL, &ciErrNum);
        
        int* tmp= &((*this)(0));
        for ( int i= 0; i < this->size(); ++i )
            printf("%d ", tmp[i]);

        clEnqueueWriteBuffer(*cq, cmPinnedPositions, CL_TRUE, 0, this->size() * sizeof(int), &((*this)(0)), 0, NULL, NULL);*/
        
        ciErrNum= clSetKernelArg(*k, 0, sizeof(cl_mem), bufin);
        ciErrNum= clSetKernelArg(*k, 1, sizeof(cl_mem), bufout);
        ciErrNum= clSetKernelArg(*k, 2, sizeof(cl_mem), bufroi);
        //ciErrNum= clSetKernelArg(*k, 3, sizeof(cl_mem), (void*)&cmPinnedPositions);

        size_t szLocalWorkSize;
        size_t szGlobalWorkSize;

        szLocalWorkSize= 1;
        szGlobalWorkSize= input.n + this->getMin() + this->getMin();
        printf("%d %d\n", int(input.n), int(szGlobalWorkSize)); fflush(stdout);
        printf("global work size %d %d\n", int(szGlobalWorkSize), input.n); fflush(stdout);

        printf("enqueue kernel\n"); fflush(stdout);
        ciErrNum= clEnqueueNDRangeKernel(*cq, *k, 1, NULL, &szGlobalWorkSize, NULL, 0, NULL, NULL);
        printf("ndrangekernel %d\n", ciErrNum);
    }
#endif
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>& Filter2<INPUT, OUTPUT, WEIGHTS>::rotate90()
    {
        int x, y;
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            x= fit->first/this->stride;
            if ( abs(fit->first%this->stride) > this->stride/2 )
                x= (fit->first > 0) ? x+1 : x-1;

            y= fit->first%this->stride;
            if ( abs(y) > this->stride/2 )
                y+= y > 0? -this->stride : this->stride;

            fit->first= -y*this->stride + x;
        }

        this->computeMinMax();
        return *this;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>& Filter2<INPUT, OUTPUT, WEIGHTS>::rotate180()
    {
        int x, y;
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            x= fit->first/this->stride;
            if ( abs(fit->first%this->stride) > this->stride/2 )
                x= (fit->first > 0) ? x+1 : x-1;

            y= fit->first%this->stride;
            if ( abs(y) > this->stride/2 )
                y+= y > 0? -this->stride : this->stride;

            fit->first= -x*this->stride - y;
        }

        this->computeMinMax();
        return *this;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter2<INPUT, OUTPUT, WEIGHTS>& Filter2<INPUT, OUTPUT, WEIGHTS>::rotate270()
    {
        int x, y;
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            x= fit->first/this->stride;
            if ( abs(fit->first%this->stride) > this->stride/2 )
                x= (fit->first > 0) ? x+1 : x-1;

            y= fit->first%this->stride;
            if ( abs(y) > this->stride/2 )
                y+= y > 0? -this->stride : this->stride;

            fit->first= y*this->stride - x;
        }

        this->computeMinMax();
        return *this;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter2<INPUT, OUTPUT, WEIGHTS>::dumpFilter(Image<WEIGHTS>& im)
    {
        printf("%d %d\n", this->getRows(), this->getColumns());
        im.resizeImage(this->getRows(), this->getColumns());
        im= -2;
        float max= -FLT_MAX;
        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( (*this)(i).second > max )
                max= (*this)(i).second;
        //im= max;

        int x, y;
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            x= fit->first/this->stride;
            if ( abs(fit->first%this->stride) > this->stride/2 )
                x= (fit->first > 0) ? x+1 : x-1;

            y= fit->first%this->stride;
            if ( abs(y) > this->stride/2 )
                y+= y > 0? -this->stride : this->stride;
            im.get(x,y)= fit->second;
        }
    }
    
    /**
     * prints the description of the parameter Filter2 object to the parameter output stream
     * @param o parameter output stream
     * @param f Filter2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << f.descriptor;
        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::CorrelationFilter2(int stride)
    : Filter2<INPUT, OUTPUT, WEIGHTS>()
    {
        this->stride= stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::CorrelationFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::~CorrelationFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        float f= 0;
        float devI= 0;
        float devF= 0;

        if ( support == NULL )
        {
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                devF+= (fit->second) * (fit->second);
                devI+= (in(n + fit->first)) * (in(n + fit->first));
            }

            devF= sqrt(devF);
            devI= sqrt(devI);

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                f+= (in(fit->first + n)) * (fit->second);
        }
        else
        {
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    devF+= (fit->second) * (fit->second);
                    devI+= (in(n + fit->first)) * (in(n + fit->first));
                }
            }

            devF= sqrt(devF);
            devI= sqrt(devI);

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(n + fit->first) > 0 )
                    f+= (in(fit->first + n)) * (fit->second);

            if ( fabs(devI) > FLT_EPSILON )
                return f/devI/devF;
        }

        if ( fabs(devI) > FLT_EPSILON )
            return f/devI/devF;
        return 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        if ( roi == NULL )
        {
            out= 0;
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i, support);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }
    
#ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::string CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::getOpenCLKernel()
    {
        printf("filter2 get opencl kernel\n"); fflush(stdout);
        std::string kernel;
        std::stringstream ss;
        ss.clear();

        ss << this->getMin();

        kernel= string("__kernel void convolution(__read_only image2d_t input,") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output, __read_only image2d_t roi)\n")
                + string("{\n"
                //"\tconst int idx= get_global_id(0) * 211 % get_global_size(0) - (") + ss.str() + string(");\n"
                "\tconst int idx= get_global_id(0) - (") + ss.str() + string(");\n"
                "\tint i; float") + string(" tmp= 0;\n");

        ss.str("");
        
        ss << "const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;\n";
        ss << "int r, c;\n";
        kernel+= ss.str();
        ss.str("");
        
        int rows, columns;
        
        printf("aaa\n"); fflush(stdout);
        ss << "r= idx / " << this->stride << ";\n";
        ss << "c= idx % " << this->stride << ";\n";
        ss << "if ( read_imagei(roi, smp, (int2)(c, r)).x > 0 )\n{\n";
        
        int x;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            x= (*this)(i).first;
            this->getCoordinate2D(x, rows, columns);
            ss << "if ( read_imagei(roi, smp, (int2)(c + (" << columns << "), r + (" <<rows << "))).x )\n";
            ss << "{\n";
            ss << "\ttmp+= read_imagef(input, smp, (int2)(c + (" << columns << "), r + (" << rows << "))).x*" << (*this)(i).second << ";\n";
            ss << "}\n";
        }
        printf("ccc\n"); fflush(stdout);
        ss << "}\n";
        
        kernel+= ss.str();
        
        ss.str("");
//        ss << string("\ttmp/= ");
//        ss << this->size();
        //ss << string(";\n");
        
        kernel+= ss.str();

        //kernel+= string("\toutput[idx]= read_imagef(input, smp, (int2)(c, r)).x;\n");
        kernel+= string("\toutput[idx]= tmp;\n");
        kernel+= string("}\n");

        printf("end get opencl kernel\n"); fflush(stdout);
        cout << kernel;
        return kernel;
    }
#endif
    
#ifdef USE_OPENCL   
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::enqueueKernel(cl_context*, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input)
    {
        printf("filter22 enqueuekernel\n"); fflush(stdout);
        printCLInfo();
        cl_int ciErrNum;
        
        /*cl_mem cmPinnedPositions;              // OpenCL host memory output buffer object:  pinned

        cmPinnedPositions = clCreateBuffer(*cc, CL_MEM_READ_ONLY, this->size() * sizeof(int), NULL, &ciErrNum);
        
        int* tmp= &((*this)(0));
        for ( int i= 0; i < this->size(); ++i )
            printf("%d ", tmp[i]);

        clEnqueueWriteBuffer(*cq, cmPinnedPositions, CL_TRUE, 0, this->size() * sizeof(int), &((*this)(0)), 0, NULL, NULL);*/
        
        ciErrNum= clSetKernelArg(*k, 0, sizeof(cl_mem), bufin);
        ciErrNum= clSetKernelArg(*k, 1, sizeof(cl_mem), bufout);
        ciErrNum= clSetKernelArg(*k, 2, sizeof(cl_mem), bufroi);
        //ciErrNum= clSetKernelArg(*k, 3, sizeof(cl_mem), (void*)&cmPinnedPositions);

        size_t szLocalWorkSize;
        size_t szGlobalWorkSize;

        szLocalWorkSize= 1;
        szGlobalWorkSize= input.n + this->getMin() + this->getMin();
        printf("%d %d\n", int(input.n), int(szGlobalWorkSize)); fflush(stdout);
        printf("global work size %d %d\n", int(szGlobalWorkSize), input.n); fflush(stdout);

        printf("enqueue kernel\n"); fflush(stdout);
        ciErrNum= clEnqueueNDRangeKernel(*cq, *k, 1, NULL, &szGlobalWorkSize, NULL, 0, NULL, NULL);
        printf("ndrangekernel %d\n", ciErrNum);
    }
#endif

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::CCorrelationFilter2(int stride)
    : Filter2<INPUT, OUTPUT, WEIGHTS>()
    {
        this->stride= stride;
        this->initialized= 0;

        devF= 0;
        meanF= 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::CCorrelationFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::~CCorrelationFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        float f= 0;
        float meanI= 0;
        float devI= 0;
        float meanI2= 0;

        if ( support == NULL )
        {
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                meanI+= in(n + fit->first);
                meanI2+= float(in(n + fit->first)) * in(n + fit->first);
            }

            meanI/= this->size();

            /*for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                devI+= (in(n + fit->first) - meanI) * (in(n + fit->first) - meanI);
            }*/
            meanI2/= this->size();

            //devI= sqrt(devI);

            devI= sqrt((meanI2 - meanI*meanI)*this->size());

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                f+= (in(fit->first + n) - meanI) * (fit->second - meanF);
        }
        else
        {
            int x= 0;
            float meanF= 0;
            float devF= 0;
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    meanF+= fit->second;
                    meanI+= in(n + fit->first);
                    x++;
                }
            }

            if ( x > 0 )
            {
                meanF/= x;
                meanI/= x;
            }
            else
            {
                meanF= 0;
                meanI= 0;
            }

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    devF+= (fit->second - meanF) * (fit->second - meanF);
                    devI+= (in(n + fit->first) - meanI) * (in(n + fit->first) - meanI);
                }
            }

            devF= sqrt(devF);
            devI= sqrt(devI);

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(n + fit->first) > 0 )
                    f+= (in(fit->first + n) - meanI) * (fit->second - meanF);

            if ( fabs(devI) > FLT_EPSILON )
	    {
	      float output= f/devI/devF;
	      /*if ( std::isnan(output) )
	      {
		printf("%f %f %f\n", f, devI, devF); fflush(stdout);
		getchar();
	      }*/
                return output;
	    }
	    else
	      return 0;
        }

        if ( fabs(devI) > FLT_EPSILON )
            return f/devI/devF;
        return 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        if ( roi == NULL )
        {
            out= 0;
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i, support);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }

    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMPWFilter2<INPUT, OUTPUT, WEIGHTS>::PWCMTMPWFilter2(int stride)
    : Filter2<INPUT, OUTPUT, WEIGHTS>()
    {
        this->stride= stride;
        this->initialized= 0;

        devF= 0;
        meanF= 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMPWFilter2<INPUT, OUTPUT, WEIGHTS>::PWCMTMPWFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMPWFilter2<INPUT, OUTPUT, WEIGHTS>::~PWCMTMPWFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT PWCMTMPWFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        float f= 0;
        float meanI= 0;
        float devI= 0;
        float meanI2= 0;

        if ( support == NULL )
        {
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                meanI+= in(n + fit->first);
                meanI2+= float(in(n + fit->first)) * in(n + fit->first);
            }

            meanI/= this->size();

            /*for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                devI+= (in(n + fit->first) - meanI) * (in(n + fit->first) - meanI);
            }*/
            meanI2/= this->size();

            //devI= sqrt(devI);

            devI= sqrt((meanI2 - meanI*meanI)*this->size());

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                f+= (in(fit->first + n) - meanI) * (fit->second - meanF);
        }
        else
        {
            int x= 0;
            float meanF= 0;
            float devF= 0;
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    meanF+= fit->second;
                    meanI+= in(n + fit->first);
                    x++;
                }
            }

            if ( x > 0 )
            {
                meanF/= x;
                meanI/= x;
            }
            else
            {
                meanF= 0;
                meanI= 0;
            }

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    devF+= (fit->second - meanF) * (fit->second - meanF);
                    devI+= (in(n + fit->first) - meanI) * (in(n + fit->first) - meanI);
                }
            }

            devF= sqrt(devF);
            devI= sqrt(devI);

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(n + fit->first) > 0 )
                    f+= (in(fit->first + n) - meanI) * (fit->second - meanF);

            if ( fabs(devI) > FLT_EPSILON )
	    {
	      float output= f/devI/devF;
	      /*if ( std::isnan(output) )
	      {
		printf("%f %f %f\n", f, devI, devF); fflush(stdout);
		getchar();
	      }*/
                return output;
	    }
	    else
	      return 0;
        }

        if ( fabs(devI) > FLT_EPSILON )
            return f/devI/devF;
        return 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void PWCMTMPWFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        if ( roi == NULL )
        {
            out= 0;
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i, support);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::PWCMTMCCorrFilter2(int stride)
    : Filter2<INPUT, OUTPUT, WEIGHTS>()
    {
        this->stride= stride;
        this->initialized= 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::PWCMTMCCorrFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::~PWCMTMCCorrFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
      this->Filter2<INPUT, OUTPUT, WEIGHTS>::updateStride(stride);
      initialized= 0;
      return 0;
    }
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* /*support*/)
    {
      int m= 2;
      
      #pragma omp critical
      {
      if ( !initialized )
      {
	levels.resize(m);
	Vector<float> borders;
	pmeans.resize(m);
	for ( unsigned int i= 0; i < this->size(); ++i )
	{
	  pattern.push_back((*this)(i).second);
	  originalPattern.push_back((*this)(i).second);
	}
	float min= pattern.getMin();
	float max= pattern.getMax();
	
	indicator.resize(this->size());
	
	float d= (max-min)/m;
	borders.push_back(min-1);
	for ( int i= 1; i < m; ++i )
	  borders.push_back(min + i*d);
	borders.push_back(max+1);
	for ( unsigned int i= 0; i < this->size(); ++i )
	  for ( unsigned int j= 0; j < borders.size()-1; ++j )
	    if ( (*this)(i).second >= borders(j) && (*this)(i).second < borders(j+1) )
	    {
	      levels(j).push_back((*this)(i).first);
	      pmeans(j)+= ((*this)(i).second);
	      indicator(i)= j;
	    }
	
	for ( unsigned int i= 0; i < pmeans.size(); ++i )
	  pmeans(i)/= levels(i).size();
	
	for ( unsigned int i= 0; i < levels.size(); ++i )
	{
	  for ( unsigned int j= 0; j < indicator.size(); ++j )
	    if ( indicator(j) == int(i) )
	    {
	      pattern(j)= pmeans(i);
	    }
	}
	
	initialized= 1;
      }
      }
      
      Vector<float> filtered;
      Vector<float> original;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	filtered.push_back(in(n + (*this)(i).first));
	original.push_back(in(n + (*this)(i).first));
      }      
      
      for ( unsigned int i= 0; i < levels.size(); ++i )
      {
	float mean= 0;
	for ( unsigned int j= 0; j < levels(i).size(); ++j )
	  mean+= in(n + levels(i)(j));
	mean/= levels(i).size();
	
	for ( unsigned int j= 0; j < indicator.size(); ++j )
	  if ( indicator(j) == int(i) )
	    filtered(j)= mean;
      }
      
      float res= original.dist2(filtered);
      res/= original.size()*original.getStandardDeviation();
      res*= -1;
      res+= 1;
      
      int violation= 1;
      for ( unsigned int i= 0; i < filtered.size()-1; ++i )
	if ( filtered(i) < filtered(i+1) && pattern(i) > pattern(i+1) )
	{
	  ++violation;
	}
	else if ( filtered(i) > filtered(i+1) && pattern(i) < pattern(i+1) )
	{
	  ++violation;
	}
	else if ( filtered(i) == filtered(i+1) && pattern(i) != pattern(i+1) )
	{
	  ++violation;
	}
      if ( violation > 1 )
	res= 0;
      
      if ( /*fabs(res) <= 1 &&*/ !isnan(res) )
	return res;
      else
	return 0;
      
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void PWCMTMCCorrFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        if ( roi == NULL )
        {
            out= 0;
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i, support);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::EuclideanFilter2(int stride)
    : Filter2<INPUT, OUTPUT, WEIGHTS>()
    {
        this->stride= stride;
        this->initialized= 0;

        devF= 0;
        meanF= 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::EuclideanFilter2(const Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::~EuclideanFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        float f= 0;
        float meanI= 0;
        float devI= 0;
        float meanI2= 0;

        if ( support == NULL )
        {
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                meanI+= in(n + fit->first);
                meanI2+= float(in(n + fit->first)) * in(n + fit->first);
            }

            meanI/= this->size();

            meanI2/= this->size();

            devI= sqrt((meanI2 - meanI*meanI)*this->size());

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                f+= (in(fit->first + n) - meanI) * (fit->second - meanF);
        }
        else
        {
            int x= 0;
            float meanF= 0;
            float devF= 0;
            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    meanF+= fit->second;
                    meanI+= in(n + fit->first);
                    x++;
                }
            }

            if ( x > 0 )
            {
                meanF/= x;
                meanI/= x;
            }
            else
            {
                meanF= 0;
                meanI= 0;
            }

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
            {
                if ( (*support)(n + fit->first) > 0 )
                {
                    devF+= (fit->second - meanF) * (fit->second - meanF);
                    devI+= (in(n + fit->first) - meanI) * (in(n + fit->first) - meanI);
                }
            }

            devF= sqrt(devF);
            devI= sqrt(devI);

            for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
                if ( (*support)(n + fit->first) > 0 )
                    f+= (in(fit->first + n) - meanI + meanF - fit->second) * (in(fit->first + n) - meanI + meanF - fit->second);

            if ( fabs(devI) > FLT_EPSILON )
	    {
	      float output= f/devI/devF;
                return output;
	    }
	    else
	      return 0;
        }

        if ( fabs(devI) > FLT_EPSILON )
            return f/devI/devF;
        return 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void EuclideanFilter2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        if ( roi == NULL )
        {
            out= 0;
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i, support);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( roi->operator()(i) )
                    out(i)= apply(in, i, support);
        }
    }
}

#endif
