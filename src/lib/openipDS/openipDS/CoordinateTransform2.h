/**
 * @file CoordinateTransform2.h
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
 * The CoordinateTransform2 class represents a 2 dimensional coordinate transform.
 */

#ifndef _COORDINATE_TRANSFORM_2D_H_
#define _COORDINATE_TRANSFORM_2D_H_

#include <cmath>

#include <openipDS/Matrix2.h>

namespace openip
{
    /**
     * CoordinateTransform2 represents a 2 dimensional coordinate transform
     */
    class CoordinateTransform2: public Matrix2<float>
    {
    public:
        /**
         * default constructor
         */
        CoordinateTransform2();

        /**
         * copy constructor
         * @param m object to copy
         */
        CoordinateTransform2(const CoordinateTransform2& m);

        /**
         * constructor based on a pointer; the first 4 elements of the
         * memory area will be used as the elements of the matrix
         * @param data pointer to at least 4 elements of type T
         */
        CoordinateTransform2(float* data);

        /**
         * constructor based on 4 parameters representing the elements of
         * the matrix
         * @param a element (0,0)
         * @param b element (0,1)
         * @param c element (1,0)
         * @param d element (1,1)
         */
        CoordinateTransform2(float a, float b, float c, float d);

        /**
         * destructor
         */
        ~CoordinateTransform2();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        CoordinateTransform2& operator=(const CoordinateTransform2& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        CoordinateTransform2& operator+=(const CoordinateTransform2& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        CoordinateTransform2& operator-=(const CoordinateTransform2& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        CoordinateTransform2& operator*=(const CoordinateTransform2& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        CoordinateTransform2& operator/=(const CoordinateTransform2& b);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform2 operator+(const CoordinateTransform2& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform2 operator-(const CoordinateTransform2& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform2 operator*(const CoordinateTransform2& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform2 operator/(const CoordinateTransform2& b) const;

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        template<typename T>
        CoordinateTransform2& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        template<typename T>
        CoordinateTransform2& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        template<typename T>
        CoordinateTransform2& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        template<typename T>
        CoordinateTransform2& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        template<typename T>
        CoordinateTransform2& operator/=(const T& t);

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform2 operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform2 operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform2 operator*(const T& t) const;

         /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform2 operator/(const T& t) const;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param t Transform2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const CoordinateTransform2& t);

    /**
     * Rotation2 represents a two dimensional rotation
     */
    class Rotation2: public CoordinateTransform2
    {
            public:
                /**
                 * constructor to create rotation transform
                 * @param a the angle of rotation in radians
                 */
                Rotation2(float a);
    };

    /**
     * row-column composition multiplying operator
     * @param a the left operand
     * @param b the right operand
     * @return a new instance
     */
    CoordinateTransform2 operator^(const CoordinateTransform2& a, const CoordinateTransform2& b);

    /**
     * transform vector multiplication by row-column composition
     * @param m the left matrix operand
     * @param v the right vector operand
     * @return new Vector2 instance
     */
    template<typename T>
    Vector2<T> operator^(const CoordinateTransform2& m, const Vector2<T>& v)
    {
        Vector2<T> tmp;

        tmp.x()= m.a*v.x() + m.b*v.y();
        tmp.y()= m.c*v.x() + m.d*v.y();

        return tmp;
    }

    /**
     * vector transform multiplication by row-column composition
     * @param v the left vector operand
     * @param m the right transform operand
     * @return new Vector2 instance
     */
    template<typename T>
    Vector2<T> operator^(const Vector2<T>& v, const CoordinateTransform2& m)
    {
        Vector2<T> tmp;

        tmp.x()= v.x()*m.a + v.y()*m.c;
        tmp.y()= v.x()*m.b + v.y()*m.d;

        return tmp;
    }

    template<typename T>
    CoordinateTransform2& CoordinateTransform2::operator=(const T& t)
    {
        this->Matrix2<float>::operator=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform2& CoordinateTransform2::operator+=(const T& t)
    {
        this->Matrix2<float>::operator+=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform2& CoordinateTransform2::operator-=(const T& t)
    {
        this->Matrix2<float>::operator-=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform2& CoordinateTransform2::operator*=(const T& t)
    {
        this->Matrix2<float>::operator*=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform2& CoordinateTransform2::operator/=(const T& t)
    {
        this->Matrix2<float>::operator/=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform2 CoordinateTransform2::operator+(const T& t) const
    {
        CoordinateTransform2 n(*this);
        return n+= t;
    }

    template<typename T>
    CoordinateTransform2 CoordinateTransform2::operator-(const T& t) const
    {
        CoordinateTransform2 n(*this);
        return n-= t;
    }

    template<typename T>
    CoordinateTransform2 CoordinateTransform2::operator*(const T& t) const
    {
        CoordinateTransform2 n(*this);
        return n*= t;
    }

    template<typename T>
    CoordinateTransform2 CoordinateTransform2::operator/(const T& t) const
    {
        CoordinateTransform2 n(*this);
        return n/= t;
    }
}

#endif
