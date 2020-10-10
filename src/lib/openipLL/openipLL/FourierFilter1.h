/**
 * @file FourierFilter1.h
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
 * The FourierFilter1 class represents a 1D Fourier filter.
 */

#ifndef _FOURIERFILTER1_H
#define	_FOURIERFILTER1_H

#include <openipDS/Vector.h>
#include <openipDS/Filter1.h>

#include <openipSC/FourierTransformer1.h>

namespace openip
{
    /**
     * FourierFilter1 represents a 1D Fourier-filter
     */
    class FourierFilter1: public Vector<complex<double> >
    {
        /**
         * constructor
         * @param n is the length of the filter (it should be equal to the vector
         * length on which the filter will be applied
         * @param f Filter1 object, representing the filter in image space
         * @param c Cache1 object to make the operation faster
         */
        template<typename INPUT, typename OUTPUT, typename WEIGHTS>
        FourierFilter1(int n, Filter1<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * copy constructor
         * @param ff instance to copy
         */
        FourierFilter1(const FourierFilter1& ff);

        /**
         * destructor
         */
        ~FourierFilter1();

        /**
         * updates the filter
         * @param f the current filter will be updated to this one
         */
        template<typename INPUT, typename OUTPUT, typename WEIGHTS>
        void setFilter(Filter1<INPUT, OUTPUT, WEIGHTS>& f);

        /**
         * applies the filter to the input vector parameter and replaces with
         * the output
         * @param input input, output vector
         */
        template<typename INPUT>
        void apply(Vector<INPUT>& input);

        /**
         * appies the filter to the input vector, the result gets into the
         * output vector
         * @param input input vector
         * @param output output vector
         */
        template<typename INPUT, typename OUTPUT>
        void apply(Vector<INPUT>& input, Vector<OUTPUT>& output);

        /**
         * 1D Fourier transformer instance
         */
        FourierTransformer1* mft;

        /**
         * temporary vector for the filter
         */
        Vector<complex<double> >* tmp;

        /**
         * the cache from which the temporary vector comes
         */
        //Cache1* c;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FourierFilter1::FourierFilter1(int n, Filter1<INPUT, OUTPUT, WEIGHTS>& f)
    : Vector<complex<double> >(n)
    {
        mft= new FourierTransformer1(n);

            tmp= new Vector<complex<double> >(n);

        Vector<WEIGHTS> vect(n);
        vect= 0;

        int x;
        for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= f.begin(); fit != f.end(); ++fit )
        {
            x= fit->first;
            if ( x < 0 )
                x+= f.stride;
            vect(x)= fit->second;
        }
        mft->transform(vect.data(), this->data());
    }

    FourierFilter1::FourierFilter1(const FourierFilter1& ff)
    : Vector<complex<double> >(ff)
    {
        this->mft= ff.mft;
        this->tmp= ff.tmp;
        //this->c= ff.c;
    }

    FourierFilter1::~FourierFilter1()
    {
        //if ( c != NULL )
           // c->put(tmp);
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FourierFilter1::setFilter(Filter1<INPUT, OUTPUT, WEIGHTS>& f)
    {
        Vector<WEIGHTS> vect(this->size());
        vect= 0;

        int x;
        for ( typename Filter<INPUT, OUTPUT, WEIGHTS>::fIt fit= f.begin(); fit != f.end(); ++fit )
        {
            x= fit->first;
            if ( x < 0 )
                x+= f.stride;
            vect(x)= fit->second;
        }
        mft->transform(vect.data(), this->data());
    }

    template<typename INPUT>
    void FourierFilter1::apply(Vector<INPUT>& input)
    {
        mft->transform(input.data(), tmp->data());
        (*tmp)*= *this;
        mft->inverseTransform(tmp->data(), input.data());
    }

    template<typename INPUT, typename OUTPUT>
    void FourierFilter1::apply(Vector<INPUT>& input, Vector<OUTPUT>& output)
    {
        mft->transform(input.data(), tmp->data());
        (*tmp)*= *this;
        mft->inverseTransform(tmp->data(), output.data());
    }
}

#endif	/* _FOURIERFILTER1_H */

