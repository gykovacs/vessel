/**
 * @file AIGraphSearch.h
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
 * AIGraphSearch is the base class for graph search techniques.
 */

#ifndef AIGRAPHSEARCH_H
#define	AIGRAPHSEARCH_H

#include <openipDS/Vector.h>

namespace openip
{
    /**
     * AIGraphSearch is the base class for graph search techniques.
     */
    template<typename STATE, typename OPERATOR>
    class AIGraphSearch
    {
    public:
        /**
         * deafult constructor
         */
        AIGraphSearch();

        /**
         * copy constructor
         * @param g instance to copy
         */
        AIGraphSearch(const AIGraphSearch& g);

        /**
         * destructor
         */
        ~AIGraphSearch();

        /**
         * start the search
         * @param a start node
         */
        virtual void search(STATE& a);

        /** actual node */
        STATE actualNode;
        /** operators */
        Vector<OPERATOR> operators;
        /** opened nodes */
        Vector<STATE> openedNodes;
        /** closed nodes */
        Vector<STATE> closedNodes;
    };

    template<typename STATE, typename OPERATOR>
    AIGraphSearch<STATE, OPERATOR>::AIGraphSearch()
    {
    }

    template<typename STATE, typename OPERATOR>
    AIGraphSearch<STATE, OPERATOR>::AIGraphSearch(const AIGraphSearch& g)
    {
    }

    template<typename STATE, typename OPERATOR>
    AIGraphSearch<STATE, OPERATOR>::~AIGraphSearch()
    {
    }

    template<typename STATE, typename OPERATOR>
    void AIGraphSearch<STATE, OPERATOR>::search(STATE&)
    {
    }
}

#endif	/* AIGRAPHSEARCH_H */

