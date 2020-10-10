/**
 * @file FilterSystem2.h
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
 * The FilterSystem2 represents a set of Filter2s
 */

#ifndef _FILTERSYSTEM2_H
#define	_FILTERSYSTEM2_H

#include <string>

#include <openipDS/FilterSet2.h>

namespace openip
{
    /**
     * FilterSystem2 represents a set of FilterSet2s.
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class FilterSystem2: public vector<FilterSet2<INPUT, OUTPUT, WEIGHTS>* >
    {
    public:
        /**
         * constructor
         */
        FilterSystem2(int filterSystemMode= FILTER_SYSTEM_MODE_MAX_WEIGHTED_SUM);

        /**
         * copy constructor
         * @param bms instance to copy
         */
        FilterSystem2(const FilterSystem2& bms);

        /**
         * destructor
         */
        ~FilterSystem2();

        /**
         * applies the filters for labeling in position n
         * @param input input image
         * @param n position in row-continuous representation
         * @return label
         */
        unsigned char apply(Image<INPUT>& input, int n);

        /**
         * applies the filter system for labeling
         * @param input input image
         * @param output output labeled image
         * @param mask the labeling will be applied in the foreground (non 0) region
         */
        void apply(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * returns the stride of masks in the sets if they are the same, in the
         * case of differing, returns -1;
         * @return stride of masks in the sets
         */
        int stride();

        /**
         * updates the stride in the masks in the sets
         * @param stride new stride
         */
        void updateStride(int stride);

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * minimum pixel position
         */
        int min;

        /**
         * maximum pixel position
         */
        int max;

        /**
         * filter system mode
         */
        int filterSystemMode;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSystem2<INPUT, OUTPUT, WEIGHTS>::FilterSystem2(int filterSystemMode)
    {
        this->filterSystemMode= filterSystemMode;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSystem2<INPUT, OUTPUT, WEIGHTS>::FilterSystem2(const FilterSystem2& fs)
    : vector<FilterSet2<INPUT, OUTPUT, WEIGHTS>* >(fs)
    {
        this->min= fs.min;
        this->max= fs.max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSystem2<INPUT, OUTPUT, WEIGHTS>::~FilterSystem2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSystem2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
    {
        if ( updateStride(input.columns) )
            computeMinMax();

        output= 0;

        if ( mask == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                output(i)= apply(input, i);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < input.n - this->max; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= apply(input, i);
        }

    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    unsigned char FilterSystem2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, int n)
    {
        unsigned char label= 0;
        OUTPUT max= -FLT_MAX;
        OUTPUT tmp;

        for ( typename FilterSystem2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->getMinWeightedSum(input, n);
            if ( tmp > max )
            {
                max= tmp;
                label= (*fit)->label;
            }
        }
        return label;
    }


    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSystem2<INPUT, OUTPUT, WEIGHTS>::stride()
    {
        if ( this->begin() == this->end() )
            return -1;

        int stride= (*this->begin())->stride();

        for ( typename FilterSystem2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( stride != (*fit)->stride() )
                return -1;
        }
        return stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSystem2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        if ( this->stride() != stride )
        {
            for ( typename FilterSystem2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
                (*fit)->updateStride(stride);
        }
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSystem2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        this->min= INT_MAX;
        this->max= -INT_MAX;

        for ( typename FilterSystem2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( (*fit)->min < min )
                min= (*fit)->min;
            if ( (*fit)->max > max )
                max= (*fit)->max;
        }
    }

    /**
     * prints the description of the parameter FeatureSystem2 object to the parameter output stream
     * @param o parameter output stream
     * @param f FeatureSystem2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const FilterSystem2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[FilterSystem2 - size: ";
        o << f.size();
        o << "]";

        return o;
    }
}


#endif	/* _FILTERSYSTEM_H */

