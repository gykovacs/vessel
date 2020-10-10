/**
 * @file StatisticalFeature2s.h
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
 * This file contains some statistical feature objects.
 */

#ifndef _STATISTICALFILTER2S_H
#define	_STATISTICALFILTER2S_H

#include <openipDS/Filter2.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/RunningStructuringElements.h>

#include <openipDS/clFunctions.h>

#include <sstream>
#include <cmath>

namespace openip
{
    /**
     * StatisticalFeature2 is the base class for all statistical descriptors.
     */
    template<typename INPUT, typename OUTPUT>
    class StatisticalFeature2: public Feature2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using StructuringElementDisk::stride;
        using StructuringElementDisk::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using StructuringElementDisk::getMin;
        using StructuringElementDisk::getMax;
        using StructuringElementDisk::getMean;
        using StructuringElementDisk::getMedian;
        using StructuringElementDisk::getVariance;
        using StructuringElementDisk::getSkewness;
        using StructuringElementDisk::getKurtosis;
        using StructuringElementDisk::getMeanAndVariance;
        using StructuringElementDisk::getStandardDeviation;
        using StructuringElementDisk::computeMinMax;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;
#ifdef USE_OPENCL        
        using Feature2<INPUT, OUTPUT>::enqueueKernel;
        using Feature2<INPUT, OUTPUT>::getOpenCLKernel;
        using Feature2<INPUT, OUTPUT>::applyGPU;
#endif
        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        StatisticalFeature2(int r, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        StatisticalFeature2(const StatisticalFeature2& s);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * returns the minimum relative position of row-major representation in the descriptor
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum relative position of row-major representation in the descriptor
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * using the proposed border, the feature can be computed in each position of the original image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();
        
        void getCoordinate2D(int n, int& rows, int& columns);
    };

    template<typename INPUT, typename OUTPUT>
    void StatisticalFeature2<INPUT, OUTPUT>::getCoordinate2D(int n, int& rows, int& columns)
    {
        rows= n / this->stride;
        if ( abs(n % this->stride) > this->stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % this->stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -this->stride : this->stride;
    }

    
    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->StructuringElementDisk::updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();
        int w= getMax() / stride + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    StatisticalFeature2<INPUT, OUTPUT>::StatisticalFeature2(int r, int stride)
    : StructuringElementDisk(r, stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    StatisticalFeature2<INPUT, OUTPUT>::StatisticalFeature2(const StatisticalFeature2& s)
    : StructuringElementDisk(s), Feature2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->StructuringElementDisk::getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int StatisticalFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->StructuringElementDisk::getMax();
    }

    /**
     * prints the description of the parameter StatisticalFeature2 object to the parameter output stream
     * @param o parameter output stream
     * @param sf StatisticalFeature2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const StatisticalFeature2<INPUT, OUTPUT>& sf)
    {
        o << sf.descriptor;
        return o;
    }

    /**
     * Signal-to-Noise Ratio feature
     */
    template<typename INPUT, typename OUTPUT>
    class SNRFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        SNRFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    SNRFeature2<INPUT, OUTPUT>::SNRFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "SNRFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SNRFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float mean, variance;
        this->getMeanAndVariance(input, n, mean, variance, support);

        if ( variance >= 1 )
            return (OUTPUT)(mean/sqrt(variance));
        else
            return 0;
    }

    /**
     * Invariant Hu's Moment 1 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment1Feature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment1Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M00(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M10(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M01(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M11(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M20(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M02(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M21(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M12(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M22(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M30(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float M03(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        void centroid(Image<INPUT>& input, int n, float& cx, float& cy, Image<unsigned char>* support= NULL);
        
        float u00(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u01(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u10(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u11(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u20(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u02(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u21(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u12(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u30(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float u03(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n00(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n01(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n10(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n11(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n20(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n02(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n21(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n12(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n30(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        float n03(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment1Feature2<INPUT, OUTPUT>::InvariantHuMoment1Feature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment1Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        /*int tmpx, tmpy;
        float sum00= 0;
        float sum02= 0;
        float sum20= 0;

        float m00= 0;
        float m10= 0;
        float m01= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
                m10+= tmpx * input(n + *sit);
                m01+= tmpy * input(n + *sit);
            }
        }

        m10/=m00;
        m01/=m00;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                sum00+= input(n + *sit);
                sum20+= (tmpx - m10)*(tmpx - m10)*input(n + *sit);
                sum02+= (tmpy - m01)*(tmpy - m01)*input(n + *sit);
            }
        }

        //printf("%f ", sum20/pow(sum00, 1 + 2/2) + sum02/pow(sum00,1 + 2/2));
        return (OUTPUT)(sum20/pow(sum00, 1 + 2/2) + sum02/pow(sum00,1 + 2/2));*/
        
        float a= n20(input, n, support);
        float b= n02(input, n, support);
        
        return a + b;
    }
    
    template<typename INPUT, typename OUTPUT>
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
//        int tmpx, tmpy;

        float m00= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
//                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
            }
        }
        return m00;
    }
        
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m10= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m10+= tmpx * input(n + *sit);
            }
        }
        return m10;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m01= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m01+= tmpy * input(n + *sit);
            }
        }
        return m01;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m11= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m11+= tmpx * tmpy * input(n + *sit);
            }
        }
        return m11;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m20= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m20+= tmpx * tmpx * input(n + *sit);
            }
        }
        return m20;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m02= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m02+= tmpy * tmpy * input(n + *sit);
            }
        }
        return m02;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m21= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m21+= tmpx * tmpx * tmpy * input(n + *sit);
            }
        }
        return m21;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m12= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m12+= tmpx * tmpy * tmpy * input(n + *sit);
            }
        }
        return m12;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m22= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m22+= tmpx * tmpy * tmpy * tmpy * input(n + *sit);
            }
        }
        return m22;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m30= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m30+= tmpx * tmpx * tmpx * input(n + *sit);
            }
        }
        return m30;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m03= 0;
        int tmpx, tmpy;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m03+= tmpy * tmpy * tmpy * input(n + *sit);
            }
        }
        return m03;
    }
    
    template<typename INPUT, typename OUTPUT>
    void InvariantHuMoment1Feature2<INPUT, OUTPUT>::centroid(Image<INPUT>& input, int n, float& cx, float& cy, Image<unsigned char>* support)
    {
        int tmpx, tmpy;

        float m00= 0, m10= 0, m01= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
                m10+= tmpx * input(n + *sit);
                m01+= tmpy * input(n + *sit);
            }
        }
        
        if ( m00 != 0 )
        {
            cx= m10/m00;
            cy= m01/m00;
        }
        else
        {
            cx= 0;
            cy= 0;
        }
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return M00(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return 0;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return 0;
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M11(input, n, support) - cx * M01(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M20(input, n, support) - cx * M10(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M02(input, n, support) - cy * M01(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M21(input, n, support) - 2 * cx * M11(input, n, support) - cy * M20(input, n, support) + 2 * cx * cx * M01(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M12(input, n, support) - 2 * cy * M11(input, n, support) - cx * M02(input, n, support) + 2 * cy * cy * M10(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M30(input, n, support) - 3 * cx * M20(input, n, support) + 2 * cx * cx * M10(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float cx, cy;
        centroid(input, n, cx, cy, support);
        
        return M03(input, n, support) - 3 * cy * M02(input, n, support) + 2 * cy * cy * M01(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return v00 / pow(v00, 1.0 + (0.0 + 0.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u10(input, n, support) / pow(v00, 1.0 + (1.0 + 0.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u01(input, n, support) / pow(v00, 1.0 + (0.0 + 1.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u11(input, n, support) / pow(v00, 1.0 + (1.0 + 1.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u20(input, n, support) / pow(v00, 1.0 + (2.0 + 0.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u02(input, n, support) / pow(v00, 1.0 + (0.0 + 2.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u21(input, n, support) / pow(v00, 1.0 + (2.0 + 1.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u12(input, n, support) / pow(v00, 1.0 + (1.0 + 2.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u30(input, n, support) / pow(v00, 1.0 + (3.0 + 0.0) / 2.0);
    }
    
    template<typename INPUT, typename OUTPUT>    
    float InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float v00= u00(input, n, support);
        return u03(input, n, support) / pow(v00, 1.0 + (0.0 + 3.0) / 2.0);
    }

    /**
     * Invariant Hu's Moment 2 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment2Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;
        

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment2Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* roi= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment2Feature2<INPUT, OUTPUT>::InvariantHuMoment2Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment2Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment2Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float a= n20(input, n, support) - n02(input, n, support);
        float b= 2 * n11(input, n, support);
        return a * a + b * b;
    }

    /**
     * Invariant Hu's Moment 3 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment3Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment3Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment3Feature2<INPUT, OUTPUT>::InvariantHuMoment3Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment3Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment3Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float a= n30(input, n, support) - 3 * n12(input, n, support);
        float b= 3 * n21(input, n, support) - n03(input, n, support);
        
        return a * a + b * b;
    }

    /**
     * Invariant Hu's Moment 4 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment4Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment4Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment4Feature2<INPUT, OUTPUT>::InvariantHuMoment4Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment4Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment4Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float a= n30(input, n, support) + n12(input, n, support);
        float b= n21(input, n, support) + n03(input, n, support);
        
        return a * a + b * b;
    }

    /**
     * Invariant Hu's Moment 5 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment5Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment5Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment5Feature2<INPUT, OUTPUT>::InvariantHuMoment5Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment5Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment5Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m30= n30(input, n, support);
        float m03= n03(input, n, support);
        float m21= n21(input, n, support);
        float m12= n12(input, n, support);
        
        return (m30 - 3*m12)*(m30 + m12)*((m30 + m12)*(m30 + m12) - 3*(m21 + m03)*(m21 + m03)) + (3*m21 - m03)*(m21 + m03)*(3*(m30 + m12)*(m30 + m12) - (m21 + m03)*(m21 + m03));
    }

    /**
     * Invariant Hu's Moment 6 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment6Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment6Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment6Feature2<INPUT, OUTPUT>::InvariantHuMoment6Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment6Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment6Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m20= n20(input, n, support);
        float m02= n02(input, n, support);
        float m30= n30(input, n, support);
        float m12= n12(input, n, support);
        float m21= n21(input, n, support);
        float m03= n03(input, n, support);
        float m11= n11(input, n, support);
        
        return (m20 - m02)*((m30 + m12)*(m30 + m12) - (m21 + m03)*(m21 + m03)) + 4 * m11 * (m30 + m12) * (m21 + m03);
    }

    /**
     * Invariant Hu's Moment 7 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment7Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment7Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment7Feature2<INPUT, OUTPUT>::InvariantHuMoment7Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment7Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment7Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m21= n21(input, n, support);
        float m03= n03(input, n, support);
        float m30= n30(input, n, support);
        float m12= n12(input, n, support);
        
        return (3*m21 - m03)*(m30 + m12)*((m30 + m12)*(m30 + m12) - 3 * (m21 + m03) * (m21 + m03)) - (m30 - 3*m12) * (m21 + m03) * (3 * (m30 + m12) * (m30 + m12) - (m21 + m03) * (m21 + m03));
    }

    /**
     * Invariant Hu's Moment 8 feature
     */
    template<typename INPUT, typename OUTPUT>
    class InvariantHuMoment8Feature2: public InvariantHuMoment1Feature2<INPUT, OUTPUT>
    {
    public:
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::descriptor;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMin;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getMax;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::stride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::updateStride;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::apply;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::getProposedBorder;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M22;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::M03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::u03;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n00;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n10;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n01;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n11;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n20;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n02;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n21;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n12;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n30;
        using InvariantHuMoment1Feature2<INPUT, OUTPUT>::n03;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        InvariantHuMoment8Feature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    InvariantHuMoment8Feature2<INPUT, OUTPUT>::InvariantHuMoment8Feature2(int r, int stride)
    : InvariantHuMoment1Feature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "InvariantHuMoment8Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT InvariantHuMoment8Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float m21= n21(input, n, support);
        float m03= n03(input, n, support);
        float m30= n30(input, n, support);
        float m12= n12(input, n, support);
        float m20= n20(input, n, support);
        float m02= n02(input, n, support);
        float m11= n11(input, n, support);
        
        return m11*((m30 + m12)*(m30 + m12) - (m03 + m21)*(m03 + m21)) - (m20 - m02)*(m30 + m12)*(m03 + m21);
    }
    
    /**
     * Eccentricity feature
     */
    template<typename INPUT, typename OUTPUT>
    class EccentricityFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        EccentricityFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    EccentricityFeature2<INPUT, OUTPUT>::EccentricityFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "EccentricityFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT EccentricityFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int tmpx, tmpy;
        float sum00= 0;
        float sum02= 0;
        float sum20= 0;
        float sum11= 0;

        float m00= 0;
        float m10= 0;
        float m01= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
                m10+= tmpx * input(n + *sit);
                m01+= tmpy * input(n + *sit);
            }
        }

        if ( fabs(m00) > 0.0001 )
        {
            m10/=m00;
            m01/=m00;
        }
        else
        {
            m10= 1.0;
            m01= 1.0;
        }

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                sum00+= input(n + *sit);
                sum20+= (tmpx - m10)*(tmpx - m10)*input(n + *sit);
                sum02+= (tmpy - m01)*(tmpy - m01)*input(n + *sit);
                sum11+= (tmpx - m10)*(tmpy - m01)*input(n + *sit);
            }
        }

        float n20= 0, n02= 0, n11= 0;
        if ( fabs(sum00) > 0.01 )
        {
            n20= sum20/sum00;
            n02= sum02/sum00;
            n11= sum11/sum00;
        }
        else
        {
            n20= 1.0;
            n02= 1.0;
            n11= 1.0;
        }

        if ( (n20 + n02)*(n20 + n02) < 4 * (n20*n02 - n11*n11) )
            return (OUTPUT)0;

        float l1= (n20 + n02 + sqrt((n20+n02)*(n20+n02) - 4*(n20*n02-n11*n11)))/2;
        float l2= (n20 + n02 - sqrt((n20+n02)*(n20+n02) - 4*(n20*n02-n11*n11)))/2;

        if ( fabs(l1) > 0.01 && l2/l1 < 1.0 )
            return (OUTPUT)(sqrt(1.0f - l2/l1));
        else
            return (OUTPUT)1.0f;
    }

    /**
     * Orientation feature
     */
    template<typename INPUT, typename OUTPUT>
    class OrientationFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        OrientationFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    OrientationFeature2<INPUT, OUTPUT>::OrientationFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "OrientationFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT OrientationFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int tmpx, tmpy;
        float m00= 0;
        float m10= 0;
        float m01= 0;
        float m11= 0;
        float m20= 0;
        float m02= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
                m10+= tmpx * input(n + *sit);
                m01+= tmpy * input(n + *sit);
                m11+= tmpx * tmpy * input(n + *sit);
                m20+= tmpx * tmpx * input(n + *sit);
                m02+= tmpy * tmpy * input(n + *sit);
            }
        }

        float n11= m11 - m10*m01/m00;
        float n20= m20 - m10*m10/m00;
        float n02= m02 - m01*m01/m00;

        if ( n20 != n02 )
            return (OUTPUT)((atan2(2*n11,(n20-n02)))/2.0);
        else
            return (OUTPUT)(0);
    }

    /**
     * Central Moment feature
     */
    template<typename INPUT, typename OUTPUT>
    class CentralMomentFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param p exponent of the x components
         * @param q exponent of the y components
         * @param stride stride of the images to apply to
         */
        CentralMomentFeature2(int r, float p= 1.0f, float q= 1.0f, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /** exponent of the x components */
        float p;
        /** exponent of the y components */
        float q;

        /** temporary data structure */
        vector<int> xCoord;
        /** temporary data structure */
        vector<int> yCoord;
    };

    template<typename INPUT, typename OUTPUT>
    CentralMomentFeature2<INPUT, OUTPUT>::CentralMomentFeature2(int r, float p, float q, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->p= p;
        this->q= q;

        std::stringstream ss;
        ss << "CentralMomentFeature2 " << r << " " << p << " " << q;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT CentralMomentFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        //float cx;
        //float cy;
        int tmpx, tmpy;
        float sum= 0;
        /*cx=*/ /*cy= 0;*/
        float m00= 0;
        float m10= 0;
        float m01= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                m00+= input(n + *sit);
                m10+= tmpx * input(n + *sit);
                m01+= tmpy * input(n + *sit);
            }
        }

        m10/=m00;
        m01/=m00;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                sum+= pow(tmpx - m10, p)*pow(tmpy - m01,q)*input(n + *sit);
            }
        }

        return (OUTPUT)(sum);
    }

    /**
     * Maximum intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class MaxFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MaxFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /**
         * computes the feature in the foreground (non-zero) regions of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region
         */
        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MaxFeature2<INPUT, OUTPUT>::MaxFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "MaxFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MaxFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMax(input, n, support));
    }

    template<typename INPUT, typename OUTPUT>
    void MaxFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        
        this->applyMax(input, output, roi, support);
    }

    /**
     * Minimum intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class MinFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MinFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MinFeature2<INPUT, OUTPUT>::MinFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "MinFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MinFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMin(input, n, support));
    }

    /**
     * Median intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class MedianFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMedian;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;
#ifdef USE_OPENCL        
        using StatisticalFeature2<INPUT, OUTPUT>::enqueueKernel;
        using StatisticalFeature2<INPUT, OUTPUT>::getOpenCLKernel;
        using StatisticalFeature2<INPUT, OUTPUT>::applyGPU;
#endif
        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MedianFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
        
        virtual void enqueueKernel(cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, Image<INPUT>& input);
#endif        
    };
    

    template<typename INPUT, typename OUTPUT>
    MedianFeature2<INPUT, OUTPUT>::MedianFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "MedianFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MedianFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMedian(input, n, support));
    }

#ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT>
    std::string MedianFeature2<INPUT, OUTPUT>::getOpenCLKernel()
    {
        std::string kernel;
        std::stringstream ss;
        ss.clear();

        ss << this->min;

        kernel= string("__kernel void convolution(const __global ") + type(typeid(INPUT)) + string("* input, ") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output)\n")
                + string("{\n"
                "\tconst int idx= get_global_id(0) - (") + ss.str() + string(");\n"
                "\tint i;\n\tint j;\n\t") + type(typeid(INPUT)) + string(" tmp;\n");

        ss.str("");
        ss << this->size();
        kernel+= string("\t__local ") + type(typeid(INPUT)) + string(" array[") + ss.str() + string("];\n");
        ss.str("");
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
             ss << "\tarray[";
            ss << i;
            if ( (*this)(i) >= 0 )
                ss << "]= input[ idx +";
            else
                ss << "]= input[ idx";
            ss << (*this)(i);
            ss << "];\n";
        }
        kernel+= ss.str();

        ss.str("");
        ss << "\tfor ( i= 1; i < ";
        ss << this->size()/2 + 1;
        ss << "; ++i)\n";
        ss << "\t\tfor ( j= ";
        ss << this->size() - 1;
        ss << "; j > i; --j )\n";
        ss << "\t\t\tif ( array[j] < array[j-1] )\n";
        ss << "\t\t\t{\n";
        ss << "\t\t\t\ttmp= array[j];\n";
        ss << "\t\t\t\tarray[j]= array[j-1];\n";
        ss << "\t\t\t\tarray[j-1]= tmp;\n";
        ss << "\t\t\t}\n";
        
        kernel+= ss.str();
        kernel+= string("\toutput[idx]= (")+ type(typeid(OUTPUT))+string(")array[");
        ss.str("");
        ss << this->size()/2;
        kernel+= ss.str();
        kernel+= string("];\n");
        kernel+= string("}\n");

        return kernel;
    }
    
    template<typename INPUT, typename OUTPUT>
    void MedianFeature2<INPUT, OUTPUT>::enqueueKernel(cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, Image<INPUT>& input)
    {
        printf("medianfeature2 enqueuekernel\n"); fflush(stdout);
        cl_int ciErrNum;
        ciErrNum= clSetKernelArg(*k, 0, sizeof(cl_mem), bufin);
        ciErrNum= clSetKernelArg(*k, 1, sizeof(cl_mem), bufout);

        size_t szLocalWorkSize;
        size_t szGlobalWorkSize;

        szLocalWorkSize= 1;
        szGlobalWorkSize= input.n + this->getMin() + this->getMin();
        printf("%d %d\n", int(input.n), int(szGlobalWorkSize)); fflush(stdout);
        printf("global work size %d %d\n", int(szGlobalWorkSize), int(input.n)); fflush(stdout);

        printf("enqueue kernel\n"); fflush(stdout);
        ciErrNum= clEnqueueNDRangeKernel(*cq, *k, 1, NULL, &szGlobalWorkSize, &szLocalWorkSize, 0, NULL, NULL);
        printf("ndrangekernel %d\n", ciErrNum);
    }
#endif    
    
    /**
     * Mean intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class MeanFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMean;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;
        using StatisticalFeature2<INPUT, OUTPUT>::getCoordinate2D;
#ifdef USE_OPENCL        
        using StatisticalFeature2<INPUT, OUTPUT>::enqueueKernel;
        using StatisticalFeature2<INPUT, OUTPUT>::getOpenCLKernel;
        using StatisticalFeature2<INPUT, OUTPUT>::applyGPU;
#endif
        
        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MeanFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

#ifdef USE_OPENCL
        virtual std::string getOpenCLKernel();
        
        virtual void enqueueKernel(cl_context* cc, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input);
#endif         
        
        /** radius of the feature*/
        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    MeanFeature2<INPUT, OUTPUT>::MeanFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride), radius(r)
    {
        std::stringstream ss;
        ss << "MeanFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MeanFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        if ( radius <= 0 )
            return (OUTPUT)(input(n));
        else
            return (OUTPUT)(this->getMean(input, n, support));
    }

#ifdef USE_OPENCL
    template<typename INPUT, typename OUTPUT>
    std::string MeanFeature2<INPUT, OUTPUT>::getOpenCLKernel()
    {
        std::string kernel;
        std::stringstream ss;
        ss.clear();

        ss << this->min;

        /*kernel= string("__kernel void convolution(const __global ") + type(typeid(INPUT)) + string("* input, ") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output, const __global unsigned char* roi)\n")
                + string("{\n"
                "\tconst int idx= get_global_id(0) * 211 % get_global_size(0) - (") + ss.str() + string(");\n"
                "\tint i; float") + string(" tmp= 0;\n");*/
        kernel= string("__kernel void convolution(__read_only image2d_t input,") +
                string("__global ") + type(typeid(OUTPUT)) + string("* output, __read_only image2d_t roi)\n")
                + string("{\n"
                //"\tconst int idx= get_global_id(0) * 211 % get_global_size(0) - (") + ss.str() + string(");\n"
                "\tconst int idx= get_global_id(0) - (") + ss.str() + string(");\n"
                "\tint i; float") + string(" tmp= 0;\n");

        ss.str("");
        /*ss << "if ( roi[idx] )\n";
        ss << "\tfor( i= 0; i < " << this->size() << "; ++i )\n";
        ss << "\t\ttmp+= input[idx + positions[i]];\n";*/
        
        ss << "const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;\n";
        ss << "int r, c, n= 0;\n";
        kernel+= ss.str();
        ss.str("");
        
        /*ss << "if ( roi[idx] )\n{\n";
        for ( int i= 0; i < this->size(); ++i )
            ss << "tmp+= " << "input[idx+" << (*this)(i) << "];\n";
        ss << "}\n";*/
        
        int rows, columns;
        //ss << "int2 coord;\n";
        //ss << "float4 x;\n";
        
        
        ss << "r= idx / " << this->stride << ";\n";
        ss << "c= idx % " << this->stride << ";\n";
        ss << "if ( read_imagei(roi, smp, (int2)(c, r)).x > 0 )\n{\n";
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            this->getCoordinate2D((*this)(i), rows, columns);
            /*ss << "\tcoord= (int2)(c ";
            if ( rows >= 0 )
                ss << "+";
                ss << rows << ", r ";
             if ( columns >= 0 )
                 ss << "+";
                ss << columns << ");\n";*/
            ss << "if ( read_imagei(roi, smp, (int2)(c + (" << columns << "), r + (" <<rows << "))).x )\n";
            ss << "{\n";
            ss << "\ttmp+= read_imagef(input, smp, (int2)(c + (" << columns << "), r + (" << rows << "))).x;\n";
            ss << "++n;\n";
            ss << "}\n";
            //ss << "\ttmp+= x.s0;\n";
        }
        ss << "}\n";
        
        kernel+= ss.str();
        
        ss.str("");
        ss << string("\ttmp/= ");
        ss << this->size();
        ss << string(";\n");
        
        kernel+= ss.str();

        kernel+= string("\toutput[idx]= tmp;\n");
        kernel+= string("}\n");

        return kernel;
    }

    template<typename INPUT, typename OUTPUT>
    void MeanFeature2<INPUT, OUTPUT>::enqueueKernel(cl_context*, cl_command_queue* cq, cl_kernel* k, void* bufin, void* bufout, void* bufroi, Image<INPUT>& input)
    {
        printf("medianfeature2 enqueuekernel\n"); fflush(stdout);
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
        printf("global work size %d %d\n", int(szGlobalWorkSize), int(input.n)); fflush(stdout);

        printf("enqueue kernel\n"); fflush(stdout);
        ciErrNum= clEnqueueNDRangeKernel(*cq, *k, 1, NULL, &szGlobalWorkSize, NULL, 0, NULL, NULL);
        printf("ndrangekernel %d\n", ciErrNum);
    }
#endif    
    
    /**
     * Variance intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class VarianceFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        VarianceFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        VarianceFeature2(const VarianceFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VarianceFeature2<INPUT, OUTPUT>::VarianceFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "VarianceFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    VarianceFeature2<INPUT, OUTPUT>::VarianceFeature2(const VarianceFeature2<INPUT, OUTPUT>& vf)
    : StatisticalFeature2<INPUT, OUTPUT>(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT VarianceFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getVariance(input, n, support));
    }

    /**
     * Standard Deviation intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class StandardDeviationFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        StandardDeviationFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        StandardDeviationFeature2(const StandardDeviationFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    StandardDeviationFeature2<INPUT, OUTPUT>::StandardDeviationFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "StandardDeviationFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    StandardDeviationFeature2<INPUT, OUTPUT>::StandardDeviationFeature2(const StandardDeviationFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT StandardDeviationFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getStandardDeviation(input, n, support));
    }
    
    template<typename INPUT, typename OUTPUT>
    class TotalVariationFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        TotalVariationFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        TotalVariationFeature2(const TotalVariationFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    TotalVariationFeature2<INPUT, OUTPUT>::TotalVariationFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "TotalVariationFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    TotalVariationFeature2<INPUT, OUTPUT>::TotalVariationFeature2(const TotalVariationFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT TotalVariationFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
      float tv= 0;
      int nn= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( !support || (n + (*this)(i) + 1 < int(input.n) && (*support)(n + (*this)(i) + 1)) )
	{
	  tv+= fabs(input(n + (*this)(i)) - input(n + (*this)(i)+1));
	  ++nn;
	}
	if ( !support || (n + (*this)(i) + input.columns < int(input.n) && (*support)(n + (*this)(i) + input.columns)) )
	{
	  tv+= fabs(input(n + (*this)(i)) - input(n + (*this)(i)+input.columns));
	  ++nn;
	}
      }
      return tv/nn;
    }

    /**
     * Variance intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class SkewnessFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getSkewness;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        SkewnessFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        SkewnessFeature2(const SkewnessFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    SkewnessFeature2<INPUT, OUTPUT>::SkewnessFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "SkewnessFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    SkewnessFeature2<INPUT, OUTPUT>::SkewnessFeature2(const SkewnessFeature2<INPUT, OUTPUT>& vf)
    : StatisticalFeature2<INPUT, OUTPUT>(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT SkewnessFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getSkewness(input, n, support));
    }

    template<typename INPUT, typename OUTPUT>
    class KurtosisFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getKurtosis;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        KurtosisFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        KurtosisFeature2(const KurtosisFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    KurtosisFeature2<INPUT, OUTPUT>::KurtosisFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "KurtosisFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    KurtosisFeature2<INPUT, OUTPUT>::KurtosisFeature2(const KurtosisFeature2<INPUT, OUTPUT>& vf)
    : StatisticalFeature2<INPUT, OUTPUT>(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT KurtosisFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getKurtosis(input, n, support));
    }

    /**
     * Standard Deviation intensity feature
     */
    template<typename INPUT, typename OUTPUT>
    class NormalizationFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        NormalizationFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        NormalizationFeature2(const NormalizationFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    NormalizationFeature2<INPUT, OUTPUT>::NormalizationFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "NormalizationFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationFeature2<INPUT, OUTPUT>::NormalizationFeature2(const NormalizationFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT NormalizationFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float mean;
        float variance;
        this->getMeanAndVariance(input, n, mean, variance, support);
        variance= sqrt(variance);
        if ( fabs(variance) > 0.001 )
            return (input(n) - mean)/(variance);
        else
            return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class NormalizationInto01Feature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        NormalizationInto01Feature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        NormalizationInto01Feature2(const NormalizationInto01Feature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    NormalizationInto01Feature2<INPUT, OUTPUT>::NormalizationInto01Feature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "NormalizationInto01Feature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    NormalizationInto01Feature2<INPUT, OUTPUT>::NormalizationInto01Feature2(const NormalizationInto01Feature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT NormalizationInto01Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        INPUT min, max;
        this->getMinMax(input, n, min, max, support);
        if ( min == max )
            return 0;
        else
            return (input(n) - min)/float((max - min));
    }
    
    template<typename INPUT, typename OUTPUT>
    class GreaterThanFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        GreaterThanFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        GreaterThanFeature2(const GreaterThanFeature2<INPUT, OUTPUT>& vf);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    GreaterThanFeature2<INPUT, OUTPUT>::GreaterThanFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "GreaterThanFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GreaterThanFeature2<INPUT, OUTPUT>::GreaterThanFeature2(const GreaterThanFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GreaterThanFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float sum= 0;
        
        if ( support )
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
            {
                if ( input(n) >= input(n + *it) )
                    sum+= 1.0f;
            }
        else
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
                if ( input(n) >= input(n + *it) )
                    sum+= 1.0f;
            
        return sum/this->size();
    }
    
    template<typename INPUT, typename OUTPUT>
    class AFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        AFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        AFeature2(const AFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
    };

    template<typename INPUT, typename OUTPUT>
    AFeature2<INPUT, OUTPUT>::AFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "AFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AFeature2<INPUT, OUTPUT>::AFeature2(const AFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void AFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT AFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float sum= 0;
        float num= this->size();
        
        if ( support )
        {
            num= 0;
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
            {
                if ( (*support)(n + *it) )
                {
                    if ( input(n) >= input(n + *it) )
                        sum+= 1.0f;
                    ++num;
                }
            }
        }
        else
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
                if ( input(n) >= input(n + *it) )
                    sum+= 1.0f;
            
        return (sum/num) * ((input(n) - minTmp) / (maxTmp - minTmp));
    }
    
    template<typename INPUT, typename OUTPUT>
    class BFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        BFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        BFeature2(const BFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
    };

    template<typename INPUT, typename OUTPUT>
    BFeature2<INPUT, OUTPUT>::BFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "BFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    BFeature2<INPUT, OUTPUT>::BFeature2(const BFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void BFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp, roi);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT BFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float sum= 0;
        float num= this->size();
        
        this->getMinMax(input, n, minTmp, maxTmp, support);
        
        if ( support )
        {
            num= 0;
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
            {
                if ( (*support)(n + *it) )
                {
                    if ( input(n) >= input(n + *it) )
                        sum+= 1.0f;
                    ++num;
                }
            }
        }
        else
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
                if ( input(n) >= input(n + *it) )
                    sum+= 1.0f;
            
        return (sum/num) * ((input(n) - minTmp) / (maxTmp - minTmp));
    }
    
    template<typename INPUT, typename OUTPUT>
    class CFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        CFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        CFeature2(const CFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
    };

    template<typename INPUT, typename OUTPUT>
    CFeature2<INPUT, OUTPUT>::CFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "CFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CFeature2<INPUT, OUTPUT>::CFeature2(const CFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void CFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT CFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float sum= 0;
        float num= this->size();
        
        if ( support )
        {
            num= 0;
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
            {
                if ( (*support)(n + *it) )
                {
                    if ( input(n) >= input(n + *it) )
                        sum+= 1.0f;
                    ++num;
                }
            }
        }
        else
            for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
                if ( input(n) >= input(n + *it) )
                    sum+= 1.0f;
            
        return (sum/num) * ((input(n) - minTmp) / (maxTmp - minTmp)) * this->getStandardDeviation(input, n, support);
    }
    
    template<typename INPUT, typename OUTPUT>
    class DFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        DFeature2(int r, float weight, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        DFeature2(const DFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
        INPUT meanTmp;
        float weight;
    };

    template<typename INPUT, typename OUTPUT>
    DFeature2<INPUT, OUTPUT>::DFeature2(int r, float weight, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->weight= weight;
        std::stringstream ss;
        ss << "DFeature2 " << r << " " << weight;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DFeature2<INPUT, OUTPUT>::DFeature2(const DFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void DFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp, roi);
        meanTmp= input.getMean(roi);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT DFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        INPUT minL, maxL;
        this->getMinMax(input, n, minL, maxL, support);
        //meanL= this->getMean(input, n, support);

        float threshold= minL + (maxL - minL)*weight;
        
        if ( input(n) > threshold )
            return input(n) * (maxL - input(n));
        else
            return input(n) * (input(n) - minL);
    }
    
    template<typename INPUT, typename OUTPUT>
    class EFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        EFeature2(int r, float weight, float weight2, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        EFeature2(const EFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
        INPUT meanTmp;
        float weight;
        float weight2;
    };

    template<typename INPUT, typename OUTPUT>
    EFeature2<INPUT, OUTPUT>::EFeature2(int r, float weight, float weight2, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->weight= weight;
        this->weight2= weight2;
        std::stringstream ss;
        ss << "EFeature2 " << r << " " << weight << " " << weight2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    EFeature2<INPUT, OUTPUT>::EFeature2(const EFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void EFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp, roi);
        meanTmp= input.getMean(roi);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT EFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
        //INPUT minL, maxL, meanL;
        //this->getMinMax(input, n, minL, maxL, support);

        float threshold= minTmp + (maxTmp - minTmp)*weight;
        
        float num= 0;
        for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
        {
            if ( input(n + *it) < input(n) )
                num+= 1;
        }
        
        if ( input(n) > threshold )
            return input(n) * (1.0 + weight2 *num);
        else
            return input(n) * (1.0 - weight2 *num);
    }
    
    template<typename INPUT, typename OUTPUT>
    class FFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        FFeature2(int r, float weight, float weight2, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        FFeature2(const FFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
        INPUT meanTmp;
        float weight;
        float weight2;
    };

    template<typename INPUT, typename OUTPUT>
    FFeature2<INPUT, OUTPUT>::FFeature2(int r, float weight, float weight2, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        this->weight= weight;
        this->weight2= weight2;
        std::stringstream ss;
        ss << "FFeature2 " << r << " " << weight << " " << weight2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FFeature2<INPUT, OUTPUT>::FFeature2(const FFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void FFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp);
        meanTmp= input.getMean(roi);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT FFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        //INPUT minL, maxL, meanL;
        //this->getMinMax(input, n, minL, maxL, support);

        //float threshold= minTmp + (maxTmp - minTmp)*weight;
        
        //float num= 0;
        //int pos= 0;
        /*for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
        {
            if ( input(n + *it) > input(n + pos) )
                pos= *it;
        }*/
        
        return input(n) * this->getStandardDeviation(input, n, support); 
    }
    
    template<typename INPUT, typename OUTPUT>
    class GFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        GFeature2(int r, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        GFeature2(const GFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
        
        INPUT minTmp;
        INPUT maxTmp;
        INPUT meanTmp;
    };

    template<typename INPUT, typename OUTPUT>
    GFeature2<INPUT, OUTPUT>::GFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "GFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GFeature2<INPUT, OUTPUT>::GFeature2(const GFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void GFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp, roi);
        meanTmp= input.getMean(roi);
        this->updateStride(input.columns);
        
        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        INPUT minL;
        minL= this->getMin(input, n, support);

        return minL;
    }

    template<typename INPUT, typename OUTPUT>
    class HFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getStandardDeviation;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        HFeature2(int r, float tmp, int stride= 4000);

        /**
         * copy constructor
         * @param vf instance to copy
         */
        HFeature2(const HFeature2<INPUT, OUTPUT>& vf);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        INPUT minTmp;
        INPUT maxTmp;
        INPUT meanTmp;
        float tmp;
    };

    template<typename INPUT, typename OUTPUT>
    HFeature2<INPUT, OUTPUT>::HFeature2(int r, float tmp, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "HFeature2 " << r << " " << tmp;
        descriptor= ss.str();
        this->tmp= tmp;
    }

    template<typename INPUT, typename OUTPUT>
    HFeature2<INPUT, OUTPUT>::HFeature2(const HFeature2<INPUT, OUTPUT>& vf)
    : Feature2<INPUT, OUTPUT>(vf), StructuringElementDisk(vf)
    {
        this->tmp= vf.tmp;
    }

    template<typename INPUT, typename OUTPUT>
    void HFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        input.getMinMax(minTmp, maxTmp, roi);
        meanTmp= input.getMean(roi);
        this->updateStride(input.columns);

        if ( roi == NULL )
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                output(i)= apply(input, i, support);
            }
        }
        else
        {
            for ( unsigned int i= -this->getMin(); i < input.n - this->getMax(); ++i )
            {
                if ( (*roi)(i) )
                    output(i)= apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT HFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
        OUTPUT out= 0;

        for ( StructuringElementDisk::iterator it= this->begin(); it != this->end(); ++it )
        {
            if ( input(n + *it) > input(n)*tmp )
                ++out;
        }

        return float(out)/float(this->size());
    }

    template<typename INPUT, typename OUTPUT>
    class DistanceOfCenterOfGravityFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        DistanceOfCenterOfGravityFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    DistanceOfCenterOfGravityFeature2<INPUT, OUTPUT>::DistanceOfCenterOfGravityFeature2(int r, int stride)
    : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "DistanceOfCenterOfGravityFeature2 " << r;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT DistanceOfCenterOfGravityFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int tmpx, tmpy;
        float cx= 0, cy= 0;
        float m= 0;

        for ( StructuringElementDisk::iterator sit= this->begin(); sit != this->end(); ++sit )
        {
            if ( support == NULL || (*support)(n + *sit) > 0 )
            {
                this->getXY(*sit, tmpy, tmpx);
                cx+= tmpx*input(n + *sit);
                cy+= tmpy*input(n + *sit);
                m+= input(n + *sit);
            }
        }
        cx/= m;
        cy/= m;
        return sqrt(cx*cx + cy*cy);
    }

    template<typename INPUT, typename OUTPUT>
    class IdenticalFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        IdenticalFeature2(int r, int c, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        int r, c;
    };

    template<typename INPUT, typename OUTPUT>
    IdenticalFeature2<INPUT, OUTPUT>::IdenticalFeature2(int r, int c, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(abs(r) + abs(c), stride)
    {
        this->r= r;
        this->c= c;

        std::stringstream ss;
        ss << "IdenticalFeature2 " << r << " " << c << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT IdenticalFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
        return input(n + r*input.columns + c);
    }
}

#endif	/* _STATISTICALFILTER2S_H */

