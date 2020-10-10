/**
 * @file Image.h
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
 * The Image class represents a template image type and has
 * several useful operators and member functions defined.
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <limits>

#include <openipDS/Matrix.h>
#include <openipDS/Pixel.h>
#include <openipDS/constants.h>

#include <algorithm>

namespace openip
{
    /**
     * Border2 represents the border extension of an image
     */
    class Border2
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
        Border2(int tb= 0, int bb= 0, int lb= 0, int rb= 0, int bm= BORDER_MODE_MIRRORED);

        Border2(Border2 a, Border2 b);

        /**
         * copy constructor
         * @param b instance to copy
         */
        Border2(const Border2& b);

        /**
         * destructor
         */
        ~Border2();

        /**
         * assignment operator
         * @param b the values of fields of b is assigned to the fields of *this object
         * @return reference to *this object
         */
        Border2& operator=(const Border2& b);

        int getMin(int stride);

        int getMax(int stride);

        void dump();

        /** top border */
        int topBorder;
        /** bottom border */
        int bottomBorder;
        /** left border */
        int leftBorder;
        /** right border */
        int rightBorder;
        /** border mode */
        int borderMode;
    };

    


    /**
     * returns the maximum of the Border2 parameters in each direction
     * @param a Border2 object
     * @param b Border2 object
     * @return the new maximized Border2 object
     */
    Border2 maximizeBorders(Border2& a, Border2& b);

    template<typename T>
    class ImageFactory;

    /**
     * Image represents an image with size template type T
     */
    template<typename T>
    class Image : public Matrix<T>
    {
    public:
        using Matrix<T>::operator ();
        using Matrix<T>::operator !=;
        using Matrix<T>::operator ==;

        /**
         * Image<T>::iterator typedef
         */
        typedef typename Image<T>::iterator iIt;

        /**
         * Image<T>::const_iterator typedef
         */
        typedef typename Image<T>::const_iterator const_iIt;

        /**
         * default constructor
         */
        Image();

        /**
         * copy constructor
         * @param b object to copy
         */
        Image(const Image& b);

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param topBorder number of top border rows
         * @param bottomBorder number of bottom border rows
         * @param leftBorder number of left border columns
         * @param rightBorder number of right border columns
         * @param borderMode mode of filling border pixels
         */
        Image(unsigned int rows, unsigned int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param b border descriptor object
         */
        Image(unsigned int rows, unsigned int columns, Border2 b);

        /**
         * desctructor
         */
        ~Image();

        /**
         * accessing the element of the matrix (stored row-continuosly) as rvalue
         * @param x Pixel2 type coordinates of element to access
         * @return the (x.x() x.y())th element
         */
        inline T operator()(Pixel2 x) const;

        /**
         * accessing the element of the matrix (stored row-continuosly) as lvalue
         * @param x Pixel2 type coordinates of element to access
         * @return the (x.x() x.y())th element
         */
        inline T& operator()(Pixel2 x);

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Image& operator=(const Image& b);

        /**
         * assignment operator for different types of images
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Image<T>& operator=(const Image<S>& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Image& operator+=(const Image& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Image& operator-=(const Image& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Image& operator*=(const Image& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Image& operator/=(const Image& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Image& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Image& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Image& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Image& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Image& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Image operator+(const Image& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Image operator-(const Image& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Image operator*(const Image& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Image operator/(const Image& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Image operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Image operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Image operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Image operator/(const T& t) const;

        /**
         * fills the border pixels of the image
         */
        void fillBorder();

        /**
         * resizes the image to the specified dimensions
         * @param rows number of rows
         * @param columns number of columns
         * @param topBorder top border
         * @param bottomBorder bottom border
         * @param leftBorder left border
         * @param rightBorder right border
         * @param borderMode border mode
         */
        void resizeImage(unsigned int rows, unsigned int columns, unsigned int topBorder= 0, unsigned int bottomBorder= 0, unsigned int leftBorder= 0, unsigned int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * resizes the image to the specified dimensions
         * @param rows number of rows
         * @param columns number of columns
         * @param b border specification
         */
        void resizeImage(unsigned int rows, unsigned int columns, Border2 b);

        /**
         * resizes the image border
         * @param topBorder top border
         * @param bottomBorder bottom border
         * @param leftBorder left border
         * @param rightBorder right border
         * @param borderMode border mode
         */
        void resizeBorder(unsigned int topBorder= 0, unsigned int bottomBorder= 0, unsigned int leftBorder= 0, unsigned int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * resizes the image border
         * @param b border specification
         */
        void resizeBorder(Border2 b);

        /**
         * resizes the image border
         * @param b border specification
         */
        void setBorder(Border2 b);

        /**
         * resizes the image to size of the b image object
         * @param b the image to which to resize
         */
        template<typename S>
        void resizeImage(const Image<S>& b);

        /**
         * get row continuous coordinate of x and y
         * @param r input parameter row index
         * @param c input parameter column index
         * @param n output parameter row-continuous index
         */
        void getCoordinate1D(int r, int c, int& n);

        /**
         * get x and y index from row-continuous index
         * @param n input row-continuous index
         * @param r output row index
         * @param c output column index
         */
        void getCoordinate2D(int n, int& r, int& c);

        /**
         * returns the data pointer of the image container object
         * @return data pointer
         */
        T* data();

        /**
         * computes correlation with the parameter image
         * @param i parameter to compute correlation with
         * @return the correlation value
         */
        template<typename S>
        double correlation(Image<S>& i, Image<unsigned char>* mask= NULL);

        /**
         * computes Euclidean distance from the parameter image
         * @param i parameter to compute Euclidean distance from
         * @return the Euclidean distance
         */
        float euclideanDistance(Image<T>& i);

        /**
         * computes symmetric difference with the parameter image
         * @param i parameter to compute symmetric difference with
         * @return the symmetric difference
         */
        float symmetricDifference(Image<T>& i);

        /**
         * computes the contrast (standard deviation) of the intensity image
         * @return the contrast value
         */
        float contrast();

        /**
         * computes the total variation of the image
         * @return the total variation
         */
        float totalVariation();

        /**
         * returns the first coordinate of the original image content
         * @return the first coordinate of the original image content
         */
        int getFirstCoordinate();

        /**
         * returns the last coordinate of the original image content
         * @return the last coordinate of the original image content
         */
        int getLastCoordinate();

        /**
         * removes the image border
         */
        void removeBorder();
        
        bool isRealImagePixel(int i);
        bool isRealImagePixel(int r, int c);
	
	int isLocalMinimum8(int p);

        int getRealColumns();

        int getRealRows();

        void setBorderTo(T value);

        void setContentTo(T value);

        T firstNonZero();

        Border2 getBorder2();
	
        /**
         * number of elements in the image: w*h
         */
        unsigned int n;

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
         * mode of the border filling
         */
        int borderMode;

        std::string filename;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param image Image object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Image<T>& image);

    template<typename T>
    void fitToFourier(Border2& b, Image<T>& image)
    {
        int t[]= {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
        vector<float> v;
        for ( int i= 0; i < 13; ++i )
        {
            v.push_back(t[i]);
            v.push_back(t[i]*3);
            //v.push_back(t[i]*3*3);
            v.push_back(t[i]*5);
            //v.push_back(t[i]*5*5);
            //v.push_back(t[i]*3*5);
        }
        sort(v.begin(), v.end());

        int max= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
            if ( max < b.topBorder + b.bottomBorder + image.rows )
                max= v[i];
            else
                break;

        b.topBorder= (max - image.rows)/2;
        b.bottomBorder= max - image.rows - (max - image.rows)/2;

        max= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
            if ( max < b.leftBorder + b.rightBorder + image.columns )
                max= v[i];
            else
                break;

        b.leftBorder= (max - image.columns)/2;
        b.rightBorder= max - image.columns - (max - image.columns)/2;
    }
}

template<typename T>
openip::Image<T>::Image()
: Matrix<T>(0,0), n(0), topBorder(0), bottomBorder(0), leftBorder(0), rightBorder(0), borderMode(BORDER_MODE_MIRRORED)
{
}

template<typename T>
openip::Image<T>::Image(unsigned int w_, unsigned int h_, int topBorder_, int bottomBorder_, int leftBorder_, int rightBorder_, int borderMode_)
: Matrix<T>(w_,h_), n(w_*h_), topBorder(topBorder_), bottomBorder(bottomBorder_), leftBorder(leftBorder_), rightBorder(rightBorder_), borderMode(borderMode_)
{
}

template<typename T>
openip::Image<T>::Image(unsigned int r, unsigned int c, Border2 b)
: Matrix<T>(r, c), n(r*c), topBorder(b.topBorder), bottomBorder(b.bottomBorder), leftBorder(b.leftBorder), rightBorder(b.rightBorder), borderMode(b.borderMode)
{
}

template<typename T>
openip::Image<T>::Image(const Image& b)
    : Matrix<T>(b), n(b.n), topBorder(b.topBorder), bottomBorder(b.bottomBorder), leftBorder(b.leftBorder), rightBorder(b.rightBorder), borderMode(b.borderMode), filename(b.filename)
{
}

template<typename T>
openip::Image<T>::~Image()
{
}

template<typename T>
inline T openip::Image<T>::operator()(Pixel2 p) const
{
    return this->operator()(p.r()*(this->Matrix<T>::columns) + p.c());
}

template<typename T>
inline T& openip::Image<T>::operator()(Pixel2 p)
{
    return this->operator()(p.r()*(this->Matrix<T>::columns) + p.c());
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator=(const openip::Image<T>& b)
{
    this->Matrix<T>::operator=(b);
    this->n= b.n;
    this->topBorder= b.topBorder;
    this->bottomBorder= b.bottomBorder;
    this->leftBorder= b.leftBorder;
    this->rightBorder= b.rightBorder;
    this->borderMode= b.borderMode;
    this->filename= b.filename;
    return *this;
}

template<typename T> template<typename S>
openip::Image<T>& openip::Image<T>::operator=(const Image<S>& b)
{
    this->Matrix<T>::operator=(b);
    this->n= b.n;
    this->topBorder= b.topBorder;
    this->bottomBorder= b.bottomBorder;
    this->leftBorder= b.leftBorder;
    this->rightBorder= b.rightBorder;
    this->borderMode= b.borderMode;
    this->filename= b.filename;
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator+=(const openip::Image<T>& b)
{
    this->Matrix<T>::operator+=(b);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator-=(const openip::Image<T>& b)
{
    this->Matrix<T>::operator-=(b);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator*=(const openip::Image<T>& b)
{
    this->Matrix<T>::operator*=(b);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator/=(const openip::Image<T>& b)
{
    this->Matrix<T>::operator/=(b);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator=(const T& t)
{
    this->Matrix<T>::operator=(t);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator+=(const T& t)
{
    this->Matrix<T>::operator+=(t);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator-=(const T& t)
{
    this->Matrix<T>::operator-=(t);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator*=(const T& t)
{
    this->Matrix<T>::operator*=(t);
    return *this;
}

template<typename T>
openip::Image<T>& openip::Image<T>::operator/=(const T& t)
{
    this->Matrix<T>::operator/=(t);
    return *this;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator+(const openip::Image<T>& b) const
{
    openip::Image<T> n(*this);
    return n+= b;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator-(const openip::Image<T>& b) const
{
    openip::Image<T> n(*this);
    return n-= b;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator*(const openip::Image<T>& b) const
{
    openip::Image<T> n(*this);
    return n*= b;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator/(const openip::Image<T>& b) const
{
    openip::Image<T> n(*this);
    return n/= b;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator+(const T& t) const
{
    openip::Image<T> n(*this);
    return n+= t;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator-(const T& t) const
{
    openip::Image<T> n(*this);
    return n-= t;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator*(const T& t) const
{
    openip::Image<T> n(*this);
    return n*= t;
}

template<typename T>
openip::Image<T> openip::Image<T>::operator/(const T& t) const
{
    openip::Image<T> n(*this);
    return n/= t;
}

template<typename T>
openip::Border2 openip::Image<T>::getBorder2()
{
    return Border2(topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
}

template<typename T>
void openip::Image<T>::fillBorder()
{
    if ( (borderMode & BORDER_MODE_ZERO) > 0)
    {
        int tmp1= this->rows - 1;
        int tmp2= this->columns - 1;

        for ( int i= 0; i < this->topBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(i,j)= 0;
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(tmp1 - i, j)= 0;
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j,i)= 0;
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j, tmp2 - i)= 0;
        return;
    }
    else if ( (borderMode & BORDER_MODE_255) > 0 )
    {
        int tmp1= this->rows - 1;
        int tmp2= this->columns - 1;

        for ( int i= 0; i < this->topBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(i,j)= 255;
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(tmp1 - i, j)= 255;
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j,i)= 255;
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j, tmp2 - i)= 255;
        return;
    }
    else if ( (borderMode & BORDER_MODE_MIRRORED) > 0 )
    {
        int tmp1= 2 * this->topBorder - 1;
        int tmp2= this->rows - 1;
        int tmp3= this->rows - 2 * this->bottomBorder;
        int tmp4= 2 * this->leftBorder - 1;
        int tmp5= this->columns - 1;
        int tmp6= this->columns - 2 * this->rightBorder;

        //printf("aa"); fflush(stdout);
        for ( int i= 0; i < this->topBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(i,j)= (*this)(tmp1 - i, j);
        //printf("bb"); fflush(stdout);
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(tmp2 - i, j)= (*this)(tmp3 + i, j);
        //printf("cc"); fflush(stdout);
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j,i)= (*this)(j, tmp4 - i);
        //printf("dd"); fflush(stdout);
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j, tmp5 - i)= (*this)(j, tmp6 + i);
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

        for ( int i= 0; i < this->topBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(i,j)= (*this)(tmp1 + i, j);
        for ( int i= 0; i < this->bottomBorder; ++i )
            for ( int j= 0; j < this->columns; ++j )
                (*this)(tmp2 - i,j)= (*this)(tmp3 - i, j);
        for ( int i= 0; i < this->leftBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j,i)= (*this)(j, tmp4 + i);
        for ( int i= 0; i < this->rightBorder; ++i )
            for ( int j= 0; j < this->rows; ++j )
                (*this)(j, tmp5 - i)= (*this)(j, tmp6 - i);
        return;
    }
}

template<typename T>
bool openip::Image<T>::isRealImagePixel(int i)
{
    int r, c;
    this->getCoordinate2D(i, r, c);
    return isRealImagePixel(r, c);
}

template<typename T>
bool openip::Image<T>::isRealImagePixel(int r, int c)
{
    return r >= topBorder && r < this->rows - bottomBorder && c >= leftBorder && c < this->columns - rightBorder;
}

template<typename T>
int openip::Image<T>::isLocalMinimum8(int p)
{
  int flag= 1;
  flag &= ((*this)(p) <= (*this)(p + 1));
  flag &= ((*this)(p) <= (*this)(p - 1));
  flag &= ((*this)(p) <= (*this)(p + this->columns));
  flag &= ((*this)(p) <= (*this)(p - this->columns));
  flag &= ((*this)(p) <= (*this)(p + 1 + this->columns));
  flag &= ((*this)(p) <= (*this)(p + 1 - this->columns));
  flag &= ((*this)(p) <= (*this)(p - 1 + this->columns));
  flag &= ((*this)(p) <= (*this)(p - 1 - this->columns));
  
  return flag;
}

template<typename T>
T openip::Image<T>::firstNonZero()
{
    for ( unsigned int i= 0; i < this->n; ++i )
        if ( (*this)(i) > 0 )
            return (*this)(i);
    return -1;
}

template<typename T>
void openip::Image<T>::resizeImage(unsigned int rows, unsigned int columns, unsigned int topBorder, unsigned int bottomBorder, unsigned int leftBorder, unsigned int rightBorder, int borderMode)
{
    this->Matrix<T>::resize(topBorder + rows + bottomBorder, leftBorder + columns + rightBorder);
    this->n= this->size();
    this->topBorder= topBorder;
    this->bottomBorder= bottomBorder;
    this->leftBorder= leftBorder;
    this->rightBorder= rightBorder;
    this->borderMode= borderMode;
    //this->fillBorder();
}

template<typename T>
void openip::Image<T>::resizeImage(unsigned int rows, unsigned int columns, openip::Border2 b)
{
    this->resizeImage(rows, columns, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<typename T> template<typename S>
void openip::Image<T>::resizeImage(const Image<S>& b)
{
    this->resizeImage(b.rows - b.topBorder - b.bottomBorder, b.columns - b.leftBorder - b.rightBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<typename T>
void openip::Image<T>::resizeBorder(openip::Border2 b)
{
    this->resizeBorder(b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
}

template<typename T>
void openip::Image<T>::setBorder(openip::Border2 b)
{
    this->resizeBorder(b);
}

template<typename T>
void openip::Image<T>::resizeBorder(unsigned int tb, unsigned int bb, unsigned int lb, unsigned int rb, int bm)
{
    int offset= topBorder*(this->columns) + leftBorder;

    int n= 0;
    for ( int i= 0; i < this->rows - topBorder - bottomBorder; ++i )
    {
        for ( int j= 0; j < this->columns - leftBorder - rightBorder; ++j )
        {
            (*this)(n)= (*this)(n + offset);
            ++n;
        }
        offset+= leftBorder + rightBorder;
    }

    int oldr, oldc;
    oldr= this->rows - topBorder - bottomBorder;
    oldc= this->columns - leftBorder - rightBorder;
    this->Matrix<T>::resize((tb + this->rows + bb - topBorder - bottomBorder),(lb + this->columns + rb - leftBorder - rightBorder));

    n= oldr * oldc-1;
    offset= (oldc + lb + rb) * tb + (oldr-1)*(lb + rb) + lb;
    for ( int i= oldr - 1; i >= 0; --i )
    {
        for ( int j= oldc - 1; j >= 0; --j )
        {
            (*this)(n + offset)= (*this)(n);
            --n;
        }
        offset-= lb + rb;
    }

    topBorder= tb;
    bottomBorder= bb;
    leftBorder= lb;
    rightBorder= rb;
    this->n= (this->rows) * (this->columns);
    this->borderMode= bm;
    this->fillBorder();
}

template<typename T>
void openip::Image<T>::getCoordinate1D(int r, int c, int& n)
{
	n= r * this->columns + c;
}

template<typename T>
void openip::Image<T>::getCoordinate2D(int n, int& r, int& c)
{
	c= n % this->columns;
    r= n / this->columns;
}

template<typename T>
T* openip::Image<T>::data()
{
        return &(this->Vector<T>::front());
}

template<typename T> template<typename S>
double openip::Image<T>::correlation(Image<S>& img, Image<unsigned char>* mask)
{
    double mean1= 0;
    double var1= 0;
    double mean2= 0;
    double var2= 0;

    float corr= 0;

    if ( mask == NULL )
    {
        for ( unsigned int i= 0; i < this->n; ++i )
        {
            mean1+= (*this)(i);
            mean2+= img(i);
        }
        mean1/= this->n;
        mean2/= img.n;

        for ( unsigned int i= 0; i < this->n; ++i )
        {
            var1+= (mean1 - (*this)(i)) * (mean1 - (*this)(i));
            var2+= (mean2 - img(i)) * (mean2 - img(i));
        }
        var1/= this->n;
        var2/= img.n;

        for ( unsigned int i= 0; i < this->n; ++i )
            corr+= ((*this)(i) - mean1)*(img(i) - mean2);
        corr/=(this->n)*sqrt(var1)*sqrt(var2);
    }
    else
    {
        int n= 0;

        for ( unsigned int i= 0; i < this->n; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                mean1+= (*this)(i);
                mean2+= img(i);
                ++n;
            }
        }
        if ( n > 0 )
        {
            mean1/= n;
            mean2/= n;
        }
        else
        {
            mean1= 0;
            mean2= 0;
        }

        for ( unsigned int i= 0; i < this->n; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                var1+= (mean1 - (*this)(i)) * (mean1 - (*this)(i));
                var2+= (mean2 - img(i)) * (mean2 - img(i));
            }
        }

        if ( n > 0 )
        {
            var1/= n;
            var2/= n;
        }
        else
        {
            var1= 0;
            var2= 0;
        }

        for ( unsigned int i= 0; i < this->n; ++i )
            if ( (*mask)(i) > 0 )
                corr+= ((*this)(i) - mean1)*(img(i) - mean2);
        if ( var1 > 0 && var2 > 0 && n > 0 )
            corr/=n*sqrt(var1)*sqrt(var2);
        else
            corr= 0.0;
    }

//    printf("%f\n", corr);

    //if ( isnan(corr) )
//        printf("%d %f %f %f %f\n", n, mean1, mean2, var1, var2);

    return corr;
}

template<typename T>
float openip::Image<T>::contrast()
{
    return 0;
}

template<typename T>
float openip::Image<T>::totalVariation()
{
    float sum= 0;
    for ( unsigned int i= 0; i < this->n - this->columns - 1; ++i )
        sum+= fabs((*this)(i) - (*this)(i+1)) + fabs((*this)(i) - (*this)(i+this->columns));
    return sum;
}

template<typename T>
float openip::Image<T>::euclideanDistance(Image<T>& img)
{
    float dist= 0;
    for ( unsigned int i= 0; i < this->n; ++i )
        dist+= ((*this)(i) - img(i)) * ((*this)(i) - img(i));

    return sqrt(dist);
}

template<typename T>
float openip::Image<T>::symmetricDifference(Image<T>& img)
{
    int diff= 0;
    for ( unsigned int i= 0; i < this->n; ++i )
        if ( (*this)(i) != img(i) )
            diff+= 1;
    
    return (float)diff;
}

template<typename T>
int openip::Image<T>::getFirstCoordinate()
{
    return topBorder * (leftBorder + this->columns + rightBorder) + leftBorder;
}

template<typename T>
int openip::Image<T>::getLastCoordinate()
{
    return (topBorder + this->rows) * (leftBorder + this->columns + rightBorder) - rightBorder;
}

template<typename T>
int openip::Image<T>::getRealColumns()
{
    return this->columns - leftBorder - rightBorder;
}

template<typename T>
int openip::Image<T>::getRealRows()
{
    return this->rows - leftBorder - rightBorder;
}

template<typename T>
void openip::Image<T>::removeBorder()
{
    this->resizeBorder(0, 0, 0, 0);
}

template<typename T>
void openip::Image<T>::setBorderTo(T value)
{
    for ( int i= 0; i < leftBorder; ++i )
        for ( int j= 0; j < this->rows; ++j )
            (*this)(j,i)= value;
    for ( int i= this->columns - rightBorder; i < this->columns; ++i )
        for ( int j= 0; j < this->rows; ++j )
            (*this)(j,i)= value;
    for ( int i= leftBorder; i < this->columns - rightBorder; ++i )
        for ( int j= 0; j < topBorder; ++j )
            (*this)(j,i)= value;
    for ( int i= leftBorder; i < this->columns - rightBorder; ++i )
        for ( int j= this->rows - bottomBorder; j < this->rows; ++j )
            (*this)(j,i)= value;
}

template<typename T>
void openip::Image<T>::setContentTo(T value)
{
    for ( int i= leftBorder; i < this->columns - rightBorder; ++i )
        for ( int j= topBorder; j < this->rows - bottomBorder; ++j )
            (*this)(j,i)= value;
}

template<typename T>
std::ostream& openip::operator<<(std::ostream& o, const openip::Image<T>& image)
{
    o << "[Image - size: ";
    o << image.rows;
    o << " x ";
    o << image.columns;
    o << "]";

    return o;
}




#endif
