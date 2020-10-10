/**
 * @file StructuringElement2.h
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
 * The StructuringElement2 class represents a structuring element, that is
 * a set of pixels relative to a center pixel.
 */

#ifndef _STRUCTURING_ELEMENT2_H_
#define _STRUCTURING_ELEMENT2_H_

#include <vector>
#include <algorithm>

#include <openipDS/Image.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/PixelSet2.h>

using namespace std;

namespace openip
{
    /**
     * StructuringElement2 represents a structuring element, that is a set
     * of pixels relative to a center pixel
     */
    class StructuringElement2 : public PixelSet1
    {
    public:
        using PixelSet1::set;
        using PixelSet1::translate;
        using PixelSet1::operator=;
        using PixelSet1::operator+=;
        using PixelSet1::updateStride;
        using PixelSet1::getMin;
        using PixelSet1::getMax;
        using PixelSet1::computeMinMax;

        /**
         * default constructor
         */
        StructuringElement2(int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        StructuringElement2(const StructuringElement2& s);
        
        /**
         * destructor
         */
        virtual ~StructuringElement2();
        
        StructuringElement2& operator=(const StructuringElement2& s);

        /**
         * computes the row and column coordinates of a row-major coordinate
         * @param n row-major coordinate
         * @param row output parameter, row coordinate
         * @param column output parameter, column coordinate
         */
        virtual void getXY(int n, int& row, int& column);

        virtual void rotate(float alpha);
        /**
         * computes the sum of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applySum(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the max of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMax(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the min of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMin(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the mean of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMean(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the variance of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyVariance(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the standard deviation of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyStandardDeviation(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the median of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMedian(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param s StructuringElement2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const StructuringElement2& s);

    class StructuringElement22: public PixelSet2
    {
    public:
        StructuringElement22();

        StructuringElement22(StructuringElement2& se);

        StructuringElement22(const StructuringElement22& se);

        ~StructuringElement22();
    };
}

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applySum(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getSum(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getSum(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyMax(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMax(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMax(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyMin(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMin(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMin(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyMean(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMean(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMean(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyVariance(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getVariance(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getVariance(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyStandardDeviation(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getStandardDeviation(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getStandardDeviation(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement2::applyMedian(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(in.columns);
        
        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMedian(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMedian(in, i, support));
    }
}

#endif
