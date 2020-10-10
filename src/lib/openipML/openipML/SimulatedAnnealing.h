/**
 * @file SimulatedAnnealing.h
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
 * SimulatedAnnealing is the base class for all simulated annealing based
 * optimization problems.
 */

#ifndef _SIMULATEDANNEALING_H
#define	_SIMULATEDANNEALING_H

#include <openipSC/RandomNumberGenerator.h>

namespace openip
{
    /** Simulated annealing is represents a base class for simulated annealing
    * based optimization methods
    */
    class SimulatedAnnealing
    {
    public:
	/**
	* constructor
	* @param maximumIteration maximum number of iterations
	* @param epsilon convergence tolerance
        * @param annealing decrease of temperature
        * @param temperature start temperature
	*/
        SimulatedAnnealing(long int maximumIteration= 0, double epsilon= 0, double annealing= 0.9999, double temperature= 4.0, int output= -1);

	/**
	* copy constructor
	* sa instance to copy
	*/
        SimulatedAnnealing(const SimulatedAnnealing& sa);

	/**
	* destructor
	*/
        ~SimulatedAnnealing();

	/**
	* initialize the optimization
	*/
        virtual void init()= 0;

	/**
	* sampling
	*/
        virtual void sample()= 0;

	/**
	* main iteration
	*/
        virtual void solve();

	/**
	* stop condition
	* @return logical value, wether the stopping condition is satisfied
	*/
        virtual int stopCondition();

	/**
	* evaluating objective function
        * @return value of the objective function
	*/
        virtual double evaluate()= 0;

        virtual void dump();

	/**
	* rollBack sampling
	*/
        virtual void revert()= 0;

        /**
         * checks if the parameter is better than the current objectiv function value
         * @param newObjective
         * @return non-zero if the parameter is better, zero otherwise
         */
        virtual int betterObjectiveFunction(double newObjective);

        /**
         * decreaseing temperature
         */
        virtual void decreaseTemperature();

	/**
	* current objective function value
	*/
        double objective;

        /**
         * new objective function
         */
        double newObjective;

        double lastObjective;
        
        /**
        * current temperature
        */
        double temperature;
        
        /**
        * current iteration
        */
        long int iteration;
        
        /**
        * maximum iteration
        */
        long int maximumIteration;
        
        /**
        * convergence limit
        */
        double epsilon;

        /**
         * decrease of the temperature
         */
        double annealing;

        int output;

        /**
        * random number generator from Gaussian distribution
        */
        UniformRandomFloatGenerator urfg;
    };
}

#endif	/* _SIMULATEDANNEALING_H */

