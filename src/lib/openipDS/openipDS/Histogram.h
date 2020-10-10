/**
 * @file Histogram.h
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
 * The Histogram class represents the statistical histogram of a Vector object
 */

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

/** 
* function parameter for Historam members
*/
#define HISTOGRAM_LESS_OR_EQUAL 1

/**
* function parameter for Histogram members
*/
#define HISTOGRAM_GREATER 2

#include <math.h>
#include <stdlib.h>

#include <openipDS/Image.h>
#include <openipDS/Vector.h>
#include <openipDS/VectorN.h>
#include <openipDS/StructuringElement2.h>

namespace openip
{
    /**
    * Peak class representing one peak of 1d histograms for
    * histogram analysis
    */
    class Peak
    {
    public:

	/**
	* default constructor
	*/
        Peak();

	/**
	* copy constructor
	*/
        Peak(const Peak& p);

	/**
	* constructor
	* @param start start index of the peak
	* @param end end index of the peak
	* @param maximum maximum of the peak
	*/
        Peak(int start, int end, int maximum);

	/**
	* destructor
	*/
        ~Peak();

	/**
	* comparison operator
	* @param p Peak to compare with
	* @return true if equals, false if not equals
	*/
        bool operator == (const Peak& p);

	/**
	* comparison operator
	* @param p Peak to compare with
	* @return true if not equals, false if equals
	*/
        bool operator != (const Peak& p);

	/**
	* comparison operator
	* @param p Peak to compare with
	* @return true if this is smaller or equals, else false
	*/
        bool operator <= (const Peak& p);

	/**
	* comparison operator
	* @param p Peak to compare with
	* @return true if this is greater or equals, else false
	*/
        bool operator >= (const Peak& p);

	/**
	* getter method
	* @return start index of the peak
	*/
        int getStart() const;

	/**
	* getter method
	* @return end index of the peak
	*/
        int getEnd() const;

	/**
	* getter method
	* @return maximum of the peak
	*/
        int getMaximum() const;

    private:
	/**
	* start index of the peak
	*/
        int start;
        
        /**
        * end index of the peak
        */
        int end;
        
        /**
        * maximum of the peak
        */
        int maximum;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param p Peak object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const Peak& p);

    /**
     * Histogram of a Vector object
     */
    class Histogram : public Vector<float>
    {
    public:
        using Vector<float>::operator=;

        /**
         * default constructor
         */
        Histogram();
        
        /**
         * copy constructor
         */ 
        Histogram(const Histogram& h);

        Histogram(int bins);

        /**
         * constructor with initial paramteres
         * @param v Vector object to compute the histogram of
         * @param numberOfBins the number of bins in the histogram
         * @param min histogram value
         * @param max histogram value
         * @param mask if not NULL, the histogram will be computed only in 
         * the region covered by the mask
         */
        template<typename T>
        Histogram(Vector<T>* v, int numberOfBins, T min= 0, T max= 255, Vector<unsigned char>* mask= NULL);
        
        /**
         * destructor
         */ 
        ~Histogram();

        /**
         * returns the value of the appropriate bin as lvalue, transforming x: (x-min)*(max - min)
         * @param x element of a vector or image
         */
        template<typename T>
        float& get(T x);

        /**
         * returns the value of the appropriate bin as rvalue, transforming x: (x-min)*(max - min)
         * @param x element of a vector or image
         */
        template<typename T>
        float get(T x) const;

        /**
         * returns the histogram index of the value from the data domain
         * @param x
         * @return the histogram index of the value x from the data domain
         */
        template<typename T>
        int getIndex(T x);

        template<typename T>
        void setParameters(T min, T max, int numberOfBins);

        template<typename T>
        void computeHistogram(Vector<T>* v, int n, StructuringElement2* se, Vector<unsigned char>* mask= NULL);

        /**
         * computes the histogram of the Vector objects in the region covered by
         * the foreground (non 0) of the mask
         * @param v Vector object to compute the histogram of
         * @param numberOfBins the number of bins in the histogram
         * @param min left border of the range
         * @param max right border of the range
         * @param mask if not NULL, the histogram will be computed only in the
         * region covered by the mask
         */
        template<typename T>
        void computeHistogram(Vector<T>* v, int numberOfBins, T min= 0, T max= 255, Vector<unsigned char>* mask= NULL);

        template<typename T>
        void computeWeightedHistogram(Vector<T>* v, Vector<T>* w, int numberOfBins, T min= 0, T max= 255, Vector<unsigned char>* mask= NULL);

        /**
         * computes the histogram of the parameter vector in a given range
         * @param v parameter vector
         * @param dimension dimension of the parameter
         * @param numberOfBins number of binst
         * @param min left border of the range
         * @param max right border of the range
         * @param mask if not NULL, the histogram will be computed only in the region covered by the non-zero elements of the mask
         */
        template<typename T, int SIZE>
        void computeHistogramInLimits(Vector<VectorN<T, SIZE> >* v, int dimension, int numberOfBins, T min= 0, T max= 255, Vector<unsigned char>* mask= NULL);

        /**
         * computes the histogram of the parameter vector
         * @param v parameter vector
         * @param dimension dimension of the parameter
         * @param numberOfBins number of bins
         * @param mask if not NULL, the histogram will be computed only in the region covered by the non-zero elements of the mask
         */
        template<typename T, int SIZE>
        void computeHistogram(Vector<VectorN<T, SIZE> >* v, int dimension, int numberOfBins, Vector<unsigned char>* mask= NULL);

        /**
         * normalizes the histogram to 1
         */ 
        void normalize();

        /**
         * normalizes the histogram to 1
         */
        void normalize(Histogram& hN, unsigned int t, unsigned int min= 0, unsigned int max= 255);
        
        /**
         * accumulates the histogram
         */ 
        void accumulate();
        
        /**
         * computes the mean of a part of the histogram, depending on the mode 
         * parameter
         * @param t threshold
         * @param mode mode parameter: HISTOGRAM_LESS_OR_EQUAL or HISTOGRAM_GREATER
         * @param limit upper limit
         * @return the computed mean
         */
        float mean(unsigned int t, int mode, unsigned int limit= 0);

        /**
         * finds two peaks from left and right direction in the histogram
         * @param mB output parameter, left peak value
         * @param mF output parameter, right peak value
         */
        //void findTwoPeak(float& mB, float& mF);

        /**
         * computes the variance until or from index t
         * @param t start or end point of variance computation
         * @param mode HISTOGRAM_LESS_OR_EQUAL or HISTOGRAM_GREATER
         * @return the variance
        */
        //float variance(unsigned int t, int mode);

        /**
         * computes the value of the probability mass function at g
         * @param g final index
         * @return the pmf function value
         */
        //float probabilityMassFunction(unsigned int g);

        /**
         * computes the value of the probability mass function between start and end
         * @param start start index
         * @param end end index
         * @return the pmf function value
         */
        //float probabilityMassFunction(unsigned int start, unsigned int end);

        /**
         * finds the index of the first and last nonzero element
         * @param min output parameter, index of first non zero element
         * @param max output parameter, index of last non zero element
         */
        //void minMaxNonZeroElements( unsigned int& min, unsigned int& max );

        /**
         * returns the index of the minimum element between indices begin and end, where mask is foreground
         * @param begin first index of search
         * @param end last index of search
         * @param mask mask parameter, the search will be performed in the foreground (non 0) region
         * @return the index
         */
        int getMinIndex(int begin= 0, int end= 255, Vector<unsigned char>* mask= NULL);

        /**
         * returns the index of the maximum element between indices begin and end, where the mask is foreground
         * @param begin first index of search
         * @param end last index of search
         * @param mask mask parameter, the search will be performed in the foreground (non 0) region
         * @return the index
         */
        int getMaxIndex(int begin= 0, int end= 255, Vector<unsigned char>* mask= NULL);

        /**
         * computes the minimum value between indices begin and end, where the mask is foreground
         * @param begin first index of the search
         * @param end last index of the search
         * @param mask mask parameter, the search will be performed in the foreground (non 0) retion
         * @return the value
         */
        float getMin(int begin= 0, int end= 255, Vector<unsigned char>* mask= NULL);

        /**
         * computes the maximum value between indices begin and end, where the mask is foreground
         * @param begin first index of the search
         * @param end last index of the search
         * @param mask mask parameter, the search will be performed in the foreground (non 0) region
         * return the value
         */
        float getMax(int begin= 0, int end= 255, Vector<unsigned char>* mask= NULL);

        float mfIndex(int index);

        float mfIndex255(int index);

        float mfValue(float value);

        float mbIndex(int index);

        float mbIndex255(int index);

        float mbValue(float value);

        float vfIndex(int index);

        float vfValue(float value);

        float vbIndex(int index);

        float vbValue(float value);

        float sfIndex(int index);

        float sfValue(float value);

        float sbIndex(int index);

        float sbValue(float value);

        float pIndex(int index);

        float pValue(float value);

        bool normalized;

        /**
         * computes the convex hull of the histogram
         * @return convex histogram
         */
        //Histogram histogramConvexHull();

        /**
         * peak detection signal
         */
        //void peakDetectionSignal(int, Vector<openip::Peak>&);

        /**
         * computes fuzzy area
         * @param input input image
         * @param mu fuzzyness parameter
         * @param mask mask parameter
         * @return fuzzy area
         */
        /*template <typename INPUT>
        float areaOfFuzzy(Image<INPUT>& input, Vector<float> mu, Image<unsigned char>* mask= NULL);*/

        /**
         * comptues fuzzy perimeter
         * @param input input image
         * @param mu fuzzyness parameter
         * @param mask mask parameter
         * @return fuzzy perimeter
         */
        /*template <typename INPUT>
        float perimOfFuzzy(Image<INPUT>& input, Vector<float> mu, Image<unsigned char>* mask= NULL);*/

        /**
         * computes standard S function
         * @param input input image
         * @param a a parameter
         * @param b b parameter
         * @param c c parameter
         * @param mask mask parameter, the computation will be performed in the foreground (non 0) region
         * @return standard S function values
         */
        /*template <typename INPUT>
        Vector<float> standardSFunction(Image<INPUT>& input, float a, float b, float c, Image<unsigned char>* mask= NULL);*/

        /**
         * computes S function
         * @param input input parameter
         * @param a a parameter
         * @param b b parameter
         * @param c c parameter
         * @param mask mask parameter, the computation will be performed in the foreground (non 0) region
         * @return S function values
         */
        /*template <typename INPUT>
        Vector<float> sFunction(Vector<INPUT> input, float a, float b, float c, Image<unsigned char>* mask= NULL);*/

        /**
         * draws the image of the histogram
         * @param picture image object to draw in
         */
        /*template <typename INPUT>
        void createHistogramPicture(Image<INPUT>& picture);*/

        /**
         * separates the histogram into two parts, separates at index t
         * @param lower the lower part of the histogram
         * @param upper the upper part of the histogram
         * @param t the index to separate at
         */ 
        //void separate(Histogram& lower, Histogram& upper, int t);

	/**
	* returns the original function value to an index of the histogram
	* @param i histogram index
	* @return the function value
	*/
        float indexToValue(int i);

        int valueToIndex(float v);

        /**
         * min value in the vector from which the histogram is generated
         */
        double min;

        /**
         * max value in the vector from which the histogram is generated
         */
        double max;

        double d;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param h Histogram object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const Histogram& h);

    template<typename T>
    Histogram::Histogram(Vector<T>* v, int numberOfBins, T min, T max, Vector<unsigned char>* mask)
    {
        this->computeHistogram(v, numberOfBins, min, max, mask);
    }

    template<typename T>
    float& Histogram::get(T x)
    {
        return (*this)((x - this->min) / (this->max - this->min) * (this->size() - 1));
    }

    template<typename T>
    float Histogram::get(T x) const
    {
        return (*this)((x - this->min) / (this->max - this->min) * (this->size() - 1));
    }

    template<typename T>
    int Histogram::getIndex(T x)
    {
        return (x - this->min) / (this->max - this->min) * (this->size() - 1);
    }

    template<typename T>
    void  Histogram::setParameters(T min, T max, int numberOfBins)
    {
        this->min= min;
        this->max= max;
        this->resize(numberOfBins);
    }

    template<typename T>
    void Histogram::computeHistogram(Vector<T>* v, int n, StructuringElement2* se, Vector<unsigned char>* mask)
    {
        *this= 0;

        T min, max;
        se->getMinMax(*v, n, min, max, mask);

        d= (max - min)/float(this->size());

        if ( mask == NULL )
        {
            for ( typename StructuringElement2::iterator sit= se->begin(); sit != se->end(); ++sit )
            {
                (*this)(int(((*v)(*sit + n) - min)/d))++;
            }
        }
        else
        {
            for ( typename StructuringElement2::iterator sit= se->begin(); sit != se->end(); ++sit )
            {
                if ( (*mask)(*sit + n) > 0 )
                    (*this)(int(((*v)(*sit + n) - min)/d))++;
            }
        }
    }


    template<typename T>
    void Histogram::computeHistogram(Vector<T>* v, int numberOfBins, T min, T max, Vector<unsigned char>* mask)
    {
        this->resize(numberOfBins);

        this->min= v->getMin(mask);
        this->max= v->getMax(mask);

        (*this)= 0;
        d= float(max - min)/float(numberOfBins - 1);

        //printf("%f %f %f\n", min, max, d);
        if ( mask == NULL )
        {
            int idx;
            for ( typename Vector<T>::vIt vit= v->begin(); vit != v->end(); ++vit )
            {
                idx= (int)floor(((float)((*vit - min)))/d);
                //printf("(%f,%d)", *vit, idx);
                if ( idx >= 0 && idx < int(this->size()) )
                    (*this)(idx)++;
                else if ( idx < 0 )
                    printf(".(%d, %f %f)", idx, *vit, d);
                else
                    printf(".(%d, %f %f)", idx, *vit, d);
            }
        }
        else
        {
            Vector<unsigned char>::vIt mit= mask->begin();
            int idx;
            for ( typename Vector<T>::vIt vit= v->begin(); vit != v->end(); ++vit, ++mit )
            {
                if ( *mit )
                {
                    idx= (int)floor((float)((*vit - min))/d);
                    if ( idx >= 0 && idx < int(this->size()) )
                        (*this)(idx)++;
                    else if ( idx < 0 )
                        printf(".(%d, %f %f)", idx, *vit, d);
                    else
                        printf(".(%d, %f %f)", idx, *vit, d);
                }
            }
        }
    }

    template<typename T>
    void Histogram::computeWeightedHistogram(Vector<T>* v, Vector<T>* w, int numberOfBins, T min, T max, Vector<unsigned char>* mask)
    {
        this->resize(numberOfBins);

        this->min= v->getMin(mask);
        this->max= v->getMax(mask);

        (*this)= 0;
        d= float(max - min)/float(numberOfBins - 1);

        if ( mask == NULL )
        {
            int idx;
            for ( int i= 0; i < v->size(); ++i )
            {
                idx= (int)floor(((float)(((*v)(i) - min)))/d);
                if ( idx >= 0 && idx < this->size() )
                    (*this)(idx)+= (*w)(i);
                else
                    printf(".");
            }
        }
        else
        {
            int idx;
            for ( int i= 0; i < v->size(); ++i )
            {
                if ( (*mask)(i) )
                {
                    idx= (int)floor((float)(((*v)(i) - min))/d);
                    if ( idx >= 0 && idx < this->size() )
                        (*this)(idx)+= (*w)(i);
                    else
                        printf(".");
                }
            }
        }
    }

    template<typename T, int SIZE>
    void Histogram::computeHistogramInLimits(Vector<VectorN<T, SIZE> >* v, int dimension, int numberOfBins, T min, T max, Vector<unsigned char>* mask)
    {
        this->resize(numberOfBins);
        this->min= min;
        this->max= max;
        (*this)= 0;
        d= float(max - min)/float(numberOfBins - 1);

        if ( mask == NULL )
        {
            for ( typename Vector<VectorN<T, SIZE> >::iterator vit= v->begin(); vit != v->end(); ++vit )
            {
                (*this)((int)floor(((float)(((*vit)(dimension) - min)))/d + 0.5))++;
            }
        }
        else
        {
            Vector<unsigned char>::vIt mit= mask->begin();
            for ( typename Vector<VectorN<T, SIZE> >::iterator vit= v->begin(); vit != v->end(); ++vit, ++mit )
            {
                if ( *mit )
                {
                    (*this)((int)floor((float)(((*vit)(dimension) - min))/d + 0.5))++;
                }
            }
        }
    }

    template<typename T, int SIZE>
    void Histogram::computeHistogram(Vector<VectorN<T, SIZE> >* v, int dimension, int numberOfBins, Vector<unsigned char>* mask)
    {
        this->resize(numberOfBins);

        min= max= (*v)(0)(dimension);

        for ( typename Vector<VectorN<T, SIZE> >::iterator vit= v->begin(); vit != v->end(); ++vit )
        {
            if ( (*vit)(dimension) < min )
                min= (*vit)(dimension);
            if ( (*vit)(dimension) > max )
                max= (*vit)(dimension);
        }

        (*this)= 0;
        d= float(max - min)/float(numberOfBins - 1);

        if ( mask == NULL )
        {
            for ( typename Vector<VectorN<T, SIZE> >::iterator vit= v->begin(); vit != v->end(); ++vit )
            {
                (*this)((int)floor(((float)(((*vit)(dimension) - min)))/d + 0.5))++;
            }
        }
        else
        {
            Vector<unsigned char>::vIt mit= mask->begin();
            for ( typename Vector<VectorN<T, SIZE> >::iterator vit= v->begin(); vit != v->end(); ++vit, ++mit )
            {
                if ( *mit )
                {
                    (*this)((int)floor((float)(((*vit)(dimension) - min))/d + 0.5))++;
                }
            }
        }
    }

    /*template <typename INPUT>
    float Histogram::areaOfFuzzy(Image<INPUT>& input, Vector<float> mu, Image<unsigned char>* mask)
    {
        float area= 0.0;
        if ( mask == NULL )
        {
            for ( int i= 0; i < input.n; i++ )
                area+= mu[i];
        }
        else
        {
            for ( int i= 0; i < input.n; i++ )
                if( (*mask)(i) > 0 )
                    area+= mu[i];
        }
        
        return area;
    }

    template <typename INPUT>
    float Histogram::perimOfFuzzy(Image<INPUT>& input, Vector<float> mu, Image<unsigned char>* mask)
    {
        int i, j;
        float perim= 0.0;

        if ( mask == NULL )
        {
            for ( i= 0; i < input.rows; i++ )
                for ( j= 0; j < input.columns - 1; j++ )
                    perim+= fabs(mu[i * input.columns + j] - mu[i * input.columns + j + 1]);

            for ( j= 0; j < input.columns; j++ )
                for ( i= 0; i < input.rows - 1; i++ )
                    perim+= fabs(mu[i * input.columns + j] - mu[i * input.columns + j + input.columns]);
        }
        else
        {
            for ( i= 0; i < input.rows; i++ )
                for ( j= 0; j < input.columns - 1; j++ )
                    if( (*mask)(i * input.columns + j) > 0 )
                        perim+= fabs(mu[i * input.columns + j] - mu[i * input.columns + j + 1]);

            for ( j= 0; j < input.columns; j++ )
                for ( i= 0; i < input.rows - 1; i++ )
                    if( (*mask)(i * input.columns + j) > 0 )
                        perim+= fabs(mu[i * input.columns + j] - mu[i * input.columns + j + input.columns]);
        }

        return perim;
    }

    template <typename INPUT>
    Vector<float> Histogram::standardSFunction(Image<INPUT>& input, float a, float b, float c, Image<unsigned char>* mask)
    {
        Vector<float> mu;
        mu.resize(input.n);
        
        int i;
        float tmp;

        if ( mask == NULL )
        {
            for ( i= 0; i < input.n; i++ )
            {
                if(input(i) <= a)
                    mu[i]= 0.0;     

                if((a <= input(i)) && (input(i) <= b))
                {
                    tmp= (input(i) - a) / (c - a);
                    mu[i]= 2.0 * (tmp * tmp);
                }

                if((b <= input(i)) && (input(i) <= c))
                {
                    tmp= (input(i) - c) / (c - a);
                    mu[i]= 1.0 - 2.0 * (tmp * tmp);
                }

                if(input(i) >= c)
                    mu[i]= 1.0;
            }
        }
        else
        {
            for ( i= 0; i < input.n; i++ )
                if( (*mask)(i) > 0 )
                {
                    if(input(i) <= a)
                        mu[i]= 0.0;

                    if((a <= input(i)) && (input(i) <= b))
                    {
                        tmp= (input(i) - a) / (c - a);
                        mu[i]= 2.0 * (tmp * tmp);
                    }

                    if((b <= input(i)) && (input(i) <= c))
                    {
                        tmp= (input(i) - c) / (c - a);
                        mu[i]= 1.0 - 2.0 * (tmp * tmp);
                    }

                    if(input(i) >= c)
                        mu[i]= 1.0;
                }
        }

        return mu;
    }

    template <typename INPUT>
    Vector<float> Histogram::sFunction(Vector<INPUT> input, float a, float b, float c, Image<unsigned char>* mask)
    {
        Vector<float> mu;
        int i;

        if ( mask == NULL )
        {
            for ( i= 0; i < input.size(); i++ )
            {
                if ( input[i] <= a)
                    mu= 0.0;

                if ( (a <= input[i]) && (input[i] <= b) )
                    mu= ((input[i] - a) * (input[i] - a)) / ((b - a) * (c - a));

                if ( (b <= input[i]) && (input[i] <= c) )
                    mu= 1.0 - (((input[i] - c) * (input[i] - c)) / ((c - b) * (c - a)));

                if ( input[i] >= c )
                    mu= 1.0;
            }
        }
        else
        {
            for ( i= 0; i < input.size(); i++ )
                if( (*mask)(i) > 0 )
                {
                    if ( input[i] <= a)
                        mu= 0.0;

                    if ( (a <= input[i]) && (input[i] <= b) )
                        mu= ((input[i] - a) * (input[i] - a)) / ((b - a) * (c - a));

                    if ( (b <= input[i]) && (input[i] <= c) )
                        mu= 1.0 - (((input[i] - c) * (input[i] - c)) / ((c - b) * (c - a)));

                    if ( input[i] >= c )
                        mu= 1.0;
                }
        }

        return mu;
    }

    template <typename INPUT>
    void Histogram::createHistogramPicture(Image<INPUT>& picture)
    {
        int size= 256;
        int i;

        float max;
        int width= picture.columns;
        int height= picture.rows;

        max= this->getMax();

        for(i= 0; i < size; i++)
        {
            operator[](i)= operator[](i) / max;
        }

        for(i= 0; i < picture.n; i++)
            picture(i)= 0;

        int colorHist = 180;

        double columnSize= (width - 20 - 20) / ((double) size);

        for(i= 7; i < 17; i++)
        {
            picture(0 + 20 * width + i)= colorHist;
            picture(0 + (height - 20) * width + i)= colorHist;
            picture(0 + ((height - 20 - 20) / 2 + 20) * width + i)= colorHist;
            picture(20 + (height - 20) * width + (i * width))= colorHist;
            picture(20 + (columnSize * size) + (height - 20) * width + (i * width) + columnSize)= colorHist;
            picture(20 + (columnSize * size) / 2 + (height - 20) * width + (i * width))= colorHist;
        }

        int x= 20;
        double tmp;
        tmp= x + (height - 20) * width;
        i= 0;

        for(int j, y, k; i < size; i++)
        {
            tmp+= columnSize;
            y= (height - 20 - 20) * operator[](i);
            y*= width;

            for(k=tmp; k < tmp + columnSize; k++)
                for(j= width; j <= y; j+= width)
                    picture(k - j)= 255;
        }
    }*/
}
#endif
