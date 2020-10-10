#ifndef ORIENTATIONFILTERFEATURE2_H
#define ORIENTATIONFILTERFEATURE2_H

#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/VectorTransform2s.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/OrientationEstimators.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class OrientationBasedFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::descriptor;

        OrientationBasedFilter2(int r, float theta0, float thetas, float theta1);

        OrientationBasedFilter2(const OrientationBasedFilter2& o);

        ~OrientationBasedFilter2();

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        int min;
        int max;

        FilterSet2<INPUT, OUTPUT, float>* filters;
        //CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        GVFOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilter2<INPUT, OUTPUT>::OrientationBasedFilter2(int r, float theta0, float thetas, float theta1)
        : Filter2<INPUT, OUTPUT, float>()
    {
        //orientation= new CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        //orientation= StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>(7, 5, theta0, thetas, theta1);
        orientation= new GVFOrientationIndexFeature2<INPUT, OUTPUT>(7, 0.1, theta0, thetas, theta1);
        filters= new FilterSet2<INPUT, OUTPUT, float>();
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilter2<INPUT, OUTPUT>::OrientationBasedFilter2(const OrientationBasedFilter2& o)
        : Filter2<INPUT, OUTPUT, float>(o)
    {
        this->orientation= orientation;
        this->filters= filters;
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilter2<INPUT, OUTPUT>::~OrientationBasedFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilter2<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( this->stride != stride )
        {
            orientation->updateStride(stride);
            for ( int i= 0; i < filters->size(); ++i )
                (*filters)(i)->updateStride(stride);
        }
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFilter2<INPUT, OUTPUT>::computeMinMax()
    {
        min= FLT_MAX;
        max= -FLT_MAX;

        for ( int i= 0; i < filters->size(); ++i )
        {
            if ( (*filters)(i)->getMin() < min )
                min= (*filters)(i)->getMin();
            if ( (*filters)(i)->getMax() > max )
                max= (*filters)(i)->getMax();
        }
        if ( orientation->getMin() < min )
            min= orientation->getMin();
        if ( orientation->getMax() > max )
            max= orientation->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilter2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilter2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        for ( int n= -getMin(); n < input.n - getMax(); ++n )
            if ( !roi || (*roi)(n) )
                output(n)= apply(input, n, support);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT OrientationBasedFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int idx= orientation->apply(input, n, support);
        return (*filters)(idx)->apply(input, n, support);
    }

    template<typename INPUT, typename OUTPUT>
    class OrientationBasedFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::updateStride;
        using Feature2<INPUT, OUTPUT>::computeMinMax;
        using Feature2<INPUT, OUTPUT>::stride;
        using Feature2<INPUT, OUTPUT>::descriptor;

        OrientationBasedFeature2(int r, float theta0, float thetas, float theta1);

        OrientationBasedFeature2(const OrientationBasedFeature2& o);

        ~OrientationBasedFeature2();

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        int min;
        int max;

        FeatureSet2<INPUT, OUTPUT>* filters;
        //CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        GVFOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeature2<INPUT, OUTPUT>::OrientationBasedFeature2(int r, float theta0, float thetas, float theta1)
        : Filter2<INPUT, OUTPUT, float>()
    {
        //orientation= new CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        orientation= new GVFOrientationIndexFeature2<INPUT, OUTPUT>(7, 0.1, theta0, thetas, theta1);
        filters= new FilterSet2<INPUT, OUTPUT, float>();
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeature2<INPUT, OUTPUT>::OrientationBasedFeature2(const OrientationBasedFeature2& o)
        : Filter2<INPUT, OUTPUT, float>(o)
    {
        this->orientation= orientation;
        this->filters= filters;
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeature2<INPUT, OUTPUT>::~OrientationBasedFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( this->stride != stride )
        {
            orientation->updateStride(stride);
            for ( int i= 0; i < filters->size(); ++i )
                (*filters)(i)->updateStride(stride);
        }
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        min= FLT_MAX;
        max= -FLT_MAX;

        for ( int i= 0; i < filters->size(); ++i )
        {
            if ( (*filters)(i)->getMin() < min )
                min= (*filters)(i)->getMin();
            if ( (*filters)(i)->getMax() > max )
                max= (*filters)(i)->getMax();
        }
        if ( orientation->getMin() < min )
            min= orientation->getMin();
        if ( orientation->getMax() > max )
            max= orientation->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeature2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeature2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        for ( int n= -getMin(); n < input.n - getMax(); ++n )
            if ( !roi || (*roi)(n) )
                output(n)= apply(input, n, support);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT OrientationBasedFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int idx= orientation->apply(input, n, support);
        return (*filters)(idx)->apply(input, n, support);
    }

    template<typename INPUT, typename OUTPUT>
    class OrientationBasedFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::stride;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;

        OrientationBasedFilterSet2(int r, float theta0, float thetas, float theta1);

        OrientationBasedFilterSet2(const OrientationBasedFilterSet2& o);

        ~OrientationBasedFilterSet2();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support);

        virtual int updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        virtual Border2 getProposedBorder();

        int min;
        int max;

        Vector<FilterSet2<INPUT, OUTPUT, float>*>* filters;
        //CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //GVFOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilterSet2<INPUT, OUTPUT>::OrientationBasedFilterSet2(int /*r*/, float theta0, float thetas, float theta1)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        //orientation= new CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        orientation= new CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        //orientation= new AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>(3, theta0, thetas, theta1);
        //orientation= new GVFOrientationIndexFeature2<INPUT, OUTPUT>(7, 0.2, theta0, thetas, theta1);
        filters= new Vector<FilterSet2<INPUT, OUTPUT, float>* >();
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilterSet2<INPUT, OUTPUT>::OrientationBasedFilterSet2(const OrientationBasedFilterSet2& o)
        : FilterSet2<INPUT, OUTPUT, float>(o)
    {
        this->orientation= orientation;
        this->filters= filters;
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFilterSet2<INPUT, OUTPUT>::~OrientationBasedFilterSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride(int stride)
    {
        //if ( this->stride() != stride )
        {
            orientation->updateStride(stride);
            for ( unsigned int i= 0; i < filters->size(); ++i )
                (*filters)(i)->updateStride(stride);
        }
        computeMinMax();
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax()
    {
        min= 1000000;
        max= -1000000;

        for ( unsigned int i= 0; i < filters->size(); ++i )
        {
            if ( (*filters)(i)->getMin() < min )
                min= (*filters)(i)->getMin();
            if ( (*filters)(i)->getMax() > max )
                max= (*filters)(i)->getMax();
        }
        if ( orientation->getMin() < min )
            min= orientation->getMin();
        if ( orientation->getMax() > max )
            max= orientation->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();

        Border2 m;
        for ( unsigned int i= 0; i < filters->size(); ++i )
        {
            Border2 b= (*filters)(i)->getProposedBorder();
            m= maximizeBorders(m, b);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        printf("applying orientationbasedfilterset2 %d %d\n", getMin(), getMax()); fflush(stdout);
        for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
        {
            if ( !roi || (*roi)(i) )
            {
                //printf("%d,", i); fflush(stdout);
                int idx= orientation->apply(input, i, support);

                //printf("%d ", i, idx); fflush(stdout);
                for ( unsigned int j= 0; j < (*filters)(idx)->size(); ++j )
                {
                    output(n + j)(i)= (*filters)(idx)->operator[](j)->apply(input, i, support);
                }
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        /*printf("a %d %d\n", k, n); fflush(stdout);*/
        if ( input.columns != (*orientation).filters(0)->stride )
            updateStride(input.columns);

        int idx= orientation->apply(input, k, support);
        //cout << (*filters)(idx)->size() << endl;
        for ( unsigned int j= 0; j < (*filters)(idx)->size(); ++j )
            output(n + j)= (*filters)(idx)->operator[](j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT OrientationBasedFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int idx= orientation->apply(input, n, support);
        float min= FLT_MAX, max= -FLT_MAX;
        for ( unsigned int i= 0; i < (*filters)(idx)->operator[](0)->size(); ++i )
        {
            if ( (*filters)(idx)->operator[](0)->operator[](i).second < min )
                min= (*filters)(idx)->operator[](0)->operator[](i).second;
            if ( (*filters)(idx)->operator[](0)->operator[](i).second > max )
                max= (*filters)(idx)->operator[](0)->operator[](i).second;
        }
        for ( unsigned int i= 0; i < (*filters)(idx)->operator[](0)->size(); ++i )
            input(n + (*filters)(idx)->operator[](0)->operator[](i).first)= (input(n + (*filters)(idx)->operator[](0)->operator[](i).first) + ((*filters)(idx)->operator[](0)->operator[](i).second - min)/(max - min) * 255)/2;
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFilterSet2<INPUT, OUTPUT>::dimensions()
    {
        return (*filters)(0)->size();
    }

    template<typename INPUT, typename OUTPUT>
    class OrientationBasedFeatureSet2: public FeatureSet2<INPUT, OUTPUT>
    {
    public:
        using FeatureSet2<INPUT, OUTPUT>::apply;
        using FeatureSet2<INPUT, OUTPUT>::getMin;
        using FeatureSet2<INPUT, OUTPUT>::getMax;
        using FeatureSet2<INPUT, OUTPUT>::updateStride;
        using FeatureSet2<INPUT, OUTPUT>::computeMinMax;
        using FeatureSet2<INPUT, OUTPUT>::stride;
        using FeatureSet2<INPUT, OUTPUT>::descriptor;

        OrientationBasedFeatureSet2(int r, float theta0, float thetas, float theta1);

        OrientationBasedFeatureSet2(const OrientationBasedFeatureSet2& o);

        ~OrientationBasedFeatureSet2();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual int updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        int min;
        int max;

        Vector<FeatureSet2<INPUT, OUTPUT>*> filters;
        //CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        GVFOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeatureSet2<INPUT, OUTPUT>::OrientationBasedFeatureSet2(int r, float theta0, float thetas, float theta1)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        //orientation= new CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        orientation= new GVFOrientationIndexFeature2<INPUT, OUTPUT>(7, 0.1, theta0, thetas, theta1);
        filters= new Vector<FeatureSet2<INPUT, OUTPUT>* >();
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeatureSet2<INPUT, OUTPUT>::OrientationBasedFeatureSet2(const OrientationBasedFeatureSet2& o)
        : FeatureSet2<INPUT, OUTPUT>(o)
    {
        this->orientation= orientation;
        this->filters= filters;
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedFeatureSet2<INPUT, OUTPUT>::~OrientationBasedFeatureSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeatureSet2<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( this->stride != stride )
        {
            orientation->updateStride(stride);
            for ( int i= 0; i < filters->size(); ++i )
                (*filters)(i)->updateStride(stride);
        }
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFeatureSet2<INPUT, OUTPUT>::computeMinMax()
    {
        min= FLT_MAX;
        max= -FLT_MAX;

        for ( int i= 0; i < filters->size(); ++i )
        {
            if ( (*filters)(i)->getMin() < min )
                min= (*filters)(i)->getMin();
            if ( (*filters)(i)->getMax() > max )
                max= (*filters)(i)->getMax();
        }
        if ( orientation->getMin() < min )
            min= orientation->getMin();
        if ( orientation->getMax() > max )
            max= orientation->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeatureSet2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeatureSet2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFeatureSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        for ( int i= -getMin(); i < input.n - getMax(); ++i )
        {
            int idx= orientation->apply(input, i, support);
            for ( int j= 0; j < (*filters)(idx)->size(); ++j )
            {
                output(n + j)(i)= (*filters)(idx)->operator[](j)->apply(input, i, support);
            }
            getchar();
        }
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedFeatureSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        int idx= orientation->apply(input, k, support);
        for ( int j= 0; j < (*filters)(idx)->size(); ++j )
            output(n + j)= (*filters)(idx)->operator[](j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedFeatureSet2<INPUT, OUTPUT>::dimensions()
    {
        return (*filters)(0)->size();
    }
}

#endif // ORIENTATIONFILTERFEATURE2_H
