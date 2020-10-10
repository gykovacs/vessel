/**
 * @file Matrix2.h
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
 * The Matrix2 class represents an 2x2 matrix and has
 * several useful operators and member functions defined.
 */

#ifndef _MATRIX_2x2_H_
#define _MATRIX_2x2_H_

#include <openipDS/Vector2.h>

namespace openip
{
    /**
     * Matrix2 represents a static 2x2 sized Matrix of template type T
     */
    template<typename T>
    class Matrix2
    {
    public:
        /**
         * default constructor
         */
        Matrix2();

        /**
         * copy constructor
         * @param m object to copy
         */
        Matrix2(const Matrix2& m);

        /**
         * constructor based on a pointer; the first 4 elements of the
         * memory area will be used as the elements of the matrix
         * @param data pointer to at least 4 elements of type T
         */
        Matrix2(T* data);

        /**
         * constructor based on 4 parameters representing the elements of
         * the matrix
         * @param a element (0,0)
         * @param b element (0,1)
         * @param c element (1,0)
         * @param d element (1,1)
         */
        Matrix2(T a, T b, T c, T d);

        /**
         * destructor
         */
        ~Matrix2();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        Matrix2& operator=(const Matrix2& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Matrix2& operator+=(const Matrix2& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Matrix2& operator-=(const Matrix2& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Matrix2& operator*=(const Matrix2& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Matrix2& operator/=(const Matrix2& b);

        /**
         * multiplies the parameter to this matrix with row-column composition
         *
         * @param b the matrix to multiply with
         * @return this
         */
        Matrix2& operator^=(const Matrix2& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Matrix2& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Matrix2& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Matrix2& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Matrix2& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Matrix2& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix2 operator+(const Matrix2& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix2 operator-(const Matrix2& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix2 operator*(const Matrix2& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix2 operator/(const Matrix2& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix2 operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix2 operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix2 operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix2 operator/(const T& t) const;

        /**
         * (0,0)th element
         */
        T a;

        /**
         * (0,1)th element
         */
        T b;

        /**
         * (1,0)th element
         */
        T c;

        /**
         * (1,1)th element
         */
        T d;
    };

    /**
     * matrix multiplication with row-column composition
     * @param a left operand
     * @param b right operand
     * @return result as new matrix instance
     */
    template<typename T>
    Matrix2<T> operator^(const Matrix2<T>& a, const Matrix2<T>& b)
    {
        Matrix2<T> tmp;
        tmp.a= a.a*b.a + a.b*b.c;
        tmp.b= a.a*b.b + a.b*b.d;
        tmp.c= a.c*b.a + a.d*b.c;
        tmp.d= a.c*b.b + a.d*b.d;

        return tmp;
    }

    /**
     * matrix vector multiplication with row-column composition
     * @param m left matrix operand
     * @param v right vector operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector2<T> operator^(const Matrix2<T>& m, const Vector2<T>& v)
    {
        Vector2<T> tmp;

        tmp.x()= m.a*v.x() + m.b*v.y();
        tmp.y()= m.c*v.x() + m.d*v.y();

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector2<T> operator^(const Vector2<T>& v, const Matrix2<T>& m)
    {
        Vector2<T> tmp;

        tmp.x()= v.x()*m.a + v.y()*m.c;
        tmp.y()= v.x()*m.b + v.y()*m.d;

        return tmp;
    }

    template<class T>
    Matrix2<T>::Matrix2()
    {
        a= b= c= d= 0;
    }

    template<class T>
    Matrix2<T>::Matrix2(const Matrix2& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
    }

    template<typename T>
    Matrix2<T>::Matrix2(T* data)
    {
        a= data[0];
        b= data[1];
        c= data[2];
        d= data[3];
    }

    template<typename T>
    Matrix2<T>::Matrix2(T a, T b, T c, T d)
    {
        this->a= a;
        this->b= b;
        this->c= c;
        this->d= d;
    }


    template<class T>
    Matrix2<T>::~Matrix2()
    {
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator=(const Matrix2<T>& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator+=(const Matrix2<T>& b)
    {
        this->a+= b.a;
        this->b+= b.b;
        this->c+= b.c;
        this->d+= b.d;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator-=(const Matrix2<T>& b)
    {
        this->a-= b.a;
        this->b-= b.b;
        this->c-= b.c;
        this->d-= b.d;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator*=(const Matrix2<T>& b)
    {
        this->a*= b.a;
        this->b*= b.b;
        this->c*= b.c;
        this->d*= b.d;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator/=(const Matrix2<T>& b)
    {
        this->a/= b.a;
        this->b/= b.b;
        this->c/= b.c;
        this->d/= b.d;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator^=(const Matrix2<T>& m)
    {
        this->a= a*m.a + b*m.c;
        this->b= a*m.b + b*m.d;
        this->c= c*m.a + d*m.c;
        this->d= c*m.b + d*m.d;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator=(const T& t)
    {
        a= b= c= d= t;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator+=(const T& t)
    {
        a+= t;
        b+= t;
        c+= t;
        d+= t;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator-=(const T& t)
    {
        a-= t;
        b-= t;
        c-= t;
        d-= t;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator*=(const T& t)
    {
        a*= t;
        b*= t;
        c*= t;
        d*= t;
        return *this;
    }

    template<typename T>
    Matrix2<T>& Matrix2<T>::operator/=(const T& t)
    {
        a/= t;
        b/= t;
        c/= t;
        d/= t;
        return *this;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator+(const Matrix2<T>& b) const
    {
        Matrix2<T> n(*this);
        return n+= b;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator-(const Matrix2<T>& b) const
    {
        Matrix2<T> n(*this);
        return n-= b;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator*(const Matrix2<T>& b) const
    {
        Matrix2<T> n(*this);
        return n*= b;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator/(const Matrix2<T>& b) const
    {
        Matrix2<T> n(*this);
        return n/= b;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator+(const T& t) const
    {
        Matrix2<T> n(*this);
        return n+= t;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator-(const T& t) const
    {
        Matrix2<T> n(*this);
        return n-= t;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator*(const T& t) const
    {
        Matrix2<T> n(*this);
        return n*= t;
    }

    template<typename T>
    Matrix2<T> Matrix2<T>::operator/(const T& t) const
    {
        Matrix2<T> n(*this);
        return n/= t;
    }

    /**
     * prints the description of the parameter Matrix2 object to the parameter output stream
     * @param o parameter output stream
     * @param m Matrix2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Matrix2<T>& m)
    {
        o << "[Matrix2 ";
        o << m.a;
        o << ", ";
        o << m.b;
        o << ", ";
        o << m.c;
        o << ", ";
        o << m.d;
        o << "]";
        
        return o;
    }
}

#endif
