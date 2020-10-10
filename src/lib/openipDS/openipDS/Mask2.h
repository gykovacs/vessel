/**
 * @file Mask2.h
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
 * The Mask2 class represents a general Mask object, applicable on images.
 */

#ifndef _MASK2_H
#define	_MASK2_H

#include <openipDS/Filter2.h>

namespace openip
{
    /**
    * Mask2 represents a general Mask object, applicable on images
    */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class Mask2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter2<INPUT, OUTPUT, WEIGHTS>::stride;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::computeMinMax;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::apply;
        using Filter2<INPUT, OUTPUT, WEIGHTS>::addElement;

        /**
         * Mask::iterator typedef
         */
        typedef typename Mask2<INPUT, OUTPUT, WEIGHTS>::iterator mIt;

        /**
         * Mask::const_iterator typedef
         */
        typedef typename Mask2<INPUT, OUTPUT, WEIGHTS>::const_iterator const_mIt;

        /**
         * default constructor
         * @param stride stride of the image on which the mask will be applied
         * @param matches response for matching
         * @param notMatches response for not matching
         */
        Mask2(int stride= 4000, OUTPUT matches= 255, OUTPUT notMatches= 0);

        /**
         * copy constructor
         * @param f instance to copy
         */
        Mask2(const Mask2& f);

        /**
         * destructor
         */
        ~Mask2();

        /**
         * applies the filter in position n
         * @param in input image
         * @param n position in row-continuous representation
         * @param roi if not NULL, only the foreground (non-zero) region of the roi is used by the operation
         * @returns mask response
         */
        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* roi= NULL);

        /**
         * applies the filter
         * @param in input image
         * @param out output image
         * @param mask the filter is applied in the foreground(non 0) region
         */
        virtual void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* mask= NULL);

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpMask(Image<WEIGHTS>& im);

        /**
         * updates the stride
         * @param stride new stride
         */
        int updateStride(int stride);

	/**
	* mask response if the pixel environment matches
	*/
        OUTPUT matches;
        
        /**
        * mask response if the pixel environment does not match
        */
        OUTPUT notMatches;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param m Mask2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const Mask2<INPUT, OUTPUT, WEIGHTS>& m)
    {
        o << "[Filter2 - size: ";
        o << m.size();
        o << " elements: ";
        for ( typename Mask2<INPUT, OUTPUT, WEIGHTS>::const_mIt it = m.begin(); it != m.end(); ++it )
        {
            o << *it;
            o << ", ";
        }
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask2<INPUT, OUTPUT, WEIGHTS>::Mask2(int stride, OUTPUT matches, OUTPUT notMatches)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(stride)
    {
        //this->stride= stride;
        this->matches= matches;
        this->notMatches= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask2<INPUT, OUTPUT, WEIGHTS>::Mask2(const Mask2& f)
    : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
        this->stride= f.stride;
        this->matches= f.matches;
        this->notMatches= f.notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask2<INPUT, OUTPUT, WEIGHTS>::~Mask2()
    {
    }

    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Mask2<INPUT, OUTPUT, WEIGHTS>::addElement(int row, int column, WEIGHTS weight)
    {
        this->addElement(row * stride + column, weight);
    }*/

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT Mask2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>*)
    {
        for ( typename Mask2<INPUT, OUTPUT, WEIGHTS>::mIt fit= this->begin(); fit != this->end(); ++fit )
            if ( in(fit->first + n) != (fit->second) )
                return notMatches;
        return matches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Mask2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* mask)
    {
        if ( updateStride(in.columns) )
            computeMinMax();

        if ( mask == NULL )
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                out(i)= apply(in, i);
        }
        else
        {
            int end= in.size() - this->max;
            #pragma omp parallel for
            for ( int i= -this->min; i < end; ++i )
                if ( mask->operator()(i) )
                    out(i)= apply(in, i);
                else
                    out(i)= in(i);
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Mask2<INPUT, OUTPUT, WEIGHTS>::dumpMask(Image<WEIGHTS>& im)
    {
        im.resizeImage(this->getRows(), this->getColumns());

        int x, y;
        for ( mIt fit= this->begin(); fit != this->end(); ++fit )
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

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int Mask2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        //printf("mask2::updateStride\n"); fflush(stdout);
        if ( this->stride != stride )
        {
            int x, y;
            for ( mIt fit= this->begin(); fit != this->end(); ++fit )
            {
                x= fit->first/this->stride;
                if ( abs(fit->first%this->stride) > this->stride/2 )
                    x= (fit->first > 0) ? x+1 : x-1;

                y= fit->first%this->stride;
                if ( abs(y) > this->stride/2 )
                    y+= y > 0? -this->stride : this->stride;
                fit->first= y*stride + x;
            }
            this->stride= stride;
            computeMinMax();
            return 1;
        }
        return 0;
    }
}

#endif	/* _MASK2_H */

