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

#ifndef CONTINUOUSFEATUREVECTOR_H
#define	CONTINUOUSFEATUREVECTOR_H

#include <openipDS/FeatureVector.h>

namespace openip
{
    /**
    * ContinuousFeatureVector represents a vector of real numbers
    */
    class ContinuousFeatureVector: public FeatureVector<float>
    {
    public:
        using FeatureVector<float>::classIndex;
	/**
	* default constructor
	*/
        ContinuousFeatureVector();

	/**
	* copy constructor
	* @param fv instance to copy
	*/
        ContinuousFeatureVector(const ContinuousFeatureVector& fv);

        /**
         * constructor
         * @param size size of the vector
         */
        ContinuousFeatureVector(int size);

        /**
         * constructor to initialize the vector
         * @param classIndex class index of the vector
         * @param size size of the vector
         * @param ... the float values of the vector
         */
        ContinuousFeatureVector(int classIndex, int size, ...);

        /**
         * constructor to initialize from an existing vector
         * @param classIndex class index of the vector
         * @param v vector object to get the values from it
         */
        template<typename T>
        ContinuousFeatureVector(int classIndex, Vector<T>& v);

	/**
	* destructor
	*/
        ~ContinuousFeatureVector();

	/**
	* assignment operator
	* @param b vector to assign
	*/
        ContinuousFeatureVector& operator=(const ContinuousFeatureVector& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        ContinuousFeatureVector& operator+=(const ContinuousFeatureVector& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        ContinuousFeatureVector& operator-=(const ContinuousFeatureVector& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        ContinuousFeatureVector& operator*=(const ContinuousFeatureVector& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        ContinuousFeatureVector& operator/=(const ContinuousFeatureVector& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        ContinuousFeatureVector operator+(const ContinuousFeatureVector& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        ContinuousFeatureVector operator-(const ContinuousFeatureVector& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        ContinuousFeatureVector operator*(const ContinuousFeatureVector& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        ContinuousFeatureVector operator/(const ContinuousFeatureVector& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        ContinuousFeatureVector& operator=(const float& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        ContinuousFeatureVector& operator+=(const float& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        ContinuousFeatureVector& operator-=(const float& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        ContinuousFeatureVector& operator*=(const float& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        ContinuousFeatureVector& operator/=(const float& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        ContinuousFeatureVector operator+(const float& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        ContinuousFeatureVector operator-(const float& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        ContinuousFeatureVector operator*(const float& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        ContinuousFeatureVector operator/(const float& t) const;
    };


    template<typename T>
    ContinuousFeatureVector::ContinuousFeatureVector(int classIndex, Vector<T>& v)
    : FeatureVector<float>()
    {
        this->resize(v.size());
        this->classIndex= classIndex;

        for ( int i= 0; i < v.size(); ++i )
            (*this)(i)= v(i);
    }

    /**
     * prints the description of the parameter ContinuousFeatureVector to the standard output
     * @param o output stream
     * @param v parameter vector to describe
     * @return reference to the parameter output stream
     */
    std::ostream& operator<<(std::ostream& o, const ContinuousFeatureVector& v);
}


#endif	/* CONTINUOUSFEATUREVECTOR_H */

