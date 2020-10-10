/**
 * @file PixelSet.h
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
 * The PixelSet class represents a set of pixels to represent patches and
 * objects in an image.
 */

#ifndef _PIXELSET_H
#define	_PIXELSET_H

#include <openipDS/Pixel.h>
#include <openipDS/Set.h>
#include <openipDS/Image.h>

#include <limits.h>
using namespace std;

namespace openip
{
    /**
     * PixelSet represents a set of pixels from Pixel type P
     */
    template<typename P>
    class PixelSet : public Set<P>
    {
    public:
        /**
         * PixelSet::iterator typedef
         */
        typedef typename PixelSet::iterator pIt;

        /**
         * PixelSet::const_iterator typedef
         */
        typedef typename PixelSet::const_iterator const_pIt;

        /**
         * default constructor
         */
        PixelSet();

        /**
         * copy constructor
         */
        PixelSet(const PixelSet& s);
	
	~PixelSet();

        /**
         * assignment operator
         * @param s the object to copy
         * @return this
         */
        PixelSet& operator=(const PixelSet& s);

        /**
         * union operator, adds the elements of s to this Set
         * @param s the Set whose elements are added to this
         * @return this
         */
        PixelSet& operator+=(const PixelSet& s);

        /**
         * intersetion operator, deletes the elements of this which are not
         * present in s
         * @param s the Set to intersect with
         * @return this
         */
        PixelSet& operator*=(const PixelSet& s);

        /**
         * subtract operator, deletes the elements of Set s from this
         * @param s the Set whose elements are deleted from this
         * @return this
         */
        PixelSet& operator/=(const PixelSet& s);

        /**
         * union operator, union with this and s, returns a new instance
         * @param s Set to union with
         * @return new Set instance
         */
        PixelSet operator+(const PixelSet& s) const;

        /**
         * intersection operator, intersection with this and s, returns a new
         * instance
         * @param s Set to intersect with
         * @return new Set instance
         */
        PixelSet operator*(const PixelSet& s) const;

        /**
         * subtract operator, deletes the elements of Set s from this and
         * returns a new instance
         * @param s the Set whose elements are deleted from this
         * @return a new Set instance
         */
        PixelSet operator/(const PixelSet& s) const;

        /**
         * bounding box size of the pixel set
         * @return the size of the bounding box
         */
        int getBoundingBoxSize();

        /**
         * width of bounding box of the pixel set
         * @return width of the bounding box
         */
        int getWidth();

        /**
         * height of bounding box of the pixel set
         * @return height of the bounding box
         */
        int getHeight();

        /**
         * returns the minimum element of the set in row-major representation
         * @return the minimum element
         */
        virtual int getMin();

        /**
         * returns the maximum element of the set in row-major representation
         * @return the maximum element
         */
        virtual int getMax();

        /**
         * computes the min, max, minX, minY, maxX, maxY values
         */
        virtual void computeMinMax();

        /**
         * computes minX, minY, maxX, maxY values
         */
        virtual void computeMinMaxXY();

        /**
         * computes min and max values
         */
        virtual void computeMinMaxOnly();

        /**
         * the center of gravity of pixels in the Set
         * @return the center of gravity
         */
        P getCenterOfGravity();

        /**
         * checks wether the instance is overlapping with s
         * @param s to check the overlapping with
         * @return true if overlapping; false if non-overlapping
         */
        bool isOverlapping(const PixelSet<P>& s) const;

        /**
         * sets the elements of b in the positions of the pixels in set to value
         * @param b the image to set the pixels of
         * @param value the value to set the pixels to
         */
        template<typename T>
        void set(Image<T>& b, T value= 255);

        /**
         * sets the elements of b in the positions of the pixels in this
         * set relative to n to value
         * @param b the image to set the pixels of
         * @param n base pixel
         * @param value the value to set the pixels to
         */
        template<typename T>
        void set(Image<T>& b, P n, T value= 255);

        /**
         * translate the pixels in the set with n
         * @param n the value to translate with
         */
        void translate(P n);

        /**
         * get sum of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute sum in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return sum of the values
         */
        template<typename T>
        float getSum(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get max of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute max in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return max of the values
         */
        template<typename T>
        T getMax(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get min of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute min in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return min of the values
         */
        template<typename T>
        T getMin(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get min and max of the pixel values in the image b relative to the
         * pixel n in row-continuous representation
         * @param b image to compute min and max in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param min output, minimum value
         * @param max output, maximum value
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         */
        template<typename T>
        void getMinMax(Image<T>& b, P n, T& min, T& max, Image<unsigned char>* support= NULL);

        /**
         * get mean of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute mean in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return mean of the values
         */
        template<typename T>
        float getMean(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        template<typename T>
        float getSkewness(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        template<typename T>
        float getKurtosis(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get variance of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get variance of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation, based on pre-computed mean
         * @param b image to compute variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param mean pre-computed mean value
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Image<T>& b, P n, float mean, Image<unsigned char>* support= NULL);

        /**
         * get mean and variance of the pixels in the image b, relatively to the 
         * pixel n in row-continuous representation
         * @param b image to comput mean and variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param mean output variable for mean
         * @param var output variable for variance
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         */
        template<typename T>
        void getMeanAndVariance(Image<T>& b, P n, float& mean, float& var, Image<unsigned char>* support= NULL);

        /**
         * get standard deviation of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute standard deviation in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return standard deviation of the values
         */
        template<typename T>
        float getStandardDeviation(Image<T>& b, P n, Image<unsigned char>* support= NULL);

        /**
         * get median of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute median in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Image<T>& b, P n, Image<unsigned char>* support= NULL);

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
         * get min and max of the pixel values in the image b relative to the
         * pixel n in row-continuous representation
         * @param b image to compute min and max in
         * @param min output, minimum value
         * @param max output, maximum value
         */
        template<typename T>
        void getMinMax(Image<T>& b, T& min, T& max);

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
         * get mean and variance of the pixels in the image b in the position of pixels
         * in this set
         * pixel n in row-continuous representation
         * @param b image to comput mean and variance in
         * @param mean output variable for mean
         * @param var output variable for variance
         */
        template<typename T>
        void getMeanAndVariance(Image<T>& b, float& mean, float& var);

        /**
         * get median of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute median in
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Image<T>& b);

        template<typename T>
        float getSkewness(Image<T>& b);

        template<typename T>
        float getKurtosis(Image<T>& b);

        /** first pixel in the set in row-continuous representation*/
        int min;
        /** last pixel in the set in row-continuous representation*/
        int max;
        /** minimum x coordinate*/
        int minX;
        /** minimum y coordinate*/
        int minY;
        /** maximum x coordinate*/
        int maxX;
        /** maximum y coordinate*/
        int maxY;

        /**
         * stride of the images to which the pixel set belongs
         */
        int stride;
    };

    template<typename P>
    PixelSet<P>::PixelSet()
    : Set<P>()
    {
      min= max= minX= minY= maxX= maxY= stride= 0;
    }

    template<typename P>
    PixelSet<P>::PixelSet(const PixelSet<P>& s)
    : Set<P>(s)
    {
        this->min= s.min;
        this->max= s.max;
        this->minX= s.minX;
        this->minY= s.minY;
        this->maxX= s.maxX;
        this->maxY= s.maxY;
        this->stride= s.stride;
    }

    template<typename P>
    PixelSet<P>::~PixelSet()
    {
    }
    
    template<typename P>
    P PixelSet<P>::getCenterOfGravity()
    {
        P sum= 0;
        for ( typename PixelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            sum+= *pit;
        sum/= this->size();
        return sum;
    }

    template<typename P>
    bool PixelSet<P>::isOverlapping(const PixelSet<P>& s) const
    {
        for ( typename PixelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            if ( s.find(*pit) != s.end() )
                return true;
        return false;
    }

    template<typename P>
    int PixelSet<P>::getBoundingBoxSize()
    {
        return getWidth() > getHeight() ? getWidth() : getHeight();
    }

    template<typename P>
    int PixelSet<P>::getWidth()
    {
        return maxX - minX;
    }

    template<typename P>
    int PixelSet<P>::getHeight()
    {
        return maxY - minY;
    }

    template<typename P>
    int PixelSet<P>::getMin()
    {
        return min;
    }

    template<typename P>
    int PixelSet<P>::getMax()
    {
        return max;
    }

    template<typename P>
    PixelSet<P>& PixelSet<P>::operator=(const PixelSet<P>& s)
    {
        this->Set<P>::operator=(s);
        this->stride= s.stride;
        this->min= s.min;
        this->max= s.max;
        this->minX= s.minX;
        this->maxX= s.maxX;
        this->minY= s.minY;
        this->maxY= s.maxY;
        return *this;
    }

    template<typename P>
    PixelSet<P>& PixelSet<P>::operator+=(const PixelSet<P>& s)
    {
        this->Set<P>::operator+=(s);
        return *this;
    }

    template<typename P>
    PixelSet<P>& PixelSet<P>::operator*=(const PixelSet<P>& s)
    {
        this->Set<P>::operator*=(s);
        return *this;
    }

    template<typename P>
    PixelSet<P>& PixelSet<P>::operator/=(const PixelSet<P>& s)
    {
        this->Set<P>::operator/=(s);
        return *this;
    }

    template<typename P>
    PixelSet<P> PixelSet<P>::operator+(const PixelSet<P>& s) const
    {
        return (PixelSet<P>(*this)) += s;
    }

    template<typename P>
    PixelSet<P> PixelSet<P>::operator*(const PixelSet<P>& s) const
    {
        return PixelSet<P>(*this) *= s;
    }

    template<typename P>
    PixelSet<P> PixelSet<P>::operator/(const PixelSet<P>& s) const
    {
        return PixelSet<P>(*this) /= s;
    }

    template<typename P> template<typename T>
    void PixelSet<P>::set(Image<T>& b, T value)
    {
        for( typename PixelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            b(*pit)= value;
    }

    template<typename P> template<typename T>
    void PixelSet<P>::set(Image<T>& b, P n, T value)
    {
        for( typename PixelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            b(*pit + n)= value;
    }

    template<typename P>
    void PixelSet<P>::translate(P n)
    {
        for( typename PixelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            *pit= *pit + n;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getSum(Image<T>& b, P n, Image<unsigned char>* support)
    {
        float f= 0;

        if ( support == NULL )
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
        else
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                    f+= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMax(Image<T>& b, P n, Image<unsigned char>* support)
    {
        T f= numeric_limits<T>::min();

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(n + *psit) > f )
                        f= b(n + *psit);
        }
        else
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 && b(n + *psit) > f )
                        f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMin(Image<T>& b, P n, Image<unsigned char>* support)
    {
        T f= numeric_limits<T>::max();

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    if ( b(n + *psit) < f )
                            f= b(n + *psit);
        }
        else
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    if ( (*support)(n + *psit) > 0 && b(n + *psit) < f )
                            f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    void PixelSet<P>::getMinMax(Image<T>& b, P n, T& min, T& max, Image<unsigned char>* support)
    {
        min= max= b(n + *(this->begin()));

        T tmp;

        if ( support == NULL )
            for ( typename PixelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
            {
                tmp= b(n + *psit);
                if ( tmp < min )
                    min= tmp;
                if ( tmp > max )
                    max= tmp;
            }
        else
            for ( typename PixelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= b(n + *psit);
                    if ( tmp < min )
                        min= tmp;
                    if ( tmp > max )
                        max= tmp;
                }
            }
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getMean(Image<T>& b, P n, Image<unsigned char>* support)
    {
        float f= 0;

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
            f/= this->size();
        }
        else
        {
            int x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }
            f/= x;
        }

        return f;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getSkewness(Image<T>& b, P n, Image<unsigned char>* support)
    {
        float f= 0;
        float tmp= 0;
        float tmp2= 0;
        float tmp3= 0;

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
            f/= this->size();
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= (b(n + *psit) - f) * (b(n + *psit) - f);
                tmp2+= tmp;
                tmp3+= tmp * (b(n + *psit) - f);
            }
        }
        else
        {
            int x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }
            f/= x;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= (b(n + *psit) - f) * (b(n + *psit) - f);
                    tmp2+= tmp;
                    tmp3+= tmp * (b(n + *psit) - f);
                }
            }
        }

        if ( tmp2 > 0 )
            return tmp3 / pow(tmp2, 3.0/2.0);
        else
            return 0;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getKurtosis(Image<T>& b, P n, Image<unsigned char>* support)
    {
        float f= 0;
        float tmp= 0;
        float tmp2= 0;
        float tmp4= 0;

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
            f/= this->size();
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= (b(n + *psit) - f) * (b(n + *psit) - f);
                tmp2+= tmp;
                tmp4+= tmp * tmp;
            }
        }
        else
        {
            int x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }
            f/= x;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= (b(n + *psit) - f) * (b(n + *psit) - f);
                    tmp2+= tmp;
                    tmp4+= tmp * tmp;
                }
            }
        }

        if ( tmp2 > 0 )
            return tmp4 / (tmp2*tmp2);
        else
            return 0;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getVariance(Image<T>& b, P n, Image<unsigned char>* support)
    {
        float f= 0;
        float v= 0;
        float tmp;

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);

            f/= this->size();

            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= f - b(n + *psit);
                v+= tmp*tmp;
            }
            v/= this->size();
        }
        else
        {
            int x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }

            f/= x;

            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= f - b(n + *psit);
                    v+= tmp*tmp;
                }
            }
            v/= x;

        }

        return v;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getVariance(Image<T>& b, P n, float mean, Image<unsigned char>* support)
    {
        float v= 0;
        float tmp;

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= mean - b(n + *psit);
                v+= tmp*tmp;
            }
            if ( this->size() > 0 )
                v/= this->size();
        }
        else
        {
            int x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= mean - b(n + *psit);
                    v+= tmp*tmp;
                    ++x;
                }
            }
            if ( x > 0 )
                v/= x;
        }

        return v;
    }

    template<typename P> template<typename T>
    void PixelSet<P>::getMeanAndVariance(Image<T>& b, P n, float& mean, float& var, Image<unsigned char>* support)
    {
        mean= 0;
        var= 0;
        float tmp;
        float x= this->size();

        if ( support == NULL )
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= b(n + *psit);
                mean+= tmp;
                var+= tmp*tmp;
            }
        }
        else
        {
            x= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= b(n + *psit);
                    mean+= tmp;
                    var+= tmp*tmp;
                    x= x + 1.0f;
                }
            }
        }

        mean/= x;
        var= (var/x - mean*mean);
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getStandardDeviation(Image<T>& b, P n, Image<unsigned char>* support)
    {
        return sqrt(getVariance(b, n, support));
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMedian(Image<T>& input, P n, Image<unsigned char>* support)
    {
        vector<T> a;

        if ( support == NULL )
        {
            a.resize(this->size());

            int j= 0;
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    a[j++]= input.operator()(n + *psit);
        }
        else
        {
            for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                    a.push_back(input(n + *psit));
        }


        if ( a.size() > 0 )
        {
            sort(a.begin(), a.end());
            return a[a.size()/2];
        }
        else
            return input[n];
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getSum(Image<T>& b)
    {
        float f= 0;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMax(Image<T>& b)
    {
        float f= b(*(this->begin()));

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) > f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMin(Image<T>& b)
    {
        float f= b(*(this->begin()));

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) < f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    void PixelSet<P>::getMinMax(Image<T>& b, T& min, T& max)
    {
        min= max= b(*(this->begin()));

        T tmp;

        for ( typename PixelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
        {
            tmp= b(*psit);
            if ( tmp < min )
                min= tmp;
            if ( tmp > max )
                max= tmp;
        }
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getMean(Image<T>& b)
    {
        float f= 0;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            if ( *psit > 0 && *psit < int(b.n) )
                        f+= b(*psit);
        }

        f/= this->size();

        return f;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getVariance(Image<T>& b)
    {
        float f= b(0);
        float v= 0;
        float tmp;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                        f+= b(*psit);

        f/= this->size();

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                tmp= b(*psit);
                v+= (f - tmp) * (f - tmp);
        }

        return v / this->size();
    }

    template<typename P> template<typename T>
    void PixelSet<P>::getMeanAndVariance(Image<T>& b, float& mean, float& var)
    {
        float tmp;

        mean= 0;
        var= 0;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            mean+= b(*psit);

        mean/= this->size();

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            tmp= mean - b(*psit);
            var+= tmp*tmp;
        }

        var/= this->size();
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getSkewness(Image<T>& b)
    {
        float tmp, tmp2= 0, tmp3= 0;
        float mean= 0;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            mean+= b(*psit);
        mean/= this->size();

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            tmp= (b(*psit) - mean);
            tmp*= tmp;
            tmp2+= tmp;
            tmp3+= tmp * (b(*psit) - mean);
        }

        if ( tmp2 > 0 )
            return tmp3/pow(tmp2,3.0/2.0);
        else
            return 0;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getKurtosis(Image<T>& b)
    {
        float tmp, tmp2= 0, tmp3= 0;
        float mean= 0;

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            mean+= b(*psit);
        mean/= this->size();

        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            tmp= (b(*psit) - mean);
            tmp*= tmp;
            tmp2+= tmp;
            tmp3+= tmp*tmp;
        }

        if ( tmp2 > 0 )
            return tmp3/(tmp2*tmp2);
        else
            return 0;
    }

    template<typename P> template<typename T>
    float PixelSet<P>::getStandardDeviation(Image<T>& b)
    {
        return sqrt(getVariance(b));
    }

    template<typename P> template<typename T>
    T PixelSet<P>::getMedian(Image<T>& input)
    {
        vector<T> a;
        a.resize(this->size());

        int j= 0;
        for ( typename PixelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                a[j++]= input.operator()(*psit);

        sort(a.begin(), a.end());
        return a[this->size()/2];
    }

    template<typename P>
    void PixelSet<P>::computeMinMax()
    {

    }

    template<typename P>
    void PixelSet<P>::computeMinMaxXY()
    {

    }

    template<typename P>
    void PixelSet<P>::computeMinMaxOnly()
    {

    }

    /**
     * prints the description of the parameter PixelSet object to the parameter output stream
     * @param o parameter output stream
     * @param p PixelSet object to describe
     * @return reference to the parameter output stream
     */
    template<typename P>
    std::ostream& operator<<(std::ostream& o, const PixelSet<P>& p)
    {
        o << "[PixelSet - size: ";
        o << p.size();
        o << " elements: ";
        for ( typename PixelSet<P>::const_pIt it = p.begin(); it != p.end(); ++it )
        {
            o << *it;
            o << " ";
        }
        o << "]";

        return o;
    }
}

#endif	/* _PIXELSET_H */

