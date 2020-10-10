/**
 * @file PixelSequence.h
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
 * The PixelSequence class represents a sequence of pixels to represent
 * contours in an image.
 */

#ifndef _PIXELSEQUENCE_H
#define	_PIXELSEQUENCE_H

#include <openipDS/Pixel.h>
#include <openipDS/Image.h>
#include <openipDS/Vector.h>

namespace openip
{
    /**
     * PixelSequence represents a sequence of pixels to represent contours
     * of objects in an image
     */
    template<typename P>
    class PixelSequence : public Vector<P>
    {
    public:
        /**
         * PixelSequence<P>::iterator typedef
         */
        typedef typename PixelSequence<P>::iterator pIt;

        /**
         * PixelSequence<P>::const_iterator typedef
         */
        typedef typename PixelSequence<P>::const_iterator const_pIt;
        
        /**
         * default constructor
         */
        PixelSequence();

        /**
         * copy constructor
         * @param s instance to copy
         */
        PixelSequence(const PixelSequence& s);

        /**
         * destructor
         */
        ~PixelSequence();

        /**
         * assignment operator
         * @param s the object to copy
         * @return this
         */
        PixelSequence& operator=(const PixelSequence& s);

        /**
         * the size of the bounding square of the pixels
         * @return the size of the bounding square
         */
        int getBoundingSquareSize();

        /**
         * get the width of the pixels in the pixelsequence
         * @return the width of the pixels
         */
        int getWidth();

        /**
         * get the height of the pixels in the pixelsequence
         * @return the height of the pixels
         */
        int getHeight();

        /**
         * computes minX, maxX, minY, maxY values
         * @param stride stride of the image
         */
        virtual void computeMinMaxXY(int stride)= 0;

        /**
         * computes min, max, minX, maxX, minY, maxY values
         * @param stride stride of the image
         */
        virtual void computeMinMax(int stride)= 0;

        /**
         * computes min, max values only
         * @param stride stride of the image
         */
        virtual void computeMinMaxOnly(int stride)= 0;

        /**
         * compute center of gravity in two coordinate representation
         * @param stride, stride of pixels
         * @return the center of gravity
         */
        P getCenterOfGravity(int stride);

        /**
         * get sum of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute sum in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return sum of the values
         */
        template<typename T>
        float getSum(Image<T>& b, P n);

        /**
         * get max of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute max in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return max of the values
         */
        template<typename T>
        T getMax(Image<T>& b, P n);

        /**
         * get min of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute min in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return min of the values
         */
        template<typename T>
        T getMin(Image<T>& b, P n);

        /**
         * get mean of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute mean in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return return mean of the values
         */
        template<typename T>
        float getMean(Image<T>& b, P n);

        /**
         * get variance of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Image<T>& b, P n);

        /**
         * get variance of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation, where the mean is pre-computed
         * @param b image to compute variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param mean pre-computed mean
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Image<T>& b, P n, float mean);

        /**
         * get standard deviation of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute standard deviation in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return return standard deviation of the values
         */
        template<typename T>
        float getStandardDeviation(Image<T>& b, P n);

        /**
         * get median of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute median in
         * @param n coordinate, the pixels in the set will be relative to this
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Image<T>& b, P n);

        /**
         * get sum of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute sum in
         * @return return sum of the values
         */
        template<typename T>
        float getSum(Image<T>& b);

        /**
         * get max of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute max in
         * @return return max of the values
         */
        template<typename T>
        T getMax(Image<T>& b);

        /**
         * get min of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute min in
         * @return return min of the values
         */
        template<typename T>
        T getMin(Image<T>& b);

        /**
         * get mean of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute mean in
         * @return return mean of the values
         */
        template<typename T>
        float getMean(Image<T>& b);

        /**
         * get variance of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute variance in
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Image<T>& b);

        /**
         * get standard deviation of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute standard deviation in
         * @return return standard deviation of the values
         */
        template<typename T>
        float getStandardDeviation(Image<T>& b);

        /**
         * get median of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute median in
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Image<T>& b);

        /**
         * minimum x coordinate
         */
        int minX;

        /**
         * maximum x coordinate
         */
        int maxX;

        /**
         * minimum y coordinate
         */
        int minY;

        /**
         * maximum y coordinate
         */
        int maxY;

        /**
         * minimum element in row-continuous representation
         */
        int min;

        /**
         * maximum element in row-continuous representation
         */
        int max;
    };

    template<typename P>
    PixelSequence<P>::PixelSequence()
    : Vector<P>()
    {
    }

    template<typename P>
    PixelSequence<P>::PixelSequence(const PixelSequence<P>& s)
    : Vector<P>(s)
    {
    }

    template<typename P>
    PixelSequence<P>::~PixelSequence()
    {
    }

    template<typename P>
    PixelSequence<P>& PixelSequence<P>::operator=(const PixelSequence<P>& s)
    {
        this->vector<int>::operator=(s);
        return *this;
    }

    template<typename P>
    int PixelSequence<P>::getWidth()
    {
        return maxX - minX;
    }

    template<typename P>
    int PixelSequence<P>::getHeight()
    {
        return maxY - minY;
    }

    template<typename P>
    P PixelSequence<P>::getCenterOfGravity(int stride)
    {
        P sum= 0;

        typename PixelSequence<P>::pIt psit= this->begin();

        while( psit != this->end() )
        {
            sum+= *psit;

            psit++;
        }

        sum/= this->size();
        return sum;
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getSum(Image<T>& b, P n)
    {
        float f= 0;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMax(Image<T>& b, P n)
    {
        float f= b(0);

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(n + *psit) > f )
                        f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMin(Image<T>& b, P n)
    {
        float f= b(0);

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(n + *psit) < f )
                        f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getMean(Image<T>& b, P n)
    {
        float f= 0;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                        f+= b(n + *psit);
        }

        f/= this->size();

        return f;
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getVariance(Image<T>& b, P n)
    {
        float f= b(0);
        float v= 0;
        float tmp;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                        f+= b(n + *psit);

        f/= this->size();

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                tmp= f - b(n + *psit);
                v+= tmp * tmp;
        }

        return v / this->size();
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getVariance(Image<T>& b, P n, float mean)
    {
        float v= 0;
        float tmp;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                tmp= mean - b(n + *psit);
                v+= tmp * tmp;
        }

        return v / this->size();
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getStandardDeviation(Image<T>& b, P n)
    {
        return sqrt(getVariance(b, n));
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMedian(Image<T>& input, P n)
    {
        vector<T> a;
        a.resize(this->size());

        int j= 0;
        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                a[j++]= input.operator()(n + *psit);

        sort(a.begin(), a.end());
        return a[this->size()/2];
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getSum(Image<T>& b)
    {
        float f= 0;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMax(Image<T>& b)
    {
        float f= b(0);

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) > f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMin(Image<T>& b)
    {
        float f= b(0);

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) < f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getMean(Image<T>& b)
    {
        float f= 0;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            if ( *psit > 0 && *psit < b.n )
                        f+= b(*psit);
        }

        f/= this->size();

        return f;
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getVariance(Image<T>& b)
    {
        float f= b(0);
        float v= 0;
        float tmp;

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                        f+= b(*psit);

        f/= this->size();

        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                tmp= b(*psit);
                v+= (f - tmp) * (f - tmp);
        }

        return v / this->size();
    }

    template<typename P> template<typename T>
    float PixelSequence<P>::getStandardDeviation(Image<T>& b)
    {
        return sqrt(getVariance(b));
    }

    template<typename P> template<typename T>
    T PixelSequence<P>::getMedian(Image<T>& input)
    {
        vector<T> a;
        a.resize(this->size());

        int j= 0;
        for ( typename PixelSequence<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                a[j++]= input.operator()(*psit);

        sort(a.begin(), a.end());
        return a[this->size()/2];
    }

    /**
     * prints the description of the parameter PixelSequence object to the parameter output stream
     * @param o parameter output stream
     * @param p PixelSequence object to describe
     * @return reference to the parameter output stream
     */
    template<typename P>
    std::ostream& operator<<(std::ostream& o, const PixelSequence<P>& p)
    {
        o << "[PixelSequence - size: ";
        o << p.size();
        o << " elements: ";
        for ( typename PixelSequence<P>::const_pIt it = p.begin(); it != p.end(); ++it )
        {
            o << *it;
            o << " ";
        }
        o << "]";

        return o;
    }
}

#endif	/* _PIXELSEQUENCE_H */

