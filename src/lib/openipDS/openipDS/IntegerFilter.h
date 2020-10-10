/**
 * @file IntegerFilter.h
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
 * The IntegerFilter class represents filters optimized for integer arithmetic.
 */

#ifndef _INTEGERFILTER_H
#define	_INTEGERFILTER_H

#include <openipDS/Filter.h>

namespace openip
{
    /**
    * IntegerFilter2 represents a filter with integer weights
    */
    template<typename INPUT, typename OUTPUT>
    class IntegerFilter2 : virtual public Filter2<INPUT, OUTPUT, int>
    {
    public:
        using Filter2<INPUT, OUTPUT, int>::stride;
        using Filter2<INPUT, OUTPUT, int>::apply;
        using Filter2<INPUT, OUTPUT, int>::updateStride;
        using Filter2<INPUT, OUTPUT, int>::computeMinMax;

	/**
	* constructor to generate integer filter from arbitrary filter
	*/
        template<typename WEIGHTS>
        IntegerFilter2(Filter2<INPUT, OUTPUT, WEIGHTS>& f);

	/**
	* application method of the integer filter
	* @param input input image to apply on
	* @param n position to apply in
	* @return filter response
	*/
        OUTPUT apply(Image<INPUT>& input, int n);

	/**
	* scaling factor handled automatically
	*/
        float normalizer;
    };

    template<typename INPUT, typename OUTPUT> template<typename WEIGHTS>
    IntegerFilter2<INPUT, OUTPUT>::IntegerFilter2(Filter2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        WEIGHTS min1= FLT_MAX;

        for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fit= f.begin(); fit != f.end(); ++fit )
        {
            if ( fit->second > 0 && fabs(fit->second) < min1 )
                min1= fabs(fit->second);
        }

        min1/= 4;

        for ( typename Filter2<INPUT, OUTPUT, WEIGHTS>::iterator fit= f.begin(); fit != f.end(); ++fit )
        {
            this->push_back(PositionWeightPair<int>(fit->first, fit->second/min1));
        }

        stride= f.stride;
        normalizer= min1;
        this->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT IntegerFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n)
    {
        int f= 0;
        for ( typename Filter2<INPUT, OUTPUT, int>::fIt fit= this->begin(); fit != this->end(); ++fit )
            f+= fit->second * input(fit->first + n);

        return f*normalizer;
    }
}

#endif	/* _INTEGERFILTER_H */

