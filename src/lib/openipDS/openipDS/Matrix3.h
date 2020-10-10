/**
 * @file Matrix3.h
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
 * The Matrix3 class represents an 3x3 matrix and has
 * several useful operators and member functions defined.
 */

#ifndef _MATRIX_3x3_H_
#define _MATRIX_3x3_H_

#include <openipDS/MatrixNN.h>
#include <openipDS/Vector3.h>

namespace openip
{
    /**
     * Matrix3 represents a static 3x3 Matrix of template type T
     */
    template<typename T>
    class Matrix3
    {
    public:
        /**
         * default constructor
         */
        Matrix3();

        /**
         * copy constructor
         * @param m object to copy
         */
        Matrix3(const Matrix3& m);

        /**
         * constructor based on a pointer; the first 9 elements of the
         * memory area will be used as the elements of the matrix
         * @param data pointer to at least 9 elements of type T
         */
        Matrix3(T* data);

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
        Matrix3(T a, T b, T c, T d, T e, T f, T g, T h, T i);

        /**
         * destructor
         */
        ~Matrix3();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        Matrix3& operator=(const Matrix3& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Matrix3& operator+=(const Matrix3& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Matrix3& operator-=(const Matrix3& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Matrix3& operator*=(const Matrix3& b);

        /**
         * multiplies the parameter to this matrix with row-column composition
         *
         * @param b the matrix to multiply with
         * @return this
         */
        Matrix3& operator^=(const Matrix3& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Matrix3& operator/=(const Matrix3& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Matrix3& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Matrix3& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Matrix3& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Matrix3& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Matrix3& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix3 operator+(const Matrix3& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix3 operator-(const Matrix3& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix3 operator*(const Matrix3& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix3 operator/(const Matrix3& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix3 operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix3 operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix3 operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix3 operator/(const T& t) const;

        /**
         * (0,0)th element
         */
        T a;

        /**
         * (0,1)th element
         */
        T b;

        /**
         * (0,2)th element
         */
        T c;

        /**
         * (1,0)th element
         */
        T d;

        /**
         * (1,1)th element
         */
        T e;

        /**
         * (1,2)th element
         */
        T f;

        /**
         * (2,0)th element
         */
        T g;

        /**
         * (2,1)th element
         */
        T h;

        /**
         * (2,2)th element
         */
        T i;
    };

    /**
     * matrix multiplication with row-column composition
     * @param a left operand
     * @param b right operand
     * @return result as new matrix instance
     */
    template<typename T>
    Matrix3<T> operator^(const Matrix3<T>& a, const Matrix3<T>& b)
    {
        Matrix3<T> tmp;
        tmp.a= a.a*b.a + a.b*b.d + a.c*b.g;
        tmp.b= a.a*b.b + a.b*b.e + a.c*b.h;
        tmp.c= a.a*b.c + a.b*b.f + a.c*b.i;

        tmp.d= a.d*b.a + a.e*b.d + a.f*b.g;
        tmp.e= a.d*b.b + a.e*b.e + a.f*b.h;
        tmp.f= a.d*b.c + a.e*b.f + a.f*b.i;

        tmp.g= a.g*b.a + a.h*b.d + a.i*b.g;
        tmp.h= a.g*b.b + a.h*b.e + a.i*b.h;
        tmp.i= a.g*b.c + a.h*b.f + a.i*b.i;

        return tmp;
    }

    /**
     * matrix vector multiplication with row-column composition
     * @param m left matrix operand
     * @param v right vector operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector3<T> operator^(const Matrix3<T>& m, const Vector3<T>& v)
    {
        Vector3<T> tmp;

        tmp.x()= m.a*v.x() + m.b*v.y() + m.c*v.z();
        tmp.y()= m.d*v.x() + m.e*v.y() + m.f*v.z();
        tmp.z()= m.g*v.x() + m.h*v.y() + m.i*v.z();

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */ 
    template<typename T>
    Vector3<T> operator^(const Vector3<T>& v, const Matrix3<T>& m)
    {
        Vector3<T> tmp;

        tmp.x()= v.x()*m.a + v.y()*m.d + v.z()*m.g;
        tmp.y()= v.x()*m.b + v.y()*m.e + v.z()*m.h;
        tmp.z()= v.x()*m.c + v.y()*m.f + v.z()*m.i;

        return tmp;
    }

    template<class T>
    Matrix3<T>::Matrix3()
    {
        a= b= c= d= e= f= g= h= i= 0;
    }

    template<class T>
    Matrix3<T>::Matrix3(const Matrix3& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
        this->e= b.e;
        this->f= b.f;
        this->g= b.g;
        this->h= b.h;
        this->i= b.i;
    }

    template<typename T>
    Matrix3<T>::Matrix3(T* data)
    {
        this->a= data[0];
        this->b= data[1];
        this->c= data[2];
        this->d= data[3];
        this->e= data[4];
        this->f= data[5];
        this->g= data[6];
        this->h= data[7];
        this->i= data[8];
    }

    template<typename T>
    Matrix3<T>::Matrix3(T a, T b, T c, T d, T e, T f, T g, T h, T i)
    {
        this->a= a;
        this->b= b;
        this->c= c;
        this->d= d;
        this->e= e;
        this->f= f;
        this->g= g;
        this->h= h;
        this->i= i;
    }


    template<class T>
    Matrix3<T>::~Matrix3()
    {
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
        this->e= b.e;
        this->f= b.f;
        this->g= b.g;
        this->h= b.h;
        this->i= b.i;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& b)
    {
        this->a+= b.a;
        this->b+= b.b;
        this->c+= b.c;
        this->d+= b.d;
        this->e+= b.e;
        this->f+= b.f;
        this->g+= b.g;
        this->h+= b.h;
        this->i+= b.i;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& b)
    {
        this->a-= b.a;
        this->b-= b.b;
        this->c-= b.c;
        this->d-= b.d;
        this->e-= b.e;
        this->f-= b.f;
        this->g-= b.g;
        this->h-= b.h;
        this->i-= b.i;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T>& b)
    {
        this->a*= b.a;
        this->b*= b.b;
        this->c*= b.c;
        this->d*= b.d;
        this->e*= b.e;
        this->f*= b.f;
        this->g*= b.g;
        this->h*= b.h;
        this->i*= b.i;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator^=(const Matrix3<T>& b)
    {
        this->a= this->a*b.a + this->b*b.d + this->c*b.g;
        this->b= this->a*b.b + this->b*b.e + this->c*b.h;
        this->c= this->a*b.c + this->b*b.f + this->c*b.i;

        this->d= this->d*b.a + this->e*b.d + this->f*b.g;
        this->e= this->d*b.b + this->e*b.e + this->f*b.h;
        this->f= this->d*b.c + this->e*b.f + this->f*b.i;

        this->g= this->g*b.a + this->h*b.d + this->i*b.g;
        this->h= this->g*b.b + this->h*b.e + this->i*b.h;
        this->i= this->g*b.c + this->h*b.f + this->i*b.i;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator/=(const Matrix3<T>& b)
    {
        this->a/= b.a;
        this->b/= b.b;
        this->c/= b.c;
        this->d/= b.d;
        this->e/= b.e;
        this->f/= b.f;
        this->g/= b.g;
        this->h/= b.h;
        this->i/= b.i;
        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator=(const T& t)
    {
        this->a= t;
        this->b= t;
        this->c= t;
        this->d= t;
        this->e= t;
        this->f= t;
        this->g= t;
        this->h= t;
        this->i= t;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator+=(const T& t)
    {
        this->a+= t;
        this->b+= t;
        this->c+= t;
        this->d+= t;
        this->e+= t;
        this->f+= t;
        this->g+= t;
        this->h+= t;
        this->i+= t;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator-=(const T& t)
    {
        this->a-= t;
        this->b-= t;
        this->c-= t;
        this->d-= t;
        this->e-= t;
        this->f-= t;
        this->g-= t;
        this->h-= t;
        this->i-= t;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator*=(const T& t)
    {
        this->a*= t;
        this->b*= t;
        this->c*= t;
        this->d*= t;
        this->e*= t;
        this->f*= t;
        this->g*= t;
        this->h*= t;
        this->i*= t;

        return *this;
    }

    template<typename T>
    Matrix3<T>& Matrix3<T>::operator/=(const T& t)
    {
        this->a/= t;
        this->b/= t;
        this->c/= t;
        this->d/= t;
        this->e/= t;
        this->f/= t;
        this->g/= t;
        this->h/= t;
        this->i/= t;

        return *this;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator+(const Matrix3<T>& b) const
    {
        Matrix3<T> n(*this);
        return n+= b;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator-(const Matrix3<T>& b) const
    {
        Matrix3<T> n(*this);
        return n-= b;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& b) const
    {
        Matrix3<T> n(*this);
        return n*= b;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator/(const Matrix3<T>& b) const
    {
        Matrix3<T> n(*this);
        return n/= b;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator+(const T& t) const
    {
        Matrix3<T> n(*this);
        return n+= t;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator-(const T& t) const
    {
        Matrix3<T> n(*this);
        return n-= t;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator*(const T& t) const
    {
        Matrix3<T> n(*this);
        return n*= t;
    }

    template<typename T>
    Matrix3<T> Matrix3<T>::operator/(const T& t) const
    {
        Matrix3<T> n(*this);
        return n/= t;
    }

    /**
     * prints the description of the parameter Matrix3 object to the parameter output stream
     * @param o parameter output stream
     * @param m Matrix3 object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Matrix3<T>& m)
    {
        o << m.a;
        o << ", ";
        o << m.b;
        o << ", ";
        o << m.c;
        o << ", ";
        o << m.d;
        o << ", ";
        o << m.e;
        o << ", ";
        o << m.f;
        o << ", ";
        o << m.g;
        o << ", ";
        o << m.h;
        o << ", ";
        o << m.i;
        o << "]";
        
        return o;
    }
}

#endif
