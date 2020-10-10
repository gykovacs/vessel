/**
 * @file Vector.h
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
 * The Vector class represents an N dimensional vector and has
 * several useful operators and member functions defined.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdio>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <float.h>
#include <typeinfo>
#include <mm_malloc.h>
#include <stdarg.h>

#include <openipDS/constants.h>

/**
 * allocator for aligned memory allocations
 */

using namespace std;

/**
 * align_allocator struct
 */
template <typename T> struct align_allocator
{
	/** typedef size_type*/
	typedef size_t		size_type;
	/** typdef difference_type*/
	typedef ptrdiff_t	difference_type;
	/** typdef pointer*/
	typedef T*			pointer;
	/** typedef const_pointer*/
	typedef const T*	const_pointer;
	/** typedef reference*/
	typedef T&			reference;
	/** typedef const_reference*/
	typedef const T&	const_reference;
	/** typedef value_type*/
	typedef T			value_type;

	/**rebind struct*/
	template <typename U> struct rebind
	{
		/** typedef other*/
		typedef align_allocator<U>	other;
	};

	/**
	* address
	* @param x idx
	* @return pointer
	*/
	T* address(T& x) const;
	
	/**
	* address
	* @param x idx
	* @return constant pointer
	*/
	const T* address(const T& x) const;

	/**
	* allocator
	* @param n size
	* @return pointer
	*/
	T* allocate(size_t n) const;
	
	/**
	* deallocator
	* @param p pointer
	*/
	void deallocate(T* p, size_t) const;

	/**
	* construct
	* @param p poitner
	* @param t t
	*/
	void construct(T* p, const T& t) const;
	
	/**
	* destroy
	* @param p pointer
	*/
	void destroy(T* p) const;

	/**
	* maximum size
	* @return maximum size
	*/
	size_t max_size() const;
};

template <typename T> inline T* align_allocator<T>::address(T& x) const
{
	return &x;
}

template <typename T> inline const T* align_allocator<T>::address(const T& x) const
{
	return &x;
}

template <typename T> inline size_t align_allocator<T>::max_size() const
{
	return size_t(-1) / sizeof(T);
}


template <typename T> inline void align_allocator<T>::construct(T* p, const T& t) const
{
	new(p) T(t);
}

template <typename T> inline void align_allocator<T>::destroy(T* p) const
{
	p->~T();
}

template <typename T> inline T* align_allocator<T>::allocate(size_t n) const
{
	return reinterpret_cast<T*>(_mm_malloc(n * sizeof(T), 16));
}

template <typename T> inline void align_allocator<T>::deallocate(T* p, size_t) const
{
	_mm_free(p);
}

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    class Border1
    {
    public:
        /**
         * constructor
         * @param tb top border
         * @param bb bottom border
         * @param lb left border
         * @param rb right border
         * @param bm border mode
         */
        Border1(int lb= 0, int rb= 0, int bm= BORDER_MODE_MIRRORED);

        Border1(Border1 a, Border1 b);

        /**
         * copy constructor
         * @param b instance to copy
         */
        Border1(const Border1& b);

        /**
         * destructor
         */
        ~Border1();

        /**
         * assignment operator
         * @param b the values of fields of b is assigned to the fields of *this object
         * @return reference to *this object
         */
        Border1& operator=(const Border1& b);

        int getMin();

        int getMax();

        void dump();

        /** left border */
        int leftBorder;
        /** right border */
        int rightBorder;
        /** border mode */
        int borderMode;
    };
  
    Border1 maximizeBorders(Border1& a, Border1& b);
    
    /**
    * distance of two float numbers
    * @param a number1
    * @param b number2
    * @return the distance of a and b
    */
    float distance(float a, float b);

    /**
    * square of the distance of two float numbers
    * @param a number1
    * @param b number2
    * @return the square of the distance of a and b
    */
    float distanceSquare(float a, float b);

    template<typename T>
    class Matrix;

    /**
     * Vector class representing a general sequence of elements
     */
    template<typename T>
    class Vector: public std::vector<T>
    {
    public:
        using std::vector<T>::resize;
        using std::vector<T>::erase;
        using std::vector<T>::clear;
        using std::vector<T>::size;
        using std::vector<T>::insert;
        using std::vector<T>::push_back;

        /**
         * Vector iterator type
         */
        typedef typename Vector<T>::iterator vIt;
        /**
         * Vector const_iterator type
         */
        typedef typename Vector<T>::const_iterator const_vIt;

        /**
         * Default constructor, creates a new vector instance with size
         * and type appropriate to the template paramteres
         */
        Vector();

        /**
         * Similar to the default constructor, but initializes the
         * vector with value values.
         */
        Vector(int size, const T& value= T());

        /**
         * Copy constructor
         *
         * @param p instance to copy
         */
        Vector(const Vector& p);

        virtual ~Vector();

        /**
         * std::set like find function based on algorithm::find.
         * Used for representing set like structures in a vector.
         *
         * @param object object to find
         * @return iterator to the object if found, else this->end()
         */
        const_vIt find(const T& object) const;

        /**
         * std::set like find function based on algorithm::find.
         * Used for representing set like structures in a vector.
         *
         * @param object object to find
         * @return iterator to the object if found, else this->end()
         */
        vIt find(const T& object);

        /**
         * std::set like insert function: inserts the element if it
         * is not yet in the vector
         *
         * @param object object to insert
         */
        void insert(const T& object);

        /**
         * accessing the elements of the vector through the () operator
         *
         * @param x index of the element to access
         * @return the xth element as a right value
         */
        inline T operator()(int x) const;

        /**
         * accessing the elements of the vector through the () operator
         *
         * @param x index of the element to access
         * @return the xth element as a left value
         */
        inline T& operator()(int x);

        /**
         * accessing the elements of the vector, 0 index is the middle of the vector
         * @param x index of the element to access
         * @return the xth element as left value
         */
        inline T get(int x) const;

        /**
         * accessing the elements of the vector, 0 index is the middle of the vector
         * @param x index of the element to access
         * @return the xth element as right value
         */
        inline T& get(int x);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator==(const Vector& b);

        /**
         * comparison operator element-wise
         *
         * @param b object to compare
         * @return true if equals, false if not equals
         */
        bool operator!=(const Vector& b);

        /**
         * sets the values of this instance to the values of the other
         *
         * @param b the instance to copy
         * @return this instance
         */
        Vector& operator=(const Vector& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Vector& operator+=(const Vector& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Vector& operator-=(const Vector& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Vector& operator*=(const Vector& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Vector& operator/=(const Vector& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector operator+(const Vector& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector operator-(const Vector& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector operator*(const Vector& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Vector operator/(const Vector& b) const;

	/**
	* computes the inner product of the vectors
	* @param b to compute the inner product with
	* @return the inner product
	*/
        float operator^(const Vector& b);

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        Vector& operator=(const T& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector& operator+=(const T& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector& operator-=(const T& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector& operator*=(const T& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        Vector& operator/=(const T& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector operator+(const T& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector operator-(const T& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector operator*(const T& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        Vector operator/(const T& t) const;

        /**
         * computes the square of the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        float dist(const Vector& v) const;

        /**
         * computes the distance of this instance and vector v
         *
         * @param v the vector to measure the distance from
         * @return the distance of this instance and v
         */
        float dist2(const Vector& v);

        /**
         * sets the values of this to the values of the paramter b, when b is
         * from an other type of vector with the same size
         *
         * @param b the values to copy
         * @return this instance
         */
        template<typename S>
        Vector<T>& operator=(const Vector<S>& b);

        /**
         * normalizes the values of the vector
         */
        void normalize();

        /**
         * returns the minimum element of the vector
         * @param mask binary mask (0, non 0), computes the minimum value
         * in the region covered by the foreground (non 0) of the mask
         * @return the minimum element of the vector
         */
        T getMin(Vector<unsigned char>* mask= NULL);

        /**
         * returns the maximum element of the vector
         * @param mask binary mask (0, non 0), computes the maximum value
         * in the region covered by the foreground (non 0) of the mask
         * @return the maximum element of the vector
         */
        T getMax(Vector<unsigned char>* mask= NULL);

        /**
         * returns the mean of the vector elements
         * @param mask binary mask (0, non 0), computes the mean value
         * in the region covered by the foreground (non 0) of the mask
         * @return the mean of the elements of the vector
         */
        float getMean(Vector<unsigned char>* mask= NULL);
	
	float getGMean(Vector<unsigned char>* mask= NULL);
	
	T getMedian(Vector<unsigned char>* mask= NULL);

        /**
         * returns the variance of the vector elements
         * @param mask binary mask (0, non 0), computes the variance of the
         * elements in the region covered by the foreground (non 0) of the mask
         * @return the variance of the elements of the vector
         */
        float getVariance(Vector<unsigned char>* mask= NULL);

        /**
         * returns mean and variance
         * @param mean output variable
         * @param var output variable
         * @param mask the mean and variance is computed in the foreground (non 0)
         * of the mask
         */
        void getMeanAndVariance(float& mean, float& var, Vector<unsigned char>* mask= NULL);

        /**
         * returns the standard deviation of the vector elements
         * @param mask binary mask (0, non 0), computes the standard deviation
         * of the elements in the region covered by the foreground (non 0) of
         * the mask
         * @return the standard deviation of the elements of the vector
         */
        float getStandardDeviation(Vector<unsigned char>* mask= NULL);

        /**
         * returns the minimum and maximum elements of the vector as output parameters
         *
         * @param min output parameter for the minimum element of the vector
         * @param max output parameter for the maximum element of the vector
         * @param mask binary mask (0, non 0), computes min max only in the
         * foreground region of the mask
         */
        void getMinMax(T& min, T& max, Vector<unsigned char>* mask= NULL);

        float getKurtosis(Vector<unsigned char>* mask= NULL);

        float getSkewness(Vector<unsigned char>* mask= NULL);

        float getNorm2();

	/**
	* computes the index of the closest element of the vector to a, in the foreground (non 0) region of mask
	* @param a to compute the distance of
	* @param mask the operation is performed in the foreground region
	* @return index of the closest element
	*/
        int getIndexOfClosestElement(T& a, Vector<unsigned char>* mask= NULL);

	/**
	* computes the distance of the closest element of the vector to a, in the foreground (non 0) region of mask
	* @param a to compute the distance of
	* @param mask the operation is performed in the foreground region
	* @return the distance
	*/
        float getDistanceOfClosestElement(T& a, Vector<unsigned char>* mask= NULL);

	/**
	* computes the square of the distane of the closest element of the vector to a, in the foreground (non 0) region of mask
	* @param a to compute the distance of
	* @param mask the operation is performed in the foreground region
	* @return the square of the distance
	*/
        float getDistanceSquareOfClosestElement(T& a, Vector<unsigned char>* mask= NULL);

        bool contains(const T& v);

	/**
	* computes the iner product of this and a
	* @param a to compute the inner product with
	* @return the inner product
	*/
        float inner(const Vector<T>& a);

        Vector<T> proj(const Vector<T>& v);
        /**
         * normalizes the elements of the vector in the foreground (non 0) region
         * of the mask into the range [min, max]
         *
         * @param min lower bound of the range to normalize into
         * @param max upper bound of the range to normalize into
         * @param mask mask, the forground region will be normalized
         */
        void normalize(T min, T max, Vector<unsigned char>* mask= NULL);
	
	void normalize(T wmin, T wmax, T min, T max, Vector<unsigned char>* mask= NULL);

        /**
         * computes the sum of elements in the vector
         * @param mask binary mask (0, non 0), computes the sum of elements
         * in the region covered by the foreground (non 0) of the mask
         * @return the sum of elements in the vector
         */
        float getSum(Vector<unsigned char>* mask= NULL);
	
	float getCorrelation(Vector<T>& v);

        /**
         * computes column-row product with the parameter vector
         * @param v parameter to compute the column-row product with
         * @return the product matrix
         */
        Matrix<T> columnRowProduct(Vector<T> v);

        int numberOfNonZeroElements(Vector<unsigned char>* mask= NULL);
        int numberOfZeroElements();
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v Vector object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Vector<T>& v);

    /**
     * fills the parameter Vector object from the parameter input file stream
     * @param file parameter input file stream
     * @param v Vector object to describe
     * @return reference to the parameter input file stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, openip::Vector<T>& v);

    /**
     * prints the description of the parameter Vector object to the parameter output file stream
     * @param file parameter output file stream
     * @param v Vector object to describe
     * @return reference to the parameter output file stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, openip::Vector<T>& v);

    /**
    * computes the Euclidean distance of two Vector objects
    * @param a vector1
    * @param b vector2
    * @return Euclidean distance of Vectors a and b
    */
    template<typename T>
    float distance(Vector<T>& a, Vector<T>& b);

    template<typename T>
    float distance(Vector<T>& a, Vector<T>& b, Vector<unsigned char>* mask);

    /**
    * computes the square of the Euclidean distance of two Vector objects
    * @param a vector1
    * @param b vector2
    * @return the square of the Euclidean distance of Vectors a and b
    */
    template<typename T>
    float distanceSquare(Vector<T>& a, Vector<T>& b);
}

template<typename T>
float openip::distance(openip::Vector<T>& a, openip::Vector<T>& b)
{
    return a.dist(b);
}

template<typename T>
float openip::distance(openip::Vector<T>& a, openip::Vector<T>& b, openip::Vector<unsigned char>* mask)
{
    float sum= 0;
    for ( int i= 0; i < b.size(); ++i )
        if ( (*mask)(i) )
            sum+= (a(i) - b(i))*(a(i) - b(i));
    return sqrt(sum);
}

template<typename T>
float openip::distanceSquare(openip::Vector<T>& a, openip::Vector<T>& b)
{
    return a.dist2(b);
}

template<typename T>
openip::Vector<T>::Vector()
: std::vector<T>()
{
}

template<typename T>
openip::Vector<T>::Vector(int size, const T& value)
: std::vector<T>(size, value)
{
}

template<typename T>
openip::Vector<T>::Vector(const Vector<T>& v)
: std::vector<T>(v)
{
}

template<typename T>
openip::Vector<T>::~Vector()
{
}

template<class T>
inline T openip::Vector<T>::operator()(int x) const
{
    return this->vector<T>::operator[](x);
}

template<class T>
inline T& openip::Vector<T>::operator()(int x)
{
    return this->vector<T>::operator[](x);
}

template<class T>
inline T openip::Vector<T>::get(int x) const
{
    return (*this)[x + this->size()/2];
}

template<class T>
inline T& openip::Vector<T>::get(int x)
{
    return (*this)[x + this->size()/2];
}

template<typename T>
bool openip::Vector<T>::operator==(const Vector& b)
{
    const_vIt vitB= b.begin();
    vIt vit;

    for ( vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        if ( *vit != *vitB )
            return false;
    return true;
}

template<typename T>
bool openip::Vector<T>::operator!=(const Vector& b)
{
    return ! this->operator==(b);
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator=(const Vector<T>& b)
{
    this->vector<T>::operator=(b);
    /*const_vIt vitB= b.begin();
    vIt vit;

    for ( vit= this->begin(); vit != this->end(); ++vit, ++vitB )
            *vit= *vitB;*/

    return *this;
}

template<typename T> template<typename S>
openip::Vector<T>& openip::Vector<T>::operator=(const Vector<S>& b)
{
    this->resize(b.size());
    typename Vector<S>::const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit= *vitB;
    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator+=(const Vector<T>& b)
{
    const_vIt vitB= b.begin();
    vIt vit;

    for ( vit= this->begin(); vit != this->end(); ++vit, ++vitB )
        *vit+= *vitB;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator-=(const Vector<T>& b)
{
    const_vIt vitB= b.begin();
    vIt vit;

    for ( vit= this->begin(); vit != this->end(); ++vit, ++vitB )
            *vit-= *vitB;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator*=(const Vector<T>& b)
{
    const_vIt vitB= b.begin();
    vIt vit;

    for ( vit= this->begin(); vit != this->end(); ++vit, ++vitB )
            *vit*= *vitB;

    return *this;

}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator/=(const Vector<T>& b)
{
    const_vIt vitB= b.begin();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++vitB )
            *vit/= *vitB;

    return *this;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator+(const Vector<T>& b) const
{
	openip::Vector<T> n(*this);

	return n+= b;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator-(const Vector<T>& b) const
{
	openip::Vector<T> n(*this);

	return n-= b;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator*(const Vector<T>& b) const
{
	openip::Vector<T> n(*this);

	return n*= b;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator/(const Vector<T>& b) const
{
	openip::Vector<T> n(*this);

	return n/= b;
}

template<typename T>
float openip::Vector<T>::operator^(const Vector<T>& b)
{
    return this->inner(b);
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
            *vit= t;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator+=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
            *vit+= t;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator-=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
            *vit-= t;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator*=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
            *vit*= t;

    return *this;
}

template<typename T>
openip::Vector<T>& openip::Vector<T>::operator/=(const T& t)
{
    for ( vIt vit= this->begin(); vit != this->end(); ++vit )
            *vit/= t;

    return *this;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator+(const T& t) const
{
	openip::Vector<T> n(*this);

	return n+= t;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator-(const T& t) const
{
	openip::Vector<T> n(*this);

	return n-= t;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator*(const T& t) const
{
	openip::Vector<T> n(*this);

	return n*= t;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::operator/(const T& t) const
{
	openip::Vector<T> n(*this);

	return n/= t;
}

template<class T>
typename openip::Vector<T>::const_vIt openip::Vector<T>::find(const T& object) const
{
    return std::find(this->begin(), this->end(), object);
}

template<typename T>
typename openip::Vector<T>::vIt openip::Vector<T>::find(const T& object)
{
    return std::find(this->begin(), this->end(), object);
}

template<typename T>
void openip::Vector<T>::insert(const T& object)
{
    if ( this->find(object) == this->end() )
        this->push_back(object);
}

template<typename T>
float openip::Vector<T>::getSum(openip::Vector<unsigned char>* mask)
{
    float sum= 0;

    if ( mask == NULL )
        for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            sum+= (float)(*vit);
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit)
            if ( *mit )
                sum+= (float)(*vit);
    }

    return sum;
}

template<typename T>
T openip::Vector<T>::getMin(openip::Vector<unsigned char>* mask)
{
    T min;
    int i;

    if ( mask == NULL )
    {
        min= *(this->begin());
        for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            if ( *vit < min )
            {
                min= *vit;
            }
    }
    else
    {
        for ( i= 0; (*mask)(i) == 0; ++i );
        min= (*this)[i];

        Vector<unsigned char>::vIt mit= mask->begin() + i;
        for ( vIt vit= this->begin() + i; vit != this->end(); ++vit, ++mit)
            if ( *mit && *vit < min )
                min= *vit;
    }
    return min;
}

template<typename T>
T openip::Vector<T>::getMax(openip::Vector<unsigned char>* mask)
{
    T max;
    int i;

    if ( mask == NULL )
    {
        max= *(this->begin());
        for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            if ( *vit > max )
            {
                max= *vit;
            }
    }
    else
    {
        for ( i= 0; (*mask)(i) == 0; ++i );
        max= (*this)[i];

        Vector<unsigned char>::vIt mit= mask->begin() + i;
        for ( vIt vit= this->begin() + i; vit != this->end(); ++vit, ++mit)
            if ( *mit && *vit > max )
            {
                max= *vit;
            }
    }
    return max;
}

template<typename T>
float openip::Vector<T>::getMean(openip::Vector<unsigned char>* mask)
{
    float sum= 0;
    int n= 0;

    if ( mask == NULL )
    {
        for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            sum+= (float)(*vit);
        sum/= this->size();
    }
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit)
            if ( *mit )
            {
                ++n;
                sum+= (float)(*vit);
            }
        if ( n == 0 )
            return 0;
        sum/= n;
    }
    return sum;
}

template<typename T>
float openip::Vector<T>::getGMean(openip::Vector<unsigned char>* /*mask*/)
{
    float gmean= 1;
    
    for ( unsigned int i= 0; i < this->size(); ++i )
      gmean*= (*this)(i);
    
    return pow(gmean, 1.0/this->size());
}

template<typename T>
T openip::Vector<T>::getMedian(openip::Vector<unsigned char>* mask)
{
  Vector<T> tmp;
  
  for ( unsigned int i= 0; i < this->size(); ++i )
    if ( !mask || (*mask)(i) )
      tmp.push_back((*this)(i));

  sort(tmp.begin(), tmp.end());
  if ( tmp.size() > 0 )
    return tmp(tmp.size()/2);
  else
    return 0;
}

template<typename T>
float openip::Vector<T>::getVariance(openip::Vector<unsigned char>* mask)
{
    float mean= this->getMean(mask);

    float var= 0;
    int n= 0;
    float v;

    if ( mask == NULL )
    {
        for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        {
            v= (float)(*vit - mean);
            var+= v*v;
        }
        var/= (this->size() - 1);
    }
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
            if ( *mit )
            {
                ++n;
                v= (float)(*vit - mean);
                var+= v*v;
            }
        if ( n == 0 )
            return 0;
        var/= n;
    }

    return var;
}

template<typename T>
void openip::Vector<T>::getMeanAndVariance(float& mean, float& var, Vector<unsigned char>* mask)
{
    float sum= 0, sumSqr= 0;
    float tmp;

    if ( mask == NULL )
    {
        //printf("mask=NULL\n");
        for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        {
            tmp= *vit;
            sum+= tmp;
            sumSqr+= tmp*tmp;
        }
        mean= sum/this->size();
        var= (sumSqr/this->size() - mean * mean);
        //printf("%f %f\n", mean, var);
    }
    else
    {
        //printf("mask!=NULL\n");
        int n= 0;
        /*Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if ( *mit > 0 )
            {
                tmp= *vit;
                sum+= tmp;
                sumSqr+= tmp*tmp;
                ++n;
            }
        }
        mean= sum/n;
        var= (sumSqr/n - mean * mean);*/
        
        float min, max;
        min= FLT_MAX;
        max= -FLT_MAX;
        mean= 0;
        var= 0;
        
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if ( *mit > 0 )
            {
                mean+= *vit;
                ++n;
                
                if ( *vit < min )
                    min= *vit;
                else if ( *vit > max )
                    max= *vit;
            }
        }
        mean= mean/n;
        
        mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if ( *mit > 0 )
            {
                var+= (*vit - mean)*(*vit - mean);
            }
        }
        var= var/n;
        
        //printf("min: %.1f max: %.1f mean: %.1f var: %.1f\n", min, max, mean, var);
    }
}

template<typename T>
float openip::Vector<T>::getStandardDeviation(openip::Vector<unsigned char>* mask)
{
    return sqrt(this->getVariance(mask));
}

template<typename T>
void openip::Vector<T>::getMinMax(T& min, T& max, openip::Vector<unsigned char>* mask)
{
  if ( !mask )
    min= max= (*this)(0);
  else
  {
    min= max= 0;
    for ( unsigned int i= 0; i < mask->size(); ++i )
      if ( (*mask)(i) )
      {
	min= max= (*this)(i);
	break;
      }
  }

    if ( mask == NULL )
    {
        for ( vIt vit= this->begin(); vit != this->end(); ++vit)
        {
            if ( *vit > max )
                max= *vit;
            if ( *vit < min )
                min= *vit;
        }
    }
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if (*mit > 0)
            {
                if ( *vit > max )
                    max= *vit;
                if ( *vit < min )
                    min= *vit;
            }
        }
    }
}

template<typename T>
float openip::Vector<T>::getKurtosis(Vector<unsigned char> *mask)
{
    float tmp= 0;
    float tmp2= 0;
    float tmp3= 0;

    float mean= this->getMean(mask);

    if ( mask == NULL )
    {
        for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        {
            tmp= *vit - mean;
            tmp*= tmp;
            tmp2+= tmp;
            tmp3+= tmp*tmp;
        }
    }
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if ( *mit > 0 )
            {
                tmp= *vit - mean;
                tmp*= tmp;
                tmp2+= tmp;
                tmp3+= tmp*tmp;
            }
        }
    }

    if ( tmp2 > 0 )
        return tmp3/(tmp2*tmp2);
    else
        return 0;
}

template<typename T>
float openip::Vector<T>::getSkewness(Vector<unsigned char>* mask)
{
    float tmp= 0;
    float tmp2= 0;
    float tmp3= 0;

    float mean= this->getMean(mask);

    if ( mask == NULL )
    {
        for ( vIt vit= this->begin(); vit != this->end(); ++vit )
        {
            tmp= *vit - mean;
            tmp*= tmp;
            tmp2+= tmp;
            tmp3+= tmp * (*vit - mean);
        }
    }
    else
    {
        Vector<unsigned char>::vIt mit= mask->begin();
        for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
        {
            if ( *mit > 0 )
            {
                tmp= *vit - mean;
                tmp*= tmp;
                tmp2+= tmp;
                tmp3+= tmp * (*vit - mean);
            }
        }
    }

    if ( tmp2 > 0 )
        return tmp3/pow(tmp2,3.0/2.0);
    else
        return 0;
}

template<typename T>
float openip::Vector<T>::getCorrelation(Vector<T>& v)
{
  float meana= this->getMean();
  float meanb= v.getMean();
  float stdeva= this->getStandardDeviation();
  float stdevb= v.getStandardDeviation();
  
  float result= 0;
  for ( unsigned int i= 0; i < this->size(); ++i )
    result+= ((*this)(i) - meana)*(v(i) - meanb);
  result/=this->size();
  result/=(stdeva*stdevb);

  return result;
}

template<typename T>
float openip::Vector<T>::getNorm2()
{
    float norm= 0;
    for ( unsigned int i= 0; i < this->size(); ++i )
        norm+= (*this)(i)*(*this)(i);
    return sqrt(norm);
}

template<typename T>
int openip::Vector<T>::getIndexOfClosestElement(T& a, Vector<unsigned char>* mask)
{
    float dist= FLT_MAX;
    int idx= -1;
    float tmp;

    if ( mask == NULL )
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= openip::distance((*this)[i], a);
            if ( tmp < dist )
            {
                dist= tmp;
                idx= i;
            }
        }
    }
    else
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                tmp= openip::distance((*this)[i], a);
                if ( tmp < dist )
                {
                    dist= tmp;
                    idx= i;
                }
            }
        }
    }

    return idx;
}

template<typename T>
float openip::Vector<T>::getDistanceOfClosestElement(T& a, Vector<unsigned char>* mask)
{
    float dist= FLT_MAX;
    float tmp;

    if ( mask == NULL )
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= openip::distance((*this)[i], a);
            if ( tmp < dist )
                dist= tmp;
        }
    }
    else
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                tmp= openip::distance((*this)[i], a);
                if ( tmp < dist )
                    dist= tmp;
            }
        }
    }

    return dist;
}

template<typename T>
float openip::Vector<T>::getDistanceSquareOfClosestElement(T& a, Vector<unsigned char>* mask)
{
    float dist= FLT_MAX;
    float tmp;

    if ( mask == NULL )
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= openip::distanceSquare((*this)[i], a);
            if ( tmp < dist )
                dist= tmp;
        }
    }
    else
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                tmp= openip::distanceSquare((*this)[i], a);
                if ( tmp < dist )
                    dist= tmp;
            }
        }
    }

    return dist;
}

template<typename T>
bool openip::Vector<T>::contains(const T& v)
{
    for ( unsigned int i= 0; i < this->size(); ++i )
        if ( (*this)(i) == v )
            return true;
    return false;
}

template<typename T>
void openip::Vector<T>::normalize()
{
    float sum= this->getSum();
    for ( vIt vit= this->begin(); vit != this->end(); ++vit)
        *vit= (T)(*vit/sum);
}

template<typename T>
void openip::Vector<T>::normalize(T min, T max, openip::Vector<unsigned char>* mask)
{
    T tmin, tmax;
    this->getMinMax(tmin, tmax, mask);
    float td= (float)(tmax - tmin);
    float d= (float)(max - min);

    if ( mask == NULL )
    {
        if ( d > 0 )
            for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            {
                *vit= (T)((float(*vit) - tmin) / td * d + (float)min);
            }
    }
    else
    {
        Vector<unsigned char>::iterator mit= mask->begin();
        if ( d > 0 )
            for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
                if ( *mit > 0 )
                    *vit= (T)(((float)(*vit) - tmin) / td * d + (float)min);
    }
}

template<typename T>
void openip::Vector<T>::normalize(T wmin, T wmax, T min, T max, openip::Vector<unsigned char>* mask)
{
    T tmin= wmin/*, tmax= wmax*/;
    float td= (float)(wmax - wmin);
    float d= (float)(max - min);

    if ( mask == NULL )
    {
        if ( d > 0 )
            for ( vIt vit= this->begin(); vit != this->end(); ++vit)
            {
	      if ( *vit < wmin )
		*vit= min;
	      else if ( *vit > wmax )
		*vit= max;
	      else
                *vit= (T)((float(*vit) - tmin) / td * d + (float)min);
            }
    }
    else
    {
        Vector<unsigned char>::iterator mit= mask->begin();
        if ( d > 0 )
            for ( vIt vit= this->begin(); vit != this->end(); ++vit, ++mit )
                if ( *mit > 0 )
		{
		  if ( *vit < wmin )
		    *vit= min;
		  else if ( *vit > wmax )
		    *vit= max;
		  else
                    *vit= (T)(((float)(*vit) - tmin) / td * d + (float)min);
		}
    }
}

template<typename T>
float openip::Vector<T>::dist(const Vector<T>& v) const
{
    float d= 0;
    const_vIt vitV= v.begin();

    for ( const_vIt vit= this->begin(); vit != this->end(); ++vit, ++vitV)
        d+= (float)((*vit - *vitV) * (*vit - *vitV));

    return d;
}

template<typename T>
float openip::Vector<T>::dist2(const Vector<T>& v)
{
    return sqrt(this->dist(v));
}

template<typename T>
float openip::Vector<T>::inner(const Vector<T>& a)
{
    float ip= 0;
    for ( unsigned int i= 0; i < this->size(); ++i )
        ip+= (*this)(i)*a(i);

    return ip;
}

template<typename T>
openip::Vector<T> openip::Vector<T>::proj(const Vector<T>& v)
{
    float f= this->inner(v);
    f/= this->inner(*this);
    return (*this)*f;
}

template<typename T>
openip::Matrix<T> openip::Vector<T>::columnRowProduct(Vector<T> v)
{
    openip::Matrix<T> m(this->size(), this->size());
    for ( unsigned int i= 0; i < this->size(); ++i )
        for ( unsigned int j= 0; j < this->size(); ++j )
        {
            m(i,j)= (*this)(i)*v(j);
        }
    return m;
}

template<typename T>
int openip::Vector<T>::numberOfNonZeroElements(Vector<unsigned char>* mask)
{
    int n= 0;
    for ( unsigned int i= 0; i < this->size(); ++i )
      if ( !mask || (*mask)(i) )
	if ( (*this)(i) )
	  ++n;
    return n;
}

template<typename T>
int openip::Vector<T>::numberOfZeroElements()
{
    return this->size() - numberOfNonZeroElements();
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Vector<T>& v)
{
    o << "[Vector - size: ";
    o << v.size();
    o << " :";

    if ( v.size() < 400 )
    {
        for ( typename Vector<T>::const_iterator it= v.begin(); it != v.end(); ++it )
        {
            o << " ";
            o << *it;
        }
    }

    o << "]";

    return o;
}

template<typename T>
std::ofstream& openip::operator<<(std::ofstream& file, openip::Vector<T>& v)
{
    file << v.size() << " ";
    for ( unsigned int i= 0; i < v.size(); ++i )
    {
        file << v(i) << " ";
    }

    return file;
}

template<typename T>
std::ifstream& openip::operator>>(std::ifstream& file, openip::Vector<T>& v)
{
    int k;
    T tmp;
    file >> k;
    v.resize(k);
    for ( int i= 0; i < k; ++i )
    {
        file >> tmp;
        v(i)= tmp;
    }
    
    return file;
}

#endif
