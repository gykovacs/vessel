/* 
 * File:   FramedImage.h
 * Author: gykovacs
 *
 * Created on July 18, 2011, 12:40 PM
 */

#ifndef FRAMEDIMAGE_H
#define	FRAMEDIMAGE_H

#include <openipDS/Frame2.h>
#include <openipDS/Frame3.h>
#include <openipDS/Image.h>

namespace openip
{
    template<typename T>
    class Frame2Image: public Image<T>, public Frame2
    {
    public:
        using Image<T>::operator!=;
        using Image<T>::operator==;
        using Image<T>::removeBorder;
        using Image<T>::getLastCoordinate;
        using Image<T>::getFirstCoordinate;
        using Image<T>::totalVariation;
        using Image<T>::contrast;
        using Image<T>::symmetricDifference;
        using Image<T>::euclideanDistance;
        using Image<T>::correlation;
        using Image<T>::data;
        using Image<T>::getCoordinate2D;
        using Image<T>::getCoordinate1D;
        using Image<T>::resizeImage;
        using Image<T>::resizeBorder;
        using Image<T>::fillBorder;
        using Image<T>::operator();
        
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
        Frame2Image();

        /**
         * copy constructor
         * @param b object to copy
         */
        Frame2Image(const Frame2Image& b);

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
        Frame2Image(unsigned int rows, unsigned int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param b border descriptor object
         */
        Frame2Image(unsigned int rows, unsigned int columns, Border2 b);

        /**
         * desctructor
         */
        ~Frame2Image();

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Frame2Image<T>& operator=(const Image<T>& b);

        /**
         * assignment operator for different types of images
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Frame2Image<T>& operator=(const Image<S>& b);
        
        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Frame2Image<T>& operator=(const Frame2Image<T>& b);

        /**
         * assignment operator for different types of images
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Frame2Image<T>& operator=(const Frame2Image<S>& b);


        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Frame2Image<T>& operator+=(const Image<T>& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Frame2Image<T>& operator-=(const Image<T>& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Frame2Image<T>& operator*=(const Image<T>& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Frame2Image<T>& operator/=(const Image<T>& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Frame2Image<T>& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Frame2Image<T>& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Frame2Image<T>& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Frame2Image<T>& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Frame2Image<T>& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame2Image<T> operator+(const Image<T>& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame2Image<T> operator-(const Image<T>& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame2Image<T> operator*(const Image<T>& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame2Image<T> operator/(const Image<T>& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame2Image<T> operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame2Image<T> operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame2Image<T> operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame2Image<T> operator/(const T& t) const;
    };
    
    template<typename T>
    class Frame3Image: public Image<T>, public Frame3
    {
    public:
        using Image<T>::removeBorder;
        using Image<T>::getLastCoordinate;
        using Image<T>::getFirstCoordinate;
        using Image<T>::totalVariation;
        using Image<T>::contrast;
        using Image<T>::symmetricDifference;
        using Image<T>::euclideanDistance;
        using Image<T>::correlation;
        using Image<T>::data;
        using Image<T>::getCoordinate2D;
        using Image<T>::getCoordinate1D;
        using Image<T>::resizeImage;
        using Image<T>::resizeBorder;
        using Image<T>::fillBorder;
        using Image<T>::operator();
        
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
        Frame3Image();

        /**
         * copy constructor
         * @param b object to copy
         */
        Frame3Image(const Frame3Image& b);

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
        Frame3Image(unsigned int rows, unsigned int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param b border descriptor object
         */
        Frame3Image(unsigned int rows, unsigned int columns, Border2 b);

        /**
         * desctructor
         */
        ~Frame3Image();

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Frame3Image<T>& operator=(const Image<T>& b);

        /**
         * assignment operator for different types of images
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Frame3Image<T>& operator=(const Image<S>& b);
        
                /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        Frame3Image<T>& operator=(const Frame3Image<T>& b);

        /**
         * assignment operator for different types of images
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        Frame3Image<T>& operator=(const Frame3Image<S>& b);


        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Frame3Image<T>& operator+=(const Image<T>& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Frame3Image<T>& operator-=(const Image<T>& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Frame3Image<T>& operator*=(const Image<T>& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Frame3Image<T>& operator/=(const Image<T>& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Frame3Image<T>& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Frame3Image<T>& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Frame3Image<T>& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Frame3Image<T>& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Frame3Image<T>& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame3Image<T> operator+(const Image<T>& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame3Image<T> operator-(const Image<T>& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame3Image<T> operator*(const Image<T>& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Frame3Image<T> operator/(const Image<T>& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame3Image<T> operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame3Image<T> operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame3Image<T> operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Frame3Image<T> operator/(const T& t) const;
    };
    
    template<typename T>
    Frame2Image<T>::Frame2Image()
    : Image<T>(), Frame2()
    {
    }

    template<typename T>
    Frame2Image<T>::Frame2Image(const Frame2Image& b)
    : Image<T>(b), Frame2(b)
    {
    }

    template<typename T>
    Frame2Image<T>::Frame2Image(unsigned int rows, unsigned int columns, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    : Image<T>(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode), Frame2()
    {
    }

    template<typename T>
    Frame2Image<T>::Frame2Image(unsigned int rows, unsigned int columns, Border2 b)
    : Image<T>(rows, columns, b), Frame2()
    {
    }

    template<typename T>
    Frame2Image<T>::~Frame2Image()
    {
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator=(const Image<T>& b)
    {
        this->Image<T>::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    Frame2Image<T>& Frame2Image<T>::operator=(const Image<S>& b)
    {
        this->Image<T>::operator=(b);
        return *this;
    }
    
    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator=(const Frame2Image<T>& b)
    {
        this->Image<T>::operator=(b);
        this->Frame2::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    Frame2Image<T>& Frame2Image<T>::operator=(const Frame2Image<S>& b)
    {
        this->Image<T>::operator=(b);
        this->Frame2::operator=(b);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator+=(const Image<T>& b)
    {
        this->Image<T>::operator+=(b);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator-=(const Image<T>& b)
    {
        this->Image<T>::operator-=(b);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator*=(const Image<T>& b)
    {
        this->Image<T>::operator*=(b);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator/=(const Image<T>& b)
    {
        this->Image<T>::operator/=(b);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator=(const T& t)
    {
        this->Image<T>::operator=(t);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator+=(const T& t)
    {
        this->Image<T>::operator+=(t);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator-=(const T& t)
    {
        this->Image<T>::operator-=(t);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator*=(const T& t)
    {
        this->Image<T>::operator*=(t);
        return *this;
    }

    template<typename T>
    Frame2Image<T>& Frame2Image<T>::operator/=(const T& t)
    {
        this->Image<T>::operator/=(t);
        return *this;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator+(const Image<T>& b) const
    {
        Frame2Image<T> tmp(*this);
        tmp+= b;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator-(const Image<T>& b) const
    {
        Frame2Image<T> tmp(*this);
        tmp-= b;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator*(const Image<T>& b) const
    {
        Frame2Image<T> tmp(*this);
        tmp*= b;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator/(const Image<T>& b) const
    {
        Frame2Image<T> tmp(*this);
        tmp/= b;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator+(const T& t) const
    {
        Frame2Image<T> tmp(*this);
        tmp+= t;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator-(const T& t) const
    {
        Frame2Image<T> tmp(*this);
        tmp-= t;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator*(const T& t) const
    {
        Frame2Image<T> tmp(*this);
        tmp*= t;
        return tmp;
    }

    template<typename T>
    Frame2Image<T> Frame2Image<T>::operator/(const T& t) const
    {
        Frame2Image<T> tmp(*this);
        tmp/= t;
        return tmp;
    }
    
    
    template<typename T>
    Frame3Image<T>::Frame3Image()
    : Image<T>(), Frame3()
    {
    }

    template<typename T>
    Frame3Image<T>::Frame3Image(const Frame3Image<T>& b)
    : Image<T>(b), Frame3(b)
    {
    }

    template<typename T>
    Frame3Image<T>::Frame3Image(unsigned int rows, unsigned int columns, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    : Image<T>(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode), Frame2()
    {
    }

    template<typename T>
    Frame3Image<T>::Frame3Image(unsigned int rows, unsigned int columns, Border2 b)
    : Image<T>(rows, columns, b), Frame3()
    {
    }

    template<typename T>
    Frame3Image<T>::~Frame3Image()
    {
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator=(const Image<T>& b)
    {
        this->Image<T>::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    Frame3Image<T>& Frame3Image<T>::operator=(const Image<S>& b)
    {
        this->Image<T>::operator=(b);
        return *this;
    }
    
    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator=(const Frame3Image<T>& b)
    {
        this->Image<T>::operator=(b);
        this->Frame3::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    Frame3Image<T>& Frame3Image<T>::operator=(const Frame3Image<S>& b)
    {
        this->Image<T>::operator=(b);
        this->Frame3::operator=(b);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator+=(const Image<T>& b)
    {
        this->Image<T>::operator+=(b);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator-=(const Image<T>& b)
    {
        this->Image<T>::operator-=(b);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator*=(const Image<T>& b)
    {
        this->Image<T>::operator*=(b);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator/=(const Image<T>& b)
    {
        this->Image<T>::operator/=(b);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator=(const T& t)
    {
        this->Image<T>::operator=(t);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator+=(const T& t)
    {
        this->Image<T>::operator+=(t);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator-=(const T& t)
    {
        this->Image<T>::operator-=(t);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator*=(const T& t)
    {
        this->Image<T>::operator*=(t);
        return *this;
    }

    template<typename T>
    Frame3Image<T>& Frame3Image<T>::operator/=(const T& t)
    {
        this->Image<T>::operator/=(t);
        return *this;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator+(const Image<T>& b) const
    {
        Frame3Image<T> tmp(*this);
        tmp+= b;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator-(const Image<T>& b) const
    {
        Frame3Image<T> tmp(*this);
        tmp-= b;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator*(const Image<T>& b) const
    {
        Frame3Image<T> tmp(*this);
        tmp*= b;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator/(const Image<T>& b) const
    {
        Frame3Image<T> tmp(*this);
        tmp/= b;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator+(const T& t) const
    {
        Frame3Image<T> tmp(*this);
        tmp+= t;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator-(const T& t) const
    {
        Frame3Image<T> tmp(*this);
        tmp-= t;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator*(const T& t) const
    {
        Frame3Image<T> tmp(*this);
        tmp*= t;
        return tmp;
    }

    template<typename T>
    Frame3Image<T> Frame3Image<T>::operator/(const T& t) const
    {
        Frame3Image<T> tmp(*this);
        tmp/= t;
        return tmp;
    }
}

#endif	/* FRAMEDIMAGE_H */

