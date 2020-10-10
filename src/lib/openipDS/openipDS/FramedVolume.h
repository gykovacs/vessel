/* 
 * File:   FramedVolume.h
 * Author: gykovacs
 *
 * Created on July 18, 2011, 12:40 PM
 */

#ifndef FRAMEDVOLUME_H
#define	FRAMEDVOLUME_H

#include <openipDS/Frame3.h>
#include <openipDS/Volume.h>

namespace openip
{
    template<typename T>
    class FramedVolume: public Volume<T>, public Frame3
    {
    public:
        using Volume<T>::operator();
        using Volume<T>::operator !=;
        using Volume<T>::operator ==;
        using Volume<T>::getCoordinate3D;
        using Volume<T>::getCoordinate1D;
        using Volume<T>::resizeBorder;
        using Volume<T>::resizeVolume;
        using Volume<T>::fillBorder;
        using Frame3::getRealCoordinates;
        using Frame3::setFrame;

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
        FramedVolume();

        /**
         * copy constructor
         * @param b object to copy
         */
        
        FramedVolume(const FramedVolume<T>& b);

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
        FramedVolume(unsigned int slices, unsigned int rows, unsigned int columns, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
        
        FramedVolume(unsigned int slices, unsigned int rows, unsigned int columns, Border3 b);

        /**
         * desctructor
         */
        ~FramedVolume();

        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        FramedVolume<T>& operator=(const Volume<T>& b);

        /**
         * assignment operator for different types of Volumes
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        FramedVolume<T>& operator=(const Volume<S>& b);
        
        /**
         * assignment operator
         * @param b the instance to copy
         * @return this
         */
        FramedVolume<T>& operator=(const FramedVolume<T>& b);

        /**
         * assignment operator for different types of Volumes
         * @param b the instance to copy
         * @return this
         */
        template<typename S>
        FramedVolume<T>& operator=(const FramedVolume<S>& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        FramedVolume<T>& operator+=(const Volume<T>& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        FramedVolume<T>& operator-=(const Volume<T>& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        FramedVolume<T>& operator*=(const Volume<T>& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        FramedVolume<T>& operator/=(const Volume<T>& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        FramedVolume<T>& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        FramedVolume<T>& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        FramedVolume<T>& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        FramedVolume<T>& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        FramedVolume<T>& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FramedVolume<T> operator+(const Volume<T>& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FramedVolume<T> operator-(const Volume<T>& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FramedVolume<T> operator*(const Volume<T>& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FramedVolume<T> operator/(const Volume<T>& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        FramedVolume<T> operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        FramedVolume<T> operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        FramedVolume<T> operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        FramedVolume<T> operator/(const T& t) const;
        
        template<typename S>
        void setFrame(const FramedVolume<S>& f);
	
	template<typename S>
        void resizeVolume(Volume<S>& v);
	
	template<typename S>
	void resizeVolume(const FramedVolume<S>& b);
    };
    
    template<typename T>
    FramedVolume<T>::FramedVolume()
    : Volume<T>(), Frame3()
    {
    }

    template<typename T>
    FramedVolume<T>::FramedVolume(const FramedVolume<T>& b)
    : Volume<T>(b), Frame3(b)
    {
    }

    template<typename T>
    FramedVolume<T>::FramedVolume(unsigned int slices, unsigned int rows, unsigned int columns, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    : Volume<T>(slices, rows, columns, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode), Frame3()
    {
    }

    template<typename T>
    FramedVolume<T>::FramedVolume(unsigned int slices, unsigned int rows, unsigned int columns, Border3 b)
    : Volume<T>(slices, rows, columns, b), Frame3()
    {
    }

    template<typename T>
    FramedVolume<T>::~FramedVolume()
    {
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator=(const Volume<T>& b)
    {
        this->Volume<T>::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    FramedVolume<T>& FramedVolume<T>::operator=(const Volume<S>& b)
    {
        this->Volume<T>::operator=(b);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator=(const FramedVolume<T>& b)
    {
        this->Volume<T>::operator=(b);
        this->Frame3::operator=(b);
        return *this;
    }

    template<typename T> template<typename S>
    FramedVolume<T>& FramedVolume<T>::operator=(const FramedVolume<S>& b)
    {
        this->Volume<T>::operator=(b);
        this->Frame3::operator=(b);
        return *this;
    }
    
    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator+=(const Volume<T>& b)
    {
        this->Volume<T>::operator+=(b);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator-=(const Volume<T>& b)
    {
        this->Volume<T>::operator-=(b);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator*=(const Volume<T>& b)
    {
        this->Volume<T>::operator*=(b);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator/=(const Volume<T>& b)
    {
        this->Volume<T>::operator/=(b);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator=(const T& t)
    {
        this->Volume<T>::operator=(t);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator+=(const T& t)
    {
        this->Volume<T>::operator+=(t);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator-=(const T& t)
    {
        this->Volume<T>::operator-=(t);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator*=(const T& t)
    {
        this->Volume<T>::operator*=(t);
        return *this;
    }

    template<typename T>
    FramedVolume<T>& FramedVolume<T>::operator/=(const T& t)
    {
        this->Volume<T>::operator/=(t);
        return *this;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator+(const Volume<T>& b) const
    {
        FramedVolume<T> tmp(*this);
        tmp+= b;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator-(const Volume<T>& b) const
    {
        FramedVolume<T> tmp(*this);
        tmp-= b;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator*(const Volume<T>& b) const
    {
        FramedVolume<T> tmp(*this);
        tmp*= b;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator/(const Volume<T>& b) const
    {
        FramedVolume<T> tmp(*this);
        tmp/= b;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator+(const T& t) const
    {
        FramedVolume<T> tmp(*this);
        tmp+= t;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator-(const T& t) const
    {
        FramedVolume<T> tmp(*this);
        tmp-= t;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator*(const T& t) const
    {
        FramedVolume<T> tmp(*this);
        tmp*= t;
        return tmp;
    }

    template<typename T>
    FramedVolume<T> FramedVolume<T>::operator/(const T& t) const
    {
        FramedVolume<T> tmp(*this);
        tmp*= t;
        return tmp;
    }
    
    template<typename T> template<typename S>
    void FramedVolume<T>::setFrame(const FramedVolume<S>& f)
    {
        this->origin= f.origin;
        this->baseX= f.baseX;
        this->baseY= f.baseY;
        this->baseZ= f.baseZ;
    }
    
    template<class T> template<typename S>
    void openip::FramedVolume<T>::resizeVolume(const FramedVolume<S>& b)
    {
	this->resizeVolume(b.slices - b.frontBorder - b.backBorder, b.rows - b.topBorder - b.bottomBorder, b.columns - b.leftBorder - b.rightBorder, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
	this->setFrame(b);
    }
}

#endif	/* FRAMEDVOLUME_H */

