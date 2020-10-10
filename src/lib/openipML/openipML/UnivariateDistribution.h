/**
 * @file UnivariateDistribution.h
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
 * UnivariateDistribution represents an univariate probability distribution.
 */

#ifndef UNIVARIATEDISTRIBUTION_H
#define	UNIVARIATEDISTRIBUTION_H

#include <openipML/Distribution.h>

namespace openip
{
    /**
     * UnivariateDistribution represents an univariate probability distribution.
     */
    template<typename T>
    class UnivariateDistribution: public Distribution<T>
    {
    public:
        using Distribution<T>::saveDistributionToFile;
        using Distribution<T>::openDistributionFromFile;

        /**
         * constructor
         */
        UnivariateDistribution();

        /**
         * copy constructor
         * @param u instance to copy
         */
        UnivariateDistribution(const UnivariateDistribution& u);

        /**
         * destructor
         */
        ~UnivariateDistribution();

        /**
         * probability/density of the parameter
         * @param f parameter
         * @return probability/density function value of the parameter
         */
        virtual float probability(T& f);
    };

    template<typename T>
    UnivariateDistribution<T>::UnivariateDistribution()
    : Distribution<T>()
    {
    }

    template<typename T>
    UnivariateDistribution<T>::UnivariateDistribution(const UnivariateDistribution& u)
    : Distribution<T>(u)
    {
    }

    template<typename T>
    UnivariateDistribution<T>::~UnivariateDistribution()
    {
    }

    template<typename T>
    float UnivariateDistribution<T>::probability(T&)
    {
        return 0.0f;
    }
}

#endif	/* UNIVARIATEDISTRIBUTION_H */

