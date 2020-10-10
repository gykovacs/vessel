/**
 * @file RegressionVector.h
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
 * RegressionVector represents a data vector for regression problems.
 */

#ifndef REGRESSIONVECTOR_H
#define	REGRESSIONVECTOR_H

#include <openipDS/Vector.h>

namespace openip
{
    /**
     * RegressionVector represents a data vector for regression problems.
     */
    class RegressionVector: public Vector<float>
    {
    public:
        /**
         * default constructor
         */
        RegressionVector();

        /**
         * copy constructor
         * @param r instance to copy
         */
        RegressionVector(const RegressionVector& r);

        /**
         * destructor
         */
        ~RegressionVector();

        /** target value */
        float value;
    };
}

#endif	/* REGRESSIONVECTOR_H */

