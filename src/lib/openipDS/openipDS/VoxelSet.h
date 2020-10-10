/* 
 * File:   VoxelSet.h
 * Author: gykovacs
 *
 * Created on July 19, 2011, 9:13 PM
 */

#ifndef VOXELSET_H
#define	VOXELSET_H

#include <openipDS/Voxel.h>
#include <openipDS/Set.h>
#include <openipDS/Volume.h>

namespace openip
{
    /**
     * VoxelSet represents a set of pixels from Voxel type P
     */
    template<typename P>
    class VoxelSet : public Set<P>
    {
    public:
        /**
         * VoxelSet::iterator typedef
         */
        typedef typename VoxelSet::iterator pIt;

        /**
         * VoxelSet::const_iterator typedef
         */
        typedef typename VoxelSet::const_iterator const_pIt;

        /**
         * default constructor
         */
        VoxelSet();

        /**
         * copy constructor
         */
        VoxelSet(const VoxelSet& s);

        /**
         * assignment operator
         * @param s the object to copy
         * @return this
         */
        VoxelSet& operator=(const VoxelSet& s);

        /**
         * union operator, adds the elements of s to this Set
         * @param s the Set whose elements are added to this
         * @return this
         */
        VoxelSet& operator+=(const VoxelSet& s);

        /**
         * intersetion operator, deletes the elements of this which are not
         * present in s
         * @param s the Set to intersect with
         * @return this
         */
        VoxelSet& operator*=(const VoxelSet& s);

        /**
         * subtract operator, deletes the elements of Set s from this
         * @param s the Set whose elements are deleted from this
         * @return this
         */
        VoxelSet& operator/=(const VoxelSet& s);

        /**
         * union operator, union with this and s, returns a new instance
         * @param s Set to union with
         * @return new Set instance
         */
        VoxelSet operator+(const VoxelSet& s) const;

        /**
         * intersection operator, intersection with this and s, returns a new
         * instance
         * @param s Set to intersect with
         * @return new Set instance
         */
        VoxelSet operator*(const VoxelSet& s) const;

        /**
         * subtract operator, deletes the elements of Set s from this and
         * returns a new instance
         * @param s the Set whose elements are deleted from this
         * @return a new Set instance
         */
        VoxelSet operator/(const VoxelSet& s) const;

        /**
         * width of bounding box of the pixel set
         * @return width of the bounding box
         */
        int getColumns();

        /**
         * height of bounding box of the pixel set
         * @return height of the bounding box
         */
        int getRows();
        
        int getSlices();

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
        virtual void computeMinMaxXYZ();

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
        bool isOverlapping(const VoxelSet<P>& s) const;

        /**
         * sets the elements of b in the positions of the pixels in set to value
         * @param b the image to set the pixels of
         * @param value the value to set the pixels to
         */
        template<typename T>
        void set(Volume<T>& b, T value);

        /**
         * sets the elements of b in the positions of the pixels in this
         * set relative to n to value
         * @param b the image to set the pixels of
         * @param n base pixel
         * @param value the value to set the pixels to
         */
        template<typename T>
        void set(Volume<T>& b, P n, T value);

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
        float getSum(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

        /**
         * get max of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute max in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return max of the values
         */
        template<typename T>
        T getMax(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

        /**
         * get min of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute min in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return min of the values
         */
        template<typename T>
        T getMin(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

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
        void getMinMax(Volume<T>& b, P n, T& min, T& max, Volume<unsigned char>* support= NULL);

        /**
         * get mean of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute mean in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return mean of the values
         */
        template<typename T>
        float getMean(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

        /**
         * get variance of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute variance in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

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
        float getVariance(Volume<T>& b, P n, float mean, Volume<unsigned char>* support= NULL);

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
        void getMeanAndVariance(Volume<T>& b, P n, float& mean, float& var, Volume<unsigned char>* support= NULL);

        /**
         * get standard deviation of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute standard deviation in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return standard deviation of the values
         */
        template<typename T>
        float getStandardDeviation(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

        /**
         * get median of the pixel values in the image b relatively to the pixel n
         * in row-continuous representation
         * @param b image to compute median in
         * @param n coordinate, the pixels in the set will be relative to this
         * @param roi if not NULL, only the pixels in the foreground (non-zero) region of the roi are used
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Volume<T>& b, P n, Volume<unsigned char>* support= NULL);

        /**
         * get sum of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute sum in
         * @return return sum of the values
         */
        template<typename T>
        float getSum(Volume<T>& b);

        /**
         * get max of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute max in
         * @return return max of the values
         */
        template<typename T>
        T getMax(Volume<T>& b);

        /**
         * get min of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute min in
         * @return return min of the values
         */
        template<typename T>
        T getMin(Volume<T>& b);

        /**
         * get min and max of the pixel values in the image b relative to the
         * pixel n in row-continuous representation
         * @param b image to compute min and max in
         * @param min output, minimum value
         * @param max output, maximum value
         */
        template<typename T>
        void getMinMax(Volume<T>& b, T& min, T& max);

        /**
         * get mean of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute mean in
         * @return return mean of the values
         */
        template<typename T>
        float getMean(Volume<T>& b);

        /**
         * get variance of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute variance in
         * @return return variance of the values
         */
        template<typename T>
        float getVariance(Volume<T>& b);

        /**
         * get standard deviation of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute standard deviation in
         * @return return standard deviation of the values
         */
        template<typename T>
        float getStandardDeviation(Volume<T>& b);

        /**
         * get mean and variance of the pixels in the image b in the position of pixels
         * in this set
         * pixel n in row-continuous representation
         * @param b image to comput mean and variance in
         * @param mean output variable for mean
         * @param var output variable for variance
         */
        template<typename T>
        void getMeanAndVariance(Volume<T>& b, float& mean, float& var);

        /**
         * get median of the pixel values in the image b in the position of pixels
         * in this set
         * @param b image to compute median in
         * @return return median of the values
         */
        template<typename T>
        T getMedian(Volume<T>& b);

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
        int minZ;
        int maxZ;

        /**
         * stride of the images to which the pixel set belongs
         */
        int stride;
        
        int slicePitch;
    };

    template<typename P>
    VoxelSet<P>::VoxelSet()
    : Set<P>()
    {
    }

    template<typename P>
    VoxelSet<P>::VoxelSet(const VoxelSet<P>& s)
    : Set<P>(s)
    {
        this->min= s.min;
        this->max= s.max;
        this->minX= s.minX;
        this->minY= s.minY;
        this->maxX= s.maxX;
        this->maxY= s.maxY;
        this->stride= s.stride;
        this->slicePitch= s.slicePitch;
    }

    template<typename P>
    P VoxelSet<P>::getCenterOfGravity()
    {
        P sum= 0;
        for ( typename VoxelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            sum+= *pit;
        sum/= this->size();
        return sum;
    }

    template<typename P>
    bool VoxelSet<P>::isOverlapping(const VoxelSet<P>& s) const
    {
        for ( typename VoxelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            if ( s.find(*pit) != s.end() )
                return true;
        return false;
    }

    template<typename P>
    int VoxelSet<P>::getColumns()
    {
        return maxX - minX;
    }

    template<typename P>
    int VoxelSet<P>::getRows()
    {
        return maxY - minY;
    }
    
    template<typename P>
    int VoxelSet<P>::getSlices()
    {
        return maxZ - minZ;
    }

    template<typename P>
    int VoxelSet<P>::getMin()
    {
        return min;
    }

    template<typename P>
    int VoxelSet<P>::getMax()
    {
        return max;
    }

    template<typename P>
    VoxelSet<P>& VoxelSet<P>::operator=(const VoxelSet<P>& s)
    {
        this->Set<P>::operator=(s);
        this->stride= s.stride;
        this->slicePitch= s.slicePitch;
        this->min= s.min;
        this->max= s.max;
        this->minX= s.minX;
        this->minY= s.minY;
        this->minZ= s.minZ;
        this->maxX= s.maxX;
        this->maxY= s.maxY;
        this->maxZ= s.maxZ;
        return *this;
    }

    template<typename P>
    VoxelSet<P>& VoxelSet<P>::operator+=(const VoxelSet<P>& s)
    {
        this->Set<P>::operator+=(s);
        return *this;
    }

    template<typename P>
    VoxelSet<P>& VoxelSet<P>::operator*=(const VoxelSet<P>& s)
    {
        this->Set<P>::operator*=(s);
        return *this;
    }

    template<typename P>
    VoxelSet<P>& VoxelSet<P>::operator/=(const VoxelSet<P>& s)
    {
        this->Set<P>::operator/=(s);
        return *this;
    }

    template<typename P>
    VoxelSet<P> VoxelSet<P>::operator+(const VoxelSet<P>& s) const
    {
        return VoxelSet<P>(*this) += s;
    }

    template<typename P>
    VoxelSet<P> VoxelSet<P>::operator*(const VoxelSet<P>& s) const
    {
        return VoxelSet<P>(*this) *= s;
    }

    template<typename P>
    VoxelSet<P> VoxelSet<P>::operator/(const VoxelSet<P>& s) const
    {
        return VoxelSet<P>(*this) /= s;
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::set(Volume<T>& b, T value)
    {
        for( typename VoxelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            b(*pit)= value;
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::set(Volume<T>& b, P n, T value)
    {
        for( typename VoxelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            b(*pit + n)= value;
    }

    template<typename P>
    void VoxelSet<P>::translate(P n)
    {
        for( typename VoxelSet<P>::pIt pit= this->begin(); pit != this->end(); ++pit )
            *pit= *pit + n;
    }

    template<typename P> template<typename T>
    float VoxelSet<P>::getSum(Volume<T>& b, P n, Volume<unsigned char>* support)
    {
        float f= 0;

        if ( support == NULL )
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
        else
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                    f+= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMax(Volume<T>& b, P n, Volume<unsigned char>* support)
    {
        T f= b(n + *(this->begin()));

        if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(n + *psit) > f )
                        f= b(n + *psit);
        }
        else
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 && b(n + *psit) > f )
                        f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMin(Volume<T>& b, P n, Volume<unsigned char>* support)
    {
        T f= b(n + *(this->begin()));

        if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    if ( b(n + *psit) < f )
                            f= b(n + *psit);
        }
        else
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    if ( (*support)(n + *psit) > 0 && b(n + *psit) < f )
                            f= b(n + *psit);

        return f;
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::getMinMax(Volume<T>& b, P n, T& min, T& max, Volume<unsigned char>* support)
    {
        min= max= b(n + *(this->begin()));

        T tmp;

        if ( support == NULL )
            for ( typename VoxelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
            {
                tmp= b(n + *psit);
                if ( tmp < min )
                    min= tmp;
                if ( tmp > max )
                    max= tmp;
            }
        else
            for ( typename VoxelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
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
    float VoxelSet<P>::getMean(Volume<T>& b, P n, Volume<unsigned char>* /*support*/)
    {
        float f= 0;

        //if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);
            f/= this->size();
        }
        /*else
        {
            int x= 0;
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }
            f/= x;
        }*/

        return f;
    }

    template<typename P> template<typename T>
    float VoxelSet<P>::getVariance(Volume<T>& b, P n, Volume<unsigned char>* support)
    {
        float f= 0;
        float v= 0;
        float tmp;

        if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(n + *psit);

            f/= this->size();

            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= f - b(n + *psit);
                v+= tmp*tmp;
            }
            v/= this->size();
        }
        else
        {
            int x= 0;
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( (*support)(n + *psit) > 0 )
                {
                    f+= b(n + *psit);
                    ++x;
                }

            f/= x;

            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
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
    float VoxelSet<P>::getVariance(Volume<T>& b, P n, float mean, Volume<unsigned char>* support)
    {
        float v= 0;
        float tmp;

        if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= mean - b(n + *psit);
                v+= tmp*tmp;
            }
            v/= this->size();
        }
        else
        {
            int x= 0;
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                if ( (*support)(n + *psit) > 0 )
                {
                    tmp= mean - b(n + *psit);
                    v+= tmp*tmp;
                    ++x;
                }
            }
            v/= x;
        }

        return v;
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::getMeanAndVariance(Volume<T>& b, P n, float& mean, float& var, Volume<unsigned char>* support)
    {
        mean= 0;
        var= 0;
        float tmp;
        float x= this->size();

        if ( support == NULL )
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            {
                tmp= b(n + *psit);
                mean+= tmp;
                var+= tmp*tmp;
            }
        }
        else
        {
            x= 0;
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
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
    float VoxelSet<P>::getStandardDeviation(Volume<T>& b, P n, Volume<unsigned char>* support)
    {
        return sqrt(getVariance(b, n, support));
    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMedian(Volume<T>& input, P n, Volume<unsigned char>* support)
    {
        vector<T> a;

        if ( support == NULL )
        {
            a.resize(this->size());

            int j= 0;
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                    a[j++]= input.operator()(n + *psit);
        }
        else
        {
            for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
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
    float VoxelSet<P>::getSum(Volume<T>& b)
    {
        float f= 0;

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                f+= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMax(Volume<T>& b)
    {
        float f= b(0);

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) > f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMin(Volume<T>& b)
    {
        float f= b(0);

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                if ( b(*psit) < f )
                        f= b(*psit);

        return f;
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::getMinMax(Volume<T>& b, T& min, T& max)
    {
        min= max= b(*(this->begin()));

        T tmp;

        for ( typename VoxelSet<P>::pIt psit= this->begin() + 1; psit != this->end(); ++psit )
        {
            tmp= b(*psit);
            if ( tmp < min )
                min= tmp;
            if ( tmp > max )
                max= tmp;
        }
    }

    template<typename P> template<typename T>
    float VoxelSet<P>::getMean(Volume<T>& b)
    {
        float f= 0;

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            if ( *psit > 0 && *psit < int(b.n) )
                        f+= b(*psit);
        }

        f/= this->size();

        return f;
    }

    template<typename P> template<typename T>
    float VoxelSet<P>::getVariance(Volume<T>& b)
    {
        float f= b(0);
        float v= 0;
        float tmp;

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                        f+= b(*psit);

        f/= this->size();

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
                tmp= b(*psit);
                v+= (f - tmp) * (f - tmp);
        }

        return v / this->size();
    }

    template<typename P> template<typename T>
    void VoxelSet<P>::getMeanAndVariance(Volume<T>& b, float& mean, float& var)
    {
        float tmp;

        mean= 0;
        var= 0;

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
            mean+= b(*psit);

        mean/= this->size();

        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
        {
            tmp= mean - b(*psit);
            var+= tmp*tmp;
        }

        var/= this->size();
    }

    template<typename P> template<typename T>
    float VoxelSet<P>::getStandardDeviation(Volume<T>& b)
    {
        return sqrt(getVariance(b));
    }

    template<typename P>
    void VoxelSet<P>::computeMinMax()
    {

    }

    template<typename P>
    void VoxelSet<P>::computeMinMaxXYZ()
    {

    }

    template<typename P>
    void VoxelSet<P>::computeMinMaxOnly()
    {

    }

    template<typename P> template<typename T>
    T VoxelSet<P>::getMedian(Volume<T>& input)
    {
        vector<T> a;
        a.resize(this->size());

        int j= 0;
        for ( typename VoxelSet<P>::pIt psit= this->begin(); psit != this->end(); ++psit )
                a[j++]= input.operator()(*psit);

        sort(a.begin(), a.end());
        return a[this->size()/2];
    }

    /**
     * prints the description of the parameter VoxelSet object to the parameter output stream
     * @param o parameter output stream
     * @param p VoxelSet object to describe
     * @return reference to the parameter output stream
     */
    template<typename P>
    std::ostream& operator<<(std::ostream& o, const VoxelSet<P>& p)
    {
        o << "[VoxelSet - size: ";
        o << p.size();
        o << " elements: ";
        for ( typename VoxelSet<P>::const_pIt it = p.begin(); it != p.end(); ++it )
        {
            o << *it;
            o << " ";
        }
        o << "]";

        return o;
    }
}


#endif	/* VOXELSET_H */

