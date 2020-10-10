/**
 * @file Volume.h
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
 * The Volume class represents a template Volume type and has
 * several useful operators and member functions defined.
 */


#ifndef _VOLUME_H
#define	_VOLUME_H

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <openipDS/Matrix.h>
#include <openipDS/Pixel.h>
#include <openipDS/constants.h>
#include <openipDS/Voxel.h>
#include <openipDS/Image.h>

namespace openip
{
    class Border3: public Border2
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
        Border3(int tb= 0, int bb= 0, int lb= 0, int rb= 0, int fb= 0, int backb= 0, int bm= BORDER_MODE_MIRRORED);

        Border3(Border3 a, Border3 b);

        /**
         * copy constructor
         * @param b instance to copy
         */
        Border3(const Border3& b);

        /**
         * destructor
         */
        ~Border3();

        /**
         * assignment operator
         * @param b the values of fields of b is assigned to the fields of *this object
         * @return reference to *this object
         */
        Border3& operator=(const Border3& b);

        void dump();

        /** top border */
        int frontBorder;
        /** bottom border */
        int backBorder;
    };

    Border3 maximizeBorders(Border3& a, Border3& b);

    /**
     * Volume represents an Volume with size template type T
     */
    template<typename T>
    class Volume : public Vector<T>
    {
    public:
        using Vector<T>::operator();
        using Vector<T>::operator !=;
        using Vector<T>::operator ==;

        /**
         * Volume<T>::iterator typedef
         */
        typedef typename Volume<T>::iterator vIt;

        /**
         * Volume<T>::const_iterator typedef
         */
        typedef typename Volume<T>::const_iterator const_vIt;

        /**
         * default constructor
         */
        Volume();

        /**
         * copy constructor
         * @param b object to copy
         */
        
        Volume(const Volume& b);

        /**
         * constructor
         * @param slices number of slices
         * @param rows number of rows
         * @param columns number of columns
         * @param topBorder number of top border rows
         * @param bottomBorder number of bottom border rows
         * @param leftBorder number of left border columns
         * @param rightBorder number of right border columns
         * @param frontBorder number of front border columns
         * @param backBorder number of back border columns
         * @param borderMode mode of filling border pixels
         */
        Volume(unsigned int slices, unsigned int rows, unsigned int columns, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
        
        Volume(unsigned int slices, unsigned int rows, unsigned int columns, Border3 b);

        /**
         * desctructor
         */
        ~Volume();

        /**
         * accessing the element of the matrix (stored row-continuosly) as rvalue
         * @param x Pixel2 type coordinates of element to access
         * @return the (x.x() x.y())th element
         */
        inline T operator()(Voxel1 x) const;

        /**
         * accessing the element of the matrix (stored row-continuosly) as lvalue
         * @param x Pixel2 type coordinates of element to access
         * @return the (x.x() x.y())th element
         */
        inline T& operator()(Voxel1 x);

        /**
         * access operator
         * @param s slice index
         * @param r row index
         * @param c column index
         * @return accessed element
         */
        inline T operator()(int s, int r, int c) const;

        /**
         * access operator
         * @param s slice index
         * @param r row index
         * @param c column index
         * @return accessed element
         */
        inline T& operator()(int s, int r, int c);

        /**
         * access operator
         * @param v voxel index
         * @return accessed element
         */
        inline T operator()(Voxel3& v) const;

        /**
         * access operator
         * @param v voxel index
         * @return accessed element
         */
        inline T& operator()(Voxel3& v);

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Volume& operator=(const Volume& b);

        /**
         * assignment operator for different types of Volumes
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Volume<T>& operator=(const Volume<S>& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Volume& operator+=(const Volume& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Volume& operator-=(const Volume& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Volume& operator*=(const Volume& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Volume& operator/=(const Volume& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Volume& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Volume& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Volume& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Volume& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Volume& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Volume operator+(const Volume& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Volume operator-(const Volume& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Volume operator*(const Volume& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Volume operator/(const Volume& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Volume operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Volume operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Volume operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Volume operator/(const T& t) const;

        /**
         * fills the border pixels of the Volume
         */
        void fillBorder();

        /**
         * resizes the Volume to size rows x columns
         * @param rows the number of rows the Volume to resize will have
         * @param columns the number of columns the Volume to resize will have
         * @param slices the number of slices the Volume to resize will have
         */
        void resizeVolume(unsigned int slices, unsigned int rows, unsigned int columns, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
        
        void resizeVolume(unsigned int slices, unsigned int rows, unsigned int columns, Border3 b);
        
        template<typename S>
        void resizeVolume(Volume<S>& v);
        
        void resizeBorder(Border3 b);
        
        void resizeBorder(int leftBorder, int rightBorder, int topBorder, int bottomBorder, int frontBorder, int backBorder, int borderMode= BORDER_MODE_MIRRORED);

        void removeBorder();

        Border3 getBorder();

        /**
         * get row continuous coordinate
         * @param s input parameter slice index
         * @param r input parameter row index
         * @param c input parameter column index
         * @param n output parameter row-continuous index
         */
        void getCoordinate1D(int s, int r, int c, int& n);

        /**
         * get x and y index from row-continuous index
         * @param n input row-continuous index
         * @param s output slice index
         * @param r output row index
         * @param c output column index
         */
        void getCoordinate3D(int n, int& s, int& r, int& c);

        /**
         * returns the data pointer of the Volume container object
         * @return data pointer
         */
        T* data();

        bool isRealVolumeVoxel(int i);
        bool isRealVolumeVoxel(int s, int r, int c);

        template<typename S>
        void getImage(Image<S>& image, int dim, int slice);

        template<typename S>
        void setImage(Image<S>& image, int dim, int slice);

        template<typename S>
        void addImage(Image<S>& image, int dim, int slice);

        template<typename S>
        void maxImage(Image<S>& image, int dim, int slice);
	
	void clearSlice(int i);

        /**
         * number of elements in the Volume: w*h
         */
        unsigned int n;

        /** slice size */
        unsigned int sliceSize;

        /** number of slices */
        unsigned int slices;

        /** number of rows */
        unsigned int rows;

        /** number of columns */
        unsigned int columns;

        /**
         * number of top border rows
         */
        int topBorder;

        /**
         * number of bottom border rows
         */
        int bottomBorder;

        /**
         * number of left border rows
         */
        int leftBorder;

        /**
         * number of right border rows
         */
        int rightBorder;

        /**
         * number of front border columns
         */
        int frontBorder;

        /**
         * number of back border columns
         */
        int backBorder;

        /**
         * mode of the border filling
         */
        int borderMode;

        int mincVersion;
        std::string filename;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v Volume object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Volume<T>& v);

    /**
    * << operator: file output
    *
    * @param o file output stream object
    * @param v Volume object to write into file
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& o, const Volume<T>& v);

    /**
    * >> operator: file input
    *
    * @param o file input stream object
    * @param v Volume object to fill from the input stream
    * @return the parameter input stream
    * reads the contents of a Volume into the volume parameter
    */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, Volume<T>& v);
}

template<class T>
openip::Volume<T>::Volume()
    : Vector<T>(0), n(0), topBorder(0), bottomBorder(0), leftBorder(0), rightBorder(0), frontBorder(0), backBorder(0), borderMode(BORDER_MODE_MIRRORED), mincVersion(2)
{
}

template<class T>
openip::Volume<T>::Volume(unsigned int s_, unsigned int r_, unsigned int c_, int frontBorder_, int backBorder_, int topBorder_, int bottomBorder_, int leftBorder_, int rightBorder_, int borderMode_)
    : Vector<T>(s_*r_*c_), n(s_*r_*c_), sliceSize(r_*c_), topBorder(topBorder_), bottomBorder(bottomBorder_), leftBorder(leftBorder_), rightBorder(rightBorder_), frontBorder(frontBorder_), backBorder(backBorder_), borderMode(borderMode_), mincVersion(2)
{
    rows= r_;
    columns= c_;
    slices= s_;
}

template<class T>
openip::Volume<T>::Volume(unsigned int s_, unsigned int r_, unsigned int c_, Border3 b)
    : Vector<T>(s_*r_*c_), n(s_*r_*c_), sliceSize(r_*c_), topBorder(b.topBorder), bottomBorder(b.bottomBorder), leftBorder(b.leftBorder), rightBorder(b.rightBorder), frontBorder(b.frontBorder), backBorder(b.backBorder), borderMode(b.borderMode), mincVersion(2)
{
    rows= r_;
    columns= c_;
    slices= s_;
}

template<class T>
openip::Volume<T>::Volume(const Volume& b)
: Vector<T>(b), n(b.n)
{
    rows= b.rows;
    columns= b.columns;
    slices= b.slices;
    frontBorder= b.frontBorder;
    backBorder= b.backBorder;
    topBorder= b.topBorder;
    bottomBorder= b.bottomBorder;
    leftBorder= b.leftBorder;
    rightBorder= b.rightBorder;
    borderMode= b.borderMode;
    sliceSize= b.sliceSize;
    mincVersion= b.mincVersion;
}

template<class T>
openip::Volume<T>::~Volume()
{
}

template<class T>
inline T openip::Volume<T>::operator()(Voxel1 p) const
{
    return this->Vector<T>::operator()(p);
}

template<class T>
inline T& openip::Volume<T>::operator()(Voxel1 p)
{
    return this->Vector<T>::operator()(p);
}

template<class T>
inline T openip::Volume<T>::operator()(int s, int r, int c) const
{
    return this->operator()(s*rows*columns + r*columns + c);
}

template<class T>
inline T& openip::Volume<T>::operator()(int s, int r, int c)
{
    return this->operator()(s*rows*columns + r*columns + c);
}

template<class T>
inline T openip::Volume<T>::operator()(Voxel3& p) const
{
    return this->operator()(p.s, p.r, p.c);
}

template<class T>
inline T& openip::Volume<T>::operator()(Voxel3& p)
{
    return this->operator()(p.s, p.r, p.c);
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator=(const openip::Volume<T>& b)
{
    this->Vector<T>::operator=(b);
    this->n= b.n;
    this->sliceSize= b.sliceSize;
    this->topBorder= b.topBorder;
    this->bottomBorder= b.bottomBorder;
    this->leftBorder= b.leftBorder;
    this->rightBorder= b.rightBorder;
    this->frontBorder= b.frontBorder;
    this->backBorder= b.backBorder;
    this->rows= b.rows;
    this->columns= b.columns;
    this->borderMode= b.borderMode;
    this->mincVersion= b.mincVersion;

    return *this;
}

template<typename T> template<typename S>
openip::Volume<T>& openip::Volume<T>::operator=(const Volume<S>& b)
{
    this->Vector<T>::operator=(b);
    this->n= b.n;
    this->sliceSize= b.sliceSize;
    this->topBorder= b.topBorder;
    this->bottomBorder= b.bottomBorder;
    this->leftBorder= b.leftBorder;
    this->rightBorder= b.rightBorder;
    this->frontBorder= b.frontBorder;
    this->backBorder= b.backBorder;
    this->rows= b.rows;
    this->columns= b.columns;
    this->borderMode= b.borderMode;
    this->mincVersion= b.mincVersion;

    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator+=(const openip::Volume<T>& b)
{
    this->Vector<T>::operator+=(b);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator-=(const openip::Volume<T>& b)
{
    this->Vector<T>::operator-=(b);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator*=(const openip::Volume<T>& b)
{
    this->Vector<T>::operator*=(b);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator/=(const openip::Volume<T>& b)
{
    this->Vector<T>::operator/=(b);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator=(const T& t)
{
    this->Vector<T>::operator=(t);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator+=(const T& t)
{
    this->Vector<T>::operator+=(t);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator-=(const T& t)
{
    this->Vector<T>::operator-=(t);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator*=(const T& t)
{
    this->Vector<T>::operator*=(t);
    return *this;
}

template<class T>
openip::Volume<T>& openip::Volume<T>::operator/=(const T& t)
{
    this->Vector<T>::operator/=(t);
    return *this;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator+(const openip::Volume<T>& b) const
{
    openip::Volume<T> n(*this);
    return n+= b;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator-(const openip::Volume<T>& b) const
{
    openip::Volume<T> n(*this);
    return n-= b;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator*(const openip::Volume<T>& b) const
{
    openip::Volume<T> n(*this);
    return n*= b;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator/(const openip::Volume<T>& b) const
{
    openip::Volume<T> n(*this);
    return n/= b;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator+(const T& t) const
{
    openip::Volume<T> n(*this);
    return n+= t;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator-(const T& t) const
{
    openip::Volume<T> n(*this);
    return n-= t;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator*(const T& t) const
{
    openip::Volume<T> n(*this);
    return n*= t;
}

template<class T>
openip::Volume<T> openip::Volume<T>::operator/(const T& t) const
{
    openip::Volume<T> n(*this);
    return n/= t;
}

template<class T>
void openip::Volume<T>::fillBorder()
{
    if ( (borderMode & BORDER_MODE_ZERO) > 0)
    {
        for ( unsigned int i= 0; i < slices; ++i )
            for ( unsigned int j= 0; j < rows; ++j )
                for ( unsigned int k= 0; k < columns; ++k )
                    if ( int(i) < frontBorder || (i) >= slices - frontBorder )
                        (*this)(i,j,k)= 0;
                    else if ( int(j) < topBorder || (j) >= rows - topBorder )
                        (*this)(i,j,k)= 0;
                    else if ( int(k) < leftBorder || (k) >= columns - leftBorder )
                        (*this)(i,j,k)= 0;
                
        return;
    }
    else if ( (borderMode & BORDER_MODE_ONES) > 0)
    {
        for ( unsigned int i= 0; i < slices; ++i )
            for ( unsigned int j= 0; j < rows; ++j )
                for ( unsigned int k= 0; k < columns; ++k )
                    if ( int(i) < frontBorder || (i) >= slices - frontBorder )
                        (*this)(i,j,k)= 1;
                    else if ( int(j) < topBorder || (j) >= rows - topBorder )
                        (*this)(i,j,k)= 1;
                    else if ( int(k) < leftBorder || (k) >= columns - leftBorder )
                        (*this)(i,j,k)= 1;

        return;
    }
    else if ( (borderMode & BORDER_MODE_MIRRORED) > 0 )
    {
        int tmp1= 2 * this->topBorder;
        int tmp2= this->rows - 1;
        int tmp3= this->rows - 2 * this->bottomBorder;
        int tmp4= 2 * this->leftBorder;
        int tmp5= this->columns - 1;
        int tmp6= this->columns - 2 * this->rightBorder;
        int tmp7= 2 * this->frontBorder;
        int tmp8= this->slices - 1;
        int tmp9= this->slices - 2 * this->backBorder;

        for ( int k= 0; k < this->frontBorder; ++k )
            for ( unsigned int i= 0; i < this->columns; ++i )
                for ( unsigned int j= 0; j < this->rows; ++j )
                    (*this)(k, j, i)= (*this)(tmp7 - k, j, i);
        for ( int k= 0; k < this->backBorder; ++k )
            for ( unsigned int i= 0; i < this->columns; ++i )
                for ( unsigned int j= 0; j < this->rows; ++j )
                    (*this)(tmp8 - k, j, i)= (*this)(tmp9 + k, j, i);
		
        // TODO: probably wrong indexing is used below
        for ( int i= 0; i < this->topBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, i, j)= (*this)(k, tmp1 - i, j);
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, tmp2 - i, j)= (*this)(k, tmp3 + i, j);
        
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, i)= (*this)(k, j, tmp4 - i);
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, tmp5 - i)= (*this)(k, j, tmp6 + i);
        return;
    }
    else if ( (borderMode & BORDER_MODE_PERIODIC) > 0 )
    {
        int tmp1= this->rows - this->bottomBorder - this->topBorder;
        int tmp2= this->rows - 1;
        int tmp3= this->topBorder + this->bottomBorder;
        int tmp4= this->columns - this->rightBorder - this->leftBorder;
        int tmp5= this->columns - 1;
        int tmp6= this->leftBorder + this->rightBorder;
        int tmp7= this->slices - this->frontBorder - this->backBorder;
        int tmp8= this->slices - 1;
        int tmp9= this->frontBorder + this->backBorder;

        for ( int i= 0; i < this->topBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, i, j)= (*this)(k, tmp1 + i, j);
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, tmp2 - i, j)= (*this)(k, tmp3 - i, j);
            
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, i)= (*this)(k, j, tmp4 + i);
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, tmp5 - i)= (*this)(k, j, tmp6 - i);
            
        for ( int i= 0; i < this->frontBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->columns; ++k )
                    (*this)(i, j, k)= (*this)(tmp7 + i, j, k);
        for ( int i= 0; i < this->backBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->columns; ++k )
                    (*this)(tmp8 - i, j, k)= (*this)(tmp9 - i, j, k);
        return;
    }
    else if ( (borderMode & BORDER_MODE_LASTREPEATED) > 0 )
    {
        int tmp2= this->rows - 1;
        int tmp5= this->columns - 1;
        int tmp8= this->slices - 1;
      
	for ( int i= 0; i < this->topBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, i, j)= (*this)(k, topBorder, j);
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( unsigned int j= 0; j < this->columns; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, tmp2 - i, j)= (*this)(k, this->rows - 1 - bottomBorder, j);
            
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, i)= (*this)(k, j, leftBorder);
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->slices; ++k )
                    (*this)(k, j, tmp5 - i)= (*this)(k, j, this->columns - rightBorder - 1);
            
        for ( int i= 0; i < this->frontBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->columns; ++k )
                    (*this)(i, j, k)= (*this)(frontBorder, j, k);
        for ( int i= 0; i < this->backBorder; ++i )
            for ( unsigned int j= 0; j < this->rows; ++j )
                for ( unsigned int k= 0; k < this->columns; ++k )
                    (*this)(tmp8 - i, j, k)= (*this)(this->slices - backBorder - 1, j, k);
        return;
    }
}

template<class T>
void openip::Volume<T>::resizeVolume(unsigned int slices, unsigned int rows, unsigned int columns, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
{
    this->Vector<T>::resize((slices + frontBorder + backBorder)*(rows + topBorder + bottomBorder)*(columns + leftBorder + rightBorder));
    this->n= this->size();
    this->rows= rows + topBorder + bottomBorder;
    this->columns= columns + leftBorder + rightBorder;
    this->slices= slices + frontBorder + backBorder;
    this->sliceSize= (this->rows)*(this->columns);
    this->frontBorder= frontBorder;
    this->backBorder= backBorder;
    this->topBorder= topBorder;
    this->bottomBorder= bottomBorder;
    this->leftBorder= leftBorder;
    this->rightBorder= rightBorder;
    this->borderMode= borderMode;
}

template<class T>
void openip::Volume<T>::resizeVolume(unsigned int slices, unsigned int rows, unsigned int columns, Border3 b)
{
    resizeVolume(slices - b.frontBorder - b.backBorder, rows - b.topBorder - b.bottomBorder, columns - b.leftBorder - b.rightBorder, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<class T> template<typename S>
void openip::Volume<T>::resizeVolume(Volume<S>& b)
{
    resizeVolume(b.slices - b.frontBorder - b.backBorder, b.rows - b.topBorder - b.bottomBorder, b.columns - b.leftBorder - b.rightBorder, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<class T>
void openip::Volume<T>::resizeBorder(Border3 b)
{
    this->resizeBorder(b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<class T>
void openip::Volume<T>::removeBorder()
{
    this->resizeBorder(0, 0, 0, 0, 0, 0);
}

template<class T>
void openip::Volume<T>::resizeBorder(int fb, int backb, int tb, int bb, int lb, int rb, int bm)
{
    int offset= frontBorder * sliceSize + topBorder*columns + leftBorder;

    int n= 0;
    for ( unsigned int k= 0; k < this->slices - frontBorder - backBorder; ++k )
    {
        for ( unsigned int i= 0; i < this->rows - topBorder - bottomBorder; ++i )
        {
            for ( unsigned int j= 0; j < this->columns - leftBorder - rightBorder; ++j )
            {
                (*this)(n)= (*this)(n + offset);
                ++n;
            }
            offset+= leftBorder + rightBorder;
        }
        offset+= (topBorder + bottomBorder)*columns;
    }

    int oldr, oldc, olds;
    
    olds= this->slices - frontBorder - backBorder;
    oldr= this->rows - topBorder - bottomBorder;
    oldc= this->columns - leftBorder - rightBorder;
    
    this->resizeVolume((fb + this->slices + backb - frontBorder - backBorder), (tb + this->rows + bb - topBorder - bottomBorder),(lb + this->columns + rb - leftBorder - rightBorder));

    n= olds * oldr * oldc - 1;
    offset= fb*(oldc + lb + rb)*(oldr + tb + bb);
    offset+= (olds-1)*((oldc + lb + rb) * (oldr + tb + bb) - oldc * oldr);
    offset+= (oldc + lb + rb) * tb + (oldr-1)*(lb + rb) + lb;
    
    for ( int k= olds - 1; k >= 0; --k )
    {
        for ( int i= oldr - 1; i >= 0; --i )
        {
            for ( int j= oldc - 1; j >= 0; --j )
            {
                (*this)(n + offset)= (*this)(n);
                --n;
            }
            offset-= lb + rb;
        }
        offset -= (tb + bb)*(oldc + rb + lb);
    }

    topBorder= tb;
    bottomBorder= bb;
    leftBorder= lb;
    rightBorder= rb;
    frontBorder= fb;
    backBorder= backb;
    this->n= (this->rows) * (this->columns) * (this->slices);
    this->sliceSize= (this->rows) * (this->columns);
    this->borderMode= bm;
    this->fillBorder();
}


template<class T>
void openip::Volume<T>::getCoordinate1D(int s, int r, int c, int& n)
{
    n= s * this->rows * this->columns + r * this->columns + c;
}

template<class T>
void openip::Volume<T>::getCoordinate3D(int n, int& s, int& r, int& c)
{
    int tmp;
    s= n / sliceSize;
    tmp= n % sliceSize;
    c= tmp % this->columns;
    r= tmp / this->columns;
}

template<class T>
T* openip::Volume<T>::data()
{
    return &(this->Vector<T>::front());
}

template<class T>
bool openip::Volume<T>::isRealVolumeVoxel(int i)
{
    int s, r, c;
    this->getCoordinate3D(i, s, r, c);
    return this->isRealVolumeVoxel(s, r, c);
}

template<class T>
bool openip::Volume<T>::isRealVolumeVoxel(int s, int r, int c)
{
    if ( s >= frontBorder && s < int(slices) - backBorder )
        if ( r >= topBorder && r < int(rows) - bottomBorder )
            if ( c >= leftBorder && c < int(columns) - rightBorder )
                return true;
    return false;
}

template<class T> template<typename S>
void openip::Volume<T>::getImage(Image<S>& image, int dim, int slice)
{
    if ( dim == 0 )
    {
        image.resizeImage(this->rows, this->columns, image.getBorder2());
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                image(i,j)= (*this)(slice, i-image.topBorder, j-image.leftBorder);
    }

    if ( dim == 1 )
    {
        image.resizeImage(this->slices, this->columns, image.getBorder2());
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                image(i,j)= (*this)(i - image.topBorder, slice, j-image.leftBorder);
    }

    if ( dim == 2 )
    {
        image.resizeImage(this->slices, this->rows, image.getBorder2());
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                image(i,j)= (*this)(i - image.topBorder, j - image.leftBorder, slice);
    }

    image.fillBorder();
}

template<class T> template<typename S>
void openip::Volume<T>::setImage(Image<S>& image, int dim, int slice)
{
    if ( dim == 0 )
    {
        //image.resizeImage(this->rows, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(slice, i - image.topBorder, j - image.leftBorder)= image(i,j);
    }

    if ( dim == 1 )
    {
        //image.resizeImage(this->slices, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, slice, j - image.leftBorder)= image(i,j);
    }

    if ( dim == 2 )
    {
        //image.resizeImage(this->slices, this->rows);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, j - image.leftBorder, slice)= image(i,j);
    }
}

template<class T> template<typename S>
void openip::Volume<T>::addImage(Image<S>& image, int dim, int slice)
{
    if ( dim == 0 )
    {
        //image.resizeImage(this->rows, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(slice, i - image.topBorder, j - image.leftBorder)+= image(i,j);
    }
    if ( dim == 1 )
    {
        //image.resizeImage(this->slices, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, slice, j - image.leftBorder)+= image(i,j);
    }

    if ( dim == 2 )
    {
        //image.resizeImage(this->slices, this->rows);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, j - image.leftBorder, slice)+= image(i,j);
    }
}

template<class T> template<typename S>
void openip::Volume<T>::maxImage(Image<S>& image, int dim, int slice)
{
    if ( dim == 0 )
    {
        //image.resizeImage(this->rows, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(slice, i - image.topBorder, j - image.leftBorder)= image(i,j) > (*this)(slice, i - image.topBorder, j - image.leftBorder) ? image(i,j) : (*this)(slice, i - image.topBorder, j - image.leftBorder);
    }
    if ( dim == 1 )
    {
        //image.resizeImage(this->slices, this->columns);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, slice, j - image.leftBorder)= image(i,j) > (*this)(i - image.topBorder, slice, j - image.leftBorder) ? image(i,j) : (*this)(i - image.topBorder, slice, j - image.leftBorder);
    }

    if ( dim == 2 )
    {
        //image.resizeImage(this->slices, this->rows);
        for ( int i= image.topBorder; i < image.rows - image.bottomBorder; ++i )
            for ( int j= image.leftBorder; j < image.columns - image.rightBorder; ++j )
                (*this)(i - image.topBorder, j - image.leftBorder, slice)= image(i,j) > (*this)(i - image.topBorder, j - image.leftBorder, slice) ? image(i,j) : (*this)(i - image.topBorder, j - image.leftBorder, slice);
    }
}

template<class T>
Border3 openip::Volume<T>::getBorder()
{
    return Border3(topBorder, bottomBorder, leftBorder, rightBorder, frontBorder, backBorder, borderMode);
}

template<class T>
void openip::Volume<T>::clearSlice(int i)
{
  for ( unsigned int j= 0; j < rows; ++j )
    for ( unsigned int k= 0; k < columns; ++k )
      (*this)(i,j,k)= 0;
}

template<class T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Volume<T>& v)
{
    o << "[Volume - size: ";
    o << v.slices;
    o << " x ";
    o << v.rows;
    o << " x ";
    o << v.columns;
    o << "]";

    return o;
}

template<typename T>
std::ofstream& openip::operator<<(std::ofstream& file, const openip::Volume<T>& v)
{
    file << v.slices << " " << v.rows << " " << v.columns << std::endl;
    for ( unsigned int i= 0; i < v.slices; ++i )
    {
        for ( unsigned int j= 0; j < v.rows; ++j )
        {
            for ( unsigned int k= 0; k < v.columns; ++k )
            {
                file << v(i,j,k) << " ";
            }
        }
    }

    return file;
}

template<typename T>
std::ifstream& openip::operator>>(std::ifstream& file, openip::Volume<T>& v)
{
    int slices, rows, columns;

    file >> slices;
    file >> rows;
    file >> columns;

    v.resizeVolume(slices, rows, columns);

    for ( unsigned int i= 0; i < v.slices*v.rows*v.columns; ++i )
    {
        file >> v(i);
    }

    return file;
}


#endif	/* _VOLUME_H */

