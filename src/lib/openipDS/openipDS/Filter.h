/**
 * @file Filter.h
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
 * The Filter class represents a general filter applicable on vectors and has
 * several useful operators and member functions defined.
 */

#ifndef _FILTER_H
#define	_FILTER_H

#include <vector>
#include <set>

#include <openipDS/Image.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/StructuringElement2.h>
#include <openipDS/PositionWeightPair.h>
#include <openipDS/drawing.h>
#include <openipDS/Set.h>
#include <openipDS/Pixel.h>
#include <openipDS/Template.h>

#include <stdio.h>
#include <omp.h>

namespace openip
{
    /**
     * Filter represents a filter as a set of PositionWeightPairs
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class Filter : public Template<WEIGHTS>
    {
    public:
        using Template<WEIGHTS>::size;
	using Template<WEIGHTS>::addElement;
	using Template<WEIGHTS>::dumpTemplate;
	using Template<WEIGHTS>::zeroMean;
	using Template<WEIGHTS>::zeroMiddle;
	using Template<WEIGHTS>::add;
	using Template<WEIGHTS>::invert;
	using Template<WEIGHTS>::getWeights;
	using Template<WEIGHTS>::setWeights;
	using Template<WEIGHTS>::normalize;
	using Template<WEIGHTS>::normalizer;
	
        /**
         * Filter::iterator typedef
         */
        typedef typename Template<WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Template<WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
         */
        Filter();

        /**
         * copy constructor
         * @param f Filter object to copy
         */
        Filter(const Filter& f);

        /**
         * destructor
         */
        ~Filter();

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpFilter(Vector<WEIGHTS>& im);

        /**
         * computes the min and max values, representing the smallest and
         * greatest pixel positions relative to 0
         */
        //virtual void computeMinMax();
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param f Filter object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const Filter<INPUT, OUTPUT, WEIGHTS>& f);
}
namespace openip
{
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const Filter<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[Filter - size: ";
        o << f.size();
        o << " elements: ";
        for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::const_fIt it = f.begin(); it != f.end(); ++it )
        {
            o << *it;
            o << ", ";
        }
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter<INPUT, OUTPUT, WEIGHTS>::Filter()
    : Template<WEIGHTS>()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter<INPUT, OUTPUT, WEIGHTS>::Filter(const Filter& f)
    : Template<WEIGHTS>(f)
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter<INPUT, OUTPUT, WEIGHTS>::~Filter()
    {
    }
}

#endif	/* _FILTER_H */

