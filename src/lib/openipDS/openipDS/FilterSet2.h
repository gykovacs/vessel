/**
 * @file FilterSet2.h
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
 * The FilterSet2 is a set of filter objects. The effect of applying a filter set
 * to an image is that in each position of the image the filters of the set are
 * applied and one value is chosen for output.
 */

#ifndef _FILTER_SET2_H_
#define _FILTER_SET2_H_

#include <float.h>
#include <string>

#include <openipDS/Filter2.h>
#include <openipDS/Image.h>
#include <openipDS/Pixel.h>
#include <openipDS/Feature2.h>

//#include <openipLL/imageIO.h>

namespace openip
{
    template<typename INPUT>
    class GramSchmidtOrthogonalization
    {
    public:
        GramSchmidtOrthogonalization();

        GramSchmidtOrthogonalization(const GramSchmidtOrthogonalization& gso);

        ~GramSchmidtOrthogonalization();

        void apply(Vector<Vector<INPUT> >& input, Vector<Vector<INPUT> >& output);
    };

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::GramSchmidtOrthogonalization()
    {
    }

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::GramSchmidtOrthogonalization(const GramSchmidtOrthogonalization& gso)
    {
    }

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::~GramSchmidtOrthogonalization()
    {
    }

    template<typename INPUT>
    void GramSchmidtOrthogonalization<INPUT>::apply(Vector<Vector<INPUT> >& input, Vector<Vector<INPUT> >& output)
    {
        output.resize(input.size());
        output(0)= input(0);

        for ( unsigned int i= 1; i < input.size(); ++i )
        {
            Vector<INPUT> u= input(i);
            for ( unsigned int j= 0; j < i; ++j )
            {
                u-= output(j).proj(input(i));
            }
            output(i)= u;
        }

        for ( unsigned int i= 0; i < output.size(); ++i )
        {
            if ( output(i).getNorm2() != output(i).getNorm2() )
            {
                printf("!!!!!!!!!!!\n");
                fflush(stdout);
            }
            output(i)/= output(i).getNorm2();
        }
    }

    /**
     * FilterSet represents a set of filters
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class FilterSet2: public vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >, public Feature2<INPUT, OUTPUT>
    {
    public:
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::push_back;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::resize;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::size;

        /**
         * constructor, creates an empty filter set
         * @param label label of matching
         */
        FilterSet2(OUTPUT label= 0);

        /**
         * copy constructor
         * @param f instance to copy
         */
        FilterSet2(const FilterSet2& f);

        /**
         * destructor
         */
        ~FilterSet2();

        /**
         * apply method
         * @param input input image
         * @param output output image
         * @param mask the filter set will be applied in the foreground (non 0) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * applies the filter set in position n
         * @param input input image
         * @param n position in row-continuous representation
         * @param roi the operation uses only the pixels covered by the foreground (non-zero) region of the roi
         * @return the result
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        void getWeights(Vector<Vector<WEIGHTS> >& v);

        void setWeights(Vector<Vector<WEIGHTS> >& v);

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * returns the minimum position of row-major representation in which the filter can be applied
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation in which the filter can be applied
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * returns stride, if the stride of the filters is the same, else
         * returns -1
         */
        int stride();

        virtual void orthogonalize();

        virtual int dimensions();

        /**
         * update stride of filters to new stride
         */
        virtual int updateStride(int stride);

        /**
         * minimum position
         */
        int min;

        /**
         * maximum position
         */
        int max;

        /**
         * label for labeling with several filter sets, using filter system
         */
        OUTPUT label;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSet2<INPUT, OUTPUT, WEIGHTS>::FilterSet2(OUTPUT label)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (), Feature2<INPUT, OUTPUT>()
    {
        this->label= label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSet2<INPUT, OUTPUT, WEIGHTS>::FilterSet2(const FilterSet2& fs)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (fs), Feature2<INPUT, OUTPUT>(fs)
    {
        this->min= fs.min;
        this->max= fs.max;
        //this->stride= fs.stride;
        this->label= fs.label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    FilterSet2<INPUT, OUTPUT, WEIGHTS>::~FilterSet2()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            delete (*this)[i];
        (*this).clear();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSet2<INPUT, OUTPUT, WEIGHTS>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSet2<INPUT, OUTPUT, WEIGHTS>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT FilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;

        for ( typename FilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->apply(in, n, support);
            if ( tmp > max )
                max= tmp;
        }

        return max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::orthogonalize()
    {
        GramSchmidtOrthogonalization<WEIGHTS> gso;
        Vector<Vector<WEIGHTS> > input;
        input.resize(this->size());
        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)[i]->getWeights(input(i));
        Vector<Vector<WEIGHTS> > output;
        gso.apply(input, output);
        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)[i]->setWeights(output(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            //printf("without roi\n"); fflush(stdout);
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)((input.n) - this->max); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)((input.n) - this->max); ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, i, support);
                    //++n;
                }
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        /*printf("apply %d %d\n", k, n); fflush(stdout);
        for ( int i= 0; i < this->size(); ++i )
            cout << (*this)[i]->getDescriptor() << " " << (*this)[i]->getMin() << " " << (*this)[i]->getMax() << endl;*/
        if ( input.columns != stride() )
            updateStride(input.columns);

        int maxIdx= 0;
        Vector<OUTPUT> tmp;
        tmp.resize(this->size());

        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            //output(n + i)= (*this)[i]->apply(input, k, support);
            tmp(i)= (*this)[i]->apply(input, k, support);
            if ( tmp(i) > tmp(maxIdx) )
                maxIdx= i;
        }
        for ( unsigned int i= 0; i < this->size(); ++i )
            output(n + i)= tmp((maxIdx + i) % this->size());
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        printf("applying filterset\n"); fflush(stdout);
        updateStride(input.columns);
        computeMinMax();
        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)[i]->apply(input, output(n + i), roi, support);

        int maxIdx;
        Vector<OUTPUT> tmp;
        tmp.resize(this->size());
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            maxIdx= 0;
            for ( unsigned int j= 0; j < this->size(); ++j )
            {
                if ( output(n + j)(i) > output(n + maxIdx)(i))
                    maxIdx= j;
                tmp(j)= output(n + j)(i);
            }
            for ( unsigned int j= 0; j < this->size(); ++j )
                output(n + j)(i)= tmp((maxIdx + j) % this->size());
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Border2 FilterSet2<INPUT, OUTPUT, WEIGHTS>::getProposedBorder()
    {
        Border2 b(0, 0, 0, 0);
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            Border2 c= (*this)[i]->getProposedBorder();
            b= maximizeBorders(b, c);
        }

        return b;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        this->min= INT_MAX;
        this->max= -INT_MAX;
        for ( typename FilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            //cout << (*fit)->getDescriptor() << " " << (*fit)->getMin() << " " << (*fit)->getMax() << endl;
            if ( (*fit)->getMin() < min )
                min= (*fit)->getMin();
            if ( (*fit)->getMax() > max )
                max= (*fit)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSet2<INPUT, OUTPUT, WEIGHTS>::stride()
    {
        if ( this->begin() == this->end() )
            return -1;

        int stride= (*(this->begin()))->stride;

        for ( typename FilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
            if ( (*fit)->stride != stride )
                return -1;
        return stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSet2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        for ( typename FilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateStride(stride);
            (*fit)->computeMinMax();
        }
        this->computeMinMax();

        return 0;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int FilterSet2<INPUT, OUTPUT, WEIGHTS>::dimensions()
    {
        return this->size();
    }

    /**
     * prints the description of the parameter FilterSet2 object to the parameter output stream
     * @param o parameter output stream
     * @param f FilterSet2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const FilterSet2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[FilterSet - number of filters: ";
        o << f.size();
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::getWeights(Vector<Vector<WEIGHTS> >& v)
    {
        v.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->getWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void FilterSet2<INPUT, OUTPUT, WEIGHTS>::setWeights(Vector<Vector<WEIGHTS> >& v)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->setWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class MinFilterSet2: public vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >, public Feature2<INPUT, OUTPUT>
    {
    public:
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::push_back;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::resize;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::size;

        /**
         * constructor, creates an empty filter set
         * @param label label of matching
         */
        MinFilterSet2(OUTPUT label= 0);

        /**
         * copy constructor
         * @param f instance to copy
         */
        MinFilterSet2(const MinFilterSet2& f);

        /**
         * destructor
         */
        ~MinFilterSet2();

        /**
         * apply method
         * @param input input image
         * @param output output image
         * @param mask the filter set will be applied in the foreground (non 0) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * applies the filter set in position n
         * @param input input image
         * @param n position in row-continuous representation
         * @param roi the operation uses only the pixels covered by the foreground (non-zero) region of the roi
         * @return the result
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        void getWeights(Vector<Vector<WEIGHTS> >& v);

        void setWeights(Vector<Vector<WEIGHTS> >& v);

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * returns the minimum position of row-major representation in which the filter can be applied
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation in which the filter can be applied
         * @return the maximum position
         */
        virtual int getMax();

        virtual int dimensions();

        /**
         * returns stride, if the stride of the filters is the same, else
         * returns -1
         */
        int stride();

        virtual void orthogonalize();

        /**
         * update stride of filters to new stride
         */
        virtual int updateStride(int stride);

        /**
         * minimum position
         */
        int min;

        /**
         * maximum position
         */
        int max;

        /**
         * label for labeling with several filter sets, using filter system
         */
        OUTPUT label;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::MinFilterSet2(OUTPUT label)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (), Feature2<INPUT, OUTPUT>()
    {
        this->label= label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::MinFilterSet2(const MinFilterSet2& fs)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (fs), Feature2<INPUT, OUTPUT>(fs)
    {
        this->min= fs.min;
        this->max= fs.max;
        //this->stride= fs.stride;
        this->label= fs.label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::~MinFilterSet2()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            delete (*this)[i];
        (*this).clear();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::dimensions()
    {
        return 1;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT min= FLT_MAX;

        for ( typename MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->apply(in, n, support);
            if ( tmp < min )
                min= tmp;
        }

        return min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::orthogonalize()
    {
        GramSchmidtOrthogonalization<WEIGHTS> gso;
        Vector<Vector<WEIGHTS> > input;
        input.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            (*this)[i]->getWeights(input(i));
        Vector<Vector<WEIGHTS> > output;
        gso.apply(input, output);
        for ( int i= 0; i < this->size(); ++i )
            (*this)[i]->setWeights(output(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            //printf("without roi\n"); fflush(stdout);
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, i, support);
                    //++n;
                }
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        for ( int i= 0; i < this->size(); ++i )
            output(n + i)= (*this)[i]->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {

    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Border2 MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::getProposedBorder()
    {
        int w= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( (*this)[i]->getProposedBorder().topBorder > w )
                w= (*this)[i]->getProposedBorder().topBorder;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        this->min= INT_MAX;
        this->max= -INT_MAX;
        for ( typename MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( (*fit)->getMin() < min )
                min= (*fit)->getMin();
            if ( (*fit)->getMax() > max )
                max= (*fit)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::stride()
    {
        if ( this->begin() == this->end() )
            return -1;

        int stride= (*(this->begin()))->stride;

        for ( typename MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
            if ( (*fit)->stride != stride )
                return -1;
        return stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        for ( typename MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateStride(stride);
            (*fit)->computeMinMax();
        }
        this->computeMinMax();

        return 0;
    }

    /**
     * prints the description of the parameter FilterSet2 object to the parameter output stream
     * @param o parameter output stream
     * @param f FilterSet2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const MinFilterSet2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[MinFilterSet2 - number of filters: ";
        o << f.size();
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::getWeights(Vector<Vector<WEIGHTS> >& v)
    {
        v.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->getWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MinFilterSet2<INPUT, OUTPUT, WEIGHTS>::setWeights(Vector<Vector<WEIGHTS> >& v)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->setWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class MaxFilterSet2: public vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >, public Feature2<INPUT, OUTPUT>
    {
    public:
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::push_back;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::resize;
        using vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >::size;

        /**
         * constructor, creates an empty filter set
         * @param label label of matching
         */
        MaxFilterSet2(OUTPUT label= 0);

        /**
         * copy constructor
         * @param f instance to copy
         */
        MaxFilterSet2(const MaxFilterSet2& f);

        /**
         * destructor
         */
        ~MaxFilterSet2();

        /**
         * apply method
         * @param input input image
         * @param output output image
         * @param mask the filter set will be applied in the foreground (non 0) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * applies the filter set in position n
         * @param input input image
         * @param n position in row-continuous representation
         * @param roi the operation uses only the pixels covered by the foreground (non-zero) region of the roi
         * @return the result
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        void getWeights(Vector<Vector<WEIGHTS> >& v);

        void setWeights(Vector<Vector<WEIGHTS> >& v);

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * returns the minimum position of row-major representation in which the filter can be applied
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation in which the filter can be applied
         * @return the maximum position
         */
        virtual int getMax();

        virtual int dimensions();

        /**
         * returns stride, if the stride of the filters is the same, else
         * returns -1
         */
        int stride();

        virtual void orthogonalize();

        /**
         * update stride of filters to new stride
         */
        virtual int updateStride(int stride);

        /**
         * minimum position
         */
        int min;

        /**
         * maximum position
         */
        int max;

        /**
         * label for labeling with several filter sets, using filter system
         */
        OUTPUT label;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::MaxFilterSet2(OUTPUT label)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (), Feature2<INPUT, OUTPUT>()
    {
        this->label= label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::MaxFilterSet2(const MaxFilterSet2& fs)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* > (fs), Feature2<INPUT, OUTPUT>(fs)
    {
        this->min= fs.min;
        this->max= fs.max;
        //this->stride= fs.stride;
        this->label= fs.label;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::~MaxFilterSet2()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            delete (*this)[i];
        (*this).clear();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::dimensions()
    {
        return 1;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;

	
        //for ( typename MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
	int maxIdx= 0;
	for ( unsigned int i= 0; i < this->size(); i+=2 )
        {
            tmp= (*this)[i]->apply(in, n, support);
            if ( tmp > max )
	    {
                max= tmp;
		maxIdx= i;
	    }
        }
        
        int range= 0.15*this->size();
	int j;
        for ( int i= maxIdx-range; i < maxIdx+range; ++i )
	{
	  j= i;
	  if ( i < 0 )
	    j+= this->size();
	  if ( i >= int(this->size()) )
	    j-= this->size();
	  if ( j%2 == 1 )
	  {
	    tmp= (*this)[j]->apply(in, n, support);
            if ( tmp > max )
	    {
                max= tmp;
	    }
	  }
	}

        return max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::orthogonalize()
    {
        GramSchmidtOrthogonalization<WEIGHTS> gso;
        Vector<Vector<WEIGHTS> > input;
        input.resize(this->size());
        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)[i]->getWeights(input(i));
        Vector<Vector<WEIGHTS> > output;
        gso.apply(input, output);
        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)[i]->setWeights(output(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            //printf("without roi\n"); fflush(stdout);
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            //#pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, i, support);
                    //++n;
                }
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            output(n + i)= (*this)[i]->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& /*input*/, ImageVector<OUTPUT>& /*output*/, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {

    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Border2 MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::getProposedBorder()
    {
        Border2 bmax, btmp;
        
        for ( unsigned int i= 0; i < this->size(); ++i )
	{
	  btmp= (*this)[i]->getProposedBorder();
	  bmax= maximizeBorders(bmax, btmp);
	}
        return bmax;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        this->min= INT_MAX;
        this->max= -INT_MAX;
        for ( typename MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( (*fit)->getMin() < min )
                min= (*fit)->getMin();
            if ( (*fit)->getMax() > max )
                max= (*fit)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::stride()
    {
        if ( this->begin() == this->end() )
            return -1;

        int stride= (*(this->begin()))->stride;

        for ( typename MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
            if ( (*fit)->stride != stride )
                return -1;
        return stride;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        for ( typename MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateStride(stride);
            (*fit)->computeMinMax();
        }
        this->computeMinMax();

        return 0;
    }

    /**
     * prints the description of the parameter FilterSet2 object to the parameter output stream
     * @param o parameter output stream
     * @param f FilterSet2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>& f)
    {
        o << "[MaxFilterSet2 - number of filters: ";
        o << f.size();
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::getWeights(Vector<Vector<WEIGHTS> >& v)
    {
        v.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->getWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaxFilterSet2<INPUT, OUTPUT, WEIGHTS>::setWeights(Vector<Vector<WEIGHTS> >& v)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->setWeights(v(i));
    }

    template<typename INPUT, typename OUTPUT>
    class FeatureSet2: public vector<Feature2<INPUT, OUTPUT>* >, public Feature2<INPUT, OUTPUT>
    {
    public:
        using vector<Feature2<INPUT, OUTPUT>* >::push_back;
        using vector<Feature2<INPUT, OUTPUT>* >::resize;
        using vector<Feature2<INPUT, OUTPUT>* >::size;

        /**
         * constructor, creates an empty filter set
         * @param label label of matching
         */
        FeatureSet2(OUTPUT label= 0);

        /**
         * copy constructor
         * @param f instance to copy
         */
        FeatureSet2(const FeatureSet2& f);

        /**
         * destructor
         */
        ~FeatureSet2();

        /**
         * apply method
         * @param input input image
         * @param output output image
         * @param mask the filter set will be applied in the foreground (non 0) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * applies the filter set in position n
         * @param input input image
         * @param n position in row-continuous representation
         * @param roi the operation uses only the pixels covered by the foreground (non-zero) region of the roi
         * @return the result
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * returns the minimum position of row-major representation in which the filter can be applied
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation in which the filter can be applied
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * returns stride, if the stride of the filters is the same, else
         * returns -1
         */
        int stride();

        /**
         * update stride of filters to new stride
         */
        virtual int updateStride(int stride);

        /**
         * minimum position
         */
        int min;

        /**
         * maximum position
         */
        int max;

        /**
         * label for labeling with several filter sets, using filter system
         */
        OUTPUT label;
    };

    template<typename INPUT, typename OUTPUT>
    FeatureSet2<INPUT, OUTPUT>::FeatureSet2(OUTPUT label)
    : vector<Feature2<INPUT, OUTPUT>* > (), Feature2<INPUT, OUTPUT>()
    {
        this->label= label;
    }

    template<typename INPUT, typename OUTPUT>
    FeatureSet2<INPUT, OUTPUT>::FeatureSet2(const FeatureSet2& fs)
    : vector<Feature2<INPUT, OUTPUT>* > (fs), Feature2<INPUT, OUTPUT>(fs)
    {
        this->min= fs.min;
        this->max= fs.max;
        //this->stride= fs.stride;
        this->label= fs.label;
    }

    template<typename INPUT, typename OUTPUT>
    FeatureSet2<INPUT, OUTPUT>::~FeatureSet2()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
            delete (*this)[i];
        (*this).clear();
    }

    template<typename INPUT, typename OUTPUT>
    int FeatureSet2<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int FeatureSet2<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT FeatureSet2<INPUT, OUTPUT>::apply(Image<INPUT>& in, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;

        for ( typename FeatureSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->apply(in, n, support);
            if ( tmp > max )
                max= tmp;
        }

        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void FeatureSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            //#pragma omp parallel for
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            //#pragma omp parallel for
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, i, support);
                }
        }
    }

    template<typename INPUT, typename OUTPUT>
    Border2 FeatureSet2<INPUT, OUTPUT>::getProposedBorder()
    {
        int w= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( (*this)[i]->getProposedBorder().topBorder > w )
                w= (*this)[i]->getProposedBorder().topBorder;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    void FeatureSet2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= INT_MAX;
        this->max= -INT_MAX;
        for ( typename FeatureSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            if ( (*fit)->getMin() < min )
                min= (*fit)->getMin();
            if ( (*fit)->getMax() > max )
                max= (*fit)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int FeatureSet2<INPUT, OUTPUT>::stride()
    {
        if ( this->begin() == this->end() )
            return -1;

        int stride= (*(this->begin()))->stride;

        for ( typename FeatureSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
            if ( (*fit)->stride != stride )
                return -1;
        return stride;
    }

    template<typename INPUT, typename OUTPUT>
    int FeatureSet2<INPUT, OUTPUT>::updateStride(int stride)
    {
        for ( typename FeatureSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateStride(stride);
            (*fit)->computeMinMax();
        }
        this->computeMinMax();

        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    class MinFeatureSet2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::getProposedBorder;
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;

        MinFeatureSet2();

        MinFeatureSet2(const MinFeatureSet2& m);

        ~MinFeatureSet2();

        virtual OUTPUT apply(Image<INPUT>& in, int n, Image<unsigned char>* support);
    };

    template<typename INPUT, typename OUTPUT>
    MinFeatureSet2<INPUT, OUTPUT>::MinFeatureSet2()
        : FeatureSet2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MinFeatureSet2<INPUT, OUTPUT>::MinFeatureSet2(const MinFeatureSet2<INPUT, OUTPUT> &m)
        : FeatureSet2<INPUT, OUTPUT>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MinFeatureSet2<INPUT, OUTPUT>::~MinFeatureSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MinFeatureSet2<INPUT, OUTPUT>::apply(Image<INPUT> &in, int n, Image<unsigned char> *support)
    {
        OUTPUT tmp;
        OUTPUT min= FLT_MAX;

        for ( typename FeatureSet2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->apply(in, n, support);
            if ( tmp < min )
                min= tmp;
        }

        return min;
    }

    /**
     * prints the description of the parameter FilterSet2 object to the parameter output stream
     * @param o parameter output stream
     * @param f FilterSet2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const FeatureSet2<INPUT, OUTPUT>& f)
    {
        o << "[FeatureSet2 - number of features: ";
        o << f.size();
        o << "]";

        return o;
    }
}

#endif
