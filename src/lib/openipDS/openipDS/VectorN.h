/**
 * @file VectorN.h
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
 * The VectorN is an N dimensional vector.
 */

#ifndef _VectorN_H
#define _VectorN_H

#include <stdarg.h>
#include <iostream>

#include <openipDS/Vector.h>

namespace openip
{
    template<typename T, int SIZE>
    class MatrixNN;

    /**
     * VectorN class represents a vector with fixed N size
     */
    template<typename T, int SIZE>
    class VectorN: public Vector<T>
    {
    public:
        using Vector<T>::erase;
        /**
         * VectorN<T, SIZE>::iterator typedef
         */
        typedef typename VectorN<T, SIZE>::iterator vIt;

        /**
         * VectorN<T, SIZE>::const_iterator typedef
         */
        typedef typename VectorN<T, SIZE>::const_iterator const_vIt;

        /**
         * Default constructor, creates a new vector instance with size
         * and type appropriate to the template paramteres
         */
        VectorN();

        /**
         * Similar to the default constructor, but initializes the
         * vector with value values.
         */
        VectorN(const T& value);

	/**
	* sets the elements of the vector
	* @param first first element
	* @param second second element
	* @param third third element
	* @param fourth fourth element
	* @param fifth fifth element
	* @param sixth sixth element
	* @param seventh seventh element
	* @param eighth eighth element
	* @param nineth nineth element
	* @param tenth tenth element
	*/
        VectorN(T first, T second, T third= 0, T fourth= 0, T fifth= 0, T sixth= 0, T seventh= 0, T eighth= 0, T nineth= 0, T tenth= 0);
        
        /**
         * Copy constructor
         *
         * @param p instance to copy
         */
        VectorN(const VectorN& p);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator==(const VectorN& b);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator!=(const VectorN& b);

        /**
         * sets the values of this instance to the values of the other
         *
         * @param b the instance to copy
         * @return this instance
         */
        VectorN& operator=(const VectorN& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        VectorN& operator+=(const VectorN& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        VectorN& operator-=(const VectorN& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        VectorN& operator*=(const VectorN& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        VectorN& operator/=(const VectorN& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        VectorN operator+(const VectorN& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        VectorN operator-(const VectorN& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        VectorN operator*(const VectorN& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        VectorN operator/(const VectorN& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        VectorN& operator=(const T& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        VectorN& operator+=(const T& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        VectorN& operator-=(const T& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        VectorN& operator*=(const T& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        VectorN& operator/=(const T& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        VectorN operator+(const T& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        VectorN operator-(const T& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        VectorN operator*(const T& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        VectorN operator/(const T& t) const;

	/**
	* computes column-row product between this and vector v
	* @param v to compute column-row product with
	* @return the result nxn sized matrix
	*/
        MatrixNN<T, SIZE> columnRowProduct(VectorN v);
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v VectorN object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T, int SIZE>
    std::ostream& operator<<(std::ostream& o, const VectorN<T, SIZE>& v);

    /**
     * << operator: file output
     * @param o output file stream
     * @param v vector to write to file
     * @return file stream
     */
    template<typename T, int SIZE>
    std::ofstream& operator<<(std::ofstream& o, const VectorN<T, SIZE>& v);

    /**
     * >> operator: file input
     * @param o input file stream
     * @param v vector to fill from file
     * @return file stream
     */
    template<typename T, int SIZE>
    std::ifstream& operator>>(std::ifstream& o, VectorN<T, SIZE>& v);
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>::VectorN()
: openip::Vector<T>(SIZE)
{
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>::VectorN(const T& value)
: openip::Vector<T>(SIZE, value)
{
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>::VectorN(T first, T second, T third, T fourth, T fifth, T sixth, T seventh, T eighth, T nineth, T tenth)
: openip::Vector<T>(SIZE)
{
    (*this)(0)= first;
    switch(SIZE)
    {
        case 10: (*this)(9)= tenth;
        case 9: (*this)(8)= nineth;
        case 8: (*this)(7)= eighth;
        case 7: (*this)(6)= seventh;
        case 6: (*this)(5)= sixth;
        case 5: (*this)(4)= fifth;
        case 4: (*this)(3)= fourth;
        case 3: (*this)(2)= third;
        case 2: (*this)(1)= second;
    }
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>::VectorN(const VectorN<T, SIZE>& v)
: openip::Vector<T>(v)
{
}

template<typename T, int SIZE>
bool openip::VectorN<T, SIZE>::operator==(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        if ( *vit != *vitB )
            return false;
    return true;
}

template<typename T, int SIZE>
bool openip::VectorN<T, SIZE>::operator!=(const VectorN<T, SIZE>& b)
{
    return ! this->operator==(b);
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator=(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit= *vitB;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator+=(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit+= *vitB;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator-=(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit-= *vitB;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator*=(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit*= *vitB;

    return *this;

}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator/=(const VectorN<T, SIZE>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit/= *vitB;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator+(const VectorN<T, SIZE>& b) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n+= b;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator-(const VectorN<T, SIZE>& b) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n-= b;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator*(const VectorN<T, SIZE>& b) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n*= b;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator/(const VectorN<T, SIZE>& b) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n/= b;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        *vit= t;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator+=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        *vit+= t;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator-=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        *vit-= t;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator*=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        *vit*= t;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE>& openip::VectorN<T, SIZE>::operator/=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        *vit/= t;

    return *this;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator+(const T& t) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n+= t;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator-(const T& t) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n-= t;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator*(const T& t) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n*= t;
}

template<typename T, int SIZE>
openip::VectorN<T, SIZE> openip::VectorN<T, SIZE>::operator/(const T& t) const
{
    openip::VectorN<T, SIZE> n(*this);

    return n/= t;
}

template<typename T, int SIZE>
openip::MatrixNN<T, SIZE> openip::VectorN<T, SIZE>::columnRowProduct(VectorN v)
{
    MatrixNN<T, SIZE> m;
    for ( int i= 0; i < SIZE; ++i )
        for ( int j= 0; j < SIZE; ++j )
        {
            m(i,j)= (*this)(i)*v(j);
        }
    return m;
}

template<typename T, int SIZE>
std::ostream& openip::operator<<(std::ostream& o, const openip::VectorN<T, SIZE>& v)
{
    o << "[VectorN - size: ";
    o << SIZE;
    o << " elements: ";

    for ( typename VectorN<T, SIZE>::const_iterator vit= v.begin(); vit != v.end(); ++vit )
        o << *vit << " ";

    o << "]";

    return o;
}

template<typename T, int SIZE>
std::ofstream& openip::operator<<(std::ofstream& o, const openip::VectorN<T, SIZE>& v)
{
    for ( typename VectorN<T, SIZE>::const_iterator vit= v.begin(); vit != v.end(); ++vit )
        o << *vit << " ";

    return o;
}

template<typename T, int SIZE>
std::ifstream& openip::operator>>(std::ifstream& i, openip::VectorN<T, SIZE>& v)
{
    for ( int j= 0; j < SIZE; ++j )
        i >> v(j);

    return i;
}

#endif	/* _VectorN_H */

