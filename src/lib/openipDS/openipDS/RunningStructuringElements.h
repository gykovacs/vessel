/**
 * @file RunningStructuringElements.h
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
 * This file contains the definition of some special running structuring element.
 */

#ifndef _RUNNINGSTRUCTURINGELEMENTS_H
#define	_RUNNINGSTRUCTURINGELEMENTS_H

#include <openipDS/RunningStructuringElement.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/Histogram.h>

namespace openip
{
    /**
     * RunningStructuringElementMean computes the mean of the elements defined
     * by the StructuringElement parameter in running style
     */
    template<typename INPUT>
    class RunningStructuringElementMean: public RunningStructuringElement<INPUT>
    {
    public:
        using RunningStructuringElement<INPUT>::position;
        using RunningStructuringElement<INPUT>::input;
        using RunningStructuringElement<INPUT>::leftFront;
        using RunningStructuringElement<INPUT>::rightFront;
        using RunningStructuringElement<INPUT>::se;
        using RunningStructuringElement<INPUT>::init;
        using RunningStructuringElement<INPUT>::support;
        using RunningStructuringElement<INPUT>::begin;
        using RunningStructuringElement<INPUT>::end;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base Structuring Element
         */
        RunningStructuringElementMean(StructuringElement2 se);

        /**
         * copy constructor
         * @param rsem instance to copy
         */
        RunningStructuringElementMean(const RunningStructuringElementMean& rsem);

        /**
         * destructor
         */
        ~RunningStructuringElementMean();

        /**
         * computes next mean
         */
        void next();

        /**
         * initializes in position p
         */
        void init(int p);

        virtual double value();

        /**
         * temporary variable, the sum of elements
         */
        double sum;

        /**
         * actual mean
         */
        double mean;

        /**
         * size of the elements
         */
        double size;
    };

    /**
     * RunningStructuringElementMeanAndVariance computes the mean and variance of
     * elements defined by the base structuring element, in running style
     */
    template<typename INPUT>
    class RunningStructuringElementMeanAndVariance: public RunningStructuringElementMean<INPUT>
    {
    public:
        using RunningStructuringElementMean<INPUT>::position;
        using RunningStructuringElementMean<INPUT>::input;
        using RunningStructuringElementMean<INPUT>::leftFront;
        using RunningStructuringElementMean<INPUT>::rightFront;
        using RunningStructuringElementMean<INPUT>::se;
        using RunningStructuringElementMean<INPUT>::init;
        using RunningStructuringElementMean<INPUT>::sum;
        using RunningStructuringElementMean<INPUT>::mean;
        using RunningStructuringElementMean<INPUT>::size;
        using RunningStructuringElementMean<INPUT>::support;
        using RunningStructuringElementMean<INPUT>::initFronts;
        using RunningStructuringElementMean<INPUT>::begin;
        using RunningStructuringElementMean<INPUT>::end;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base structuring element, defining the shape
         */
        RunningStructuringElementMeanAndVariance(StructuringElement2 se);

        /**
         * copy constructor
         * @param rse instance to copy
         */
        RunningStructuringElementMeanAndVariance(const RunningStructuringElementMeanAndVariance& rse);

        /**
         * destructor
         */
        ~RunningStructuringElementMeanAndVariance();

        /**
         * computes the next mean and variance
         */
        void next();

        /**
         * initializes the structuring element in position p
         * @param p position in row-continuous representation
         */
        void init(int p);

        virtual double value();

        /**
         * temporary variable, sum of squares in the structuring element
         */
        double sumSqr;

        /**
         * actual variance
         */
        double variance;
    };

    /**
    * RunningStructuringElementHistogram represents a running object through the image, where
    * the histogram of the underlaying pixels is updated in each step
    */
    template<typename INPUT>
    class RunningStructuringElementHistogram: public RunningStructuringElement<INPUT>
    {
    public:
        using RunningStructuringElement<INPUT>::position;
        using RunningStructuringElement<INPUT>::input;
        using RunningStructuringElement<INPUT>::leftFront;
        using RunningStructuringElement<INPUT>::rightFront;
        using RunningStructuringElement<INPUT>::se;
        using RunningStructuringElement<INPUT>::init;
        using RunningStructuringElement<INPUT>::support;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base Structuring Element
         */
        RunningStructuringElementHistogram(StructuringElement2 se, int numberOfBins= 8);

        /**
         * copy constructor
         * @param rsem instance to copy
         */
        RunningStructuringElementHistogram(const RunningStructuringElementHistogram& rsem);

        /**
         * destructor
         */
        ~RunningStructuringElementHistogram();

        /**
         * computes next mean
         */
        void next();

        /**
         * initializes in position p
         */
        void init(int p);

        virtual double value();

        /**
         * actual histogram
         */
        Histogram h;

        /**
         * size of the elements
         */
        int size;
    };

    /**
    * RunningStructuringElementHistogram represents a running object through the image, where
    * the histogram of the underlaying pixels is updated in each step
    */
    template<typename INPUT>
    class RunningStructuringElementWeightedGradientHistogram: public RunningStructuringElement<INPUT>
    {
    public:
        using RunningStructuringElement<INPUT>::position;
        using RunningStructuringElement<INPUT>::input;
        using RunningStructuringElement<INPUT>::leftFront;
        using RunningStructuringElement<INPUT>::rightFront;
        using RunningStructuringElement<INPUT>::se;
        using RunningStructuringElement<INPUT>::init;
        using RunningStructuringElement<INPUT>::support;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base Structuring Element
         */
        RunningStructuringElementWeightedGradientHistogram(StructuringElement2 se, Image<INPUT>* weights, int numberOfBins= 8);

        /**
         * copy constructor
         * @param rsem instance to copy
         */
        RunningStructuringElementWeightedGradientHistogram(const RunningStructuringElementWeightedGradientHistogram& rsem);

        /**
         * destructor
         */
        ~RunningStructuringElementWeightedGradientHistogram();

        /**
         * computes next mean
         */
        void next();

        /**
         * initializes in position p
         */
        void init(int p);

        void init(Image<INPUT>* input, Image<INPUT>* weights, Image<unsigned char>* support);

        virtual double value();

        /**
         * actual histogram
         */
        Histogram h;

        /**
         * size of the elements
         */
        int size;

        Image<INPUT>* weights;
    };

    /**
    * RunningStructuringElementLocalGamma represents a running structuring element through the image, where
    * the optimal gamma correction factor is computed for each position
    */
    template<typename INPUT>
    class RunningStructuringElementLocalGamma: public RunningStructuringElementHistogram<INPUT>
    {
    public:
        using RunningStructuringElementHistogram<INPUT>::position;
        using RunningStructuringElementHistogram<INPUT>::input;
        using RunningStructuringElementHistogram<INPUT>::leftFront;
        using RunningStructuringElementHistogram<INPUT>::rightFront;
        using RunningStructuringElementHistogram<INPUT>::se;
        using RunningStructuringElementHistogram<INPUT>::init;
        using RunningStructuringElementHistogram<INPUT>::support;
        using RunningStructuringElementHistogram<INPUT>::h;
        using RunningStructuringElementHistogram<INPUT>::size;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base structuring element, defining the shape
         * @param order order of Taylor series expansion
         */
        RunningStructuringElementLocalGamma(StructuringElement2 se);

        /**
         * copy constructor
         * @param rse instance to copy
         */
        RunningStructuringElementLocalGamma(const RunningStructuringElementLocalGamma& rse);

        /**
         * destructor
         */
        ~RunningStructuringElementLocalGamma();

        /**
         * computes the next mean and variance
         */
        void next();

        /**
         * initializes the structuring element in position p
         * @param p position in row-continuous representation
         */
        void init(int p);

        virtual double value();

         /**
         * the actual gamma correction factor
         */
        double gamma;

        Image<float> coeff;
    };

    template<typename INPUT>
    class RunningStructuringElementMin: public RunningStructuringElement<INPUT>
    {
    public:
        using RunningStructuringElement<INPUT>::position;
        using RunningStructuringElement<INPUT>::input;
        using RunningStructuringElement<INPUT>::leftFront;
        using RunningStructuringElement<INPUT>::rightFront;
        using RunningStructuringElement<INPUT>::se;
        using RunningStructuringElement<INPUT>::init;
        using RunningStructuringElement<INPUT>::support;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base structuring element, defining the shape
         * @param order order of Taylor series expansion
         */
        RunningStructuringElementMin(StructuringElement2 se);

        /**
         * copy constructor
         * @param rse instance to copy
         */
        RunningStructuringElementMin(const RunningStructuringElementMin& rse);

        /**
         * destructor
         */
        ~RunningStructuringElementMin();

        /**
         * computes the next mean and variance
         */
        void next();

        /**
         * initializes the structuring element in position p
         * @param p position in row-continuous representation
         */
        void init(int p);

        virtual double value();

        INPUT min;
    };

    template<typename INPUT>
    class RunningStructuringElementMax: public RunningStructuringElement<INPUT>
    {
    public:
        using RunningStructuringElement<INPUT>::position;
        using RunningStructuringElement<INPUT>::input;
        using RunningStructuringElement<INPUT>::leftFront;
        using RunningStructuringElement<INPUT>::rightFront;
        using RunningStructuringElement<INPUT>::se;
        using RunningStructuringElement<INPUT>::init;
        using RunningStructuringElement<INPUT>::support;

        /**
         * constructor, se defines the shape of the structuring element
         * @param se base structuring element, defining the shape
         * @param order order of Taylor series expansion
         */
        RunningStructuringElementMax(StructuringElement2 se);

        /**
         * copy constructor
         * @param rse instance to copy
         */
        RunningStructuringElementMax(const RunningStructuringElementMax& rse);

        /**
         * destructor
         */
        ~RunningStructuringElementMax();

        /**
         * computes the next mean and variance
         */
        void next();

        /**
         * initializes the structuring element in position p
         * @param p position in row-continuous representation
         */
        void init(int p);

        virtual double value();

        INPUT max;
    };

    template<typename INPUT>
    RunningStructuringElementHistogram<INPUT>::RunningStructuringElementHistogram(StructuringElement2 se, int numberOfBins)
    : RunningStructuringElement<INPUT>(se), size(se.size())
    {
        this->size= numberOfBins;
    }

    template<typename INPUT>
    RunningStructuringElementHistogram<INPUT>::RunningStructuringElementHistogram(const RunningStructuringElementHistogram& rseh)
    : RunningStructuringElement<INPUT>(rseh), h(rseh.h), size(rseh.size)
    {
    }

    template<typename INPUT>
    RunningStructuringElementHistogram<INPUT>::~RunningStructuringElementHistogram()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementHistogram<INPUT>::value()
    {
        return 0.0;
    }

    template<typename INPUT>
    void RunningStructuringElementHistogram<INPUT>::init(int p)
    {
        h.clear();
        INPUT min, max;
        input->getMinMax(min, max, this->support);
        h.setParameters(min, max, this->size);
        h= 0;

        if ( support == NULL )
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                //h((*input)(p + *sit))+= 1;
                h.get((*input)(p+*sit))+= 1;
            }
        }
        else
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    //h((*input)(p + *sit))+= 1;
                    h.get((*input)(p+*sit))+= 1;
                }
            }
        }

        position= p;
    }

    template<typename INPUT>
    void RunningStructuringElementHistogram<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                //h((*input)(position + *pit))-= 1;
                h.get((*input)(position + *pit))-= 1;
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                //h((*input)(position + *pit))+= 1;
                h.get((*input)(position + *pit))+= 1;

        }
        else
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    //h((*input)(position + *pit))-= 1;
                    h.get((*input)(position + *pit))-= 1;
                }
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    //h((*input)(position + *pit))+= 1;
                    h.get((*input)(position + *pit))+= 1;
                }
            }

        }
        position++;
    }

    template<typename INPUT>
    RunningStructuringElementWeightedGradientHistogram<INPUT>::RunningStructuringElementWeightedGradientHistogram(StructuringElement2 se, Image<INPUT>* weights, int numberOfBins)
    : RunningStructuringElement<INPUT>(se), size(se.size())
    {
        this->size= numberOfBins;
        this->weights= weights;
    }

    template<typename INPUT>
    RunningStructuringElementWeightedGradientHistogram<INPUT>::RunningStructuringElementWeightedGradientHistogram(const RunningStructuringElementWeightedGradientHistogram& rseh)
    : RunningStructuringElement<INPUT>(rseh), h(rseh.h), size(rseh.size)
    {
    }

    template<typename INPUT>
    RunningStructuringElementWeightedGradientHistogram<INPUT>::~RunningStructuringElementWeightedGradientHistogram()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementWeightedGradientHistogram<INPUT>::value()
    {
        return 0.0;
    }


    template<typename INPUT>
    void RunningStructuringElementWeightedGradientHistogram<INPUT>::init(Image<INPUT> *input, Image<INPUT> *weights, Image<unsigned char> *support)
    {
        this->init(input, support);
        this->weights= weights;
    }

    template<typename INPUT>
    void RunningStructuringElementWeightedGradientHistogram<INPUT>::init(int p)
    {
        h.clear();
        INPUT min, max;
        input->getMinMax(min, max, this->support);
        h.setParameters(min, max, this->size);
        h= 0;

        if ( support == NULL )
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                //h((*input)(p + *sit))+= 1;
                h.get((*input)(p+*sit))+= (*weights)(p+*sit);
            }
        }
        else
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    //h((*input)(p + *sit))+= 1;
                    h.get((*input)(p+*sit))+= (*weights)(p+*sit);
                }
            }
        }

        position= p;
    }

    template<typename INPUT>
    void RunningStructuringElementWeightedGradientHistogram<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                //h((*input)(position + *pit))-= 1;
                h.get((*input)(position + *pit))-= (*weights)(position+*pit);
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                //h((*input)(position + *pit))+= 1;
                h.get((*input)(position + *pit))+= (*weights)(position+*pit);

        }
        else
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    //h((*input)(position + *pit))-= 1;
                    h.get((*input)(position + *pit))-= (*weights)(position+*pit);
                }
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    //h((*input)(position + *pit))+= 1;
                    h.get((*input)(position + *pit))+= (*weights)(position+*pit);
                }
            }

        }
        position++;
    }

    template<typename INPUT>
    RunningStructuringElementMean<INPUT>::RunningStructuringElementMean(StructuringElement2 se)
    : RunningStructuringElement<INPUT>(se), sum(0), mean(0)
    {
        this->size= se.size();
//        this->se= se;
    }

    template<typename INPUT>
    RunningStructuringElementMean<INPUT>::RunningStructuringElementMean(const RunningStructuringElementMean& rsem)
    : RunningStructuringElement<INPUT>(rsem), sum(rsem.sum), mean(rsem.mean), size(rsem.size)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMean<INPUT>::~RunningStructuringElementMean()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementMean<INPUT>::value()
    {
        return mean;
    }

    template<typename INPUT>
    void RunningStructuringElementMean<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                sum-= (*input)(position + *pit);
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                sum+= (*input)(position + *pit);

        }
        else
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    sum-= (*input)(position + *pit);
                    --size;
                }
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    sum+= (*input)(position + *pit);
                    ++size;
                }
            }
        }
        if ( size > 0 )
            mean= sum / size;
        else
            mean= 0;
        
        ++position;
    }

    template<typename INPUT>
    void RunningStructuringElementMean<INPUT>::init(int p)
    {
        sum= 0;

        if ( support == NULL )
        {
            size= se.size();
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                sum+= (*input)(p + *sit);
            }
        }
        else
        {
            size= 0;
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    sum+= (*input)(p + *sit);
                    size+= 1;
                }
            }
        }

        if ( size != 0 )
            mean= sum / size;
        else
            mean= 0;
        position= p;
    }

    template<typename INPUT>
    RunningStructuringElementMeanAndVariance<INPUT>::RunningStructuringElementMeanAndVariance(StructuringElement2 se)
    : RunningStructuringElementMean<INPUT>(se), sumSqr(0), variance(0)
    {
        this->se= se;
    }

    template<typename INPUT>
    RunningStructuringElementMeanAndVariance<INPUT>::RunningStructuringElementMeanAndVariance(const RunningStructuringElementMeanAndVariance& rsem)
    : RunningStructuringElementMean<INPUT>(rsem), sumSqr(rsem.sumSqr), variance(rsem.variance)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMeanAndVariance<INPUT>::~RunningStructuringElementMeanAndVariance()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementMeanAndVariance<INPUT>::value()
    {
        return variance;
    }

    template<typename INPUT>
    void RunningStructuringElementMeanAndVariance<INPUT>::next()
    {
        double tmp;

        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                tmp= (*input)(position + *pit);
                sum-= tmp;
                sumSqr-= tmp * tmp;
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                tmp= (*input)(position + *pit);
                sum+= tmp;
                sumSqr+= tmp * tmp;
            }
        }
        else
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    tmp= (*input)(position + *pit);
                    sum-= tmp;
                    sumSqr-= tmp*tmp;
                    --size;
                }
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    tmp= (*input)(position + *pit);
                    sum+= tmp;
                    sumSqr+= tmp*tmp;
                    ++size;
                }
            }
        }
        if ( size != 0 )
        {
            mean= sum / size;
            variance= (sumSqr - sum * mean)/size;
        }
        else
        {
            mean= variance= 0;
        }
        ++position;
    }

    template<typename INPUT>
    void RunningStructuringElementMeanAndVariance<INPUT>::init(int p)
    {
        double tmp;
        sum= 0;
        sumSqr= 0;

        if ( support == NULL )
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                tmp= (*input)(p + *sit);
                sum+= tmp;
                sumSqr+= tmp*tmp;
            }
        }
        else
        {
            size= 0;
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    tmp= (*input)(p + *sit);
                    sum+= tmp;
                    sumSqr+= tmp*tmp;
                    ++size;
                }
            }
        }

        if ( size != 0 )
        {
            mean= sum / size;
            variance= (sumSqr - sum*mean)/size;
        }
        else
        {
            mean= variance= 0;
        }
        position= p;
    }

    template<typename INPUT>
    RunningStructuringElementLocalGamma<INPUT>::RunningStructuringElementLocalGamma(StructuringElement2 se)
    : RunningStructuringElementHistogram<INPUT>(se)
    {
        coeff.resizeImage(256, 256);
        for ( int i= 0; i < 256; ++i )
            for ( int j= 0; j < 256; ++j )
            {
                coeff(i,j)= (-(log(log((i+1)/257.0)/log((j+1)/257.0)))/(log((i+1)/257.0)-log((j+1)/257.0)));
            }
    }

    template<typename INPUT>
    RunningStructuringElementLocalGamma<INPUT>::RunningStructuringElementLocalGamma(const RunningStructuringElementLocalGamma& rseh)
    : RunningStructuringElementHistogram<INPUT>(rseh)
    {
    }

    template<typename INPUT>
    RunningStructuringElementLocalGamma<INPUT>::~RunningStructuringElementLocalGamma()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementLocalGamma<INPUT>::value()
    {
        return gamma;
    }

    template<typename INPUT>
    void RunningStructuringElementLocalGamma<INPUT>::init(int p)
    {
        h.clear();
        h.resize(256);
        h= 0;

        if ( support == NULL )
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( p + *sit >= 0 && p + *sit < (int)(input->n) )
                h((*input)(p + *sit))+= 1;
            }
        }
        else
        {
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    h((*input)(p + *sit))+= 1;
                }
            }
        }

        gamma= 0;
        float sum= 0;
        for ( unsigned int i= 0; i < h.size(); ++i )
        {
            for ( unsigned int j= 0; j < h.size(); ++j )
            {
                if ( i != j )
                {
                    gamma+= h(i)*h(j)*coeff(i,j);
                    sum+= h(i)*h(j);
                }
            }
        }
        gamma/= sum;

        position= p;
    }

    template<typename INPUT>
    void RunningStructuringElementLocalGamma<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                if ( position + *pit >= 0 && position + *pit < int(input->n) )
                h((*input)(position + *pit))-= 1;
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                if ( position + *pit >= 0 && position + *pit < int(input->n) )
                h((*input)(position + *pit))+= 1;

        }
        else
        {
            PixelSet1::iterator pit;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    h((*input)(position + *pit))-= 1;
                }
            }
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
            {
                if ( (*support)(position + *pit) > 0 )
                {
                    h((*input)(position + *pit))+= 1;
                }
            }
        }

        gamma= 0;
        float sum= 0;
        for ( unsigned int i= 0; i < h.size(); ++i )
        {
            for ( unsigned int j= 0; j < h.size(); ++j )
            {
                if ( i != j )
                {
                    gamma+= h(i)*h(j)*coeff(i,j);
                    sum+= h(i)*h(j);
                }
            }
        }
        gamma/= sum;

        position++;
    }

    template<typename INPUT>
    RunningStructuringElementMin<INPUT>::RunningStructuringElementMin(StructuringElement2 se)
    : RunningStructuringElement<INPUT>(se)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMin<INPUT>::RunningStructuringElementMin(const RunningStructuringElementMin& rsem)
    : RunningStructuringElement<INPUT>(rsem)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMin<INPUT>::~RunningStructuringElementMin()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementMin<INPUT>::value()
    {
        return min;
    }

    template<typename INPUT>
    void RunningStructuringElementMin<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            char flag= 0;
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                if ( (*input)(position + *pit) < min )
                {
                    min= (*input)(position + *pit);
                    flag= 1;
                }
            if ( flag == 1 )
            {
                ++position;
                return;
            }
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                if ( (*input)(position + *pit) <= min )
                {
                    min= (*input)(position + *(rightFront.begin()));
                    break;
                }
            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*input)(position + 1 + *pit) < min )
                    min= (*input)(position + 1 + *pit);

        }
        else
        {
            PixelSet1::iterator pit;
            char flag= 0;
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                if ( (*support)(position + *pit ) > 0 )
                    if ( (*input)(position + *pit) < min )
                    {
                        min= (*input)(position + *pit);
                        flag= 1;
                    }
            if ( flag == 1 )
            {
                ++position;
                return;
            }
            flag= 0;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                if ( (*support)(position + *pit) > 0 )
                    if ( (*input)(position + *pit) <= min )
                    {
                        flag= 1;
                        break;
                    }
            if ( flag == 0 )
            {
                ++position;
                return;
            }

            flag= 0;
            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*support)(position + 1 + *pit) > 0 )
                {
                    min= (*input)(position + 1 + *pit);
                    flag= 1;
                    break;
                }

            if ( flag == 0 )
                min= (INPUT)(FLT_MAX);
            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*support)(position + 1 + *pit) > 0 )
                {
                    if ( (*input)(position + 1 + *pit) < min )
                    {
                        min= (*input)(position + 1 + *pit);
                    }
                }
        }

        ++position;
    }

    template<typename INPUT>
    void RunningStructuringElementMin<INPUT>::init(int p)
    {
        min= (INPUT)(FLT_MAX);
        if ( support == NULL )
        {
            min= (*input)(p + *(se.begin()));
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*input)(p + *sit) < min )
                    min= (*input)(p + *sit);
            }
        }
        else
        {
            char flag= 0;
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    if ( flag == 0 )
                    {
                        min= (*input)(p + *sit);
                        flag= 1;
                    }
                    if ( (*input)(p + *sit) < min )
                        min= (*input)(p + *sit);
                }
            }
        }

        position= p;
    }

    template<typename INPUT>
    RunningStructuringElementMax<INPUT>::RunningStructuringElementMax(StructuringElement2 se)
    : RunningStructuringElement<INPUT>(se)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMax<INPUT>::RunningStructuringElementMax(const RunningStructuringElementMax& rsem)
    : RunningStructuringElement<INPUT>(rsem)
    {
    }

    template<typename INPUT>
    RunningStructuringElementMax<INPUT>::~RunningStructuringElementMax()
    {
    }

    template<typename INPUT>
    double RunningStructuringElementMax<INPUT>::value()
    {
        return this->max;
    }

    template<typename INPUT>
    void RunningStructuringElementMax<INPUT>::next()
    {
        if ( support == NULL )
        {
            PixelSet1::iterator pit;
            char flag= 0;
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                if ( (*input)(position + *pit) > max )
                {
                    max= (*input)(position + *pit);
                    flag= 1;
                }
            if ( flag == 1 )
            {
                ++position;
                return;
            }
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                if ( (*input)(position + *pit) == max )
                {
                    max= (*input)(position + *(rightFront.begin()));
                    break;
                }
            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*input)(position + 1 + *pit) > max )
                    max= (*input)(position + 1 + *pit);

        }
        else
        {
            PixelSet1::iterator pit;
            char flag= 0;
            for ( pit= rightFront.begin(); pit != rightFront.end(); ++pit )
                if ( (*support)(position + *pit ) > 0 )
                    if ( (*input)(position + *pit) > max )
                    {
                        max= (*input)(position + *pit);
                        flag= 1;
                    }
            if ( flag == 1 )
            {
                ++position;
                return;
            }
            flag= 0;
            for ( pit= leftFront.begin(); pit != leftFront.end(); ++pit )
                if ( (*support)(position + *pit) > 0 )
                    if ( (*input)(position + *pit) == max )
                    {
                        flag= 1;
                        break;
                    }
            if ( flag == 0 )
            {
                ++position;
                return;
            }

            flag= 0;
            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*support)(position + 1 + *pit) > 0 )
                {
                    max= (*input)(position + 1 + *pit);
                    flag= 1;
                    break;
                }
            if ( flag == 0 )
                max= (INPUT)(-FLT_MAX);

            for ( pit= se.begin(); pit != se.end(); ++pit )
                if ( (*support)(position + 1 + *pit) > 0 )
                    if ( (*input)(position + 1 + *pit) > max )
                        max= (*input)(position + 1 + *pit);
        }

        ++position;
    }

    template<typename INPUT>
    void RunningStructuringElementMax<INPUT>::init(int p)
    {
        max= -FLT_MAX;
        if ( support == NULL )
        {
            max= (*input)(p + *(se.begin()));
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*input)(p + *sit) > max )
                    max= (*input)(p + *sit);
            }
        }
        else
        {
            char flag= 0;
            for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
            {
                if ( (*support)(p + *sit) > 0 )
                {
                    if ( flag == 0 )
                    {
                        max= (*input)(p + *sit);
                        flag= 1;
                    }
                    if ( (*input)(p + *sit) > max )
                        max= (*input)(p + *sit);
                }
            }
        }

        position= p;
    }
}

#endif	/* _RUNNINGSTRUCTURINGELEMENTS_H */

