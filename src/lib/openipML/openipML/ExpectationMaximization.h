/**
 * @file ExpectationMaximization.h
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
 * ExpactationMaximization represents a general class for EM based optimization methods.
 */

#ifndef _EXPECTATIONMAXIMIZATION_H
#define	_EXPECTATIONMAXIMIZATION_H

namespace openip
{
    /**
    * ExpectationMaximization represents a general class for EM based optimization methods
    */
    class ExpectationMaximization
    {
    public:
	/**
	* default constructor
	*/
        ExpectationMaximization();

	/**
	* copy constructor
	* @param em instance to copy
	*/
        ExpectationMaximization(const ExpectationMaximization& em);

	/**
	* destructor
	*/
        ~ExpectationMaximization();

	/**
	* initialize the iterations
	*/
        virtual void init()= 0;

	/**
	* expectation
	*/
        virtual void expect()= 0;

	/**
	* maximization
	*/
        virtual void maximize()= 0;

	/**
	* checks wether the stoppig condition is true
	* @return true if the stopping condition holds, false otherwise
	*/
        virtual int stopCondition()= 0;

	/**
	* main iteration
	*/
        void doEM();

	/**
	* current iteration
	*/
        int iteration;
    };
}

#endif	/* _EXPECTATIONMAXIMIZATION_H */

