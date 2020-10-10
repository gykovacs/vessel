/**
 * @file FourierFilterSet2.h
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
 * The FourierFilterSet2 class represents a set of 2D Fourier filters.
 */

#ifndef _FOURIERFILTERSET2_H
#define	_FOURIERFILTERSET2_H

#include <vector>

#include <openipDS/Image.h>
#include <openipDS/FilterSet2.h>

#include <openipLL/FourierFilter2.h>

namespace openip
{
    /**
     * FourierFilterSet2 represents a set of 2D Fourier filters
     */
    template<typename INPUT, typename OUTPUT>
    class FourierFilterSet2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::operator[];
        //using Transform2<INPUT, OUTPUT>::apply;
        /**
         * default constructor
         */
        FourierFilterSet2();

        /**
         * copy constructor
         * @param ffs instance to copy
         */
        FourierFilterSet2(const FourierFilterSet2& ffs);

        /**
         * destructor
         */
        ~FourierFilterSet2();

        /**
         * adds the filters from the filterset to this FourierFilterSet
         * @param rows rows of the image on which the filter set will be applied
         * @param columns columns of the image on which the filter set will be applied
         * @param fs filter set pointer
         * @param c Cache2 object
         */
        void addFilterSet2(FilterSet2<INPUT, OUTPUT, float>* fs, bool correlation= false);

        /**
         * computes the minimum weighted sum in each position of the input image,
         * the output gets into the output image
         * @param input input image
         * @param output output image
         * @param c Cache2 object to make the operation faster
         */
        //void applyMin(Image<INPUT>& input, Image<OUTPUT>& output);

        /**
         * computes the maximum weighted sum in each position of the input image,
         * the output gets into the output image
         * @param input input image
         * @param output output image
         * @param c Cache2 objecto to make the operation faster
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void updateSize(int rows, int columns);
        virtual Border2 getProposedBorder();

        /**
         * this filter is instantiated when an image space filter set is used
         */
        FourierFilter2<INPUT, OUTPUT> ff2;
        Image<std::complex<double> > tmpImage;
        
        Image<INPUT> input;
        Image<OUTPUT> output;

        Image<OUTPUT> tmp;

        bool correlation;
    };

    

    template<typename INPUT, typename OUTPUT>
    FourierFilterSet2<INPUT, OUTPUT>::FourierFilterSet2()
    : MaxFilterSet2<INPUT, OUTPUT, float>(0)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierFilterSet2<INPUT, OUTPUT>::FourierFilterSet2(const FourierFilterSet2& ffs)
    : MaxFilterSet2<INPUT, OUTPUT, float>(ffs)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierFilterSet2<INPUT, OUTPUT>::~FourierFilterSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2(FilterSet2<INPUT, OUTPUT, float>* fs, bool correlation)
    {
        this->correlation= correlation;
        for( typename FilterSet2<INPUT, OUTPUT, float>::iterator fit= fs->begin(); fit != fs->end(); ++fit )
            this->push_back(*fit);
        fs->clear();
        delete fs;
    }

    template<typename INPUT, typename OUTPUT>
    void FourierFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        //printf("a"); fflush(stdout);
        
        //printf("in: %d %d\n", in.rows, in.columns);
        //printf("out: %d %d\n", out.rows, out.columns);
        int cmin= in.columns, cmax= 0;
        int rmin= in.rows, rmax= 0;
        if ( roi )
        {
            for ( int i= 0; i < roi->rows; ++i )
                for ( int j= 0; j < roi->columns; ++j )
                {
                    if ( (*roi)(i,j) )
                    {
                        if ( i < rmin )
                            rmin= i;
                        if ( i > rmax )
                            rmax= i;
                        if ( j < cmin )
                            cmin= j;
                        if ( j > cmax )
                            cmax= j;
                    }
                }
        }

        //printf("b"); fflush(stdout);
        if ( !roi )
        {
            //printf("noroi ===> no resized\n"); fflush(stdout);
            
            input.resizeImage(in);
            output.resizeImage(out);
            input= in;
        }
        else
        {
            Border2 b= this->getProposedBorder();
            rmax= rmax + b.bottomBorder < in.rows ? rmax + b.bottomBorder : in.rows-1;
            rmin= rmin - b.topBorder >= 0 ? rmin - b.topBorder : 0;
            cmax= cmax + b.rightBorder < in.columns ? cmax + b.rightBorder : in.columns-1;
            cmin= cmin - b.leftBorder >= 0 ? cmin - b.leftBorder : 0;

            //writeImage("in.jpg", in);
            //printf("roi ===> resized %d %d\n", rmax - rmin, cmax - cmin); fflush(stdout);
            input.resizeImage(rmax - rmin + 1, cmax - cmin + 1);
            fitToFourier(b, input);
            //printf("fitted to Fourier: %d %d %d %d %d %d\n", input.rows, input.columns, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder);
            input.resizeBorder(b);
            //printf("after fitting: %d %d %d %d %d %d\n", input.rows, input.columns, input.topBorder, input.bottomBorder, input.leftBorder, input.rightBorder);
            output.resizeImage(input);
            //printf("outptu size: %d %d %d %d %d %d\n", output.rows, output.columns, output.topBorder, output.bottomBorder, output.leftBorder, output.rightBorder);
            for ( int i= rmin; i <= rmax; ++i )
                for ( int j= cmin; j <= cmax; ++j )
                    input(b.topBorder + i - rmin, b.leftBorder + j - cmin)= in(i,j);
            //writeImage("test.jpg", input);
        }

        //printf("c"); fflush(stdout);
        this->updateSize(input.rows, input.columns);

        if ( this->begin() != this->end() )
        {
            ff2.setFilter((*this)[0], correlation);
            ff2.updateSize(input.rows, input.columns);
            ff2.mft.transform(input.data(), tmpImage.data());
            ff2.tmp= tmpImage;
            ff2.tmp*= ff2;
            ff2.mft.inverseTransform(ff2.tmp.data(), output.data());

            for ( typename FourierFilterSet2<INPUT, OUTPUT>::iterator fit= this->begin() + 1; fit != this->end(); ++fit )
            {
                ff2.setFilter(*fit, correlation);
                //ff2.apply(input, tmp);
                ff2.updateSize(input.rows, input.columns);
                ff2.tmp= tmpImage;
                ff2.tmp*= ff2;
                ff2.mftf.inverseTransform(ff2.tmp.data(), tmp.data());
                for ( unsigned int i= 0; i < output.n; ++i )
                    if ( (tmp)(i) > output(i) )
                        output(i)= (tmp)(i);
            }
        }
        //printf("e"); fflush(stdout);
        if ( !roi )
            out= output;
        else
        {
            //printf("%d %d\n", out.rows, out.columns);
            //printf("%d %d %d\n", input.topBorder, input.rows, input.bottomBorder);
            //printf("%d %d %d\n", input.leftBorder, input.columns, input.rightBorder);
            //printf("%d %d\n", rmin, cmin);
            for ( int i= input.topBorder; i < input.rows - input.bottomBorder; ++i )
                for ( int j= input.leftBorder; j < input.columns - input.rightBorder; ++j )
                    out(i - input.topBorder + rmin, j - input.leftBorder + cmin)= output(i,j);
        }
        //printf("f"); fflush(stdout);
    }


    /*template<typename INPUT, typename OUTPUT>
    void FourierFilterSet2<INPUT, OUTPUT>::applyMax(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        this->updateSize(input.rows, input.columns);

        if ( this->begin() != this->end() )
        {
            (*(this->begin()))->apply(input, output);

            int i= 0;
            for ( typename FourierFilterSet2<INPUT, OUTPUT>::iterator fit= this->begin() + 1; fit != this->end(); ++fit )
            {
                (*fit)->apply(input, tmp);
                for ( unsigned int i= 0; i < output.n; ++i )
                    if ( (tmp)(i) > output(i) )
                        output(i)= (tmp)(i);
            }
        }
    }*/

    template<typename INPUT, typename OUTPUT>
    void FourierFilterSet2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        tmp.resizeImage(rows, columns);
        tmpImage.resizeImage(rows, columns);

        /*for ( typename FourierFilterSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
            (*fit)->updateSize(rows, columns);*/
    }

    template<typename INPUT, typename OUTPUT>
    Border2 FourierFilterSet2<INPUT, OUTPUT>::getProposedBorder()
    {
        return this->MaxFilterSet2<INPUT, OUTPUT, float>::getProposedBorder();
    }
}

#endif	/* _FOURIERFILTERSET2_H */

