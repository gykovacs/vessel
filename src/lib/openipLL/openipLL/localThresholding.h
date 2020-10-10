/**
 * @file localThresholding.h
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
 * This file contains local adaptive thresholding methods. Based on the
 * following paper:
 *
 * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
 * and quantitative performance evaluation, Journal of Electronic Imaging
 * 13(1), 146 -- 165 (January 2004)
 */

#ifndef _LOCALTHRESHOLDING_H
#define	_LOCALTHRESHOLDING_H

#ifdef AAA

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>

#include <openipLL/Interpolation.h>

namespace openip
{
    /**
     * Niblack' local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param k method parameter
     * @param b method parameter
     */
    template<typename INPUT>
    void thresholdingLocalNiblack(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, float k= -0.2, int b= 15);

    /**
     * Sauvola's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param k method parameter
     * @param b method parameter (window size)
     * @param R method parameter
     */
    template<typename INPUT>
    void thresholdingLocalSauvola(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, float k= 0.5, int b= 15, float R= 128);

    /**
     * White's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param w method parameter (window size)
     * @param bias method parameter
     */
    template<typename INPUT>
    void thresholdingLocalWhite(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, int w= 15, float bias= 2 );

    /**
     * Bernsen's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param w method parameter (window size)
     * @param contrast method parameter (contrast threshold)
     */
    template<typename INPUT>
    void thresholdingLocalBernsen(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, int w= 31, int contrast= 15);

    /**
     * Palumbo's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param t1 method parameter
     * @param t2 method parameter
     * @param t3 method parameter
     * @param t4 method parameter
     * @param t5 method parameter
     */
    template<typename INPUT>
    void thresholdingLocalPalumbo(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, float t1= 20, float t2= 20, float t3= 0.85, float t4= 1.0, float t5= 0);

    /**
     * Yanowitz's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param c cache object to increase speed
     * @param mask the thresholding will be applied in the foreground (non 0) region
     */
    /*template<typename INPUT>
    void thresholdingLocalYanowitz(Image<INPUT>& input, Image<unsigned char>& output, Cache2* c, Image<unsigned char>* mask= NULL);*/

    /**
     * Kamel's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0)  region
     * @param w window size
     * @param t0 threshold
     * @param b method parameter
     */
    template<typename INPUT>
    void thresholdingLocalKamel(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, int w= 17, int t0= 40, int b= 3);

    /**
     * Oh's local thresholding method
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param mask the thresholding will be applied in the foreground (non 0) region
     */
    template<typename INPUT>
    void thresholdingLocalOh(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

    /**
     * Yasuda's local thresholding
     * for details check:
     * Mehmet Sezgin, Bülent Sankur: Survey over image thresholding techniques
     * and quantitative performance evaluation, Journal of Electronic Imaging
     * 13(1), 146 -- 165 (January 2004)
     * @param input input image
     * @param output output image
     * @param c Cache2 object pointer
     * @param mask the thresholding will be applied in the foreground (non 0) region
     * @param w window size
     * @param t1 method parameter
     * @param t2 method parameter
     * @param t3 method parameter
     * @param t4 method parameter
     */
    /*template<typename INPUT>
    void thresholdingLocalYasuda(Image<INPUT>& input, Image<unsigned char>& output, Cache2* c, Image<unsigned char>* mask= NULL, int w= 31, int t1= 50, int t2= 16, int t3= 128, int t4= 35);*/
}

/*template<typename INPUT>
void openip::thresholdingLocalNiblack(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask, float k, int b)
{
    RunningStructuringElementMeanAndVariance<INPUT> rse(new StructuringElementSquare(b));

    rse.init(&input);

    if ( mask == NULL )
    {
        output= BACKGROUND;
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( input(i) > rse.mean + k * sqrt(rse.variance) )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;

            rse.next();
        }
    }
    else
    {
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                if ( i > 0 && (*mask)(i-1) == 0 )
                    rse.init(i);
                else
                    rse.next();

                if ( (input)(i) > rse.mean + k * sqrt(rse.variance) )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
        }
    }
}

template<typename INPUT>
void openip::thresholdingLocalSauvola(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask, float k, int b, float R)
{
    RunningStructuringElementMeanAndVariance<INPUT> rse(new StructuringElementSquare(b));

    rse.init(&input);

    if ( mask == NULL )
    {
        output= BACKGROUND;
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( input(i) > rse.mean + 1 + k * (sqrt(rse.variance) / R - 1) )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;

            rse.next();
        }
    }
    else
    {
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                if ( i > 0 && (*mask)(i-1) == 0 )
                    rse.init(i);
                else
                    rse.next();

                if ( (input)(i) > rse.mean + 1 + k * (sqrt(rse.variance) / R - 1)  )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
        }
    }
}

template<typename INPUT>
void openip::thresholdingLocalWhite(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask, int w, float bias)
{
    RunningStructuringElementMean<INPUT> rse(new StructuringElementSquare(w));

    rse.init(&input);

    if ( mask == NULL )
    {
        output= BACKGROUND;
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( bias * input(i) > rse.mean )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;

            rse.next();
        }
    }
    else
    {
        for ( int i= rse.begin; i < rse.end; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                if ( i > 0 && (*mask)(i-1) == 0 )
                    rse.init(i);
                else
                    rse.next();

                if ( bias * input(i) > rse.mean )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
        }
    }
}

template<typename INPUT>
void openip::thresholdingLocalBernsen(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask, int w, int contrast)
{
    StructuringElementSquare se(w);
    se.updateStride(input.columns);
    INPUT min, max;

    for ( int i= -se.min; i < input.n - se.max; ++i )
        output(i)= 2;

    if ( mask == NULL )
    {
        for ( int i= -se.min; i < input.n - se.max; ++i )
        {
            if ( output(i) == 2 )
            {
                se.getMinMax(input, i, min, max);
                if ( (max - min) < contrast )
                {
                    if ( input(i) > (max + min) / 2 )
                    {
                        for ( StructuringElementSquare::iterator sit= se.begin(); sit != se.end(); ++sit )
                            if ( output(i + *sit) == 2 )
                                output(i + *sit)= FOREGROUND;
                    }
                    else
                    {
                        for ( StructuringElementSquare::iterator sit= se.begin(); sit != se.end(); ++sit )
                            if ( output(i + *sit) == 2 )
                                output(i + *sit)= BACKGROUND;
                    }
                }
                else
                {
                    if ( input(i) > (max + min) / 2 )
                        output(i)= FOREGROUND;
                    else
                        output(i)= BACKGROUND;
                }
            }
        }
    }
    else
    {
        for ( int i= -se.min; i < input.n - se.max; ++i )
        {
            if ( (*mask)(i) && output(i) == 2 )
            {
                se.getMinMax(input, i, min, max);
                if ( (max - min) < contrast )
                {
                    if ( input(i) > (max + min) / 2 )
                    {
                        for ( StructuringElementSquare::iterator sit= se.begin(); sit != se.end(); ++sit )
                            if ( output(i + *sit) == 2 )
                                output(i + *sit)= FOREGROUND;
                    }
                    else
                    {
                        for ( StructuringElementSquare::iterator sit= se.begin(); sit != se.end(); ++sit )
                            if ( output(i + *sit) == 2 )
                                output(i + *sit)= BACKGROUND;
                    }
                }
                else
                {
                    if ( input(i) > (max + min) / 2 )
                        output(i)= FOREGROUND;
                    else
                        output(i)= BACKGROUND;
                }
            }
            else
                (*mask)(i)= BACKGROUND;
        }
    }

    for ( int i= 0; i < -se.min; ++i )
        output(i)= BACKGROUND;
    for ( int i= input.n - se.max; i < input.n; ++i )
        output(i)= BACKGROUND;
}

template<typename INPUT>
void openip::thresholdingLocalPalumbo(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, float t1= 20, float t2= 20, float t3= 0.85, float t4= 1.0, float t5= 0)
{
    StructuringElement2Square Acenter(3);
    Acenter.updateStride(input.columns);

    StructuringElement2 Aneighbour;
    Aneighbour.updateStride(input.columns);

    for ( StructuringElementSquare::iterator sit= Acenter.begin(); sit != Acenter.end(); ++sit )
    {
        Aneighbour.push_back(*sit - 3 * Acenter.stride - 3);
        Aneighbour.push_back(*sit + 3 * Acenter.stride - 3);
        Aneighbour.push_back(*sit - 3 * Acenter.stride + 3);
        Aneighbour.push_back(*sit + 3 * Acenter.stride + 3);
    }

    Aneighbour.computeMinMax();

    if ( mask == NULL )
        #pragma omp parallel for
        for ( int i= - Aneighbour.min; i < input.n - Aneighbour.max; ++i )
            if ( input(i) > t1 || Aneighbour.getMean(input, i) * t3 + t5 < Acenter.getMean(input, i) * t4 )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    else
        #pragma omp parallel for
        for ( int i= - Aneighbour.min; i < input.n - Aneighbour.max; ++i )
            if ( (*mask)(i) > 0)
            {
                if ( input(i) > t1 || (Aneighbour.getMean(input, i) * t3 + t5) < Acenter.getMean(input, i) * t4 )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
}

template<typename INPUT>
void openip::thresholdingLocalYanowitz(Image<INPUT>& input, Image<unsigned char>& output, Cache2* c, Image<unsigned char>* mask= NULL)
{
    SobelFilter<unsigned char, float> sf(input.columns);
    Image<float>* magn;
    c->get(&magn);

    sf.applyMagnitude(input, *magn);

    Histogram h;

    float min, max;
    magn->getMinMax(min, max);

    h.computeHistogram(magn, 256, min, max);

    int sum= 0;
    int th;
    for ( th= 255; th >= 0; --th )
    {
        sum+= h(th);
        if ( sum > input.n * 0.05 )
            break;
    }

    float th1= h.indexToValue(th);

    if ( mask == NULL )
        for ( int i= 0; i < magn->columns; ++i )
            for ( int j= 0; j < magn->rows; ++j )
                if ( (*magn)(j, i) > th1 )
                {
                    output(j, i)= FOREGROUND;
                    (*magn)(j, i)= input(j, i);
                }
                else if ( i == 0 || j == 0 || i == output.columns - 1 || j == output.rows - 1 )
                {
                    output(j, i)= FOREGROUND;
                    (*magn)(j, i)= input(j, i);
                }
                else
                {
                    output(j, i)= BACKGROUND;
                    (*magn)(j, i)= 0;
                }
    else
        for ( int i= 0; i < magn->columns; ++i )
            for ( int j= 0; j < magn->rows; ++j )
                if ( (*mask)(j, i) > 0 && (*magn)(j, i) > th1 )
                {
                    output(j, i)= FOREGROUND;
                    (*magn)(j, i)= input(j, i);
                }
                else if ( (*mask)(j-1, i) == 0 || (*mask)(j+1, i) == 0 || (*mask)(j-1, i-1) == 0 || (*mask)(j+1, i-1) == 0 || (*mask)(j-1, i+1) == 0 || (*mask)(j+1, i+1) == 0 || (*mask)(j, i-1) == 0 || (*mask)(j, i+1) == 0 )
                {
                    output(j, i)= FOREGROUND;
                    (*magn)(j, i)= input(j, i);
                }
                else
                {
                    output(j, i)= BACKGROUND;
                    (*magn)(j, i)= 0;
                }


    interpolationLaplace(*magn, output, 1, 1000, mask);

    if ( mask == NULL )
    {
        for ( int i= 0; i < magn->n; ++i )
            if ( input(i) > output(i) )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
    }
    else
        for ( int i= 0; i < magn->n; ++i )
            if ( (*mask)(i) > 0 )
            {
                if ( input(i) > output(i) )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
}

template<typename INPUT>
void openip::thresholdingLocalKamel(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL, int w= 17, int t0= 40, int b= 3)
{
    StructuringElementSquare* ses= new StructuringElementSquare(w);
    RunningStructuringElementMean<INPUT> rseA(ses);
    rseA.init(&input);
    RunningStructuringElementMean<INPUT> rseB(ses);
    rseB.init(&input);
    RunningStructuringElementMean<INPUT> rseC(ses);
    rseC.init(&input);
    RunningStructuringElementMean<INPUT> rseD(ses);
    rseD.init(&input);
    RunningStructuringElementMean<INPUT> rseE(ses);
    rseE.init(&input);
    RunningStructuringElementMean<INPUT> rseF(ses);
    rseF.init(&input);
    RunningStructuringElementMean<INPUT> rseG(ses);
    rseG.init(&input);
    RunningStructuringElementMean<INPUT> rseH(ses);
    rseH.init(&input);

    int center= (w / 2 + b) * input.columns + w / 2 + b;

    rseA.init(center - b*input.columns);
    rseB.init(center + b*input.columns);
    rseC.init(center - b);
    rseD.init(center + b);
    rseE.init(center - b*input.columns + b);
    rseF.init(center + b*input.columns - b);
    rseG.init(center - b*input.columns - b);
    rseH.init(center + b*input.columns + b);

    while( rseH.position != rseH.end )
    {
        if ( !mask || (*mask)(rseH.position) > 0 )
        {
            if ( ( (rseA.mean > t0 && rseB.mean > t0) || (rseC.mean > t0 && rseD.mean > t0 ) ) ||
                    (rseE.mean > t0 && rseF.mean > t0) || (rseG.mean > t0 && rseH.mean > t0 ) )
                output(center)= FOREGROUND;
            else
                output(center)= BACKGROUND;
        }
        ++center;

        rseA.next();
        rseB.next();
        rseC.next();
        rseD.next();
        rseE.next();
        rseF.next();
        rseG.next();
        rseH.next();
    }
}

template<typename INPUT>
void openip::thresholdingLocalOh(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    ///TODO
}*/

/*template<typename INPUT>
void openip::thresholdingLocalYasuda(Image<INPUT>& input, Image<unsigned char>& output, Cache2* c, Image<unsigned char>* mask, int w, int t1, int t2, int t3, int t4)
{
    StructuringElementSquare ses(w, input.columns);
    Image<INPUT>* tmp;
    c->get(&tmp);

    #pragma omp parallel for
    for ( int i= -ses.min; i < input.n - ses.max; ++i )
    {
        INPUT min, max;
        ses.getMinMax(input, i, min, max);
        if ( max - min < t1 )
            (*tmp)(i)= ses.getMean(input, i);
    }

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( int i= -ses.min; i < input.n - ses.max; ++i )
        {
            float mean, var;
            INPUT min, max;

            ses.getMinMax((*tmp), i, min, max);
            ses.getMeanAndVariance((*tmp), i, mean, var);

            if ( max - min < t2 || (*tmp)(i) < mean )
            {
                output(i)= BACKGROUND;
                continue;
            }

            if ( mean > t3 || var > t4 )
                output(i)= FOREGROUND;
            else
                output(i)= BACKGROUND;
        }
    }
    else
    {
        #pragma omp parallel for
        for ( int i= -ses.min; i < input.n - ses.max; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                float mean, var;
                INPUT min, max;

                ses.getMinMax((*tmp), i, min, max);
                ses.getMeanAndVariance((*tmp), i, mean, var);

                if ( max - min < t2 || (*tmp)(i) < mean )
                {
                    output(i)= BACKGROUND;
                    continue;
                }

                if ( mean < t3 || var > t4 )
                    output(i)= FOREGROUND;
                else
                    output(i)= BACKGROUND;
            }
        }
    }
    c->put(tmp);
}*/

#endif
#endif	/* _LOCALTHRESHOLDING_H */

