/**
 * @file rootFinding.h
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
 * This file contains the specification of the numerical bisection method.
 */

#ifndef BISECTIONMETHOD_H
#define	BISECTIONMETHOD_H

#include <openipSC/RealFunction.h>

namespace openip
{
    /**
     * The numerical bisection method for finding roots of the parameter function.
     * @param rf real valued function
     * @param a lower border of the interval
     * @param b upper border of the interval
     * @return the root
     */
    double bisectionMethod(RealFunction* rf, double a, double b);

    /**
     * The numerical Newtonian iteration method for finding roots of the parameter function.
     * @param r real valued function
     * @param initialGuess initial guess of root
     * @return the root
     */
    double newtonIteration(RealFunction* r, double initialGuess);

    /**
     * The numerical secant method for finding roots of the parameter function.
     * @param r real valued function
     * @param x0 initial guess
     * @param x1 initial guess
     * @return the root
     */
    double secantMethod(RealFunction* r, double x0, double x1);
}

#endif	/* BISECTIONMETHOD_H */

