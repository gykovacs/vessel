/**
 * @file shapeThresholding.h
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
 * Image thresholding methods based on the shape of the histogram.
 */

#ifndef _SHAPETHRESHOLDING_H_
#define _SHAPETHRESHOLDING_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>

#include <math.h>

namespace openip
{
    /**
    * Rosenfelds' histogram shape based thresholding method
    * for details check:
    * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
    * and quantitative performance evaluation, Journal of Electronic Imaging
    * 13(1), 146 -- 165 (January 2004)
    * @param input input image
    * @param output output thresholded image
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template <typename INPUT>
    void thresholdingShapeRosenfeld(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

    /**
    * Sezan's histogram shape based thresholding method
    * for details check:
    * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
    * and quantitative performance evaluation, Journal of Electronic Imaging
    * 13(1), 146 -- 165 (January 2004)
    * @param input input image
    * @param output output thresholded image
    * @param kernel method parameter, kernel size
    * @param y method parameter
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template <typename INPUT>
    void thresholdingShapeSezan(Image<INPUT>& input, Image<unsigned char>& output, unsigned int kernel= 55, double y= 1.0, Image<unsigned char>* mask= NULL);
 
    /**
    * Ramesh's histogram shape based thersholding method
    * for details check:
    * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
    * and quantitative performance evaluation, Journal of Electronic Imaging
    * 13(1), 146 -- 165 (January 2004) 
    * @param input input image
    * @param output output thresholded image
    * @param mask the operation is performed in the foreground (non 0) region of the mask
    */
    template <typename INPUT>
    void thresholdingShapeRamesh(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);
}

/*template <typename INPUT>
void openip::thresholdingShapeRosenfeld(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    Histogram h;

    INPUT min, max;
    input.getMinMax(min, max, mask);
    h.computeHistogram(&input, 256, min, max, mask);
    h.normalize();

    int size= 256;
    int i;

    Histogram arg;
    arg.resize( size );

    arg= h.histogramConvexHull();
    arg.normalize();

    //p(g) - hull(g)
    for ( i= 0; i < size; i++ )
        arg[i]= fabs(arg[i] - h[i]);

    float tOpt= arg.getMaxIndex();
    tOpt= ((tOpt * (max - min)) / (double) size) + min;

    if ( mask == NULL )
    {
        for ( i= 0; i < input.n; i++ )
            if ( input(i) > tOpt )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
    {
        for ( i= 0; i < input.n; i++ )
            if ( (*mask)(i) > 0 )
                if ( input(i) > tOpt )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
    }
}

template <typename INPUT>
void openip::thresholdingShapeSezan(Image<INPUT>& input, Image<unsigned char>& output, unsigned int kernel, double y, Image<unsigned char>* mask)
{
    int size= 256;
    
    Histogram h;

    INPUT min, max;
    input.getMinMax(min, max, mask);

    h.computeHistogram(&input, 256, min, max, mask);
    h.normalize();
    
    Vector<Peak> peaks;
    h.peakDetectionSignal(kernel, peaks);

    float tOpt= y * (double)(peaks[0].getMaximum()) + (1.0 - y) * (double)(peaks[1].getStart());
    tOpt= ((tOpt * (max - min)) / (double) size) + min;

    if ( mask == NULL )
    {
        for ( int i= 0; i < input.n; i++ )
            if ( input(i) > tOpt )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
    {
        for ( int i= 0; i < input.n; i++ )
            if ( (*mask)(i) > 0 )
                if ( input(i) > tOpt )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
    }

}

template <typename INPUT>
void openip::thresholdingShapeRamesh(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    Histogram h;

    INPUT min, max;
    input.getMinMax(min, max, mask);
    h.computeHistogram(&input, 256, min, max, mask);
    h.normalize();
    int size= 256;
    int i, g, t;

    Histogram arg;
    arg.resize( size );

    Vector<float> pTResults, mFResults, mBResults;
    pTResults.resize( size );
    mFResults.resize( size );
    mBResults.resize( size );

    for ( i= 0; i < size; ++i )
    {
        pTResults[i]= h.probabilityMassFunction( i );
        mFResults[i]= h.mean((int) (size / 2.0), HISTOGRAM_GREATER);
        mBResults[i]= h.mean((int) (size / 2.0), HISTOGRAM_LESS_OR_EQUAL);
    }

    float mResult, b1, b2;
    for ( t= 0; t < size; ++t )
    {
        arg[t]= 0.0;

        if ((mBResults[t] != 0.0) && (pTResults[t] != 0.0) )
                b1= mBResults[t] / pTResults[t];
        else
                b1= 0.0;

        for ( g= 0; g <= t; ++g )
        {
            mResult= b1 - g;
            arg[t]+= mResult * mResult;
        }

        if ((mFResults[t] != 0.0) && ((1.0 - pTResults[t]) != 0.0) )
                b2= mFResults[t] * (1.0 - pTResults[t]);
        else
                b2= 0.0;

        for ( g= t+1; g < size; ++g )
        {
            mResult= b2 - g;
            arg[t] += mResult * mResult;
        }
    }

    float tOpt;
    tOpt= arg.getMinIndex();
    tOpt= ((tOpt * (max - min)) / (double) size) + min;

    if ( mask == NULL )
    {
        for ( i= 0; i < input.n; i++ )
            if ( input(i) > tOpt )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
    {
        for ( i= 0; i < input.n; i++ )
            if ( (*mask)(i) > 0 )
                if ( input(i) > tOpt )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
    }
}*/

#endif
