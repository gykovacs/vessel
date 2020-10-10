/**
 * @file FourierMatrix.h
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
 * The FourierMatrix class represents an N x M dimensional Fourier-transformed matrix.
 */

#ifndef _FOURIERMATRIX_H
#define	_FOURIERMATRIX_H

#include <complex>

#include <openipDS/Matrix.h>

using namespace std;

namespace openip
{
    /**
     * FourierMatrix represents an N x M dimensional Fourier transformed vector
     */
    template<typename T>
    class FourierMatrix : public Matrix<complex<T> >
    {
    public:
        /**
         * FourierMatrix<T>::iterator typedef
         */
        typedef typename FourierMatrix<T>::iterator fmIt;

        /**
         * FourierMatrix<T>::const_iterator typedef
         */
        typedef typename FourierMatrix<T>::const_iterator const_fmIt;

        /**
         * default constructor
         */
        FourierMatrix();

        /**
         * copy constructor
         * @param p instance to copy
         */
        FourierMatrix(const FourierMatrix& p);

        /**
         * constructor with size parameters
         * @param rows number of rows
         * @param columns number of columns
         */
        FourierMatrix(int rows, int columns);

        /**
         * destructor
         */
        ~FourierMatrix();

        /**
         * access operator, returns the xth element of the vector as rvalue,
         * the indexing according to spectral indexing
         * @param x index of the element to return
         * @return returns the xth element of the vector
         */
        //inline complex<T> operator()(int x) const;

        /**
         * access operator, returns the xth element of the vector as lvalue,
         * the indexing according to spectral indexing
         * @param x index of the element to return
         * @return returns the xth element of the vector
         */
        //inline complex<T>& operator()(int x);

        /**
         * access operator, returns the x,yth element of the matrix as rvalue,
         * with spectral indexing
         * @param x row coordinate
         * @param y column coordinate
         * @return (x,y)th element of the Fourier transformed matrix
         */
        inline complex<T> operator()(int x, int y) const;

        /**
         * access operator, returns the x,yth element of the matrix as lvalue,
         * with spectral indexing
         * @param x row coordinate
         * @param y column coordinate
         * @return (x,y)th element of the Fourier transformed matrix
         */
        inline complex<T>& operator()(int x, int y);

        /**
         * spectral assignment operator
         * @param fv object to copy
         * @return this
         */
        FourierMatrix& operator=(const FourierMatrix& fv);

        /**
         * spectral addition
         * @param fv object to add
         * @return this
         */
        FourierMatrix& operator+=(const FourierMatrix& fv);

        /**
         * spectral subtraction
         * @param fv object to subtract
         * @return this
         */
        FourierMatrix& operator-=(const FourierMatrix& fv);

        /**
         * spectral multiplication
         * @param fv to multiply
         * @return this
         */
        FourierMatrix& operator*=(const FourierMatrix& fv);

        /**
         * spectral division
         * @param fv to divide with
         * @return this
         */
        FourierMatrix& operator/=(const FourierMatrix& fv);

        /**
         * spectral addition
         * @param fv to add
         * @return result as new instance
         */
        FourierMatrix operator+(const FourierMatrix& fv);

        /**
         * spectral subtraction
         * @param fv to subtract
         * @return result as new instance
         */
        FourierMatrix operator-(const FourierMatrix& fv);

        /**
         * spectral multiplication
         * @param fv object to multiply with
         * @return result as new instance
         */
        FourierMatrix operator*(const FourierMatrix& fv);

        /**
         * spectral division
         * @param fv object to divide with
         * @return result as new instance
         */
        FourierMatrix operator/(const FourierMatrix& fv);

        /**
         * computes the magnitude vector in v
         * @param m output parameter, magnitude image;
         * @param indexing 0: image space; 1: spectral
         */
        template<typename MAGN>
        void getMagnitude(Matrix<MAGN>& m, int indexing= 1);
    };

    template<typename T>
    FourierMatrix<T>::FourierMatrix()
        :Matrix<complex<T> >()
    {
    }

    template<typename T>
    FourierMatrix<T>::FourierMatrix(int rows, int columns)
        :Matrix<complex<T> >(rows, columns)
    {
    }

    template<typename T>
    FourierMatrix<T>::FourierMatrix(const FourierMatrix<T>& p)
        :Matrix<complex<T> >(p)
    {
    }

    template<typename T>
    FourierMatrix<T>::~FourierMatrix()
    {
    }

    /*template<typename T>
    inline complex<T> FourierMatrix<T>::operator()(int x) const
    {
        return (*this)[x >= 0 ? x : this->size() - x];
    }

    template<typename T>
    inline complex<T>& FourierMatrix
    <T>::operator()(int x)
    {
        return (*this)[x >= 0 ? x : this->size() - x];
    }*/

    template<typename T>
    inline complex<T> FourierMatrix<T>::operator()(int x, int y) const
    {
        return this->Matrix<complex<T> >::operator()((x < 0 ? -x-1 : (this->rows - x-1)), (y < 0 ? -y-1 : (this->columns - y-1)));
    }

    template<typename T>
    inline complex<T>& FourierMatrix<T>::operator()(int x, int y)
    {
        return this->Matrix<complex<T> >::operator()((x < 0 ? -x-1 : (this->rows - x-1)), (y < 0 ? -y-1 : (this->columns - y-1)));
    }

    template<typename T>
    FourierMatrix<T>& FourierMatrix<T>::operator= (const FourierMatrix<T>& fv)
    {
        this->resize(fv.rows, fv.columns);
        int endR= fv.rows/2;
        int endC= fv.columns/2;

        for ( int i= -fv.rows/2; i < endR; ++i )
            for ( int j= -fv.columns/2; j < endC; ++j )
                this->operator()(i,j)= fv(i,j);
        return *this;
    }

    template<typename T>
    FourierMatrix<T>& FourierMatrix<T>::operator+=(const FourierMatrix<T>& fv)
    {
        int endR= fv.rows/2;
        int endC= fv.columns/2;

        for ( int i= -fv.rows/2; i < endR; ++i )
            for ( int j= -fv.columns/2; j < endC; ++j )
                this->operator()(i,j)+= fv(i,j);
        return *this;
    }

    template<typename T>
    FourierMatrix<T>& FourierMatrix<T>::operator-=(const FourierMatrix<T>& fv)
    {
        int endR= fv.rows/2;
        int endC= fv.columns/2;

        for ( int i= -fv.rows/2; i < endR; ++i )
            for ( int j= -fv.columns/2; j < endC; ++j )
                this->operator()(i,j)-= fv(i,j);
        return *this;
    }

    template<typename T>
    FourierMatrix<T>& FourierMatrix<T>::operator*=(const FourierMatrix<T>& fv)
    {
        int endR= fv.rows/2;
        int endC= fv.columns/2;

        for ( int i= -fv.rows/2; i < endR; ++i )
            for ( int j= -fv.columns/2; j < endC; ++j )
                this->operator()(i,j)*= fv(i,j);
        //this->Vector<complex<T> >::operator*=(fv);
        return *this;
    }

    template<typename T>
    FourierMatrix<T>& FourierMatrix<T>::operator/=(const FourierMatrix<T>& fv)
    {
        int endR= fv.rows/2;
        int endC= fv.columns/2;

        for ( int i= -fv.rows/2; i < endR; ++i )
            for ( int j= -fv.columns/2; j < endC; ++j )
                this->operator()(i,j)/= fv(i,j);
        return *this;
    }

    template<typename T>
    FourierMatrix<T> FourierMatrix<T>::operator+(const FourierMatrix<T>& fv)
    {
        FourierMatrix<T> tmp(*this);
        return tmp+= fv;
    }

    template<typename T>
    FourierMatrix<T> FourierMatrix<T>::operator-(const FourierMatrix<T>& fv)
    {
        FourierMatrix<T> tmp(*this);
        return tmp+= fv;
    }

    template<typename T>
    FourierMatrix<T> FourierMatrix<T>::operator*(const FourierMatrix<T>& fv)
    {
        FourierMatrix<T> tmp(*this);
        return tmp*= fv;
    }

    template<typename T>
    FourierMatrix<T> FourierMatrix<T>::operator/(const FourierMatrix<T>& fv)
    {
        FourierMatrix<T> tmp(*this);
        return tmp/= fv;
    }

    template<typename T> template<typename MAGN>
    void FourierMatrix<T>::getMagnitude(Matrix<MAGN>& m, int indexing)
    {
        if ( indexing == 0 )
        {
            typename Matrix<MAGN>::mIt vit= m.begin();
            for ( typename FourierMatrix<T>::fmIt fit= this->begin(); fit != this->end(); ++fit, ++vit )
                *vit= norm(*fit);
        }
        else if ( indexing == 1 )
        {
            typename Matrix<MAGN>::mIt vit= m.begin();
            for ( int i= -this->rows/2; i < this->rows/2; ++i )
                for ( int j= -this->columns/2; j < this->columns/2; ++j )
                {
                    *vit= log(norm(this->operator()(i, j)));
                    ++vit;
                }
        }
    }

    /**
     * prints the description of the parameter FourierMatrix object to the parameter output stream
     * @param o parameter output stream
     * @param f FourierMatrix object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const FourierMatrix<T>& f)
    {
        o << "[FourierVector - size: ";
        o << f.columns << "x" << f.rows << "]";
        return o;
    }
}

#endif	/* _FOURIERMATRIX_H */

