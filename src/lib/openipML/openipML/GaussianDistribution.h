/**
 * @file GaussianDistribution.h
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
 * This file contains some classes representing Gaussian distributions.
 */

#ifndef GAUSSIANDISTRIBUTION_H
#define	GAUSSIANDISTRIBUTION_H

#include <openipDS/DataSet.h>

#include <openipML/UnivariateDistribution.h>
#include <openipML/MultivariateDistribution.h>
#include <openipML/JointDistribution.h>

#include "Distribution.h"

namespace openip
{
    /**
     * UnivariateGaussianDistribution represents univariate Gaussian distribution.
     */
    template<typename T>
    class UnivariateGaussianDistribution: public UnivariateDistribution<T>
    {
    public:
        using UnivariateDistribution<T>::saveDistributionToFile;
        using UnivariateDistribution<T>::openDistributionFromFile;

        /**
         * default constructor
         */
        UnivariateGaussianDistribution();

        /**
         * constructor
         * @param m mean
         * @param v variance
         */
        UnivariateGaussianDistribution(float m, float v);

        /**
         * copy constructor
         * @param u instance to copy
         */
        UnivariateGaussianDistribution(const UnivariateGaussianDistribution& u);

        /**
         * destructor
         */
        ~UnivariateGaussianDistribution();

        /**
         * returns the value of the density function at the parameter
         * @param t parameter
         * @return density
         */
        virtual float probability(T& t);

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

        /** mean */
        float m;
        /** variance */
        float v;
    };

    template<typename T>
    UnivariateGaussianDistribution<T>::UnivariateGaussianDistribution()
    : UnivariateDistribution<T>()
    {
        m= 0;
        v= 1;
    }

    template<typename T>
    UnivariateGaussianDistribution<T>::UnivariateGaussianDistribution(float m, float v)
    : UnivariateDistribution<T>()
    {
        this->m= m;
        this->v= v;
    }

    template<typename T>
    UnivariateGaussianDistribution<T>::UnivariateGaussianDistribution(const UnivariateGaussianDistribution& u)
    : UnivariateDistribution<T>(u)
    {
        this->m= u.m;
        this->v= u.v;
    }

    template<typename T>
    UnivariateGaussianDistribution<T>::~UnivariateGaussianDistribution()
    {
    }

    template<typename T>
    float UnivariateGaussianDistribution<T>::probability(T& t)
    {
        //if ( m != -64 )
        //printf("m: %f ", m);
        return gauss1(m-t, v);
    }

    template<typename T>
    int UnivariateGaussianDistribution<T>::saveDistributionToFile(std::ofstream& file)
    {
        //printf("bbb\n"); fflush(stdout);
        file << this->m << " ";
        file << this->v << std::endl;
        return 0;
    }

    template<typename T>
    int UnivariateGaussianDistribution<T>::openDistributionFromFile(std::ifstream& file)
    {
        file >> this->m;
        file >> this->v;
        return 0;
    }

    /**
     * writes the descriptor of a UnivariateGaussianDistribution to the parameter output stream
     * @param o output stream
     * @param v UnivariateGaussianDistribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const UnivariateGaussianDistribution<T>& v)
    {
        o << "[UnivariateGaussianDistribution - size: ";
        o << " :";

        o << v.m;
        o << " ";
        o << v.v;

        o << "]";

        return o;
    }

    /**
     * MultivariateGaussianDistribution represents a multivariate Gaussian distribution.
     */
    template<typename T>
    class MultivariateGaussianDistribution: public MultivariateDistribution<T>
    {
    public:
        using MultivariateDistribution<T>::saveDistributionToFile;
        using MultivariateDistribution<T>::openDistributionFromFile;

        /**
         * default constructor
         */
        MultivariateGaussianDistribution();

        /**
         * constructor
         * @param m mean vector
         * @param v covariance matrix
         */
        MultivariateGaussianDistribution(Vector<T> m, Matrix<T> v);

        /**
         * copy constructor
         * @param u instance to copy
         */
        MultivariateGaussianDistribution(const MultivariateGaussianDistribution& u);

        /**
         * destructor
         */
        ~MultivariateGaussianDistribution();

        virtual void setParameters(Vector<T> m, Matrix<T> v);

        /**
         * returns the value of the density function at the parameter
         * @param t parameter
         * @return density
         */
        virtual float probability(Vector<T>& t);

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

        /** mean vector */
        Vector<T> mean;
        /** covariance matrix */
        Matrix<T> covariance;
        /** inverse of the covariance matrix */
        Matrix<T> covarianceInverse;
        /** determinant of the covariance matrix */
        float covarianceDeterminant;
    };

    template<typename T>
    MultivariateGaussianDistribution<T>::MultivariateGaussianDistribution()
    : MultivariateDistribution<T>()
    {
        mean.resize(1);
        covariance.resize(1,1);
        covarianceInverse.resize(1,1);
        mean(0)= 0;
        covariance(0)= 1;
        covarianceInverse(0)= 1;
        covarianceDeterminant= 1;
    }

    template<typename T>
    MultivariateGaussianDistribution<T>::MultivariateGaussianDistribution(Vector<T> mean, Matrix<T> cov)
    : MultivariateDistribution<T>()
    {
        this->mean= mean;
        this->covariance= cov;
        this->covarianceInverse.resize(this->covariance.rows, this->covariance.columns);
        invertSquareMatrix(covariance.data(), covariance.rows, covarianceInverse.data());
        covarianceDeterminant= determinantSquareMatrix(covariance.data(), covariance.rows);
    }

    template<typename T>
    MultivariateGaussianDistribution<T>::MultivariateGaussianDistribution(const MultivariateGaussianDistribution& u)
    : MultivariateDistribution<T>(u)
    {
        this->mean= u.mean;
        this->covariance= u.covariance;
        this->covarianceInverse= u.covarianceInverse;
        this->covarianceDeterminant= u.covarianceDeterminant;
    }

    template<typename T>
    MultivariateGaussianDistribution<T>::~MultivariateGaussianDistribution()
    {
    }

    template<typename T>
    void MultivariateGaussianDistribution<T>::setParameters(Vector<T> m, Matrix<T> v)
    {
        mean= m;
        covariance= v;
        this->covarianceInverse.resize(this->covariance.rows, this->covariance.columns);
        invertSquareMatrix(covariance.data(), covariance.rows, covarianceInverse.data());
        covarianceDeterminant= determinantSquareMatrix(covariance.data(), covariance.rows);
        cout << mean << endl;
        cout << covariance << endl;
        cout << covarianceInverse << endl;
        cout << covarianceDeterminant << endl;
    }

    template<typename T>
    float MultivariateGaussianDistribution<T>::probability(Vector<T>& t)
    {
        Vector<T> tmp= t - mean;
        return 1.0/(sqrt(pow(2*M_PI, t.size())*covarianceDeterminant))*exp(-1.0/2*(tmp^(covarianceInverse^tmp)));
    }

    template<typename T>
    int MultivariateGaussianDistribution<T>::saveDistributionToFile(std::ofstream& file)
    {
        //printf("ddd\n"); fflush(stdout);
        file << this->mean;
        file << this->covariance;
        file << this->covarianceInverse;
        file << this->covarianceDeterminant;
        return 0;
    }

    template<typename T>
    int MultivariateGaussianDistribution<T>::openDistributionFromFile(std::ifstream& file)
    {
        file >> this->mean;
        file >> this->covariance;
        file >> this->covarianceInverse;
        file >> this->covarianceDeterminant;

        return 0;
    }

    /**
     * GaussianDistribution represents a univariate or multivariate Gaussian distribution
     */
    template<typename T>
    class GaussianDistribution: public UnivariateGaussianDistribution<T>, public MultivariateGaussianDistribution<T>
    {
    public:
        using UnivariateGaussianDistribution<T>::probability;
        using MultivariateGaussianDistribution<T>::probability;

        /**
         * constructor
         * @param m mean
         * @param v variance
         */
        GaussianDistribution(T m, T v);

        /**
         * constructor
         * @param m mean vector
         * @param c covariance matrix
         */
        GaussianDistribution(Vector<T>& m, Matrix<T>& c);

        /**
         * copy constructor
         */
        GaussianDistribution(const GaussianDistribution& g);

        /**
         * destructor
         */
        ~GaussianDistribution();

        /**
         * saves the distribution to file
         * @param filename file name
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDistributionToFile(char* filename);

        /**
         * opens the distribution from file
         * @param filename file name
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDistributionFromFile(char* filename);

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
    GaussianDistribution<T>::GaussianDistribution(T m, T v)
    : UnivariateGaussianDistribution<T>(m, v), MultivariateGaussianDistribution<T>()
    {
    }

    template<typename T>
    GaussianDistribution<T>::GaussianDistribution(Vector<T>& m, Matrix<T>& c)
    : UnivariateGaussianDistribution<T>(), MultivariateGaussianDistribution<T>(m, c)
    {
    }

    template<typename T>
    GaussianDistribution<T>::GaussianDistribution(const GaussianDistribution& g)
    : UnivariateGaussianDistribution<T>(g), MultivariateGaussianDistribution<T>(g)
    {
    }

    template<typename T>
    GaussianDistribution<T>::~GaussianDistribution()
    {
    }

    template<typename T>
    int GaussianDistribution<T>::saveDistributionToFile(char* filename)
    {
        //printf("eee\n"); fflush(stdout);
        std::ofstream file;
        file.open(filename);
        saveDistributionToFile(file);
        file.close();

        return 0;
    }

    template<typename T>
    int GaussianDistribution<T>::openDistributionFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openDistributionFromFile(file);
        file.close();

        return 0;
    }

    template<typename T>
    int GaussianDistribution<T>::saveDistributionToFile(std::ofstream& file)
    {
        //printf("fff\n"); fflush(stdout);
        this->UnivariateGaussianDistribution<T>::saveDistributionToFile(file);
        this->MultivariateGaussianDistribution<T>::saveDistributionToFile(file);

        return 0;
    }

    template<typename T>
    int GaussianDistribution<T>::openDistributionFromFile(std::ifstream& file)
    {
        this->UnivariateGaussianDistribution<T>::openDistributionFromFile(file);
        this->MultivariateGaussianDistribution<T>::openDistributionFromFile(file);

        return 0;
    }

    /**
     * UnivariateGaussianJointDistribution represents a joint distribution of univariate
     * Gaussian distributions.
     */
    template<typename T>
    class UnivariateGaussianJointDistribution: public JointDistribution<T>
    {
    public:
        using JointDistribution<T>::probability;
        using JointDistribution<T>::saveDistributionToFile;
        using JointDistribution<T>::openDistributionFromFile;
        using JointDistribution<T>::resize;

        /**
         * default constructor
         */
        UnivariateGaussianJointDistribution();

        /**
         * constructor
         * @param means means of the distributions
         * @param vars variances of the distributions
         */
        UnivariateGaussianJointDistribution(Vector<T>& means, Vector<T>& vars);

        /**
         * copy constructor
         * @param u instance to copy
         */
        UnivariateGaussianJointDistribution(const UnivariateGaussianJointDistribution& u);

        /**
         * destructor
         */
        ~UnivariateGaussianJointDistribution();

        /**
         * extracts the distribution from the parameter dataset
         * @param d dataset to extract the distributions from
         */
        void extract(DataSet<T>& d);

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
    UnivariateGaussianJointDistribution<T>::UnivariateGaussianJointDistribution()
    : JointDistribution<T>()
    {
    }

    template<typename T>
    UnivariateGaussianJointDistribution<T>::UnivariateGaussianJointDistribution(Vector<T>& means, Vector<T>& vars)
    : JointDistribution<T>()
    {
        for ( unsigned int i= 0; i < means.size(); ++i )
            this->push_back(new UnivariateGaussianDistribution<T>(means(i), vars(i)));
    }

    template<typename T>
    UnivariateGaussianJointDistribution<T>::UnivariateGaussianJointDistribution(const UnivariateGaussianJointDistribution& u)
    : JointDistribution<T>(u)
    {
    }

    template<typename T>
    UnivariateGaussianJointDistribution<T>::~UnivariateGaussianJointDistribution()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            delete (*this)[i];
    }

    template<typename T>
    void UnivariateGaussianJointDistribution<T>::extract(DataSet<T>& d)
    {

    }

    template<typename T>
    int UnivariateGaussianJointDistribution<T>::saveDistributionToFile(std::ofstream& file)
    {
        //printf("aaa\n"); fflush(stdout);
        file << this->size() << std::endl;
        for ( unsigned int i= 0; i < this->size(); ++i )
            file << *(dynamic_cast<UnivariateGaussianDistribution<T>* >((*this)(i)));
        return 0;
    }

    template<typename T>
    int UnivariateGaussianJointDistribution<T>::openDistributionFromFile(std::ifstream& file)
    {
        //printf("zzz\n"); fflush(stdout);
        int k;
        file >> k;
        UnivariateGaussianDistribution<T>* ugd;

        for ( int i= 0; i < k; ++i )
        {
            ugd= new UnivariateGaussianDistribution<T>();
            file >> *ugd;
            cout << "---" << *ugd << endl;
            this->push_back(ugd);
        }

        return 0;
    }

    /**
     * writes the descriptor of a UnivariateGaussianDistribution to the parameter output stream
     * @param file output stream
     * @param ugd UnivariateGaussianDistribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, UnivariateGaussianDistribution<T>& ugd)
    {
        //printf("y %f %f\n", ugd.m, ugd.v); fflush(stdout);
        file << ugd.m << " " << ugd.v << std::endl;

        return file;
    }

    /**
     * fills an UnivariateGaussianDistribution object from the standard input
     * @param file output stream
     * @param ugd UnivariateGaussianDistribution object
     * @return reference to the parameter input stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, UnivariateGaussianDistribution<T>& ugd)
    {
        file >> ugd.m;
        file >> ugd.v;

        return file;
    }

    /**
     * writes the descriptor of a MultivariateGaussianDistribution to the parameter output stream
     * @param file output stream
     * @param mgd Distribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, MultivariateGaussianDistribution<T>& mgd)
    {
        file << mgd.mean << std::endl;
        file << mgd.covariance << std::endl;
        file << mgd.covarianceInverse << std::endl;
        file << mgd.covarianceDeterminant << std::endl;

        return file;
    }

    /**
     * fills a MultivariateGaussianDistribution object from the standard input
     * @param file output stream
     * @param mgd UnivariateGaussianDistribution object
     * @return reference to the parameter input stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, MultivariateGaussianDistribution<T>& mgd)
    {
        file >> mgd.mean;
        file >> mgd.covariance;
        file >> mgd.covarianceInverse;
        file >> mgd.covarianceDeterminant;

        return file;
    }

    /**
     * writes the descriptor of an UnivariateGaussianJointDistribution to the parameter output stream
     * @param file output stream
     * @param u Distribution object
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, UnivariateGaussianJointDistribution<T>& u)
    {
        file << u.size() << std::endl;
        for ( int i= 0; i < u.size(); ++i )
            file << u(i);

        return file;
    }

    /**
     * fills a UnivariateGaussianJointDistribution object from the standard input
     * @param file output stream
     * @param u UnivariateGaussianDistribution object
     * @return reference to the parameter input stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, UnivariateGaussianJointDistribution<T>& u)
    {
        int size;
        file >> size;

        for ( int i= 0; i < size; ++i )
        {
            GaussianDistribution<T>* gd= new GaussianDistribution<T>(0, 0);
            file >> (*gd);
            u.push_back(gd);
        }


        return file;
    }
}

#endif	/* GAUSSIANDISTRIBUTION_H */

