/**
 * @file Vector3.h
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
 * The Vector3 class represents an 3 dimensional vector and has
 * several useful operators and member functions defined.
 */

#ifndef _Vector3_H_
#define _Vector3_H_

#include <cmath>
#include <vector>
#include <iostream>

namespace openip
{
    /**
     * Vector3 represents a Vector of static size 3 with template type T
     */
    template<typename T>
    class Vector3
    {
    public:
        /**
         * Default constructor, creates a new vector instance with size 3
         * and type appropriate to the template paramter
         */
        Vector3();

        /**
         * Similar to the default constructor, but initializes the
         * vector with parameter values.
         */
        Vector3(T x_, T y_, T z_);

        /**
         * Copy constructor
         *
         * @param p instance to copy
         */
        Vector3(const Vector3& p);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator==(const Vector3& b);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator!=(const Vector3& b);

        /**
         * assignment operator
         * @param b object to assign to this
         */
        Vector3& operator=(const Vector3& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Vector3& operator+=(const Vector3& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Vector3& operator-=(const Vector3& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Vector3& operator*=(const Vector3& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Vector3& operator/=(const Vector3& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector3 operator+(const Vector3& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector3 operator-(const Vector3& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector3 operator*(const Vector3& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector3 operator/(const Vector3& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        Vector3& operator=(const T& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector3& operator+=(const T& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector3& operator-=(const T& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector3& operator*=(const T& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector3& operator/=(const T& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector3 operator+(const T& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector3 operator-(const T& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector3 operator*(const T& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector3 operator/(const T& t) const;

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
         * inner product with vector b
         * @param b the vector to compute inner product with
         * @return the inner product value
         */
        T inner(const Vector3& b);

        /**
         * cross product with vector c
         * @param c the vector to compute cross product with
         * @return cross product vector
         */
        Vector3 cross(const Vector3& c) const;

        /**
         * direction vector
         * @param v the position vector
         * @return direction vector
         */
        Vector3 v(const Vector3& v) const;

        /**
         * normal vector
         * @param v position vector
         * @return normal vector
         */
        Vector3 n(const Vector3& v) const;

        /**
         * coordinates of the line aligned on this and v
         * Ax + By + Cz + D = 0
         * @param v point to align a line on
         * @param A output parameter, line coordinate
         * @param B output parameter, line coordinate
         * @param C output parameter, line coordinate
         * @param D output parameter, line coordinate
         */
        void getLineCoords(Vector3& v, float& A, float& B, float& C, float& D) const;

        /**
         * check wether x is on the line aligned on a and b
         * @param b the other point of the line
         * @param x the point to check
         * @return 0 if x is on the line; >0 if x is in the positive
         * half plane of the line; &ge 0 if x is in the negative half
         * plane of the line
         */
        float onLine(Vector3& b, Vector3& x) const;

        /**
         * distance of the vectors
         * @param v to compute the distance from
         * @return the distance
         */
        float dist(const Vector3& v) const;

        /**
         * gradient to the X axis
         * @return x component of the gradient vector
         */
        float gradientX();

        /**
         * gradient to the Y axis
         * @return y component of the gradient vector
         */
        float gradientY();

        /**
         * direction cosinus to the x axis
         * @return direction cosinus
         */
        float dirCosX();

        /**
         * direction cosinus to the y axis
         * @return direction cosinus
         */
        float dirCosY();

        /**
         * angle between the vector and the x axis
         * @return angle to the x axis
         */
        float dirCosZ();

        /**
         * normalizes the elements of the vector
         */
        void normalize();
        
        float norm() const;

        /**
         * computes the square of the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist(const Vector3<T2>& v) const;

        /**
         * computes the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist2(const Vector3<T2>& v) const;

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
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v Vector3 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Vector3<T>& v);
}

template<typename T>
openip::Vector3<T>::Vector3()
{
    this->x()= 0;
    this->y()= 0;
    this->z()= 0;
}

template<typename T>
openip::Vector3<T>::Vector3(T x, T y, T z)
{
    this->x()= x;
    this->y()= y;
    this->z()= z;
}

template<typename T>
openip::Vector3<T>::Vector3(const Vector3<T>& v)
{
    this->x()= v.x();
    this->y()= v.y();
    this->z()= v.z();
}

template<typename T>
bool openip::Vector3<T>::operator==(const Vector3& b)
{
    return this->x()==b.x() && this->y()==b.y() && this->z()==b.z();
}

template<typename T>
bool openip::Vector3<T>::operator!=(const Vector3& b)
{
    return ! this->operator==(b);
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator=(const Vector3<T>& b)
{
    this->x()= b.x();
    this->y()= b.y();
    this->z()= b.z();

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator+=(const Vector3<T>& b)
{
    this->x()+= b.x();
    this->y()+= b.y();
    this->z()+= b.z();

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator-=(const Vector3<T>& b)
{
    this->x()-= b.x();
    this->y()-= b.y();
    this->z()-= b.z();

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator*=(const Vector3<T>& b)
{
    this->x()*= b.x();
    this->y()*= b.y();
    this->z()*= b.z();

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator/=(const Vector3<T>& b)
{
    this->x()/= b.x();
    this->y()/= b.y();
    this->z()/= b.z();

    return *this;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator+(const Vector3<T>& b) const
{
    openip::Vector3<T> n(*this);

    return n+= b;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator-(const Vector3<T>& b) const
{
    openip::Vector3<T> n(*this);

    return n-= b;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator*(const Vector3<T>& b) const
{
    openip::Vector3<T> n(*this);

    return n*= b;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator/(const Vector3<T>& b) const
{
    openip::Vector3<T> n(*this);

    return n/= b;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator=(const T& t)
{
    this->x()= this->y()= this->z()= t;

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator+=(const T& t)
{
    this->x()+= t;
    this->y()+= t;
    this->z()+= t;

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator-=(const T& t)
{
    this->x()-= t;
    this->y()-= t;
    this->z()-= t;

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator*=(const T& t)
{
    this->x()*= t;
    this->y()*= t;
    this->z()*= t;

    return *this;
}

template<typename T>
openip::Vector3<T>& openip::Vector3<T>::operator/=(const T& t)
{
    this->x()/= t;
    this->y()/= t;
    this->z()/= t;

    return *this;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator+(const T& t) const
{
    openip::Vector3<T> n(*this);

    return n+= t;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator-(const T& t) const
{
    openip::Vector3<T> n(*this);

    return n-= t;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator*(const T& t) const
{
    openip::Vector3<T> n(*this);

    return n*= t;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::operator/(const T& t) const
{
    openip::Vector3<T> n(*this);

    return n/= t;
}


template<typename T>
T& openip::Vector3<T>::x()
{
    return x_;
}

template<typename T>
T& openip::Vector3<T>::y()
{
    return y_;
}

template<typename T>
T& openip::Vector3<T>::z()
{
    return z_;
}

template<typename T>
T openip::Vector3<T>::x() const
{
    return x_;
}

template<typename T>
T openip::Vector3<T>::y() const
{
    return y_;
}

template<typename T>
T openip::Vector3<T>::z() const
{
    return z_;
}

template<typename T>
float openip::Vector3<T>::dirCosX()
{
    Vector3<T> tmp(*this);
    tmp.normalize();
    return tmp.inner(Vector3<T>(1,0,0));
}

template<typename T>
float openip::Vector3<T>::dirCosY()
{
    Vector3<T> tmp(*this);
    tmp.normalize();
    return tmp.inner(Vector3<T>(0,1,0));
}

template<typename T>
float openip::Vector3<T>::dirCosZ()
{
    Vector3<T> tmp(*this);
    tmp.normalize();
    return tmp.inner(Vector3<T>(0,0,1));
}

template<typename T>
float openip::Vector3<T>::dist(const openip::Vector3<T>& b) const
{
    return sqrt((this->x() - b.x())*(this->x() - b.x()) + (this->y() - b.y())*(this->y() - b.y()) + (this->z() - b.z())*(this->z() - b.z()));
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::n(const openip::Vector3<T>& b) const
{
    openip::Vector3<T> tmp(this->y() - b.y(), b.x() - this.x());
    return tmp;
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::v(const openip::Vector3<T>& b) const
{
    openip::Vector3<T> tmp(b.x() - this->x(), b.y() - this.y(), b.z() - this.z());
    return tmp;
}

template<typename T>
T openip::Vector3<T>::inner(const openip::Vector3<T>& b)
{
    return this->x()*b.x() + this->y()*b.y() + this->z()*b.z();
}

template<typename T>
openip::Vector3<T> openip::Vector3<T>::cross(const Vector3<T>& c) const
{
    Vector3 cp(this->y() * c.z() - this->z() * c.y(), this->z() * c.x() - this->x() * c.z(), this->x() * c.y() - this->y() * c.x());

    return cp;
}

template<typename T>
void openip::Vector3<T>::normalize()
{
    x_= x_ / (x_ + y_ + z_);
    y_= y_ / (x_ + y_ + z_);
    z_= z_ / (x_ + y_ + z_);
}

template<typename T> template<typename T2>
float openip::Vector3<T>::dist(const Vector3<T2>& v) const
{
    return (x() - v.x())*(x() - v.x()) + (y() - v.y())*(y() - v.y()) + (z() - v.z())*(z() - v.z());
}

template<typename T> template<typename T2>
float openip::Vector3<T>::dist2(const Vector3<T2>& v) const
{
    return sqrt(dist(v));
}

template<typename T>
float openip::Vector3<T>::norm() const
{
    return sqrt(x_*x_ + y_*y_ + z_*z_);
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Vector3<T>& v)
{
    o << "[Vector3 ";
    o << v.x();
    o << ", ";
    o << v.y();
    o << ", ";
    o << v.z();
    o << "]";

    return o;
}

#endif
