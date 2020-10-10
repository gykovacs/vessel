/**
 * @file Distribution.h
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
 * Distribution represents a probability distribution.
 */

#ifndef DISTRIBUTION_H
#define	DISTRIBUTION_H

#include <fstream>

#include <openipDS/Vector.h>
#include <openipDS/Matrix.h>
#include <openipDS/mathFunctions.h>

#include <openipSC/matrixOperations.h>

namespace openip
{
    /**
     * Distribution represents a probability distribution.
     */
    template<typename T>
    class Distribution
    {
    public:
        /**
         * constructor
         */
        Distribution();

        /**
         * copy constructor
         * @param d instance to copy
         */
        Distribution(const Distribution& d);

        /**
         * destructor
         */
        ~Distribution();

        /**
         * probability
         * @param t element of parameter space
         * @return probability/density
         */
        virtual float probability(T& t);

        /**
         * saves the distribution to file
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDistributionToFile(std::ofstream& file);

        /**
         * opens the distribution from file
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDistributionFromFile(std::ifstream& file);

        /**
         * saves the distribution to file
         * @param filename output filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDistributionToFile(char* filename);

        /**
         * opens the distribution from file
         * @param filename input filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDistributionFromFile(char* filename);
    };

    template<typename T>
    Distribution<T>::Distribution()
    {
    }

    template<typename T>
    Distribution<T>::Distribution(const Distribution&)
    {
    }

    template<typename T>
    Distribution<T>::~Distribution()
    {
    }

    template<typename T>
    float Distribution<T>::probability(T&)
    {
        return 0.0f;
    }

    template<typename T>
    int Distribution<T>::saveDistributionToFile(std::ofstream&)
    {
        return 0;
    }

    template<typename T>
    int Distribution<T>::openDistributionFromFile(std::ifstream&)
    {
        return 0;
    }

    template<typename T>
    int Distribution<T>::saveDistributionToFile(char* filename)
    {
        std::ofstream file;
        file.open(filename);
        saveDistributionToFile(filename);
        file.close();

        return 0;
    }

    template<typename T>
    int Distribution<T>::openDistributionFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openDistributionFromFile(filename);
        file.close();
        
        return 0;
    }

    /**
     * writes the descriptor of a Distribution to the parameter output stream
     * @param o output stream
     * @param v Distribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Distribution<T>& v)
    {
        o << "[Distribution - ";
        o << "]";

        return o;
    }
}

#endif	/* DISTRIBUTION_H */

