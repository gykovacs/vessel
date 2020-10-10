/**
 * @file FourierVector.h
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
 * The FourierVector class represents an N dimensional Fourier-transformed
 * vector.
 */

#ifndef _FOURIERVECTORND_H
#define	_FOURIERVECTORND_H

#include <complex>
#include <openipDS/Vector.h>

using namespace std;

namespace openip
{
    /**
     * FourierVector represents an N dimensional Fourier transformed vector
     */
    template<typename T>
    class FourierVector : public Vector<complex<T> >
    {
    public:
        using Vector<complex<T> >::resize;
        /**
         * FourierVector<T>::iterator typedef
         */
        typedef typename FourierVector<T>::iterator fvIt;

        /**
         * FourierVector<T>::constIterator typedef
         */
        typedef typename FourierVector<T>::const_iterator const_fvIt;

        /**
         * default constructor
         */
        FourierVector();

        /**
         * copy constructor
         * @param p instance to copy
         */
        FourierVector(const FourierVector& p);

        /**
         * constructor that creates an instance of size size and values of value
         * @param size size of the vector to create
         * @param value initial value of the elements of the vector
         */
        FourierVector(int size, const complex<T>& value= complex<T>());

        /**
         * destructor
         */
        ~FourierVector();

        /**
         * access operator, returns the xth element of the vector as rvalue,
         * the indexing according to spectral indexing
         * @param x index of the element to return
         * @return returns the xth element of the vector
         */
        inline complex<T> operator()(int x) const;

        /**
         * access operator, returns the xth element of the vector as lvalue,
         * the indexing according to spectral indexing
         * @param x index of the element to return
         * @return returns the xth element of the vector
         */
        inline complex<T>& operator()(int x);

        /**
         * spectral assignment operator
         * @param fv object to copy
         * @return this
         */
        FourierVector& operator=(const FourierVector& fv);

        /**
         * spectral addition
         * @param fv object to add
         * @return this
         */
        FourierVector& operator+=(const FourierVector& fv);

        /**
         * spectral subtraction
         * @param fv object to subtract
         * @return this
         */
        FourierVector& operator-=(const FourierVector& fv);

        /**
         * spectral multiplication
         * @param fv to multiply
         * @return this
         */
        FourierVector& operator*=(const FourierVector& fv);

        /**
         * spectral division
         * @param fv to divide with
         * @return this
         */
        FourierVector& operator/=(const FourierVector& fv);

        /**
         * spectral addition
         * @param fv to add
         * @return result as new instance
         */
        FourierVector operator+(const FourierVector& fv);

        /**
         * spectral subtraction
         * @param fv to subtract
         * @return result as new instance
         */
        FourierVector operator-(const FourierVector& fv);

        /**
         * spectral multiplication
         * @param fv object to multiply with
         * @return result as new instance
         */
        FourierVector operator*(const FourierVector& fv);

        /**
         * spectral division
         * @param fv object to divide with
         * @return result as new instance
         */
        FourierVector operator/(const FourierVector& fv);

        /**
         * computes the magnitude vector in v
         * @param v output parameter, magnitude image;
         * @param indexing 0: image space; 1: spectral
         */ 
        template<typename MAGN>
        void getMagnitude(Vector<MAGN>& v, int indexing= 1);
    };

    template<typename T>
    FourierVector<T>::FourierVector()
        :Vector<complex<T> >()
    {
    }

    template<typename T>
    FourierVector<T>::FourierVector(int size, const complex<T>& value)
        :Vector<complex<T> >(size, value)
    {
    }

    template<typename T>
    FourierVector<T>::FourierVector(const FourierVector<T>& p)
        :Vector<complex<T> >(p)
    {
    }

    template<typename T>
    FourierVector<T>::~FourierVector()
    {
    }

    template<typename T>
    inline complex<T> FourierVector<T>::operator()(int x) const
    {
        return (*this)[x < 0 ? -x-1 : this->size() - x-1];
    }

    template<typename T>
    inline complex<T>& FourierVector<T>::operator()(int x)
    {
        return (*this)[x < 0 ? -x-1 : this->size() - x-1];
    }

    template<typename T>
    FourierVector<T>& FourierVector<T>::operator= (const FourierVector<T>& fv)
    {
        this->resize(fv.size());
        int end= fv.size()/2;
        for ( int i= -end; i < end; ++i )
            this->operator()(i)= fv(i);
        return *this;
    }

    template<typename T>
    FourierVector<T>& FourierVector<T>::operator+=(const FourierVector<T>& fv)
    {
        int end= fv.size()/2;
        for ( int i= -end; i < end; ++i )
            this->operator()(i)+= fv(i);

        return *this;
    }

    template<typename T>
    FourierVector<T>& FourierVector<T>::operator-=(const FourierVector<T>& fv)
    {
        int end= fv.size()/2;
        for ( int i= -end; i < end; ++i )
            this->operator()(i)-= fv(i);

        return *this;
    }

    template<typename T>
    FourierVector<T>& FourierVector<T>::operator*=(const FourierVector<T>& fv)
    {
        int end= fv.size()/2;
        for ( int i= -end; i < end; ++i )
        {
            printf("%d - %f - %f\n", i, this->operator()(i).real(), fv(i).real());fflush(stdout);
            this->operator()(i)*= fv(i);
        }

        return *this;
    }

    template<typename T>
    FourierVector<T>& FourierVector<T>::operator/=(const FourierVector<T>& fv)
    {
        int end= fv.size()/2;
        for ( int i= -end; i < end; ++i )
            this->operator()(i)/= fv(i);

        return *this;
    }

    template<typename T>
    FourierVector<T> FourierVector<T>::operator+(const FourierVector<T>& fv)
    {
        FourierVector<T> tmp(*this);
        return tmp+= fv;
    }

    template<typename T>
    FourierVector<T> FourierVector<T>::operator-(const FourierVector<T>& fv)
    {
        FourierVector<T> tmp(*this);
        return tmp+= fv;
    }

    template<typename T>
    FourierVector<T> FourierVector<T>::operator*(const FourierVector<T>& fv)
    {
        FourierVector<T> tmp(*this);
        return tmp*= fv;
    }

    template<typename T>
    FourierVector<T> FourierVector<T>::operator/(const FourierVector<T>& fv)
    {
        FourierVector<T> tmp(*this);
        return tmp/= fv;
    }

    template<typename T> template<typename MAGN>
    void FourierVector<T>::getMagnitude(Vector<MAGN>& v, int indexing)
    {
        if ( indexing == 0 )
        {
            typename Vector<MAGN>::vIt vit= v.begin();
            for ( typename FourierVector<T>::fvIt fit= this->begin(); fit != this->end(); ++fit, ++vit )
                *vit= norm(*fit);
        }
        else if ( indexing == 1 )
        {
            typename Vector<MAGN>::vIt vit= v.begin();
            for ( int i= -this->size()/2; i < this->size()/2; ++i )
                *vit= norm(this->operator()(i));
        }
    }

    /**
     * prints the description of the parameter FourierVector object to the parameter output stream
     * @param o parameter output stream
     * @param f FourierVector object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const FourierVector<T>& f)
    {
        o << "[FourierVector - size: ";
        o << f.size() << "]";
        return o;
    }
}

#endif	/* _FOURIERVECTORND_H */

