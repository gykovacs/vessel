/**
 * @file MatrixNN.h
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
 * The MatrixNN class represents an NxN static matrix and has
 * several useful operators and member functions defined.
 */

#ifndef _MATRIXNN_H
#define	_MATRIXNN_H

#include <openipDS/MatrixNM.h>

namespace openip
{
    /**
     * MatrixNN represents a static matrix with size ROWS and COLUMNS and
     * type T
     */
    template<typename T, int SIZE>
    class MatrixNN: public MatrixNM<T, SIZE, SIZE>
    {
    public:

        /**
         * default constructor
         */
        MatrixNN();

	/**
	* creates diagonal matrix from the elements of v
	* @param v elements of the diagonal
	*/
        MatrixNN(VectorN<T, SIZE> v);

        /**
         * copy constructor
         * @param b object to copy
         */
        MatrixNN(const MatrixNN& b);

	/**
	* initializes the matrix with value t
	* @param t the matrix will be initialized with this value
	*/
        MatrixNN(const T& t);

        /**
         * desctructor
         */
        ~MatrixNN();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        MatrixNN& operator=(const MatrixNN& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        MatrixNN& operator+=(const MatrixNN& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        MatrixNN& operator-=(const MatrixNN& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        MatrixNN& operator*=(const MatrixNN& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        MatrixNN& operator/=(const MatrixNN& b);

        /**
         * multiplies the parameter to this matrix with row-column composition
         *
         * @param b the matrix to multiply with
         * @return this
         */
        MatrixNN& operator^=(const MatrixNN& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        MatrixNN& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        MatrixNN& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        MatrixNN& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        MatrixNN& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        MatrixNN& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNN operator+(const MatrixNN& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNN operator-(const MatrixNN& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNN operator*(const MatrixNN& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        MatrixNN operator/(const MatrixNN& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNN operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNN operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNN operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        MatrixNN operator/(const T& t) const;

	/**
	* sets the matrix to Inn, nxn identical matrix
	*/
        void setIdentity();
    };

    /**
     * matrix multiplication with row-column composition
     * @param a left operand
     * @param b right operand
     * @return result as new matrix instance
     */
    template<typename T, int SIZE>
    MatrixNN<T, SIZE> operator^(const MatrixNN<T, SIZE>& a, const MatrixNN<T, SIZE>& b)
    {
        MatrixNN<T, SIZE> tmp;
        tmp= 0;

        for ( int i= 0; i < SIZE; ++i )
            for ( int j= 0; j < SIZE; ++j )
            {

                for ( int k= 0; k < SIZE; ++k )
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
    template<typename T, int SIZE>
    VectorN<T, SIZE> operator^(const MatrixNN<T, SIZE>& m, const VectorN<T, SIZE>& v)
    {
        VectorN<T, SIZE> tmp;
        tmp= 0;

        for ( int i= 0; i < SIZE; ++i )
            for ( int j= 0; j < SIZE; ++j )
                tmp(i)+= m(i,j)*v(j);

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */
    template<typename T, int SIZE>
    VectorN<T, SIZE> operator^(const VectorN<T, SIZE>& v, const MatrixNN<T, SIZE>& m)
    {
        VectorN<T, SIZE> tmp;
        tmp= 0;

        for ( int i= 0; i < SIZE; ++i )
            for ( int j= 0; j < SIZE; ++j )
                tmp(i)+= m(j,i)*v(j);

        return tmp;
    }

    template<class T, int SIZE>
    MatrixNN<T, SIZE>::MatrixNN()
    : MatrixNM<T, SIZE, SIZE>()
    {
    }

    template<class T, int SIZE>
    MatrixNN<T, SIZE>::MatrixNN(VectorN<T, SIZE> v)
    : MatrixNM<T, SIZE, SIZE>()
    {
        (*this)= 0;
        for ( int i= 0; i < SIZE; ++i )
            (*this)(i,i)= v(i);
    }

    template<class T, int SIZE>
    MatrixNN<T, SIZE>::MatrixNN(const T& t)
    : MatrixNM<T, SIZE, SIZE>()
    {
        (*this)= t;
    }

    template<class T, int SIZE>
    MatrixNN<T, SIZE>::MatrixNN(const MatrixNN& b)
    : MatrixNM<T, SIZE, SIZE>()
    {
        typename Matrix<T>::const_mIt mitB= b.begin();
        for( typename Matrix<T>::mIt mit= this->begin(); mit != this->end(); ++mit,++mitB )
            *mit= *mitB;
    }

    template<class T, int SIZE>
    MatrixNN<T, SIZE>::~MatrixNN()
    {
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator=(const MatrixNN<T, SIZE>& b)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator=(b);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator+=(const MatrixNN<T, SIZE>& b)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator+=(b);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator-=(const MatrixNN<T, SIZE>& b)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator-=(b);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator*=(const MatrixNN<T, SIZE>& b)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator*=(b);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator/=(const MatrixNN<T, SIZE>& b)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator/=(b);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator^=(const MatrixNN<T, SIZE>& b)
    {
        MatrixNN<T, SIZE> tmp;
        tmp= 0;

        for ( int i= 0; i < SIZE; ++i )
            for ( int j= 0; j < SIZE; ++j )
            {

                for ( int k= 0; k < SIZE; ++k )
                        tmp(i,j)+= this->operator()(i,k)*b(k,j);
            }
        *this= tmp;
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator=(const T& t)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator=(t);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator+=(const T& t)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator+=(t);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator-=(const T& t)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator-=(t);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator*=(const T& t)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator*=(t);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE>& MatrixNN<T, SIZE>::operator/=(const T& t)
    {
        this->MatrixNM<T, SIZE, SIZE>::operator/=(t);
        return *this;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator+(const MatrixNN<T, SIZE>& b) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n+= b;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator-(const MatrixNN<T, SIZE>& b) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n-= b;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator*(const MatrixNN<T, SIZE>& b) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n*= b;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator/(const MatrixNN<T, SIZE>& b) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n/= b;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator+(const T& t) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n+= t;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator-(const T& t) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n-= t;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator*(const T& t) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n*= t;
    }

    template<typename T, int SIZE>
    MatrixNN<T, SIZE> MatrixNN<T, SIZE>::operator/(const T& t) const
    {
        MatrixNN<T, SIZE> n(*this);
        return n/= t;
    }

    template<typename T, int SIZE>
    void MatrixNN<T, SIZE>:: setIdentity()
    {
        *this= 0;
        for ( int i= 0; i < SIZE; ++i )
            (*this)(i,i)= 1;
    }

    /**
     * prints the description of the parameter MatrixNN object to the parameter output stream
     * @param o parameter output stream
     * @param m MatrixNN object to describe
     * @return reference to the parameter output stream
     */
    template<typename T, int SIZE>
    std::ostream& operator<<(std::ostream& o, const MatrixNN<T, SIZE>& m)
    {
        o << "[MatrixNN ";
        o << SIZE;
        o << ", ";
        o << SIZE;
        o << " elements: ";
        o.precision(14);
        for ( int i= 0; i < SIZE * SIZE; ++i )
        {
            o << (m)(i);
            o << " ";
        }
        o << "]";

        return o;
    }

    /**
     * prints the description of the parameter MatrixNN object to the parameter output file stream
     * @param o parameter output file stream
     * @param v MatrixNN object to describe
     * @return reference to the parameter output stream
     */
    template<typename T, int SIZE>
    std::ofstream& operator<<(std::ofstream& o, const MatrixNN<T, SIZE>& v)
    {
        for ( typename MatrixNN<T, SIZE>::const_iterator vit= v.begin(); vit != v.end(); ++vit )
            o << *vit << " ";

        return o;
    }

    /**
     * fills the parameter Matrix from the parameter input file stream
     * @param i parameter input file stream
     * @param v MatrixNN object to fill from the parameter input file stream
     * @return reference to the parameter input file stream
     */
    template<typename T, int SIZE>
    std::ifstream& operator>>(std::ifstream& i, MatrixNN<T, SIZE>& v)
    {
        for ( int j= 0; j < SIZE*SIZE; ++j )
            i >> v(j);

        return i;
    }
}

#endif	/* _MATRIXNN_H */

