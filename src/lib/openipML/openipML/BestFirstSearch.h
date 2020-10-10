/**
 * @file BestFirstSearch.h
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
 * BestFirstSearch represents the graph theoretical best first search algorithm.
 */


#ifndef BESTFIRSTSEARCH_H
#define	BESTFIRSTSEARCH_H

#include <openipML/AIGraphSearch.h>
#include <openipDS/Stopper.h>

namespace openip
{
    /**
     * BestFirstSearch represents the graph theoretical best first search algorithm
     */
    template<typename STATE, typename OPERATOR>
    class BestFirstSearch: public AIGraphSearch<STATE, OPERATOR>
    {
    public:
        using AIGraphSearch<STATE, OPERATOR>::actualNode;
        using AIGraphSearch<STATE, OPERATOR>::operators;
        using AIGraphSearch<STATE, OPERATOR>::openedNodes;
        using AIGraphSearch<STATE, OPERATOR>::closedNodes;

        /**
         * default constructor
         */
        BestFirstSearch();

        /**
         * copy constructor
         * @param g instance to copy
         */
        BestFirstSearch(const BestFirstSearch& g);

        /**
         * destructor
         */
        ~BestFirstSearch();

        /**
         * do search
         * @param a start node
         */
        virtual void search(STATE& a);

        STATE bestNode;
        STATE tmpNode;
    };

    template<typename STATE, typename OPERATOR>
    BestFirstSearch<STATE, OPERATOR>::BestFirstSearch()
    {
    }

    template<typename STATE, typename OPERATOR>
    BestFirstSearch<STATE, OPERATOR>::BestFirstSearch(const BestFirstSearch& g)
    {
    }

    template<typename STATE, typename OPERATOR>
    BestFirstSearch<STATE, OPERATOR>::~BestFirstSearch()
    {
    }

    template<typename STATE, typename OPERATOR>
    void BestFirstSearch<STATE, OPERATOR>::search(STATE& a)
    {
        actualNode= a;
        float actValue= 0;
        float tmpValue;

        int n= 0;

        tprintf("loop started\n");
        while ( 1 )
        {
            int idx= -1;

            tprintf("============= Level %d ==============\n", n++);
            actualNode.dump();
            tprintf("applying operators\n");
            for ( unsigned int i= 0; i < operators.size(); ++i )
            {
                tmpNode= operators[i].apply(actualNode);
                tmpValue= tmpNode.evaluate();
		
                if ( tmpValue > actValue )
                {
                    actValue= tmpValue;
                    bestNode= tmpNode;
                    idx= i;
                }
            }

            tprintf("operators applied, idx: %d, actValue: %f\n", idx, actValue);
            if ( idx == -1 )
	    {
	      if ( actValue == 0 )
	      {
		tprintf("ERROR: no feature was selected, probably because the score function gives 0 for any combination (e.g. F1-score, when there are no true positive results)\n");
		exit(1);
	      }
              break;
	    }
            else
            {
                tprintf("better state: %f\n", actValue); fflush(stdout);
                actualNode= bestNode;
            }
        }
    }
}


#endif	/* BESTFIRSTSEARCH_H */

