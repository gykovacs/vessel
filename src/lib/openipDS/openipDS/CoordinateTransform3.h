/**
 * @file Transform3.h
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
 * The Transform3 class represents a 3 dimensional coordinate transform.
 */

#ifndef _TRANSFORM_3D_H_
#define _TRANSFORM_3D_H_

#include <cmath>

#include <openipDS/Matrix3.h>
#include <openipDS/Vector3.h>

namespace openip
{
    /**
     * Transform3 represents a 3 dimensional coordinate transform.
     */
    class CoordinateTransform3: public Matrix3<float>
    {
    public:
        /**
         * default constructor
         */
        CoordinateTransform3();

        /**
         * copy constructor
         * @param m instance to copy
         */
        CoordinateTransform3(const CoordinateTransform3& m);

        /**
         * constructor based on a pointer; the first 4 elements of the
         * memory area will be used as the elements of the matrix
         * @param data pointer to at least 4 elements of type T
         */
        CoordinateTransform3(float* data);

        /**
         * constructor based on 9 parameters representing the elements of
         * the matrix
         * @param a element (0,0)
         * @param b element (0,1)
         * @param c element (0,2)
         * @param d element (1,0)
         * @param e element (1,1)
         * @param f element (1,2)
         * @param g element (2,0)
         * @param h element (2,1)
         * @param i element (2,2)
         */
        CoordinateTransform3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

        /**
         * destructor
         */
        ~CoordinateTransform3();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        CoordinateTransform3& operator=(const CoordinateTransform3& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        CoordinateTransform3& operator+=(const CoordinateTransform3& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        CoordinateTransform3& operator-=(const CoordinateTransform3& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        CoordinateTransform3& operator*=(const CoordinateTransform3& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        CoordinateTransform3& operator/=(const CoordinateTransform3& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        template<typename T>
        CoordinateTransform3& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        template<typename T>
        CoordinateTransform3& operator+=(const T& t);

         /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        template<typename T>
        CoordinateTransform3& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        template<typename T>
        CoordinateTransform3& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        template<typename T>
        CoordinateTransform3& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform3 operator+(const CoordinateTransform3& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform3 operator-(const CoordinateTransform3& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform3 operator*(const CoordinateTransform3& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        CoordinateTransform3 operator/(const CoordinateTransform3& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform3 operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform3 operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform3 operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        template<typename T>
        CoordinateTransform3 operator/(const T& t) const;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param t Transform3 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const CoordinateTransform3& t);

    /**
     * transform transform multiplication
     * @param a left operand
     * @param b right operand
     * @return new transform instance
     */
    CoordinateTransform3 operator^(const CoordinateTransform3& a, const CoordinateTransform3& b);

    /**
     * transform vector multiplication by row-column composition
     * @param m transform as left operand
     * @param v vector as right operand
     * @return new vector instance
     */
    template<typename T>
    Vector3<T> operator^(const CoordinateTransform3& m, const Vector3<T>& v)
    {
        Vector3<T> tmp;

        tmp.x()= m.a*v.x() + m.b*v.y() + m.c*v.z();
        tmp.y()= m.d*v.x() + m.e*v.y() + m.f*v.z();
        tmp.z()= m.g*v.x() + m.h*v.y() + m.i*v.z();

        return tmp;
    }

    /**
     * vector transform multiplication by row-column composition
     * @param v vector as left operand
     * @param m transform as right operand
     * @return new vector instance
     */
    template<typename T>
    Vector3<T> operator^(const Vector3<T>& v, const CoordinateTransform3& m)
    {
        Vector3<T> tmp;

        tmp.x()= v.x()*m.a + v.y()*m.d + v.z()*m.g;
        tmp.y()= v.x()*m.b + v.y()*m.e + v.z()*m.h;
        tmp.z()= v.x()*m.c + v.y()*m.f + v.z()*m.i;

        return tmp;
    }

    template<typename T>
    CoordinateTransform3& CoordinateTransform3::operator=(const T& t)
    {
        this->Matrix3<float>::operator=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform3& CoordinateTransform3::operator+=(const T& t)
    {
        this->Matrix3<float>::operator+=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform3& CoordinateTransform3::operator-=(const T& t)
    {
        this->Matrix3<float>::operator-=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform3& CoordinateTransform3::operator*=(const T& t)
    {
        this->Matrix3<float>::operator*=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform3& CoordinateTransform3::operator/=(const T& t)
    {
        this->Matrix3<float>::operator/=(t);
        return *this;
    }

    template<typename T>
    CoordinateTransform3 CoordinateTransform3::operator+(const T& t) const
    {
        CoordinateTransform3 n(*this);
        return n+= t;
    }

    template<typename T>
    CoordinateTransform3 CoordinateTransform3::operator-(const T& t) const
    {
        CoordinateTransform3 n(*this);
        return n-= t;
    }

    template<typename T>
    CoordinateTransform3 CoordinateTransform3::operator*(const T& t) const
    {
        CoordinateTransform3 n(*this);
        return n*= t;
    }

    template<typename T>
    CoordinateTransform3 CoordinateTransform3::operator/(const T& t) const
    {
        CoordinateTransform3 n(*this);
        return n/= t;
    }
}

#endif
