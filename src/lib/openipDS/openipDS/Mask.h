/**
 * @file Mask.h
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
 * The Mask represents the general base of masks.
 */

#ifndef _MASK_H
#define	_MASK_H

#include <openipDS/Filter.h>

namespace openip
{
    /**
    * Mask represents a general mask object
    */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class Mask: public Filter<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        using Filter<INPUT, OUTPUT, WEIGHTS>::addElement;
        
        /**
         * Mask::iterator typedef
         */
        typedef typename Filter<INPUT, OUTPUT, WEIGHTS>::iterator mIt;

        /**
         * Mask::const_iterator typedef
         */
        typedef typename Filter<INPUT, OUTPUT, WEIGHTS>::const_iterator const_mIt;

        /**
         * default constructor
         */
        Mask(OUTPUT matches= 255, OUTPUT notMatches= 0);

        /**
         * copy constructor
         * @param m Mask object to copy
         */
        Mask(const Mask& m);

        /**
         * destructor
         */
        ~Mask();

        /**
         * general apply method
         * @param in input
         * @param n position in row continuous representation
         */
        virtual OUTPUT apply(Vector<INPUT>& in, int n);

        /**
         * response for matching
         */
        OUTPUT matches;

        /**
         * response for not matching
         */
        OUTPUT notMatches;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask<INPUT, OUTPUT, WEIGHTS>::Mask(OUTPUT matches, OUTPUT notMatches)
    : Filter<INPUT, OUTPUT, WEIGHTS>()
    {
        this->matches= matches;
        this->notMatches= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask<INPUT, OUTPUT, WEIGHTS>::Mask(const Mask& m)
    : Filter<INPUT, OUTPUT, WEIGHTS>(m)
    {
        this->matches= m.matches;
        this->notMatches= m.notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Mask<INPUT, OUTPUT, WEIGHTS>::~Mask()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT Mask<INPUT, OUTPUT, WEIGHTS>::apply(Vector<INPUT>& in, int n)
    {
        for ( typename Mask<INPUT, OUTPUT, WEIGHTS>::mIt mit= this->begin(); mit != this->end(); ++mit )
        {
            if ( in(n + mit->first) != mit->second )
                return notMatches;
        }
        return matches;
    }
}

#endif	/* _MASK_H */

