/**
 * @file JointDistribution.h
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
 * JointDistribution represents a set of distributions.
 */

#ifndef JOINTDISTRIBUTION_H
#define	JOINTDISTRIBUTION_H

#include <openipML/Distribution.h>

#include "Distribution.h"

namespace openip
{
    /**
     * JointDistribution represents a set of distributions.
     */
    template<typename T>
    class JointDistribution: public Vector<Distribution<T>* >, public Distribution<Vector<T> >
    {
    public:
        using Distribution<Vector<T> >::saveDistributionToFile;
        using Distribution<Vector<T> >::openDistributionFromFile;
        using Vector<Distribution<T>* >::resize;
        using Vector<Distribution<T>* >::operator();

        /**
         * constructor
         */
        JointDistribution();

        /**
         * copy constructor
         * @param j instance to copy
         */
        JointDistribution(const JointDistribution& j);

        /**
         * destructor
         */
        ~JointDistribution();

        /**
         * returns the value of the density function at parameter v
         * @param v parameter
         * @return density
         */
        virtual float probability(Vector<T>& v);

        /**
         * saves the distribution to output file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDistributionToFile(std::ofstream& file);

        /**
         * opens the distribution from input file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDistributionFromFile(std::ifstream& file);
    };

    template<typename T>
    JointDistribution<T>::JointDistribution()
    : Vector<Distribution<T>* >(), Distribution<Vector<T> >()
    {
    }

    template<typename T>
    JointDistribution<T>::JointDistribution(const JointDistribution& j)
    : Vector<Distribution<T>* >(j), Distribution<Vector<T> >(j)
    {
    }

    template<typename T>
    JointDistribution<T>::~JointDistribution()
    {
    }

    template<typename T>
    float JointDistribution<T>::probability(Vector<T>& v)
    {
        float prob= 1.0f;
        for ( unsigned int i= 0; i < v.size(); ++i )
            prob*= (*this)(i)->probability(v(i));

        return prob;
    }

    template<typename T>
    int JointDistribution<T>::saveDistributionToFile(std::ofstream& file)
    {
        //printf("ccc\n"); fflush(stdout);
        file << this->size() << std::endl;
        for ( unsigned int i= 0; i < this->size(); ++i )
            ((*this)(i))->saveDistributionToFile(file);
        return 0;
    }

    template<typename T>
    int JointDistribution<T>::openDistributionFromFile(std::ifstream& file)
    {
        file >> (*this);
        return 0;
    }

    /**
     * writes the descriptor of the parameter JointDistribution object to the parameter output stream
     * @param o output stream
     * @param v JointDistribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const openip::JointDistribution<T>& v)
    {
        o << "[JointDistribution - size: ";
        o << v.size();
        o << " :";

        if ( v.size() < 100 )
        {
            for ( typename Vector<Distribution<T>* >::const_iterator it= v.begin(); it != v.end(); ++it )
            {
                o << " ";
                o << **it;
            }
        }

        o << "]";

        return o;
    }

    /**
     * writes the descriptor of the parameter JointDistribution object to the parameter output file stream
     * @param file output file stream
     * @param v JointDistribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, const openip::JointDistribution<T>& v)
    {
        for ( int i= 0; i < v.size(); ++i )
            file << *(v(i));

        return file;
    }

    /**
     * fills the parameter JointDistribution object from the parameter input file stream
     * @param o input stream
     * @param v JointDistribution object
     * @return reference to the parameter input stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, openip::JointDistribution<T>&)
    {
        return o;
    }
}

#endif	/* JOINTDISTRIBUTION_H */

