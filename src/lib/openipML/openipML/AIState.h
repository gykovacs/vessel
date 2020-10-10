/**
 * @file AIState.h
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
 * AIState is the base class of nodes in graph search.
 */

#ifndef AISTATE_H
#define	AISTATE_H

namespace openip
{
    /**
     * AIState is the base class of nodes in graph search.
     */
    class AIState
    {
    public:
        /**
         * default constructor
         */
        AIState();

        /**
         * copy constructor
         * @param a instance to copy
         */
        AIState(const AIState& a);

        /**
         * destructor
         */
        ~AIState();

        /**
         * equals operator
         * @param a parameter to check equality with
         * @return true if this equals a, false otherwise
         */
        virtual bool operator==(AIState& a);

        /**
         * evaluates the goodness of the state for solution
         * @return goodness value
         */
        virtual float evaluate();
    };
}

#endif	/* AISTATE_H */

