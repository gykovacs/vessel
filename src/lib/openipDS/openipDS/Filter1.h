/**
 * @file Filter1.h
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
 * The Filter1 class represents a filter applicable on vectors and has
 * several useful operators and member functions defined.
 */

#ifndef _FILTER1_H
#define	_FILTER1_H

#include <vector>
#include <set>
#include <omp.h>

#include <openipDS/Image.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/StructuringElement2.h>
#include <openipDS/PositionWeightPair.h>
#include <openipDS/drawing.h>
#include <openipDS/Set.h>
#include <openipDS/Pixel.h>
#include <openipDS/Filter.h>
#include <openipDS/Feature1.h>
#include <openipDS/Template.h>

#include <stdio.h>

namespace openip
{
    /**
     * Filter represents a filter as a set of PositionWeightPairs
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class Filter1 : public Template<WEIGHTS>, public Feature1<INPUT, OUTPUT>
    {
    public:
        using Feature1<INPUT, OUTPUT>::apply;
	using Template<WEIGHTS>::min;
	using Template<WEIGHTS>::max;
	using Template<WEIGHTS>::getMin;
	using Template<WEIGHTS>::getMax;
	using Template<WEIGHTS>::dumpTemplate;
	using Template<WEIGHTS>::normalize;
        
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
        Filter1();

        /**
         * applies the filter to the nth element of the vector
         * @param in input vector
         * @param n the position to apply in
         * @return the filter response
         */
        virtual OUTPUT apply(Vector<INPUT>& in, int n, Vector<unsigned char>* support= NULL);

        /**
         * applies the filter to the input vector
         * @param in input vector
         * @param out output vector
         * @param mask the filter is applied in the foreground (non-zero) region of the mask
         */
        virtual void apply(Vector<INPUT>& in, Vector<OUTPUT>& out, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL);

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpFilter(Vector<WEIGHTS>& im);
    };

    /**
     * prints the description of the parameter Filter1 object to the parameter output stream
     * @param o parameter output stream
     * @param f Filter1 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const Filter1<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[Filter1 - size: ";
        o << f.size();
        o << " elements: ";
        for ( typename Template<WEIGHTS>::const_fIt it= f.begin(); it != f.end(); ++it )
        {
            o << *it;
            o << ", ";
        }
        o << "]";

        return o;
    }
}
namespace openip
{
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Filter1<INPUT, OUTPUT, WEIGHTS>::Filter1()
    : Template<WEIGHTS>(), Feature1<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT Filter1<INPUT, OUTPUT, WEIGHTS>::apply(Vector<INPUT>& in, int n, Vector<unsigned char>* support)
    {
	float f= 0;
	for (typename Filter1<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( support == NULL || (*support)(n + fit->first) > 0 )
                f+= in(n + fit->first) * fit->second;
        }
	return (OUTPUT)f;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter1<INPUT, OUTPUT, WEIGHTS>::apply(Vector<INPUT>& in, Vector<OUTPUT>& out, Vector<unsigned char>* roi, Vector<unsigned char>* support)
    {
	int begin= -this->min;
	int end= in.size() - this->max;

        if ( !roi )
	{
	    #pragma omp parallel for 
	    for ( int i= begin; i < end; ++i )
		out(i)= this->apply(in, i, support);
	}
	else
	{
	    #pragma omp parallel for
	    for ( int i= begin; i < end; ++i )
		if ( (*roi)(i) > 0 )
		    out(i)= this->apply(in, i, support);
	}
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void Filter1<INPUT, OUTPUT, WEIGHTS>::dumpFilter(Vector<WEIGHTS>& im)
    {
        dumpTemplate(im);
    }
}

#endif	/* _FILTER1_H */

