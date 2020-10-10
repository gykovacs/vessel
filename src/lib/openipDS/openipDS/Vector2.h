/**
 * @file Vector2.h
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
 * The Vector2 class represents an 2 dimensional vector and has
 * several useful operators and member functions defined.
 */

#ifndef _Vector2_H_
#define _Vector2_H_

#include <cmath>
#include <vector>
#include <iostream>

namespace openip
{
    /**
     * Vector2 represents a two dimensional Vector of template type T
     */
    template<typename T>
    class Vector2
    {
    public:
        /**
         * Default constructor, creates a new vector instance with size 2
         * and type appropriate to the template paramter
         */
        Vector2();

        /**
         * Similar to the default constructor, but initializes the
         * vector with parameter values.
         */
        Vector2(T x_, T y_);

        /**
         * Copy constructor
         *
         * @param p instance to copy
         */
        Vector2(const Vector2& p);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator==(const Vector2& b);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator!=(const Vector2& b);

        /**
         * sets the values of this instance to the values of the other
         *
         * @param b the instance to copy
         * @return this instance
         */
        Vector2& operator=(const Vector2& b);

        template<typename B>
        Vector2& operator=(Vector2<B>& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Vector2& operator+=(const Vector2& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Vector2& operator-=(const Vector2& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Vector2& operator*=(const Vector2& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Vector2& operator/=(const Vector2& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector2 operator+(const Vector2& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector2 operator-(const Vector2& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector2 operator*(const Vector2& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector2 operator/(const Vector2& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        Vector2& operator=(const T& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector2& operator+=(const T& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector2& operator-=(const T& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector2& operator*=(const T& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector2& operator/=(const T& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector2 operator+(const T& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector2 operator-(const T& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector2 operator*(const T& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector2 operator/(const T& t) const;

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
         * update the coordinates of the vector
         * @param x column coordinate
         * @param y row coordinate
         */
        void set(T x, T y);

        /**
         * inner product with vector b
         * @param b the vector to compute inner product with
         * @return the inner product value
         */
        T inner(const Vector2& b);

        /**
         * direction vector
         * @param v the position vector
         * @return direction vector
         */
        Vector2 v(const Vector2& v) const;

        /**
         * normal vector
         * @param v position vector
         * @return normal vector
         */
        Vector2 n(const Vector2& v) const;

        /**
         * coordinates of the line aligned on this and v
         * @param v point to align a line on
         * @param A output parameter, line coordinate
         * @param B output parameter, line coordinate
         * @param C output parameter, line coordinate
         */
        void getLineCoords(Vector2& v, float& A, float& B, float& C) const;

        /**
         * check wether x is on the line aligned on a and b
         * @param b the other point of the line
         * @param x the point to check
         * @return 0 if x is on the line; >0 if x is in the positive
         * half plane of the line; &ge 0 if x is in the negative half
         * plane of the line
         */
        float onLine(Vector2& b, Vector2& x) const;

        /**
         * distance of the vectors
         * @param v to compute the distance from
         * @return the distance
         */
        float dist(const Vector2& v) const;

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
        float angleX();

        /**
         * angle between the vector and the y axis
         * @return angle to the y axis
         */
        float angleY();

        /**
         * normalizes the elements of the vector
         */
        Vector2<T>& normalize();

        /**
         * computes the square of the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist(const Vector2<T2>& v) const;

        /**
         * computes the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        template<typename T2>
        float dist2(const Vector2<T2>& v) const;

        float norm() const;

        /**
         * x coorindate
         */
        T x_;

        /**
         * y coordinate
         */
        T y_;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v Vector2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Vector2<T>& v);
}

template<typename T>
openip::Vector2<T>::Vector2()
{
    this->x()= 0;
    this->y()= 0;
}

template<typename T>
openip::Vector2<T>::Vector2(T x, T y)
{
    this->x()= x;
    this->y()= y;
}

template<typename T>
openip::Vector2<T>::Vector2(const Vector2<T>& v)
{
    this->x()= v.x();
    this->y()= v.y();
}

template<typename T>
bool openip::Vector2<T>::operator==(const Vector2& b)
{
    return this->x()==b.x() && this->y()==b.y();
}

template<typename T>
bool openip::Vector2<T>::operator!=(const Vector2& b)
{
    return ! this->operator==(b);
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator=(const Vector2<T>& b)
{
    this->x()= b.x();
    this->y()= b.y();

    return *this;
}

template<typename T> template<typename B>
openip::Vector2<T>& openip::Vector2<T>::operator=(Vector2<B>& b)
{
    this->x()= b.x();
    this->y()= b.y();

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator+=(const Vector2<T>& b)
{
    this->x()+= b.x();
    this->y()+= b.y();

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator-=(const Vector2<T>& b)
{
    this->x()-= b.x();
    this->y()-= b.y();

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator*=(const Vector2<T>& b)
{
    this->x()*= b.x();
    this->y()*= b.y();

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator/=(const Vector2<T>& b)
{
    this->x()/= b.x();
    this->y()/= b.y();

    return *this;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator+(const Vector2<T>& b) const
{
    openip::Vector2<T> n(*this);

    return n+= b;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator-(const Vector2<T>& b) const
{
    openip::Vector2<T> n(*this);

    return n-= b;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator*(const Vector2<T>& b) const
{
    openip::Vector2<T> n(*this);

    return n*= b;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator/(const Vector2<T>& b) const
{
    openip::Vector2<T> n(*this);

    return n/= b;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator=(const T& t)
{
    this->x()= this->y()= t;

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator+=(const T& t)
{
    this->x()+= t;
    this->y()+= t;

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator-=(const T& t)
{
    this->x()-= t;
    this->y()-= t;

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator*=(const T& t)
{
    this->x()*= t;
    this->y()*= t;

    return *this;
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::operator/=(const T& t)
{
    this->x()/= t;
    this->y()/= t;

    return *this;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator+(const T& t) const
{
    openip::Vector2<T> n(*this);

    return n+= t;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator-(const T& t) const
{
    openip::Vector2<T> n(*this);

    return n-= t;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator*(const T& t) const
{
    openip::Vector2<T> n(*this);

    return n*= t;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::operator/(const T& t) const
{
    openip::Vector2<T> n(*this);

    return n/= t;
}

template<typename T>
inline T& openip::Vector2<T>::x()
{
   return x_;
}

template<typename T>
inline T openip::Vector2<T>::x() const
{
   return x_;
}

template<typename T>
inline T& openip::Vector2<T>::y()
{
   return y_;
}

template<typename T>
inline T openip::Vector2<T>::y() const
{
   return y_;
}

template<typename T>
void openip::Vector2<T>::set(T x, T y)
{
    x_= x;
    y_= y;
}

template<typename T>
T openip::Vector2<T>::inner(const openip::Vector2<T>& b)
{
    return this->x()*b.x() + this->y()*b.y();
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::n(const openip::Vector2<T>& b) const
{
    openip::Vector2<T> tmp(this->y() - b.y(), b.x() - this.x());
    return tmp;
}

template<typename T>
openip::Vector2<T> openip::Vector2<T>::v(const openip::Vector2<T>& b) const
{
    openip::Vector2<T> tmp(b.x() - this->x(), b.y() - this.y());
    return tmp;
}

template<typename T>
void openip::Vector2<T>::getLineCoords(Vector2& v, float& A, float& B, float& C) const
{
    A= v.y() - this->y();
    B= this->x() - v.x();
    C= - ( A * this->x() + B * this->y() );
}

template<typename T>
float openip::Vector2<T>::onLine(Vector2& b, Vector2& x) const
{
    float A, B, C;
    getLineCoords(b, A, B, C);

    return A * x.x() + B * x.y() + C;
}

template<typename T>
float openip::Vector2<T>::dirCosX()
{
    Vector2<T> tmp(*this);
    tmp.normalize();
    return tmp.inner(Vector2<T>(1,0));
}

template<typename T>
float openip::Vector2<T>::dirCosY()
{
    Vector2<T> tmp(*this);
    tmp.normalize();
    return tmp.inner(Vector2<T>(0,1));
}

template<typename T>
float openip::Vector2<T>::angleY()
{
    return atan(gradientY());
}

template<typename T>
float openip::Vector2<T>::angleX()
{
    return atan(gradientX());
}

template<typename T>
float openip::Vector2<T>::gradientX()
{
    return (float)(this->y())/(this->x());
}

template<typename T>
float openip::Vector2<T>::gradientY()
{
    return (float)(this->x())/(this->y());
}

template<typename T>
float openip::Vector2<T>::dist(const openip::Vector2<T>& b) const
{
    return sqrt((this->x() - b.x())*(this->x() - b.x()) + (this->y() - b.y())*(this->y() - b.y()));
}

template<typename T>
openip::Vector2<T>& openip::Vector2<T>::normalize()
{
    x_= x_/sqrt((x_*x_ + y_*y_));
    y_= y_/sqrt((x_*x_ + y_*y_));

    return *this;
}

template<typename T> template<typename T2>
float openip::Vector2<T>::dist(const Vector2<T2>& v) const
{
    return (x() - v.x())*(x() - v.x()) + (y() - v.y())*(y() - v.y());
}

template<typename T> template<typename T2>
float openip::Vector2<T>::dist2(const Vector2<T2>& v) const
{
    return sqrt(dist(v));
}

template<typename T>
float openip::Vector2<T>::norm() const
{
    return sqrt(x_*x_ + y_*y_);
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Vector2<T>& v)
{
    o << "[Vector2 ";
    o << v.x();
    o << ", ";
    o << v.y();
    o << "]";
    
    return o;
}
#endif
