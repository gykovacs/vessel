/**
 * @file MultivariateDistribution.h
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
 * MultivariateDistribution represents a multivariate probability distribution.
 */

#ifndef MULTIVARIATEDISTRIBUTION_H
#define	MULTIVARIATEDISTRIBUTION_H

#include <openipML/Distribution.h>

namespace openip
{
    /**
     * MultivariateDistribution represents a multivariate probability distribution.
     */
    template<typename T>
    class MultivariateDistribution: public Distribution<Vector<T> >
    {
    public:
        using Distribution<Vector<T> >::saveDistributionToFile;
        using Distribution<Vector<T> >::openDistributionFromFile;

        /**
         * constructor
         */
        MultivariateDistribution();

        /**
         * copy constructor
         * @param m instance to copy
         */
        MultivariateDistribution(const MultivariateDistribution& m);

        /**
         * destructor
         */
        ~MultivariateDistribution();

        /**
         * probability/density of the parameter
         * @param v parameter
         * @return probability/density
         */
        virtual float probability(Vector<T>& v);
    };

    template<typename T>
    MultivariateDistribution<T>::MultivariateDistribution()
    : Distribution<Vector<T> >()
    {
    }

    template<typename T>
    MultivariateDistribution<T>::MultivariateDistribution(const MultivariateDistribution& m)
    : Distribution<Vector<T> >(m)
    {
    }

    template<typename T>
    MultivariateDistribution<T>::~MultivariateDistribution()
    {
    }

    template<typename T>
    float MultivariateDistribution<T>::probability(Vector<T>&)
    {
        return 0.0f;
    }
}


#endif	/* MULTIVARIATEDISTRIBUTION_H */

