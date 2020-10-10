/**
 * @file AIOperator.h
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
 * AIOperator is the base class of operators in graph search.
 */

#ifndef AIOPERATION_H
#define	AIOPERATION_H

#include <openipML/AIState.h>

namespace openip
{
    /** AIOperator is the base class of operators in graph search*/
    class AIOperator
    {
    public:
        /**
         * default constructor
         */
        AIOperator();

        /**
         * copy constructor
         * @param a instance to copy
         */
        AIOperator(const AIOperator& a);

        /**
         * destructor
         */
        ~AIOperator();

        /**
         * applies the operator to the parameter state
         * @param a parameter to apply the operator on
         * @return the new state
         */
        virtual AIState apply(AIState& a);

        /**
         * equals method
         * @param a the parameter to equal with
         * @return true if this equals a, false otherwise
         */
        virtual bool operator==(AIOperator& a);
    };
}

#endif	/* AIOPERATION_H */

