/**
 * @file Matrix.h
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
 * The Matrix class represents an arbitrary matrix and has
 * several useful operators and member functions defined.
 */

#ifndef _MATRIX_H
#define	_MATRIX_H

#include <openipDS/Vector.h>

namespace openip
{
    /**
     * Matrix represents an arbitrary size dynamic matrix
     */
    template<typename T>
    class Matrix: public openip::Vector<T>
    {
    public:
        using Vector<T>::operator();

        /**
         * Matrix<T>::iterator typedef
         */
        typedef typename Matrix<T>::iterator mIt;

        /**
         * Matrix<T>::const_iterator typedef
         */
        typedef typename Matrix<T>::const_iterator const_mIt;

        /**
         * default constructor
         */
        Matrix();

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         */
        Matrix(unsigned int rows, unsigned int columns);

        /**
         * copy constructor
         * @param m object to copy
         */
        Matrix(const Matrix<T>& m);

        /**
         * desctructor
         */
        ~Matrix();

        /**
         * accessing the element of the matrix (stored row-continuosly) as rvalue
         * @param x index of the row
         * @param y index of the column
         * @return the (x,y)th element
         */
        inline T operator()(unsigned int x, unsigned int y) const;

        /**
         * accessing the element of the matrix (stored row-continuosly) as lvalue
         * @param x index of the row
         * @param y index of the column
         * @return the (x,y)th element
         */
        inline T& operator()(unsigned int x, unsigned int y);

        /**
         * accessing the element of the matrix, (0,0) is the middle element of the matrix
         * @param x index of the row
         * @param y index of the columns
         * @return (x,y)th element
         */
        inline T get(int x, int y) const;

        /**
         * accessing the element of the matrix, (0,0) is the middle element of the matrix
         * @param x index of the row
         * @param y index of the columns
         * @return (x,y)th element
         */
        inline T& get(int x, int y);

        /**
         * point-wise equals operator
         * @param b object to compare
         * @return true if equal; false if differ
         */
        bool operator==(const Matrix& b);

        /**
         * point-wise not-equals operator
         * @param b object to compare
         * @return true if differ, false if equal
         */
        bool operator!=(const Matrix& b);

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Matrix& operator=(const Matrix& b);

        /**
         * assignment operator for different types of matrices
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Matrix<T>& operator=(const Matrix<S>& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Matrix& operator+=(const Matrix& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Matrix& operator-=(const Matrix& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Matrix& operator*=(const Matrix& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Matrix& operator/=(const Matrix& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Matrix& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Matrix& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Matrix& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Matrix& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Matrix& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix operator+(const Matrix& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix operator-(const Matrix& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix operator*(const Matrix& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix operator/(const Matrix& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix operator/(const T& t) const;

        /**
         * resizes the matrix
         * @param rows number of rows after resize
         * @param columns number of columns after resize
         */
        void resize(int rows, int columns);

        /**
         * inserts a new row into the matrix
         *
         * @param v the vector to insert as a row
         * @param i the position of the row to insert (0..number of rows)
         */
        void insertRow(Vector<T>& v, int i);

        /**
         * inserts a new column into the matrix
         *
         * @param v the vector to insert as a column
         * @param i the position of the row to insert (0..number of columns)
         */
        void insertColumn(Vector<T>& v, int i);

        /**
         * extracts the ith row from the matrix
         * @param v output parameter
         * @param i the index of row to extract
         */
        void getRow(Vector<T>& v, int i);

        /**
         * extracts the ith column from the matrix
         * @param v output parameter
         * @param i the index of the column to extract
         */
        void getColumn(Vector<T>& v, int i);
	
	/**
         * sets the ith row from the matrix
         * @param v output parameter
         * @param i the index of row to extract
         */
        void setRow(Vector<T>& v, int i);

        /**
         * sets the ith column from the matrix
         * @param v output parameter
         * @param i the index of the column to extract
         */
        void setColumn(Vector<T>& v, int i);

        void transpose();

        /**
         * number of rows
         */
        int rows;

        /**
         * number of columns
         */
        int columns;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param m Matrix object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Matrix<T>& m);

    /**
     * >> input operator
     * @param file input file stream
     * @param v this matrix is filled with the numbers on the stream
     * @return returns the file stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, Matrix<T>& v);

    /**
     * << output operator
     * @param file output file stream
     * @param v the elements of this parameter matrix are written to the stream
     * @return the file stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, Matrix<T>& v);

    /**
     * matrix multiplication with row-column composition
     * @param a left parameter of multiplication
     * @param b right parameter of multiplication
     * @return the result matrix object
     */
    template<typename T>
    Matrix<T> operator^(const Matrix<T>& a, const Matrix<T>& b)
    {
        Matrix<T> tmp(a.rows, b.columns);
        tmp= 0;
        for ( int i= 0; i < a.rows; ++i )
            for ( int j= 0; j < b.columns; ++j )
            {
                for ( int k= 0; k < a.columns; ++k )
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
    template<typename T>
    Vector<T> operator^(const Matrix<T>& m, const Vector<T>& v)
    {
        Vector<T> tmp(m.rows);
        tmp= 0;

        for ( int i= 0; i < m.rows; ++i )
            for ( int j= 0; j < m.columns; ++j )
                tmp(i)+= m(i,j)*v(j);

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector<T> operator^(const Vector<T>& v, const Matrix<T>& m)
    {
        Vector<T> tmp(m.columns);
        tmp= 0;

        for ( int i= 0; i < m.columns; ++i )
            for ( int j= 0; j < m.rows; ++j )
                tmp(i)+= m(j,i)*v(j);

        return tmp;
    }
}
template<class T>
openip::Matrix<T>::Matrix()
: Vector<T>(), rows(0), columns(0)
{
}

template<class T>
openip::Matrix<T>::Matrix(unsigned int rows, unsigned int columns)
: Vector<T>(columns * rows)
{
    this->columns= columns;
    this->rows= rows;
}

template<typename T>
openip::Matrix<T>::Matrix(const Matrix<T>& m)
: Vector<T>(m)
{
    this->rows= m.rows;
    this->columns= m.columns;
}

template<typename T>
openip::Matrix<T>::~Matrix()
{
}

template<typename T>
inline T openip::Matrix<T>::operator()(unsigned int r, unsigned int c) const
{
    return this->Vector<T>::operator()(r * columns + c);
}

template<typename T>
inline T& openip::Matrix<T>::operator()(unsigned int r, unsigned int c)
{
    return this->Vector<T>::operator()(r * columns + c);
}

template<typename T>
inline T openip::Matrix<T>::get(int r, int c) const
{
    return this->Vector<T>::operator()((r + this->rows/2)*columns + c + this->columns/2);
}

template<typename T>
inline T& openip::Matrix<T>::get(int r, int c)
{
    return this->Vector<T>::operator()((r + this->rows/2)*columns + c + this->columns/2);
}

template<typename T>
bool openip::Matrix<T>::operator==(const Matrix<T>& b)
{
    return this->Vector<T>::operator==(b) && this->columns == b.columns && this->rows == b.rows;
}

template<typename T>
bool openip::Matrix<T>::operator!=(const Matrix<T>& b)
{
    return ! this->operator==(b);
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator=(const Matrix<T>& b)
{
    if ( this->rows != b.rows || this->columns != b.columns )
    {
        this->rows= b.rows;
        this->columns= b.columns;
        this->Vector<T>::resize(this->rows * this->columns);
    }
    this->Vector<T>::operator=(b);

    return *this;
}

template<typename T> template<typename S>
openip::Matrix<T>& openip::Matrix<T>::operator=(const Matrix<S>& b)
{
    if ( this->rows != b.rows || this->columns != b.columns )
    {
        this->rows= b.rows;
        this->columns= b.columns;
        this->Vector<T>::resize(this->rows * this->columns);
    }
    this->Vector<T>::operator=(b);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator+=(const Matrix<T>& b)
{
    this->Vector<T>::operator+=(b);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator-=(const Matrix<T>& b)
{
    this->Vector<T>::operator-=(b);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator*=(const Matrix<T>& b)
{
    this->Vector<T>::operator*=(b);

    return *this;

}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator/=(const Matrix<T>& b)
{
    this->Vector<T>::operator/=(b);

    return *this;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator+(const Matrix<T>& b) const
{
	openip::Matrix<T> n(*this);

	return n+= b;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator-(const Matrix<T>& b) const
{
	openip::Matrix<T> n(*this);

	return n-= b;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator*(const Matrix<T>& b) const
{
	openip::Matrix<T> n(*this);

	return n*= b;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator/(const Matrix<T>& b) const
{
	openip::Matrix<T> n(*this);

	return n/= b;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator=(const T& t)
{
    this->Vector<T>::operator=(t);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator+=(const T& t)
{
    this->Vector<T>::operator+=(t);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator-=(const T& t)
{
    this->Vector<T>::operator-=(t);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator*=(const T& t)
{
    this->Vector<T>::operator*=(t);

    return *this;
}

template<typename T>
openip::Matrix<T>& openip::Matrix<T>::operator/=(const T& t)
{
    this->Vector<T>::operator/=(t);

    return *this;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator+(const T& t) const
{
	openip::Matrix<T> n(*this);

	return n+= t;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator-(const T& t) const
{
	openip::Matrix<T> n(*this);

	return n-= t;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator*(const T& t) const
{
	openip::Matrix<T> n(*this);

	return n*= t;
}

template<typename T>
openip::Matrix<T> openip::Matrix<T>::operator/(const T& t) const
{
	openip::Matrix<T> n(*this);

	return n/= t;
}

template<typename T>
void openip::Matrix<T>::insertRow(Vector<T>& v, int i)
{
    int j= i*columns;
    for ( typename openip::Vector<T>::const_vIt vit= v.begin(); vit != v.end(); ++vit )
        this->vector<T>::insert(this->begin()+(j++), *vit);
    ++rows;
}

template<typename T>
void openip::Matrix<T>::resize(int rows, int columns)
{
    //printf("Matrix<T>::resize\n"); fflush(stdout);
    if ( this->rows == rows && this->columns == columns )
    {
    }
    else
    {
        this->vector<T>::resize(rows*columns);
        this->rows= rows;
        this->columns= columns;
    }
}

template<typename T>
void openip::Matrix<T>::insertColumn(Vector<T>& v, int i)
{
    ++columns;
    int j= 0;
    for ( typename openip::Vector<T>::const_vIt vit= v.begin(); vit != v.end(); ++vit )
        this->vector<T>::insert(this->begin()+i+(j++)*columns, *vit);
}

template<typename T>
void openip::Matrix<T>::getRow(Vector<T>& v, int i)
{
    int j= i*columns;
    for ( typename openip::Vector<T>::vIt vit= v.begin(); vit != v.end(); ++vit )
        *vit= *(this->begin() + (j++));
}

template<typename T>
void openip::Matrix<T>::getColumn(Vector<T>& v, int i)
{
    int j= 0;
    for ( typename openip::Vector<T>::vIt vit= v.begin(); vit != v.end(); ++vit )
        *vit= *(this->begin() + i + (j++)*columns);
}

template<typename T>
void openip::Matrix<T>::setRow(Vector<T>& v, int i)
{
    int j= i*columns;
    for ( typename openip::Vector<T>::vIt vit= v.begin(); vit != v.end(); ++vit )
    {
        *(this->begin() + (j++))= *vit;
    }
}

template<typename T>
void openip::Matrix<T>::setColumn(Vector<T>& v, int i)
{
    int j= 0;
    for ( typename openip::Vector<T>::vIt vit= v.begin(); vit != v.end(); ++vit )
    {
        *(this->begin() + i + (j++)*columns)= *vit;
    }
}

template<typename T>
void openip::Matrix<T>::transpose()
{
    Matrix<T> m(this->columns, this->rows);
    m= 0;
    
    //T tmp;
    for ( int i= 0; i < rows; ++i )
        for (int j= 0; j < columns; ++j )
	  m(j,i)= (*this)(i,j);
    (*this)= m;
    //printf("transpose finished\n"); fflush(stdout);
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Matrix<T>& m)
{
    o << "[Matrix - size: ";
    o << m.columns;
    o << " x ";
    o << m.rows;
    o << ": ";
    for ( typename Matrix<T>::const_iterator it= m.begin(); it != m.end(); ++it )
    {
        o << *it;
        o << " ";
    }
    o << "]";
    
    return o;
}

template<typename T>
std::ifstream& openip::operator>>(std::ifstream& file, openip::Matrix<T>& m)
{
    int rows;
    int columns;
    file >> rows;
    file >> columns;

    m.resize(rows, columns);

    T tmp;

    for ( int i= 0; i < m.rows; ++i )
        for ( int j= 0; j < m.columns; ++j )
        {
            file >> tmp;
            m(i,j)= tmp;
        }

    return file;
}

template<typename T>
std::ofstream& openip::operator<<(std::ofstream& file, openip::Matrix<T>& m)
{
    file << m.rows << " " << m.columns << std::endl;

    for ( int i= 0; i < m.rows; ++i )
    {
        for ( int j= 0; j < m.columns; ++j )
        {
            file << m(i,j) << " ";
        }
        file << std::endl;
    }

    return file;
}

#endif	/* _MATRIX_H */
