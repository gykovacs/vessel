/**
 * @file ContinuousFeatureVector.h
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
 * The ContinuousFeatureVector class represents a FeatureVector containing
 * real (float) values.
 */

#ifndef DISCRETEFEATUREVECTOR_H
#define	DISCRETEFEATUREVECTOR_H

#include <openipDS/FeatureVector.h>

namespace openip
{
    /**
    * FeatureVector represents a vector of features
    */
    class DiscreteFeatureVector: public FeatureVector<int>
    {
    public:
        using FeatureVector<int>::classIndex;
	/**
	* default constructor
	*/
        DiscreteFeatureVector();

	/**
	* copy constructor
	* @param fv instance to copy
	*/
        DiscreteFeatureVector(const DiscreteFeatureVector& fv);

        /**
         * constructor
         * @param size size of the vector
         */
        DiscreteFeatureVector(int size);

        /**
         * constructor to initialize the vector
         * @param classIndex class index of the vector
         * @param size size of the vector
         * @param ... elements of the vector
         */
        DiscreteFeatureVector(int classIndex, int size, ...);

        /**
         * constructor to initialize the vector
         * @param classIndex class index of the vector
         * @param v vector instance containing the elements of the feature vector
         */
        template<typename T>
        DiscreteFeatureVector(int classIndex, Vector<T>& v);

	/**
	* destructor
	*/
        ~DiscreteFeatureVector();

	/**
	* assignment operator
	* @param b vector to assign
	*/
        DiscreteFeatureVector& operator=(const DiscreteFeatureVector& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        DiscreteFeatureVector& operator+=(const DiscreteFeatureVector& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        DiscreteFeatureVector& operator-=(const DiscreteFeatureVector& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        DiscreteFeatureVector& operator*=(const DiscreteFeatureVector& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        DiscreteFeatureVector& operator/=(const DiscreteFeatureVector& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        DiscreteFeatureVector operator+(const DiscreteFeatureVector& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        DiscreteFeatureVector operator-(const DiscreteFeatureVector& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        DiscreteFeatureVector operator*(const DiscreteFeatureVector& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        DiscreteFeatureVector operator/(const DiscreteFeatureVector& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        DiscreteFeatureVector& operator=(const float& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        DiscreteFeatureVector& operator+=(const float& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        DiscreteFeatureVector& operator-=(const float& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        DiscreteFeatureVector& operator*=(const float& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        DiscreteFeatureVector& operator/=(const float& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        DiscreteFeatureVector operator+(const float& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        DiscreteFeatureVector operator-(const float& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        DiscreteFeatureVector operator*(const float& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        DiscreteFeatureVector operator/(const float& t) const;

    };

    template<typename T>
    DiscreteFeatureVector::DiscreteFeatureVector(int classIndex, Vector<T>& v)
    : FeatureVector<int>()
    {
        this->resize(v.size());
        this->classIndex= classIndex;

        for ( int i= 0; i < v.size(); ++i )
            (*this)(i)= v(i);
    }

    std::ostream& operator<<(std::ostream& o, const DiscreteFeatureVector& v);
}


#endif	/* DISCRETEFEATUREVECTOR_H */

