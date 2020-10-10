/**
 * @file Vector4.h
 * @author Janos Istvan Ivan <dzsekob@gmail.com>
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
 * The Vector3 class represents an 3 dimensional vector and has
 * several useful operators and member functions defined.
 */

#ifndef VECTOR4_H
#define	VECTOR4_H

#include <cmath>
#include <vector>
#include <iostream>

namespace openip
{
    /**
     * Vector4 represents a Vector of static size 4 with template type T
     */
    template<typename T>
    class Vector4
    {
    public:
        /**
         * Default constructor, creates a new vector instance with size 4
         * and type appropriate to the template paramter
         */
        Vector4();


        /**
         * Similar to the default constructor, but initializes the
         * vector with parameter values.
         */
        Vector4(T x_, T y_, T z_, T _w);

        /**
         * Copy constructor
         *
         * @param p instance to copy
         */
        Vector4(const Vector4& p);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator==(const Vector4& b);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator!=(const Vector4& b);


        /**
         * assignment operator
         * @param b object to assign to this
         */
        Vector4& operator=(const Vector4& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Vector4& operator+=(const Vector4& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Vector4& operator-=(const Vector4& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Vector4& operator*=(const Vector4& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Vector4& operator/=(const Vector4& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector4 operator+(const Vector4& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector4 operator-(const Vector4& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector4 operator*(const Vector4& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector4 operator/(const Vector4& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        Vector4& operator=(const T& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector4& operator+=(const T& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector4& operator-=(const T& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector4& operator*=(const T& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector4& operator/=(const T& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector4 operator+(const T& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector4 operator-(const T& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector4 operator*(const T& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector4 operator/(const T& t) const;

        /**
         * set method for x coordinate
         *
         * @return x
         */
        inline T&  x();

        /**
         * get method for x coordinate
         *
         * @return x
         */
        inline T  x() const;

        /**
         * set method for y coordinate
         *
         * @return y
         */
        inline T&  y();

        /**
         * get method for y coordinate
         *
         * @return y
         */
        inline T  y() const;

        /**
         * set method for z coordinate
         *
         * @return z
         */
        inline T&  z();

        /**
         * get method for z coordinate
         *
         * @return z
         */
        inline T  z() const;

        /**
         * set method for z coordinate
         *
         * @return z
         */
        inline T&  w();

        /**
         * get method for z coordinate
         *
         * @return z
         */
        inline T  w() const;


        /**
         * inner product with vector b
         * @param b the vector to compute inner product with
         * @return the inner product value
         */
        T inner(const Vector4& b);

        /**
         * normalizes the elements of the vector
         */
        void normalize();

        /**
         * computes the square of the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist(const Vector4<T2>& v) const;

        /**
         * computes the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist2(const Vector4<T2>& v) const;

        /**
         * x coordinate
         */
        T x_;

        /**
         * y coordinate
         */
        T y_;

        /**
         * z coordinate
         */
        T z_;

        /**
         * z coordinate
         */
        T w_;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v Vector4 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Vector4<T>& v);
}

template<typename T>
openip::Vector4<T>::Vector4()
{
    this->x()= 0;
    this->y()= 0;
    this->z()= 0;
    this->w()= 0;
}

template<typename T>
openip::Vector4<T>::Vector4(T x, T y, T z, T w)
{
    this->x()= x;
    this->y()= y;
    this->z()= z;
    this->w()= w;
}

template<typename T>
openip::Vector4<T>::Vector4(const Vector4<T>& v)
{
    this->x()= v.x();
    this->y()= v.y();
    this->z()= v.z();
    this->w()= v.w();
}

template<typename T>
bool openip::Vector4<T>::operator==(const Vector4& b)
{
    return this->x()==b.x() && this->y()==b.y() && this->z()==b.z() && this->w()==b.w();
}

template<typename T>
bool openip::Vector4<T>::operator!=(const Vector4& b)
{
    return ! this->operator==(b);
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator=(const Vector4<T>& b)
{
    this->x()= b.x();
    this->y()= b.y();
    this->z()= b.z();
    this->w()= b.w();

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator+=(const Vector4<T>& b)
{
    this->x()+= b.x();
    this->y()+= b.y();
    this->z()+= b.z();
    this->w()+= b.w();

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator-=(const Vector4<T>& b)
{
    this->x()-= b.x();
    this->y()-= b.y();
    this->z()-= b.z();
    this->w()-= b.w();

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator*=(const Vector4<T>& b)
{
    this->x()*= b.x();
    this->y()*= b.y();
    this->z()*= b.z();
    this->w()*= b.w();

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator/=(const Vector4<T>& b)
{
    this->x()/= b.x();
    this->y()/= b.y();
    this->z()/= b.z();
    this->w()/= b.w();

    return *this;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator+(const Vector4<T>& b) const
{
    openip::Vector4<T> n(*this);

    return n+= b;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator-(const Vector4<T>& b) const
{
    openip::Vector4<T> n(*this);

    return n-= b;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator*(const Vector4<T>& b) const
{
    openip::Vector4<T> n(*this);

    return n*= b;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator/(const Vector4<T>& b) const
{
    openip::Vector4<T> n(*this);

    return n/= b;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator=(const T& t)
{
    this->x()= this->y()= this->z()= this->w()= t;

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator+=(const T& t)
{
    this->x()+= t;
    this->y()+= t;
    this->z()+= t;
    this->w()+= t;

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator-=(const T& t)
{
    this->x()-= t;
    this->y()-= t;
    this->z()-= t;
    this->w()-= t;

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator*=(const T& t)
{
    this->x()*= t;
    this->y()*= t;
    this->z()*= t;
    this->w()*= t;

    return *this;
}

template<typename T>
openip::Vector4<T>& openip::Vector4<T>::operator/=(const T& t)
{
    this->x()/= t;
    this->y()/= t;
    this->z()/= t;
    this->w()/= t;

    return *this;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator+(const T& t) const
{
    openip::Vector4<T> n(*this);

    return n+= t;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator-(const T& t) const
{
    openip::Vector4<T> n(*this);

    return n-= t;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator*(const T& t) const
{
    openip::Vector4<T> n(*this);

    return n*= t;
}

template<typename T>
openip::Vector4<T> openip::Vector4<T>::operator/(const T& t) const
{
    openip::Vector4<T> n(*this);

    return n/= t;
}


template<typename T>
T& openip::Vector4<T>::x()
{
    return x_;
}

template<typename T>
T& openip::Vector4<T>::y()
{
    return y_;
}

template<typename T>
T& openip::Vector4<T>::z()
{
    return z_;
}

template<typename T>
T& openip::Vector4<T>::w()
{
    return w_;
}

template<typename T>
T openip::Vector4<T>::x() const
{
    return x_;
}

template<typename T>
T openip::Vector4<T>::y() const
{
    return y_;
}

template<typename T>
T openip::Vector4<T>::z() const
{
    return z_;
}

template<typename T>
T openip::Vector4<T>::w() const
{
    return w_;
}

template<typename T>
T openip::Vector4<T>::inner(const openip::Vector4<T>& b)
{
    return this->x()*b.x() + this->y()*b.y() + this->z()*b.z() + this->w()*b.w();
}

template<typename T>
void openip::Vector4<T>::normalize()
{
    x_= x_ / (x_ + y_ + z_ + w_);
    y_= y_ / (x_ + y_ + z_ + w_);
    z_= z_ / (x_ + y_ + z_ + w_);
    w_= w_ / (x_ + y_ + z_ + w_);
}

template<typename T> template<typename T2>
float openip::Vector4<T>::dist(const Vector4<T2>& v) const
{
    return (x() - v.x())*(x() - v.x()) + (y() - v.y())*(y() - v.y()) + (z() - v.z())*(z() - v.z()) + (w() - v.w())*(w() - v.w());
}

template<typename T> template<typename T2>
float openip::Vector4<T>::dist2(const Vector4<T2>& v) const
{
    return sqrt(dist(v));
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Vector4<T>& v)
{
    o << "[Vector4 ";
    o << v.x();
    o << ", ";
    o << v.y();
    o << ", ";
    o << v.z();
    o << ", ";
    o << v.w();
    o << "]";

    return o;
}

#endif	/* VECTOR4_H */

