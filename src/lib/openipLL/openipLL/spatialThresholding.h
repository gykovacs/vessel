/**
 * @file spatialThresholding.h
 * @author Janos Ivan Istvan <janos.istvan.ivan@gmail.com>
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
 * Thresholding methods based on spatial characterisitcs.
 */

#ifndef _SPATIALTHRESHOLDING_H_
#define _SPATIALTHRESHOLDING_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>

namespace openip
{
    /**
    * Pal's spatial thresholding method 1
    * for details check:
    * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
    * and quantitative performance evaluation, Journal of Electronic Imaging
    * 13(1), 146 -- 165 (January 2004)
    * @param input input image
    * @param output output thresholded image
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template <typename INPUT>
    void thresholdingSpatialPal1(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

    /**
    * Pal's spatial thresholding method 2
    * for details check:
    * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
    * and quantitative performance evaluation, Journal of Electronic Imaging
    * 13(1), 146 -- 165 (January 2004)
    * @param input input image
    * @param output output thresholded image
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template <typename INPUT>
    void thresholdingSpatialPal2(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);
}

/*template <typename INPUT>
void openip::thresholdingSpatialPal1(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    INPUT min, max;
    input.getMinMax(min, max);

    unsigned int width= input.columns;
    int n= input.n;
    int size= 256;
    int i, j, t;

    Histogram arg;
    arg.resize(size);

    float tmp= (max - min) / (float)(size - 1);
    float intensity= 0;

    if ( mask == NULL )
    {
        for ( t= 0; t < size; t++)
        {
            intensity= min + t * tmp;
            arg[t]= 0.0;

            for ( i= 0; i < n - width; i+= width)
                for ( j= 0; j < width - 1; j++)
                    if ( (input(i + j) > intensity) && (input(i + j + 1) <= intensity) ||
                         (input(i + j) > intensity) && (input(i + j + width) <= intensity) )
                                arg[t]++;
        }

        float tOpt= arg.getMaxIndex();
        tOpt= ( ( tOpt * (max - min)) / (float) 256) + min;

        for ( i= 0; i < input.n; i++ )
            if ( input(i) >= tOpt )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
    {
        for ( t= 0; t < size; t++)
        {
            intensity= min + t * tmp;
            arg[t]= 0.0;

            for ( i= 0; i < n - width; i+= width)
                for ( j= 0; j < width - 1; j++)
                    if ( (*mask)(i + j) > 0 )
                        if ( (input(i + j) > intensity) && (input(i + j + 1) <= intensity) ||
                             (input(i + j) > intensity) && (input(i + j + width) <= intensity) )
                                    arg[t]++;
        }
        float tOpt= arg.getMaxIndex();
        tOpt= ( ( tOpt * (max - min)) / (float) 256) + min;

        for ( i= 0; i < input.n; i++ )
            if ( (*mask)(i) > 0 )
                if ( input(i) >= tOpt )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
    }
}

template <typename INPUT>
void openip::thresholdingSpatialPal2(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    INPUT min, max;
    input.getMinMax(min, max);

    unsigned int width= input.columns;
    int n= input.n;
    int size= 256;
    int i, j, t;

    Histogram arg;
    arg.resize(size);

    float tmp= (max - min) / (float)(size - 1);
    float intensity= 0;

    if ( mask == NULL )
    {
        for ( t= 0; t < size; t++ )
        {
            intensity= min + t * tmp;
            arg[t]= 0.0;
            for ( i= 0; i < n - width; i+= width )
                for ( j= 0; j < width - 1; j++ )
                    if ( (input(i + j) > intensity) && (input(i + j + 1) <= intensity) ||
                         (input(i + j) <= intensity) && (input(i + j + width) > intensity) )
                            arg[t]++;
        }

        float tOpt= arg.getMaxIndex();
        tOpt= ((tOpt * (max - min)) / (float) size) + min;

        for ( i= 0; i < input.n; i++ )
            if ( input(i) > tOpt )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
    {
        for ( int t= 0; t < size; t++)
        {
            intensity= min + t * tmp;
            arg[t]= 0.0;
            for ( i= 0; i < n - width; i+= width )
                for ( j= 0; j < width - 1; j++ )
                    if ( (*mask)(i + j) > 0 )
                        if ( (input(i + j) > intensity) && (input(i + j + 1) <= intensity) ||
                             (input(i + j) <= intensity) && (input(i + j + width) > intensity) )
                                arg[t]++;
        }

        float tOpt= arg.getMaxIndex();
        tOpt= ((tOpt * (max - min)) / (float) size) + min;

        for ( i= 0; i < input.n; i++ )
            if ( (*mask)(i) > 0 )
                if ( input(i) > tOpt )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
    }
}*/

#endif
