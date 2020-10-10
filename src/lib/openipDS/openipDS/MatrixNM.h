/**
 * @file MatrixNM.h
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
 * The MatrixNM class represents an NxM static matrix and has
 * several useful operators and member functions defined.
 */

#ifndef _MATRIXNM_H_
#define _MATRIXNM_H_

#include <openipDS/Matrix.h>
#include <openipDS/VectorN.h>

using namespace openip;

namespace openip
{
    /**
     * MatrixNM represents a static matrix with size ROWS and COLUMNS and
     * type T
     */
    template<typename T, int ROWS, int COLUMNS>
    class MatrixNM: public Matrix<T>
    {
    public:

        /**
         * default constructor
         */
        MatrixNM();

        /**
         * copy constructor
         * @param b object to copy
         */
        MatrixNM(const MatrixNM& b);

        /**
         * desctructor
         */
        ~MatrixNM();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        MatrixNM& operator=(const MatrixNM& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        MatrixNM& operator+=(const MatrixNM& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        MatrixNM& operator-=(const MatrixNM& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        MatrixNM& operator*=(const MatrixNM& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        MatrixNM& operator/=(const MatrixNM& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        MatrixNM& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        MatrixNM& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        MatrixNM& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        MatrixNM& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        MatrixNM& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNM operator+(const MatrixNM& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNM operator-(const MatrixNM& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNM operator*(const MatrixNM& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNM operator/(const MatrixNM& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNM operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNM operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNM operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNM operator/(const T& t) const;
    };

    /**
     * matrix multiplication with row-column composition
     * @param a left operand
     * @param b right operand
     * @return result as new matrix instance
     */
    template<typename T, int ROWS, int COLUMNS, int INNER_DIM>
    MatrixNM<T, ROWS, COLUMNS>& operator^(const MatrixNM<T, ROWS, INNER_DIM>& a, const MatrixNM<T, INNER_DIM, COLUMNS>& b)
    {
        MatrixNM<T, ROWS, COLUMNS> tmp;
        tmp= 0;

        for ( int i= 0; i < ROWS; ++i )
            for ( int j= 0; j < COLUMNS; ++j )
            {
                
                for ( int k= 0; k < INNER_DIM; ++k )
                        tmp(i,j)+= a(i,k)*b(k,j);
            }

        return tmp;
    }

    /**
     * matrix vector multiplication with row-column composition
     * @param m left matrix operand
     * @param v right vector operand
     * @return result vector as new instance
     */
    template<typename T, int ROWS, int COLUMNS>
    VectorN<T, ROWS> operator^(const MatrixNM<T, ROWS, COLUMNS>& m, const VectorN<T, COLUMNS>& v)
    {
        VectorN<T, ROWS> tmp;
        tmp= 0;

        for ( int i= 0; i < ROWS; ++i )
            for ( int j= 0; j < COLUMNS; ++j )
                tmp(i)+= m(i,j)*v(j);

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */ 
    template<typename T, int ROWS, int COLUMNS>
    VectorN<T, COLUMNS> operator^(const VectorN<T, ROWS>& v, const MatrixNM<T, ROWS, COLUMNS>& m)
    {
        VectorN<T, COLUMNS> tmp;
        tmp= 0;

        for ( int i= 0; i < COLUMNS; ++i )
            for ( int j= 0; j < ROWS; ++j )
                tmp(i)+= m(j,i)*v(j);

        return tmp;
    }

    template<class T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>::MatrixNM()
    : Matrix<T>(ROWS,COLUMNS)
    {
    }

    template<class T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>::MatrixNM(const MatrixNM& b)
    : Matrix<T>(ROWS,COLUMNS)
    {
        typename Matrix<T>::const_mIt mitB= b.begin();
        for( typename Matrix<T>::mIt mit= this->begin(); mit != this->end(); ++mit,++mitB )
            *mit= *mitB;
    }

    template<class T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>::~MatrixNM()
    {
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator=(const MatrixNM<T, ROWS, COLUMNS>& b)
    {
        this->Matrix<T>::operator=(b);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator+=(const MatrixNM<T, ROWS, COLUMNS>& b)
    {
        this->Matrix<T>::operator+=(b);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator-=(const MatrixNM<T, ROWS, COLUMNS>& b)
    {
        this->Matrix<T>::operator-=(b);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator*=(const MatrixNM<T, ROWS, COLUMNS>& b)
    {
        this->Matrix<T>::operator*=(b);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator/=(const MatrixNM<T, ROWS, COLUMNS>& b)
    {
        this->Matrix<T>::operator/=(b);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator=(const T& t)
    {
        this->Matrix<T>::operator=(t);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator+=(const T& t)
    {
        this->Matrix<T>::operator+=(t);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator-=(const T& t)
    {
        this->Matrix<T>::operator-=(t);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator*=(const T& t)
    {
        this->Matrix<T>::operator*=(t);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS>& MatrixNM<T, ROWS, COLUMNS>::operator/=(const T& t)
    {
        this->Matrix<T>::operator/=(t);
        return *this;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator+(const MatrixNM<T, ROWS, COLUMNS>& b) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n+= b;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator-(const MatrixNM<T, ROWS, COLUMNS>& b) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n-= b;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator*(const MatrixNM<T, ROWS, COLUMNS>& b) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n*= b;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator/(const MatrixNM<T, ROWS, COLUMNS>& b) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n/= b;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator+(const T& t) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n+= t;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator-(const T& t) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n-= t;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator*(const T& t) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n*= t;
    }

    template<typename T, int ROWS, int COLUMNS>
    MatrixNM<T, ROWS, COLUMNS> MatrixNM<T, ROWS, COLUMNS>::operator/(const T& t) const
    {
        MatrixNM<T, ROWS, COLUMNS> n(*this);
        return n/= t;
    }

    /**
     * prints the description of the parameter MatrixNM object to the parameter output stream
     * @param o parameter output stream
     * @param v MatrixNM object to describe
     * @return reference to the parameter output stream
     */
    template<typename T, int ROWS, int COLUMNS>
    std::ostream& operator<<(std::ostream& o, const MatrixNM<T, ROWS, COLUMNS>& m)
    {
        o << "[MatrixNM ";
        o << ROWS;
        o << ", ";
        o << COLUMNS;
        o << "]";

        return o;
    }
}

#endif
