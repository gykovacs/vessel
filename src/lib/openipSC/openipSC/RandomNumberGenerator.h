/**
 * @file RandomNumberGenerator.h
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
 * This file contains random number generator classes, based on GNU GSL
 */

#ifndef _RANDOM_NUMBER_GENERATOR_H_
#define _RANDOM_NUMBER_GENERATOR_H_

#include <gsl/gsl_rng.h>

namespace openip
{
    /**
     * RandomIntegerGenerator to generate random integers in a range
     */
    class UniformRandomIntegerGenerator
    {
    public:
        /**
         * default constructor of the random integer generator
         */
        UniformRandomIntegerGenerator(int n= -1);

        /**
         * returns the next non-negative integer number smaller then n
         * @param n upper limit of the range
         * @return the next random integer
         */
        unsigned long int generate(int n= -1);

        /**
         * gsl object for random number generation
         */
        gsl_rng* r;

        /** range */
        int n;
    };

    /**
     * RandomFloatGenerator to generate random floating point numbers
     */
    class UniformRandomFloatGenerator
    {
    public:
        /**
         * RandomFloatGenerator default constructor
         */
        UniformRandomFloatGenerator(float a= 0.0, float b= 1.0);

        /**
         * returns the next random float number
         * @return the next random float number
         */
        float generate();

        /**
         * gsl object for random number generation
         */
        gsl_rng* r;

        /**
         * lower border of the range
         */
        float a;

        /**
         * upper border of the range
         */
        float b;
    };

    /**
     * GaussianRandomNumberGenerator class generates random numbers from a
     * Gaussian distribution
     */
    class GaussianRandomFloatGenerator
    {
    public:
        /**
         * GaussianRandomNumberGenerator constructor
         * @param mean mean of the Gaussian distribution
         * @param dev deviation of the Gaussian distribution
         */
        GaussianRandomFloatGenerator(float mean= 0, float dev= 1);

        virtual ~GaussianRandomFloatGenerator();
        /**
         * returns the next random number from the specified Gaussian
         * distribution
         * @return next random number from the specified Gaussian distribution
         */
        virtual float generate();

        /**
         * gsl object for random number generation
         */
        gsl_rng * r;

        /**
         * mean of the random numbers to generate
         */
        float mean;

        /**
         * deviation of the random numbers to generate
         */
        float dev;
    };
    
    class PoissonRandomFloatGenerator
    {
    public:
        /**
         * GaussianRandomNumberGenerator constructor
         * @param mean mean of the Gaussian distribution
         * @param dev deviation of the Gaussian distribution
         */
        PoissonRandomFloatGenerator(float mean);

        /**
         * returns the next random number from the specified Gaussian
         * distribution
         * @return next random number from the specified Gaussian distribution
         */
        virtual float generate();

        /**
         * gsl object for random number generation
         */
        gsl_rng * r;

        /**
         * mean of the random numbers to generate
         */
        float mean;
    };
}

#endif
