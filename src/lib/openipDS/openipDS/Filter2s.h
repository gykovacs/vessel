/**
 * @file Filter2s.h
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
 *log
 * @section DESCRIPTION
 *
 * This file contains some common filters.
 */

#ifndef _FILTERS_H
#define	_FILTERS_H

#include <openipDS/Filter1s.h>
#include <openipDS/Filter2.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/CoordinateTransform2.h>
#include <openipDS/FilterSet2.h>

#include <float.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <emmintrin.h>

namespace openip
{
    /**
     * Inversion filter
     */
    template<typename INPUT, typename OUTPUT>
    class InversionFilter2: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::updateStride;
        using Filter2<INPUT, OUTPUT, int>::apply;
        using Filter2<INPUT, OUTPUT, int>::descriptor;

        /**
         * constructor
         * @param stride stride of the images to apply on
         */
        InversionFilter2(int stride= 4000);

        /**
         * applies the filter to the image in, in position n
         * @param in
         * @param n
         * @return
         */
        virtual OUTPUT apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes the minimum and maximum positions in row-major representation, in which
         * the filter can be computed
         */
        void computeMinMax();
    };

    template<typename INPUT, typename OUTPUT>
    InversionFilter2<INPUT, OUTPUT>::InversionFilter2(int stride)
    : Filter2<INPUT, OUTPUT, int>()
    {
        this->stride= stride;
        this->min= 0;
        this->max= 0;
        std::stringstream ss;
        ss << "InversionFilter2 ";
        ss << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InversionFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>*)
    {
        return (OUTPUT)(255.0f - float(in(n)));
    }

    template<typename INPUT, typename OUTPUT>
    void InversionFilter2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= 0;
        this->max= 0;
    }


    /**
     * Identical filter
     */
    template<typename INPUT, typename OUTPUT>
    class IdenticalFilter2: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::updateStride;
        using Filter2<INPUT, OUTPUT, int>::apply;
        using Filter2<INPUT, OUTPUT, int>::descriptor;

        /**
         * constructor
         * @param stride stride of the images to apply on
         */
        IdenticalFilter2(int stride= 4000);

        /**
         * applies the filter to the image in, in position n
         * @param in
         * @param n
         * @return
         */
        virtual OUTPUT apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes the minimum and maximum positions in row-major representation, in which
         * the filter can be computed
         */
        void computeMinMax();
    };

    template<typename INPUT, typename OUTPUT>
    IdenticalFilter2<INPUT, OUTPUT>::IdenticalFilter2(int stride)
    : Filter2<INPUT, OUTPUT, int>()
    {
        this->stride= stride;
        this->min= 0;
        this->max= 0;
        std::stringstream ss;
        ss << "IdenticalFilter2 ";
        ss << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT IdenticalFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>*)
    {
        return in(n);
    }

    template<typename INPUT, typename OUTPUT>
    void IdenticalFilter2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= 0;
        this->max= 0;
    }

    /**
     * LaplacianFilter represents a 3x3 Laplacian filter
     */
    template<typename INPUT, typename OUTPUT>
    class LaplacianFilter: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::updateStride;
        using Filter2<INPUT, OUTPUT, int>::apply;

        /**
         * constructor to create LaplacianFilter instance
         * @param stride stride of the image
         */
        LaplacianFilter(int stride= 4000);

        /**
         * applies the filter in the position n
         * @param in input image
         * @param n position in row-continuous representation
         * @return the filter response in n
         */
        virtual OUTPUT apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * ASM optimized apply method for unsigned char input and unsigned char output images
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non 0) region of the mask
         */
        //void apply(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();
    };

    template<typename INPUT, typename OUTPUT>
    LaplacianFilter<INPUT, OUTPUT>::LaplacianFilter(int stride)
    : Filter2<INPUT, OUTPUT, int>(stride)
    {
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT LaplacianFilter<INPUT, OUTPUT>::apply(Image<INPUT>& in, Pixel1 n, Image<unsigned char>* support)
    {
        if ( support == NULL )
            return -4*in(n) + in(n-1) + in(n+1) + in(n-stride) + in(n+stride);
        else
        {
            OUTPUT sum= 0;
            sum+= -4*in(n);
            if ( (*support)(n-1) > 0 )
                sum+= in(n-1);
            if ( (*support)(n+1) > 0 )
                sum+= in(n+1);
            if ( (*support)(n-stride) > 0 )
                sum+= in(n-stride);
            if ( (*support)(n+stride) > 0 )
                sum+= in(n+stride);
            return sum;
        }
    }

    template<typename INPUT, typename OUTPUT>
    void LaplacianFilter<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -stride;
        this->max= stride;
    }

    template<typename INPUT, typename OUTPUT>
    void LaplacianFilter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        int begin= this->getMin();
        int end= this->getMax();

        this->stride= input.columns;

        if ( roi == NULL )
            for ( int i= -begin; i < int(input.n) - end; ++i )
                output(i)= apply(input, i, support);
        else
            for ( int i= -begin; i < int(input.n) - end; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= apply(input, i, support);
    }

    /*template<typename INPUT, typename OUTPUT>
    void LaplacianFilter<INPUT, OUTPUT>::apply(Image<unsigned char>& in, Image<unsigned char>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(in.columns) )
            computeMinMax();

        out= 0;
        int end= in.size() - this->max;

        #pragma omp parallel for
        for ( int n= -this->min; n < end; n += 16 )	// 9.2K
        {
                const __m128i	r0 = _mm_load_si128((const __m128i*)&in[n-stride]);
                const __m128i	r1 = _mm_loadu_si128((const __m128i*)&in[n-1]);
                const __m128i	r2 = _mm_load_si128((const __m128i*)&in[n]);
                const __m128i	r3 = _mm_loadu_si128((const __m128i*)&in[n+1]);
                const __m128i	r4 = _mm_load_si128((const __m128i*)&in[n+stride]);
                const __m128i	r5 = _mm_add_epi8(_mm_add_epi8(r0, r1), _mm_add_epi8(r3, r4));
                __m128i	r6 = _mm_sub_epi8(r5, r2);
                r6 = _mm_sub_epi8(r6, r2);
                r6 = _mm_sub_epi8(r6, r2);
                r6 = _mm_sub_epi8(r6, r2);
                _mm_store_si128((__m128i*)&out[n], r6);
                //_mm_stream_si128((__m128i*)&out[n], r6);
        }
    }*/

    /**
     * SobelFilterX represents the 3x3 X Sobel filter
     */
    template<typename INPUT, typename OUTPUT>
    class SobelFilterX: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::apply;
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::updateStride;

        /**
         * constructor to create SobelFilterX instance
         * @param stride stride of the image
         */
        SobelFilterX(int stride= 4000);

        /**
         * overriding apply method in the filter
         * @param input input image
         * @param n position in row-continouos representation
         * @return the result of application of the filter in the nth position
         */
        virtual OUTPUT apply(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();
        
        /**
         * update stride
         * @param stride new stride
         * @return returns 0 of no change was made, 1 if the stride changed
         */
        int updateStride(int stride);

        /**
         * temporary variable
         */
        int stridem1;
        /**
         * temporary variable
         */
        int stridep1;
    };

    template<typename INPUT, typename OUTPUT>
    SobelFilterX<INPUT, OUTPUT>::SobelFilterX(int stride)
    : Filter2<INPUT, OUTPUT, int>(stride)
    {
        computeMinMax();
        stridem1= stride - 1;
        stridep1= stride + 1;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SobelFilterX<INPUT, OUTPUT>::apply(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support)
    {
        if ( support == NULL )
            return -input(n-stridep1)-2*input(n-stride)-input(n-stridem1) + input(n+stridem1) + 2*input(n+stride) + input(n+stridep1);
        else
        {
            OUTPUT sum= 0;
            if ( (*support)(n-stridep1) > 0 )
                sum-= input(n-stridep1);
            if ( (*support)(n-stride) > 0 )
                sum-= 2 * input(n-stride);
            if ( (*support)(n-stridem1) > 0 )
                sum-= input(n-stridem1);
            if ( (*support)(n+stridem1) > 0 )
                sum+= input(n+stridem1);
            if ( (*support)(n+stride) > 0 )
                sum+= 2 * input(n+stride);
            if ( (*support)(n+stridep1) > 0 )
                sum+= input(n+stridep1);
            return sum;
        }
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilterX<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -stride-1;
        this->max= stride+1;
    }

    template<typename INPUT, typename OUTPUT>
    int SobelFilterX<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( this->stride != stride )
        {
            this->stride= stride;
            this->stridem1= stride-1;
            this->stridep1= stride+1;
            return 1;
        }
        return 0;
    }

    /**
     * SobelFilterY represents the 3x3 X Sobel filter
     */
    template<typename INPUT, typename OUTPUT>
    class SobelFilterY: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::apply;
        using Filter2<INPUT, OUTPUT, int>::updateStride;

        /**
         * constructor to create SobelFilterY instance
         * @param stride stride of the image
         */
        SobelFilterY(int stride= 4000);

        /**
         * overriding apply method in the filter
         * @param input input image
         * @param n position in row-continouos representation
         * @return the result of application of the filter in the nth position
         */
        virtual OUTPUT apply(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();

        /**
         * update stride
         * @param stride new stride
         * @return returns 0 of no change was made, 1 if the stride changed
         */
        int updateStride(int stride);

        /**
         * temporary variable
         */
        int stridem1;
        /**
         * temporary variable
         */
        int stridep1;
    };

    template<typename INPUT, typename OUTPUT>
    SobelFilterY<INPUT, OUTPUT>::SobelFilterY(int stride)
    : Filter2<INPUT, OUTPUT, int>(stride)
    {
        computeMinMax();
        stridep1= stride + 1;
        stridem1= stride - 1;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SobelFilterY<INPUT, OUTPUT>::apply(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support)
    {
        if ( support == NULL )
            return -input(n-stridep1)-2*input(n-1)-input(n+stridem1)+input(n-stridem1)+2*input(n+1)+input(n+stridep1);
        else
        {
            OUTPUT sum= 0;
            if ( (*support)(n-stridep1) > 0 )
                sum-= input(n-stridep1);
            if ( (*support)(n-1) > 0 )
                sum-= 2*input(n-1);
            if ( (*support)(n+stridem1) > 0 )
                sum-= input(n+stridem1);
            if ( (*support)(n-stridem1) > 0 )
                sum+= input(n-stridem1);
            if ( (*support)(n+1) > 0 )
                sum+= 2*input(n+1);
            if ( (*support)(n+stridep1) > 0 )
                sum+= input(n+stridep1);
            return sum;
        }
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilterY<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -stride-1;
        this->max= stride+1;
    }

    template<typename INPUT, typename OUTPUT>
    int SobelFilterY<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( this->stride != stride )
        {
            this->stride= stride;
            this->stridem1= stride-1;
            this->stridep1= stride+1;
            return 1;
        }
        return 0;
    }

    /**
     * PrewittFilterX represents the 3x3 X Prewitt filter
     */
    template<typename INPUT, typename OUTPUT>
    class PrewittFilterX: public SobelFilterX<INPUT, OUTPUT>
    {
    public:
        using SobelFilterX<INPUT, OUTPUT>::stridem1;
        using SobelFilterX<INPUT, OUTPUT>::stridep1;
        using SobelFilterX<INPUT, OUTPUT>::apply;

	/**
	* Prewitt X filter constructor
	* @param stride stride of the image to work on
	*/
        PrewittFilterX(int stride= 4000);

	/**
	* application of the filter in a given point
	* @param input input image to apply on
	* @param n position to apply in
	* @return filter response
	*/
        OUTPUT apply(Image<INPUT>* input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    PrewittFilterX<INPUT, OUTPUT>::PrewittFilterX(int stride)
    : SobelFilterX<INPUT, OUTPUT>(stride)
    {
	this->push_back(PositionWeightPair<int>(-(stride+1),-1));
	this->push_back(PositionWeightPair<int>(-1,-1));
	this->push_back(PositionWeightPair<int>(stride-1,-1));
	this->push_back(PositionWeightPair<int>(-(stride-1),1));
	this->push_back(PositionWeightPair<int>(1,1));
	this->push_back(PositionWeightPair<int>(stride+1,1));
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT PrewittFilterX<INPUT, OUTPUT>::apply(Image<INPUT>* input, int n, Image<unsigned char>* support)
    {
        return -input(n-stridep1)-input(n-1)-input(n+stridem1)+
        input(n-stridem1)+input(n+1)+input(n+stridep1);
    }

    /**
     * PrewittFilterY represents the 3x3 Y Prewitt filter
     */
    template<typename INPUT, typename OUTPUT>
    class PrewittFilterY: public SobelFilterY<INPUT, OUTPUT>
    {
    public:
        using SobelFilterY<INPUT, OUTPUT>::stridem1;
        using SobelFilterY<INPUT, OUTPUT>::stridep1;
        using SobelFilterY<INPUT, OUTPUT>::apply;

	/**
	* PrewittFilterY constructor
	* @param stride stride of the image to use with
	*/
        PrewittFilterY(int stride= 4000);

	/**
	* applies the filter in a given position
	* @param input input image to apply on
	* @param n position to apply in
	* @return filter response
	*/
        OUTPUT apply(Image<INPUT>* input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    PrewittFilterY<INPUT, OUTPUT>::PrewittFilterY(int stride)
    : SobelFilterY<INPUT, OUTPUT>(stride)
    {
	this->push_back(PositionWeightPair<int>(-(stride+1),-1));
	this->push_back(PositionWeightPair<int>(-1, -1));
	this->push_back(PositionWeightPair<int>(stride-1, -1));
	this->push_back(PositionWeightPair<int>(-(stride-1),1));
	this->push_back(PositionWeightPair<int>(1,1));
	this->push_back(PositionWeightPair<int>(stride+1,1));
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT PrewittFilterY<INPUT, OUTPUT>::apply(Image<INPUT>* input, int n, Image<unsigned char>* support)
    {
        return -input(n-stridep1)-input(n-1)-input(n+stridem1)+
        input(n-stridem1)+input(n+1)+input(n+stridep1);
    }

    /**
     * Sobel edge detection
     */
    template<typename INPUT, typename OUTPUT>
    class SobelFilter: public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::min;
        using Filter2<INPUT, OUTPUT, int>::max;
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::updateStride;
        using Filter2<INPUT, OUTPUT, int>::descriptor;

        /**
         * constructor to create Sobel filter
         * @param stride stride of the image
         */
        SobelFilter(int stride= 4000);

        /**
         * overriding apply method in the filter
         * @param input input image
         * @param n pixel in which position edge properties are calculated
         * @param magnitude edge magnitude in the position n of the input image
         * @param orientation edge orientation in the position n of the input image
         */
        void apply(Image<INPUT>& input, Pixel1 n, OUTPUT& magnitude, OUTPUT& orientation, Image<unsigned char>* support= NULL);

        /**
         * computes the edge magnitude in position n
         * @param input input image
         * @param n pixel in row-continuous representation
         * @return edge magnitude
         */
        OUTPUT applyMagnitude(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes the edge orientation in position n
         * @param input input image
         * @param n pixel in row-continuous representation
         * @return edge orientation
         */
        OUTPUT applyOrientation(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support= NULL);

        /**
         * computes the edge magnitude in each position of the input image
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non 0) region of the mask
         */
        void applyMagnitude(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the edge orientation in each position of the input image
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non 0) region of the mask
         */
        void applyOrientation(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

	/**
	* computes edge magnitude and applies non maximum suppression
	* @param input input image
	* @param output output image
	* @param c Cache2 object for temporary data structures
	* @param mask mask object, the operation is performed in the foreground (non 0) region
	*/
	
/*       void applyMagnitudeNonMaximumSuppression(Image<INPUT>& input, Image<OUTPUT>& output, Cache2& c, Image<unsigned char>* mask= NULL);*/

        /**
         * computes the edge magnitude and orientation in each position of the input image
         * @param input input image
         * @param magnitude magnitude output image
         * @param orientation orientation output image
         * @param mask the operation is performed in the foreground (non 0) region of the mask
         */
        void apply(Image<INPUT>& input, Image<OUTPUT>& magnitude, Image<OUTPUT>& orientation, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * overrides the Filter::apply method, computes edge magnitude
         * @param input input image
         * @param output output magnitude image
         * @param mask the operation is performed in the foreground (non 0) region of the mask
         */
        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * sobel x filter
         */
        SobelFilterX<INPUT, OUTPUT>* sfx;

        /**
         * sobel y filter
         */
        SobelFilterY<INPUT, OUTPUT>* sfy;
    };

    template<typename INPUT, typename OUTPUT>
    SobelFilter<INPUT, OUTPUT>::SobelFilter(int stride)
    : Filter2<INPUT, OUTPUT, int>(stride)
    {
        this->sfx= new SobelFilterX<INPUT, OUTPUT>(stride);
        this->sfy= new SobelFilterY<INPUT, OUTPUT>(stride);
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SobelFilter<INPUT, OUTPUT>::applyMagnitude(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support)
    {
        float tmpx= this->sfx->apply(input, n, support);
        float tmpy= this->sfy->apply(input, n, support);
        return sqrt(tmpx*tmpx + tmpy*tmpy);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SobelFilter<INPUT, OUTPUT>::applyOrientation(Image<INPUT>& input, Pixel1 n, Image<unsigned char>* support)
    {
        float tmpx= this->sfx->apply(input, n, support);
        float tmpy= this->sfy->apply(input, n, support);
        return std::atan2(tmpy,tmpx);
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Pixel1 n, OUTPUT& magnitude, OUTPUT& orientation, Image<unsigned char>* support)
    {
        float tmpx= this->sfx->apply(input, n, support);
        float tmpy= this->sfy->apply(input, n, support);
        magnitude= sqrt(tmpx*tmpx + tmpy*tmpy);
        orientation= std::atan2(tmpy,tmpx);
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::applyMagnitude(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(input.columns) )
        {
            sfx->updateStride(input.columns);
            sfy->updateStride(input.columns);
            computeMinMax();
        }

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n - this->max); ++i )
                output(i)= applyMagnitude(input, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n - this->max); ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= applyMagnitude(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::applyOrientation(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(input.columns) )
        {
            sfx->updateStride(input.columns);
            sfy->updateStride(input.columns);
            computeMinMax();
        }

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                output(i)= applyOrientation(input, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= applyOrientation(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& magnitude, Image<OUTPUT>& orientation, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(input.columns) )
        {
            sfx->updateStride(input.columns);
            sfy->updateStride(input.columns);
            computeMinMax();
        }

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                apply(input, i, magnitude(i), orientation(i), support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                if ( (*roi)(i) > 0 )
                    apply(input, i, magnitude(i), orientation(i), support);
        }
    }

/*    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::applyMagnitudeNonMaximumSuppression(Image<INPUT>& input, Image<OUTPUT>& output, Cache2& c, Image<unsigned char>* mask)
    {
        Image<OUTPUT>* orient;
        c.get(&orient);

        apply(input, output, *orient, mask);

        for ( int i= stride + 1; i < input.n - stride - 1; ++i )
        {
            if ( (*orient)(i) > -M_PI/8 && (*orient)(i) <= M_PI/8 )
            {
                if ( output(i) < output(i-stride) || output(i) < output(i+stride) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) > M_PI/8 && (*orient)(i) <= 3 * M_PI/8 )
            {
                if ( output(i) < output(i-stride-1) || output(i) < output(i+stride+1) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) > 3*M_PI/8 && (*orient)(i) <= 5 * M_PI/8 )
            {
                if ( output(i) < output(i-1) || output(i) < output(i+1) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) > 5*M_PI/8 && (*orient)(i) <= 7 * M_PI/8 )
            {
                if ( output(i) < output(i-stride + 1) || output(i) < output(i+stride-1) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) > 7*M_PI/8 || (*orient)(i) <= -7 * M_PI/8 )
            {
                if ( output(i) < output(i-stride) || output(i) < output(i+stride) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) <= -5*M_PI/8 && (*orient)(i) > -7 * M_PI/8 )
            {
                if ( output(i) < output(i-stride-1) || output(i) < output(i+stride+1) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) <= -3*M_PI/8 && (*orient)(i) > -5 * M_PI/8 )
            {
                if ( output(i) < output(i-1) || output(i) < output(i+1) )
                    output(i)= 0;
                continue;
            }
            if ( (*orient)(i) <= -1*M_PI/8 && (*orient)(i) > -3 * M_PI/8 )
            {
                if ( output(i) < output(i-stride + 1) || output(i) < output(i+stride-1) )
                    output(i)= 0;
                continue;
            }
        }
    }*/

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->applyMagnitude(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilter<INPUT, OUTPUT>::computeMinMax()
    {
        min= -stride-1;
        max= stride+1;
    }

    template<typename INPUT, typename OUTPUT>
    class SobelFilterMagnitude: public SobelFilter<INPUT, OUTPUT>
    {
    public:
        using SobelFilter<INPUT, OUTPUT>::apply;
        using SobelFilter<INPUT, OUTPUT>::applyMagnitude;
        using SobelFilter<INPUT, OUTPUT>::computeMinMax;
        using SobelFilter<INPUT, OUTPUT>::getProposedBorder;
        using SobelFilter<INPUT, OUTPUT>::updateStride;
        using SobelFilter<INPUT, OUTPUT>::descriptor;

        SobelFilterMagnitude(int stride= 4000);

        SobelFilterMagnitude(const SobelFilterMagnitude& sfm);

        ~SobelFilterMagnitude();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support);
    };

    template<typename INPUT, typename OUTPUT>
    SobelFilterMagnitude<INPUT, OUTPUT>::SobelFilterMagnitude(int stride)
        : SobelFilter<INPUT, OUTPUT>(stride)
    {
        stringstream ss;
        ss << "SobelFilterMagnitude";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    SobelFilterMagnitude<INPUT, OUTPUT>::SobelFilterMagnitude(const SobelFilterMagnitude &sfm)
        : SobelFilter<INPUT, OUTPUT>(sfm)
    {
    }

    template<typename INPUT, typename OUTPUT>
    SobelFilterMagnitude<INPUT, OUTPUT>::~SobelFilterMagnitude()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilterMagnitude<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        applyMagnitude(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class SobelFilterOrientation: public SobelFilter<INPUT, OUTPUT>
    {
    public:
        using SobelFilter<INPUT, OUTPUT>::apply;
        using SobelFilter<INPUT, OUTPUT>::applyMagnitude;
        using SobelFilter<INPUT, OUTPUT>::computeMinMax;
        using SobelFilter<INPUT, OUTPUT>::getProposedBorder;
        using SobelFilter<INPUT, OUTPUT>::updateStride;
        using SobelFilter<INPUT, OUTPUT>::descriptor;

        SobelFilterOrientation(int stride= 4000);

        SobelFilterOrientation(const SobelFilterOrientation& sfm);

        ~SobelFilterOrientation();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support);
    };

    template<typename INPUT, typename OUTPUT>
    SobelFilterOrientation<INPUT, OUTPUT>::SobelFilterOrientation(int stride)
        : SobelFilter<INPUT, OUTPUT>(stride)
    {
        stringstream ss;
        ss << "SobelFilterOrientation";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    SobelFilterOrientation<INPUT, OUTPUT>::SobelFilterOrientation(const SobelFilterOrientation &sfm)
        : SobelFilter<INPUT, OUTPUT>(sfm)
    {
    }

    template<typename INPUT, typename OUTPUT>
    SobelFilterOrientation<INPUT, OUTPUT>::~SobelFilterOrientation()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void SobelFilterOrientation<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        applyMagnitude(input, output, roi, support);
    }

    /**
    * Prewitt edge detection filter
    */
    template<typename INPUT, typename OUTPUT>
    class PrewittFilter: public SobelFilter<INPUT, OUTPUT>
    {
    public:
        using SobelFilter<INPUT, OUTPUT>::apply;
        using SobelFilter<INPUT, OUTPUT>::applyMagnitude;
        using SobelFilter<INPUT, OUTPUT>::applyOrientation;
        using SobelFilter<INPUT, OUTPUT>::computeMinMax;
        using SobelFilter<INPUT, OUTPUT>::updateStride;

	/**
	* PrewittFilter constructor
	* @param stride stride of the image to use on
	*/
        PrewittFilter(int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    PrewittFilter<INPUT, OUTPUT>::PrewittFilter(int stride)
    {
        this->stride= stride;
        this->sfx= new PrewittFilterX<INPUT, OUTPUT>(stride);
        this->sfy= new PrewittFilterY<INPUT, OUTPUT>(stride);
        computeMinMax();
    }

    /** 
    * GaussianFilter2_3x3 is an optimized 3x3 gaussian filter
    */
    
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter2_3x3: public Filter2<INPUT, OUTPUT, float>
    {
    public:
	    /**
	    * optimized GaussianFilter2_3x3 constructor
	    * @param stride stride of the image to use on
	    * @param sigma standard deviation
	    */
            GaussianFilter2_3x3(float sigma, int stride= 4000);

            /**
             * compute min, max positions
             */
            void computeMinMax();

            /**
             * updates stride
             * @param stride new stride
             * @return returns 1 if the stride changed, else 0
             */
            int updateStride(int stride);

            /**
             * vertical 1D filter
             */
            GaussianFilter1_3<INPUT, OUTPUT>* vertical;

            /**
             * horizontal 1D filter
             */
            GaussianFilter1_3<OUTPUT, OUTPUT>* horizontal;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianFilter2_3x3<INPUT, OUTPUT>::GaussianFilter2_3x3(float sigma, int stride)
    : Filter2<INPUT, OUTPUT, float>(stride)
    {
        vertical= new GaussianFilter1_3<INPUT, OUTPUT>(sigma);
        horizontal= new GaussianFilter1_3<OUTPUT, OUTPUT>(sigma);

        vertical->pos= stride;
        vertical->computeMinMax();
        this->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    int GaussianFilter2_3x3<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( stride != this->stride )
        {
            vertical->pos= stride;
            vertical->computeMinMax();
            return 1;
        }
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianFilter2_3x3<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= vertical->min;
        this->max= vertical->max;
    }

    /**
    * GaussianFilter2_5x5 is an optimized 5x5 sized Gaussian filter
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter2_5x5: public Filter2<INPUT, OUTPUT, float>
    {
    public:
	    /**
	    * GaussianFilter2_5x5 is an optimized 5x5 sized Gaussian filter
	    * @param stride stride of the image to use on
	    * @param sigma standard deviation
	    */
            GaussianFilter2_5x5(float sigma, int stride= 4000);

            /**
             * compute min, max positions
             */
            void computeMinMax();

            /**
             * updates stride
             * @param stride new stride
             * @return returns 1 if the stride changed, else 0
             */
            int updateStride(int stride);

            /**
             * vertical 1D filter
             */
            GaussianFilter1_5<INPUT, OUTPUT>* vertical;

            /**
             * horizontal 1D filter
             */
            GaussianFilter1_5<OUTPUT, OUTPUT>* horizontal;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianFilter2_5x5<INPUT, OUTPUT>::GaussianFilter2_5x5(float sigma, int stride)
    : Filter2<INPUT, OUTPUT, float>(stride)
    {
        vertical= new GaussianFilter1_5<INPUT, OUTPUT>(sigma);
        horizontal= new GaussianFilter1_5<OUTPUT, OUTPUT>(sigma);

        vertical->pos1= stride;
        vertical->pos2= 2*stride;
        vertical->computeMinMax();
        this->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    int GaussianFilter2_5x5<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( stride != this->stride )
        {
            vertical->pos1= stride;
            vertical->pos2= 2*stride;
            vertical->computeMinMax();
            return 1;
        }
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianFilter2_5x5<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= vertical->min;
        this->max= vertical->max;
    }

    /**
     * GaussianFilter2D represents a 2D Gaussian filter
     */
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter2: public Filter2<INPUT, OUTPUT, float>
    {
        public:
            using Filter2<INPUT, OUTPUT, float>::apply;
            using Filter2<INPUT, OUTPUT, float>::descriptor;
	    using Filter2<INPUT, OUTPUT, float>::normalize;
	    using Filter2<INPUT, OUTPUT, float>::computeMinMax;
            using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
            using Filter2<INPUT, OUTPUT, float>::getProposedBorder2;

            /**
             * constructor to create GaussianFilter2D instance
             * @param stride stride of the image
             * @param size of the filter
             * @param sigma sigma parameter of the Gaussian function
             * @param alpha distribution transformation parameter
             * @param beta distribution transformation parameter
             * @param delta distribution transformation parameter
             * @param gamma distribution transformation parameter
             */
            GaussianFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

            virtual ~GaussianFilter2();
            
            /**
             * overriding apply method in the filter
             * @param input input image
             * @param output output image
             * @param c cache to avoid instantiation of temporary image object
             * @param mask the filter is applied only in the foreground (non 0) region of the
             * binary mask (0, non 0)
             */
            //virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

            /**
             * compute min, max positions
             */
            //void computeMinMax();

            /**
             * updates stride
             * @param stride new stride
             * @return returns 1 if the stride changed, else 0
             */
            virtual int updateStride(int stride);

            /**
             * vertical 1D filter
             */
            GaussianFilter1<INPUT, OUTPUT>* vertical;

            /**
             * horizontal 1D filter
             */
            GaussianFilter1<OUTPUT, OUTPUT>* horizontal;

//            string toString() const;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param g GaussianFilter2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const GaussianFilter2<INPUT, OUTPUT>& g)
    {
        o << g.descriptor;

        return o;
    }

    template<typename INPUT, typename OUTPUT>
    GaussianFilter2<INPUT, OUTPUT>::GaussianFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>(stride)
    {
        if ( size == -1 )
            size= 3*sigma;
      
        //printf("generating vertical filter\n"); fflush(stdout);
        vertical= new GaussianFilter1<INPUT, OUTPUT>(sigma, size, alpha, beta, delta, gamma);
        //printf("generating horizontal filter\n"); fflush(stdout);
        horizontal= new GaussianFilter1<OUTPUT, OUTPUT>(sigma, size, alpha, beta, delta, gamma);

        //printf("updating vertical filter\n"); fflush(stdout);
        for ( typename Filter1<INPUT, OUTPUT, float>::iterator git= vertical->begin(); git != vertical->end(); ++git )
            git->first= git->first * stride;
        //printf("vertical filter updated\n"); fflush(stdout);

        vertical->min= vertical->begin()->first;
        vertical->max= -vertical->min;

        //printf("aaa\n"); fflush(stdout);
        for ( int i= -size/2; i <= size/2; ++i )
        {
            for ( int j= -size/2; j <= size/2; ++j )
            {
                //printf("%f ", gauss2(i,j,sigma, alpha, beta, delta, gamma));
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2(i,j,sigma, alpha, beta, delta, gamma)));
            }
            //printf("\n");
        }

        //printf("bbb\n"); fflush(stdout);
        this->computeMinMax();
        //printf("ccc\n"); fflush(stdout);
        this->normalize();
        //printf("ddd\n"); fflush(stdout);
        
        horizontal->normalize();
        //printf("eee\n"); fflush(stdout);
        vertical->normalize();
        //printf("fff\n"); fflush(stdout);

        stringstream ss;
        ss << "GaussianFilter2" << " " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
        //printf("ggg\n"); fflush(stdout);
    }
    
    template<typename INPUT, typename OUTPUT>
    GaussianFilter2<INPUT, OUTPUT>::~GaussianFilter2()
    {
        delete vertical;
        delete horizontal;
    }

    /*template<typename INPUT, typename OUTPUT>
    void GaussianFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( this->updateStride(input.columns) )
            computeMinMax();

        Image<OUTPUT> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        vertical->apply(input, tmp, roi, support);
        horizontal->apply(tmp, output, roi, support);
    }*/

    template<typename INPUT, typename OUTPUT>
    int GaussianFilter2<INPUT, OUTPUT>::updateStride(int stride)
    {
        int stride_old= this->stride;
        int f= this->Filter2<INPUT, OUTPUT, float>::updateStride(stride);
        this->Filter2<INPUT, OUTPUT, float>::computeMinMax();
        if ( f )
        {
            for ( typename Filter1<INPUT, OUTPUT, float>::iterator git= vertical->begin(); git != vertical->end(); ++git )
                git->first= git->first / stride_old * stride;

            vertical->min= -(vertical->size()-1)*stride;
            vertical->max= (vertical->size()-1)*stride;
            vertical->stride= stride;
            horizontal->stride= stride;
            return 1;
        }
        return 0;
    }

    /*template<typename INPUT, typename OUTPUT>
    void GaussianFilter2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= vertical->min;
        this->max= vertical->max;
	
    }*/

    /**
     * LineSegmentFilter represents a filter, which has Gaussian profiles and
     * isophotes in the orthogonal direction
     */
    template<typename INPUT, typename OUTPUT>
    class LineSegmentFilter: public Filter2<INPUT, OUTPUT, float>
    {
        public:
            using Filter2<INPUT, OUTPUT, float>::normalizer;
            using Filter2<INPUT, OUTPUT, float>::descriptor;
            using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
            using Filter2<INPUT, OUTPUT, float>::updateStride;

            /**
             * constructor to create LineSegmentFilter instance
             * @param stride stride of the image
             * @param sizex width of the line segment
             * @param sizey length of the line segment
             * @param sigma sigma parameter of the Gaussian function
             * @param a angle of the rotation
             * @param alpha distribution transformation parameter
             * @param beta distribution transformation parameter
             * @param delta distribution transformation parameter
             * @param gamma distribution transformation parameter
             */
            LineSegmentFilter(int sizex, int sizey, float sigma, float a, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    LineSegmentFilter<INPUT, OUTPUT>::LineSegmentFilter(int rows, int columns, float sigma, float a, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>(stride)
    {
        float width= floor(sqrt(rows*rows + columns*columns)+0.5);

        Vector2<float> p0(0,0);
        Vector2<float> p1(cos(a)*columns/2, sin(a)*columns/2);
        Vector2<float> p2(-sin(a)*rows/2, cos(a)*rows/2);
        
        float distA, distB;
        float uA, uB;

        Vector2<float> p3;
        Vector2<float> p3m0;
        Vector2<float> p2m0;
        Vector2<float> p1m0;

        float g;

        for ( int j= -width/2; j <= width/2; ++j )
        {
            for ( int i= -width/2; i <= width/2; ++i )
            {
                p3.set(i,j);
                
                p3m0= p3 - p0;
                p2m0= p2 - p0;
                p1m0= p1 - p0;

                uA= p3m0.inner(p2m0)/p2m0.inner(p2m0);
                Vector2<float> pI(p0 + (p2 - p0)*uA);
                distA= floor(pI.dist2(p3) + 0.5);

                uB= p3m0.inner(p1m0)/p1m0.inner(p1m0);
                Vector2<float> pJ(p0 + (p1 - p0)*uB);
                distB= floor(pJ.dist2(p3) + 0.5);

                if ( distA <= columns/2 && distB <= rows/2 )
                {
                    g= gauss1(distB, sigma, alpha, beta, delta, gamma);
                    //sum+= g;
                    this->insert(PositionWeightPair<float>(j*stride + i, -g));
                }
            }
        }

        this->normalize();
        this->zeroMean();
        
        this->computeMinMax();

        stringstream ss;
        ss << "LineSegmentFilter" << " " << rows << " " << columns << " " << sigma << " " << a << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class FixedSizeLineSegmentFilter: public Filter2<INPUT, OUTPUT, float>
    {
        public:
            using Filter2<INPUT, OUTPUT, float>::normalizer;
            using Filter2<INPUT, OUTPUT, float>::descriptor;
            using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
            using Filter2<INPUT, OUTPUT, float>::updateStride;

            /**
             * constructor to create LineSegmentFilter instance
             * @param stride stride of the image
             * @param sizex width of the line segment
             * @param sizey length of the line segment
             * @param sigma sigma parameter of the Gaussian function
             * @param a angle of the rotation
             * @param alpha distribution transformation parameter
             * @param beta distribution transformation parameter
             * @param delta distribution transformation parameter
             * @param gamma distribution transformation parameter
             */
            FixedSizeLineSegmentFilter(int sizex, int sizey, float sigma, float a, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeLineSegmentFilter<INPUT, OUTPUT>::FixedSizeLineSegmentFilter(int rows, int columns, float sigma, float a, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>(stride)
    {
        float width= floor(sqrt(rows*rows + columns*columns)+0.5);

        Vector2<float> p0(0,0);
        Vector2<float> p1(cos(a)*columns/2, sin(a)*columns/2);
        Vector2<float> p2(-sin(a)*rows/2, cos(a)*rows/2);

        float distA, distB;
        float uA, uB;

        Vector2<float> p3;
        Vector2<float> p3m0;
        Vector2<float> p2m0;
        Vector2<float> p1m0;

        float g;

        for ( int j= -width/2; j <= width/2; ++j )
        {
            for ( int i= -width/2; i <= width/2; ++i )
            {
                p3.set(i,j);

                p3m0= p3 - p0;
                p2m0= p2 - p0;
                p1m0= p1 - p0;

                uA= p3m0.inner(p2m0)/p2m0.inner(p2m0);
                Vector2<float> pI(p0 + (p2 - p0)*uA);
                distA= floor(pI.dist2(p3) + 0.5);

                uB= p3m0.inner(p1m0)/p1m0.inner(p1m0);
                Vector2<float> pJ(p0 + (p1 - p0)*uB);
                distB= floor(pJ.dist2(p3) + 0.5);

                if ( distA <= columns/2 && distB <= rows/2 )
                {
                    g= gauss1(distB, sigma, alpha, beta, delta, gamma);
                    //sum+= g;
                    this->insert(PositionWeightPair<float>(j*stride + i, -g));
                }
            }
        }

        this->normalize();
        this->zeroMean();

        this->computeMinMax();

        stringstream ss;
        ss << "FixedSizeLineSegmentFilter" << " " << rows << " " << columns << " " << sigma << " " << a << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationLineSegmentFilter: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
        public:
            using CorrelationFilter2<INPUT, OUTPUT, float>::normalizer;
            using CorrelationFilter2<INPUT, OUTPUT, float>::descriptor;
            using CorrelationFilter2<INPUT, OUTPUT, float>::getProposedBorder;
            using CorrelationFilter2<INPUT, OUTPUT, float>::updateStride;

            /**
             * constructor to create LineSegmentFilter instance
             * @param stride stride of the image
             * @param sizex width of the line segment
             * @param sizey length of the line segment
             * @param sigma sigma parameter of the Gaussian function
             * @param a angle of the rotation
             * @param alpha distribution transformation parameter
             * @param beta distribution transformation parameter
             * @param delta distribution transformation parameter
             * @param gamma distribution transformation parameter
             */
            CorrelationLineSegmentFilter(int sizex, int sizey, float sigma, float a, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationLineSegmentFilter<INPUT, OUTPUT>::CorrelationLineSegmentFilter(int rows, int columns, float sigma, float a, float alpha, float beta, float delta, float gamma, int stride)
    : CorrelationFilter2<INPUT, OUTPUT, float>(stride)
    {
        float width= floor(sqrt(rows*rows + columns*columns)+0.5);

        Vector2<float> p0(0,0);
        Vector2<float> p1(cos(a)*columns/2, sin(a)*columns/2);
        Vector2<float> p2(-sin(a)*rows/2, cos(a)*rows/2);

        float distA, distB;
        float uA, uB;

        Vector2<float> p3;
        Vector2<float> p3m0;
        Vector2<float> p2m0;
        Vector2<float> p1m0;

        float g;

        for ( int j= -width/2; j <= width/2; ++j )
        {
            for ( int i= -width/2; i <= width/2; ++i )
            {
                p3.set(i,j);

                p3m0= p3 - p0;
                p2m0= p2 - p0;
                p1m0= p1 - p0;

                uA= p3m0.inner(p2m0)/p2m0.inner(p2m0);
                Vector2<float> pI(p0 + (p2 - p0)*uA);
                distA= floor(pI.dist2(p3) + 0.5);

                uB= p3m0.inner(p1m0)/p1m0.inner(p1m0);
                Vector2<float> pJ(p0 + (p1 - p0)*uB);
                distB= floor(pJ.dist2(p3) + 0.5);

                if ( distA <= columns/2 && distB <= rows/2 )
                {
                    g= gauss1(distB, sigma, alpha, beta, delta, gamma);
                    //sum+= g;
                    this->insert(PositionWeightPair<float>(j*stride + i, -g));
                }
            }
        }

        this->normalize();
        this->zeroMean();

        this->computeMinMax();

        stringstream ss;
        ss << "CorrelationLineSegmentFilter" << " " << rows << " " << columns << " " << sigma << " " << a << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    /**
    * GaussianDxFilter2 represents the derivative Gaussian filter along x
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianDxFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::push_back;

	/**
	* GaussianDxFilter2 constructor
	* @param sigma standard deviation
	* @param size size of the filter kernel
	* @param stride stride of the images to use on
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDxFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDxFilter2<INPUT, OUTPUT>::GaussianDxFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2_dx(i, j, sigma, alpha, beta, delta, gamma)));
        this->computeMinMax();

        stringstream ss;
        ss << "GaussianDxFilter2 " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    /**
    * GaussianDyFilter2 represents the derivative Gaussian filter along y
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianDyFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDyFilter2 constructor
	* @param sigma standard deviation
	* @param size kernel size
	* @param stride stride of the images to use on
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDyFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDyFilter2<INPUT, OUTPUT>::GaussianDyFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2_dy(i, j, sigma, alpha, beta, delta, gamma)));
        this->computeMinMax();

        stringstream ss;
        ss << "GaussianDyFilter2 " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    /**
    * GaussianDxyFilter2 represents the second partial derivative Gaussian filter along x and y
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianDxyFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDxyFilter2 constructor
	* @param sigma standard deviation
	* @param size kernel size
	* @param stride stride of the images to use on
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDxyFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDxyFilter2<INPUT, OUTPUT>::GaussianDxyFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2_dxy(i, j, sigma, alpha, beta, delta, gamma)));

        this->computeMinMax();

        stringstream ss;
        ss << "GaussianDxyFilter2 " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    /**
    * GaussianDxxFilter2 represents the second derivative Gaussian filter along x and x
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianDxxFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDxxFilter2 constructor
	* @param sigma standard deviation
	* @param size size of the filter kernel
	* @param stride stride of the images to use in
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDxxFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDxxFilter2<INPUT, OUTPUT>::GaussianDxxFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2_dxx(i, j, sigma, alpha, beta, delta, gamma)));
        this->computeMinMax();

        stringstream ss;
        ss << "GaussianDxxFilter2 " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    /**
    * GaussianDyyFilter2 represents the second derivative Gaussian filter along y and y
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianDyyFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDyyFilter2 constructor
	* @param sigma standard deviation
	* @param size kernel size of the filter
	* @param stride stride of the images to use in
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDyyFilter2(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDyyFilter2<INPUT, OUTPUT>::GaussianDyyFilter2(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, gauss2_dyy(i, j, sigma, alpha, beta, delta, gamma)));
        this->computeMinMax();

        stringstream ss;
        ss << "GaussianDyyFilter2 " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LoGFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDyyFilter2 constructor
	* @param sigma standard deviation
	* @param size kernel size of the filter
	* @param stride stride of the images to use in
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        LoGFilter2(float sigma, int size= -1, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    LoGFilter2<INPUT, OUTPUT>::LoGFilter2(float sigma, int size, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, logfilter(i, j, sigma)));
        this->computeMinMax();

        stringstream ss;
        ss << "LoGFilter2 " << sigma << " " << size;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationLoGFilter2: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using CorrelationFilter2<INPUT, OUTPUT, float>::dumpFilter;
        using CorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using CorrelationFilter2<INPUT, OUTPUT, float>::descriptor;
        using CorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using CorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;

	/**
	* GaussianDyyFilter2 constructor
	* @param sigma standard deviation
	* @param size kernel size of the filter
	* @param stride stride of the images to use in
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        CorrelationLoGFilter2(float sigma, int size= -1, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationLoGFilter2<INPUT, OUTPUT>::CorrelationLoGFilter2(float sigma, int size, int stride)
    : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                this->push_back(PositionWeightPair<float>(i*stride + j, logfilter(i, j, sigma)));
        this->computeMinMax();

        stringstream ss;
        ss << "CorrelationLoGFilter2 " << sigma << " " << size;
        descriptor= ss.str();
    }

/**
     * matched T-Junction filter with convolution response
     */
    template<typename INPUT, typename OUTPUT>
    class TJunctionFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::updateStride;

        /**
         * constructor
         * @param kernel kernel size
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param r1 angle 1
         * @param r2 angle 2
         * @param r3 angle 3
         * @param stride stride of the images to apply to
         */
        TJunctionFilter2(int kernel, float alpha, float beta, float gamma, int r1, int r2, int r3, int stride= 4000);

        /**
         * copy constructor
         * @param t instance to copy
         */
        TJunctionFilter2(const TJunctionFilter2& t);

        /**
         * destructor
         */
        ~TJunctionFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    TJunctionFilter2<INPUT, OUTPUT>::TJunctionFilter2(int kernel, float a1, float b1, float g1, int r1, int r2, int r3, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "TJunctionFilter2 " << kernel << " " << a1 << " " << b1 << " " << g1 << " " << r1 << " " << r2 << " " << r3 << " " << stride;
        printf("TJunctionFilter2 %f %f %f\n", a1, b1, g1);
        descriptor= ss.str();

        float alpha= a1;
        //float beta= b1 + a1;
        //float gamma= a1 + b1 + g1;
        float beta= b1;
        float gamma= g1;
        float zeta;
        float a= 0, b= 0;
        float t1, t2, t3;
        float length;
        float d1, d2;
        float dx= 0, dy= 0;

        /*
         *        |
         *    IV. | I.
         *   -----------
         *    III.| II.
         *        |
         */

        if ( r1 != 0 )
        {
            dx= cos(alpha)*r1;
            dy= sin(alpha)*r1;
        }
        else if ( r2 != 0 )
        {
            dx= cos(beta)*r2;
            dy= sin(beta)*r2;
        }
        else if ( r3 != 0 )
        {
            dx= cos(gamma)*r3;
            dy= sin(gamma)*r3;
        }
        dx= floor(dx + 0.5);
        dy= floor(dy + 0.5);

        for ( int i= -kernel/2 + dx; i <= kernel/2 + dx; ++i )
            for ( int j= -kernel/2 + dy; j <= kernel/2 + dy; ++j )
            {
                length= sqrt(i*i + j*j);

                if ( i > 0 )
                {
                    if ( j > 0 )
                    {
                        // I.
                        zeta= acos(((float)(i))/length);
                    }
                    else
                    {
                        // II.
                        zeta= 2*M_PI - acos(((float)(i))/length);
                    }
                }
                else
                {
                    if ( j < 0 )
                    {
                        // III.
                        zeta= 2*M_PI - acos(((float)(i))/length);
                    }
                    else
                    {
                        // IV.
                        if ( i != 0 || j != 0 )
                        {
                            zeta= acos(((float)(i))/length);
                        }
                        else
                            zeta= alpha;
                    }
                }

                t1= fabs(alpha - zeta);
                t1= t1 > M_PI ? (2*M_PI - t1) : t1;
                t2= fabs(beta - zeta);
                t2= t2 > M_PI ? (2*M_PI - t2) : t2;
                t3= fabs(gamma - zeta );
                t3= t3 > M_PI ? (2*M_PI - t3) : t3;

                if ( t1 <= t3 && t2 <= t3 )
                {
                    a= alpha;
                    b= beta;
                }
                else if ( t1 <= t2 && t3 <= t2 )
                {
                    a= alpha;
                    b= gamma;
                }
                else if ( t2 <= t1 && t3 <= t1 )
                {
                    a= beta;
                    b= gamma;
                }

                d1= sqrt(length*length-length*length*cos(zeta - a)*cos(zeta - a));
                d2= sqrt(length*length-length*length*cos(zeta - b)*cos(zeta - b));

                float tmp1, tmp2;
                tmp1= fabs(zeta - a);
                tmp2= fabs(zeta - b);

                if ( tmp1 > M_PI )
                    tmp1= 2*M_PI - tmp1;
                if ( tmp2 > M_PI )
                    tmp2= 2*M_PI - tmp2;

                int finalX;
                int finalY;

                finalX= floor((float)i - dx + 0.5);
                finalY= floor((float)j - dy + 0.5);

                if ( tmp1 <= M_PI/2 || tmp2 <= M_PI/2 )
                {
                    if ( d1 <= d2 )
                        this->push_back(PositionWeightPair<float>(-finalY*stride + finalX, -gauss1(d1*2, 2)));
                    else
                        this->push_back(PositionWeightPair<float>(-finalY*stride + finalX, -gauss1(d2*2, 2)));
                }
                else
                {
                    this->push_back(PositionWeightPair<float>(-finalY*stride + finalX, -gauss1(sqrt(i*i + j*j)*2, 2)));
                }
            }
        this->stride= stride;
        this->computeMinMax();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    TJunctionFilter2<INPUT, OUTPUT>::TJunctionFilter2(const TJunctionFilter2& t)
    {
    }

    template<typename INPUT, typename OUTPUT>
    TJunctionFilter2<INPUT, OUTPUT>::~TJunctionFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class GaussianDerivedMagnitude: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        //using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        //using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;

	/**
	* GaussianDxFilter2 constructor
	* @param sigma standard deviation
	* @param size size of the filter kernel
	* @param stride stride of the images to use on
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianDerivedMagnitude(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        virtual int updateStride(int stride);

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual int getMin();

        virtual int getMax();

        virtual void computeMinMax();

        GaussianDxFilter2<INPUT, OUTPUT>* dx;
        GaussianDyFilter2<INPUT, OUTPUT>* dy;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianDerivedMagnitude<INPUT, OUTPUT>::GaussianDerivedMagnitude(float sigma, int size, float alpha, float beta, float delta, float gamma, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        dx= new GaussianDxFilter2<INPUT, OUTPUT>(sigma, size, alpha, beta, delta, gamma, stride);
        dy= new GaussianDyFilter2<INPUT, OUTPUT>(sigma, size, alpha, beta, delta, gamma, stride);

        stringstream ss;
        ss << "GaussianDerivedMagnitude " << sigma << " " << size << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    int GaussianDerivedMagnitude<INPUT, OUTPUT>::updateStride(int stride)
    {
        this->stride= stride;
        this->dx->updateStride(stride);
        this->dy->updateStride(stride);
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GaussianDerivedMagnitude<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp1= this->dx->apply(input, n, support), tmp2= this->dy->apply(input, n, support);
        return sqrt(tmp1*tmp1 + tmp2*tmp2);
    }

    template<typename INPUT, typename OUTPUT>
    int GaussianDerivedMagnitude<INPUT, OUTPUT>::getMin()
    {
        return this->dx->getMin() < this->dy->getMin() ? this->dx->getMin() : this->dy->getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int GaussianDerivedMagnitude<INPUT, OUTPUT>::getMax()
    {
        return this->dx->getMax() > this->dy->getMax() ? this->dx->getMax() : this->dy->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianDerivedMagnitude<INPUT, OUTPUT>::computeMinMax()
    {
        this->dx->computeMinMax();
        this->dy->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class SamplingFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param stride stride of the images to apply on
         */
        SamplingFilter2(float angle, float dist, int stride= 4000);
    };

    template<typename INPUT, typename OUTPUT>
    SamplingFilter2<INPUT, OUTPUT>::SamplingFilter2(float angle, float dist, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        this->stride= stride;
        std::stringstream ss;
        ss << "SamplingFilter2 " << angle << " " << dist << " " << stride;
        descriptor= ss.str();

        float cr= sin(angle)*dist;
        float cc= cos(angle)*dist;

        int cc0= floor(cc);
        int cc1= ceil(cc);
        int cr0= floor(cr);
        int cr1= ceil(cr);

        if ( cc0 == cc1 )
            cc1= cc0 + 1;
        if ( cr0 == cr1 )
            cr1= cr0 + 1;

        float a= fabs(cc1 - cc);
        float b= fabs(cc - cc0);
        float c= fabs(cr1 - cr);
        float d= fabs(cr - cr0);

        this->push_back(PositionWeightPair<float>(cr0*stride + cc0, a*c));
        this->push_back(PositionWeightPair<float>(cr0*stride + cc1, b*c));
        this->push_back(PositionWeightPair<float>(cr1*stride + cc0, a*d));
        this->push_back(PositionWeightPair<float>(cr1*stride + cc1, b*d));
    }

    
}

#endif	/* _FILTERS_H */

