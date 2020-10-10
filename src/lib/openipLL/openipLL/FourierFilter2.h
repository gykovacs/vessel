/**
 * @file FourierFilter2.h
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
 * The FourierFilter2 class represents a 2D Fourier filter.
 */

#ifndef _FOURIERFILTER2_H
#define	_FOURIERFILTER2_H

#include <openipDS/openipDS.h>
#include <openipDS/GaborFilter2.h>

#include <openipSC/FourierTransformer2.h>

namespace openip
{
    /**
    * FourierFilter2 represents a 2-dimensional FourierFilter object
    */
    template<typename INPUT, typename OUTPUT>
    class FourierFilter2: public Image<complex<double> >
    {
    public:
        /**
         * constructor
         * @param rows rows of the image on which the filter will be applied
         * @param columns columns of the image on which the filter will be applied
         * @param f filter in image space from which the Fourier filter is generated
         * @param c Cache2 object to make the operation faster
         */
        FourierFilter2(Filter2<INPUT, OUTPUT, float>* f= NULL, bool correlation= false);

        /**
         * copy constructor
         * @param ff instance to copy
         */
        FourierFilter2(const FourierFilter2& ff);

        /**
         * destructor
         */
        ~FourierFilter2();

        /**
         * changes the filter to the parameter
         * @param f filter in image space
         */
        virtual void setFilter(Filter2<INPUT, OUTPUT, float>* f, bool correlation);

        /**
         * applies the filter to the input image and replaces the input with the
         * output
         * @param input input/output image
         */
        //virtual void apply(Image<INPUT>& input);

        /**
         * applies the filter to the input image and the output gets into the
         * output image
         * @param input input image
         * @param output output image
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output);

        virtual void updateSize(int rows, int columns);

        /**
         * FourierTransformer2 instance
         */
        FourierTransformer2<INPUT, OUTPUT> mft;
        FourierTransformer2<float, OUTPUT> mftf;

        /**
         * temporary image instance
         */
        Image<complex<double> > tmp;

        Filter2<INPUT, OUTPUT, float>* filter;

        bool correlation;
    };

    template<typename INPUT, typename OUTPUT>
    void FourierFilter2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        if ( this->rows != rows || this->columns != columns )
        {
            Image<float> im;

            mft.updateSize(rows, columns);
            mftf.updateSize(rows, columns);
            tmp.resizeImage(rows, columns);
            im.resizeImage(rows, columns);
            this->resizeImage(rows, columns);

            im= 0;

            int x= 0, y= 0;

            for ( typename Filter2<INPUT, OUTPUT, float>::fIt fit= filter->begin(); fit != filter->end(); ++fit )
            {
                x= fit->first/filter->stride;
                if ( abs(fit->first%filter->stride) > filter->stride/2 )
                    x= (fit->first > 0) ? x+1 : x-1;

                y= fit->first%filter->stride;
                if ( abs(y) > filter->stride/2 )
                    y+= y > 0 ? - filter->stride : filter->stride;

                if ( x < 0 )
                    x= im.columns+x;
                if ( y < 0 )
                    y= im.rows+y;

                im(y,x)= fit->second;
            }
            mftf.transform(im.data(), this->data());

            if ( correlation )
                for ( unsigned int i= 0; i < this->n; ++i )
                    (*this)(i)= std::complex<double>(real((*this)(i)), -imag((*this)(i)));

            this->rows= rows;
            this->columns= columns;
        }
    }

    template<typename INPUT, typename OUTPUT>
    FourierFilter2<INPUT, OUTPUT>::FourierFilter2(Filter2<INPUT, OUTPUT, float>* f, bool correlation)
    : Image<complex<double> >()
    {
        this->filter= f;
        this->correlation= correlation;
    }

    template<typename INPUT, typename OUTPUT>
    FourierFilter2<INPUT, OUTPUT>::FourierFilter2(const FourierFilter2& ff)
    : Image<complex<double> >(ff)
    {
        this->mft= ff.mft;
        this->tmp= ff.tmp;
        this->correlation= ff.correlation;
    }

    template<typename INPUT, typename OUTPUT>
    FourierFilter2<INPUT, OUTPUT>::~FourierFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierFilter2<INPUT, OUTPUT>::setFilter(Filter2<INPUT, OUTPUT, float>* f, bool correlation)
    {
        this->filter= f;
        this->correlation= correlation;
        this->rows= this->columns= -1;
    }

    /*template<typename INPUT, typename OUTPUT>
    void FourierFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input)
    {
        this->updateSize(input.rows, input.columns);
        mft.transform(input.data(), tmp.data());
        (tmp)*= *this;
        mft.inverseTransform(tmp.data(), input.data());
    }*/

    template<typename INPUT, typename OUTPUT>
    void FourierFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        this->updateSize(input.rows, input.columns);
        mft.transform(input.data(), tmp.data());
        (tmp)*= *this;
        mft.inverseTransform(tmp.data(), output.data());
    }

    
}

#endif	/* _FOURIERFILTER2_H */

