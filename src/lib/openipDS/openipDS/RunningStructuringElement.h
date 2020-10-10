/**
 * @file RunningStructuringElement.h
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
 * This file contains the definition of the RunningStructuringElement class.
 */

#ifndef _RUNNINGSTRUCTURINGELEMENT_H
#define	_RUNNINGSTRUCTURINGELEMENT_H

#include <limits.h>
#include <openipDS/StructuringElement2.h>
#include <openipDS/PixelSet1.h>

namespace openip
{
    /**
     * RunningStructuringElement is a simple structuring element, but
     * the operations can be performed in running style.
     */
    template<typename INPUT>
    class RunningStructuringElement
    {
    public:
        /**
         * constructor
         * @param se the base structuring element
         */
        RunningStructuringElement(StructuringElement2 se);

        /**
         * copy constructor
         * @param rse instance to copy
         */
        RunningStructuringElement(const RunningStructuringElement& rse);

        /**
         * destructor
         */
        virtual ~RunningStructuringElement();

        /**
         * go to the next pixel
         */
        virtual void next();

        /**
         * initializes the running structuring element by setting the
         * input and mask images
         * @param input input image
         * @param mask mask image
         */
        virtual void init(Image<INPUT>* input, Image<unsigned char>* support= NULL);

        /**
         * initializes the running structuring element in the p position
         * @param p position in row-continuous representation
         */
        virtual void init(int p);

        /**
         * initializes the left and right fronts of the running structuring element
         * @param se base structuring element
         */
        void initFronts(StructuringElement2* se);

        virtual double value();

        /**
         * input image
         */
        Image<INPUT>* input;

        Image<unsigned char>* support;

        /**
         * structuring element
         */
        StructuringElement2 se;

        /**
         * actual position
         */
        int position;

        /**
         * the minimum position in which the structuring element can be applied
         */
        int begin;

        /**
         * the first position in which the structuring element can not be applied
         */
        int end;

        /**
         * left front of the structuring element
         */
        PixelSet1 leftFront;

        /**
         * right front of the structuring element
         */
        PixelSet1 rightFront;
    };

    template<typename INPUT>
    RunningStructuringElement<INPUT>::RunningStructuringElement(StructuringElement2 se1)
    :input(NULL), support(NULL), se(se1),  position(0), begin(0), end(0)
    {
        initFronts(&se);
    }

    template<typename INPUT>
    RunningStructuringElement<INPUT>::RunningStructuringElement(const RunningStructuringElement& rse)
    :input(rse.input), support(rse.support), position(rse.position), begin(rse.begin), end(rse.end), leftFront(rse.leftFront), rightFront(rse.rightFront)
    {
        this->se= rse.se;
    }

    template<typename INPUT>
    RunningStructuringElement<INPUT>::~RunningStructuringElement()
    {
    }

    template<typename INPUT>
    void RunningStructuringElement<INPUT>::initFronts(StructuringElement2* se)
    {
        position= 0;
        begin= 0;
        end= 0;
        
        this->se= *se;

        leftFront.clear();
        rightFront.clear();

        for ( StructuringElement2::iterator sit= se->begin(); sit != se->end(); ++sit )
        {
            if ( se->find(*sit - 1) == se->end() )
                leftFront.push_back(*sit);
            if ( se->find(*sit + 1) == se->end() )
                rightFront.push_back(*sit + 1);
        }
    }

    template<typename INPUT>
    void RunningStructuringElement<INPUT>::next()
    {
    }

    template<typename INPUT>
    void RunningStructuringElement<INPUT>::init(Image<INPUT>* input, Image<unsigned char>* support)
    {
        this->input= input;
        this->support= support;

        this->se.updateStride(input->columns);
        initFronts(&(this->se));

        begin= -this->se.getMin();
        end= input->n - this->se.getMax();

        init(begin);
    }

    template<typename INPUT>
    void RunningStructuringElement<INPUT>::init(int p)
    {
        position= p;
    }

    template<typename INPUT>
    double RunningStructuringElement<INPUT>::value()
    {
        return 0.0;
    }
}

#endif	/* _RUNNINGSTRUCTURINGELEMENT_H */

