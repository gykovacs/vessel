/**
 * @file RealFunction.h
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
 * RealFunction objects represent real valued functions defined on the domain of
 * real numbers.
 */

#ifndef REALFUNCTION_H
#define	REALFUNCTION_H

namespace openip
{
    /**
     * RealFunction objects represent real valued functions defined on the domain of real numbers.
     */
    class RealFunction
    {
    public:
        /**
         * default constructor
         */
        RealFunction();

        /**
         * copy constructor
         * @param r instance to copy
         */
        RealFunction(const RealFunction& r);

        /**
         * destructor
         */
        ~RealFunction();

        /**
         * evaluates the function at x
         * @return the function value
         */
        virtual double value(double) { return 0.0; }

        /**
         * evaluates the derivative of the function at x
         * @return the derivative function value
         */
        virtual double derivative(double) { return 0.0; }

        /**
         * evaluates the second derivative of the function at x
         * @return the second derivative function value
         */
        virtual double secondDerivative(double) { return 0.0; }
    };
}
#endif	/* REALFUNCTION_H */

