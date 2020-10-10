#ifndef ORIENTATIONMULTISCALEFILTERFEATURE2S_H
#define ORIENTATIONMULTISCALEFILTERFEATURE2S_H

#include <openipDS/OrientationFilterFeature2.h>
#include <openipDS/ShiftedFeatureFilter2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class OrientationBasedMultiScaleFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
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

        OrientationBasedMultiScaleFilterSet2(int r, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float sigma= 3.5);

        OrientationBasedMultiScaleFilterSet2(const OrientationBasedMultiScaleFilterSet2& o);

        ~OrientationBasedMultiScaleFilterSet2();

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
        MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
        //GVFOrientationIndexFeature2<INPUT, OUTPUT>* orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::OrientationBasedMultiScaleFilterSet2(int /*r*/, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float sigma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        //orientation= new CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(2.2, theta0, thetas, theta1, 7, 3.14, 0.5);
        orientation= new MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(sigma, theta0, thetas, theta1, lambda0, lambdas, lambda1, 3.14, 0.7);
        //orientation= new MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>(sigma, theta0, thetas, theta1, lambda0, lambdas, lambda1, 3.14, 0.5);
        //orientation= new AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>(3, theta0, thetas, theta1);
        //orientation= new GVFOrientationIndexFeature2<INPUT, OUTPUT>(7, 0.2, theta0, thetas, theta1);
        filters= new Vector<FilterSet2<INPUT, OUTPUT, float>* >();
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::OrientationBasedMultiScaleFilterSet2(const OrientationBasedMultiScaleFilterSet2& o)
        : FilterSet2<INPUT, OUTPUT, float>(o)
    {
        this->orientation= orientation;
        this->filters= filters;
    }

    template<typename INPUT, typename OUTPUT>
    OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::~OrientationBasedMultiScaleFilterSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride(int stride)
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
    void OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax()
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
    int OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();

        Border2 m= orientation->getProposedBorder();
        for ( unsigned int i= 0; i < filters->size(); ++i )
        {
            Border2 b= (*filters)(i)->getProposedBorder();
            m= maximizeBorders(m, b);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        printf("applying multiscaleorientationbasedfilterset2 %zd\n", orientation->size()); fflush(stdout);
        for ( unsigned int i= -getMin(); i < input.n - getMax(); ++i )
        {
            if ( !roi || (*roi)(i) )
            {
                int idx= orientation->apply(input, i, support);

                for ( unsigned int j= 0; j < (*filters)(idx)->size(); ++j )
                {
                    output(n + j)(i)= (*filters)(idx)->operator[](j)->apply(input, i, support);
                }
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    void OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        /*printf("apply %d %d\n", k, n); fflush(stdout);

        printf("%d %d\n", idx, (*filters)(idx)->size()); fflush(stdout);*/
        if ( input.columns != orientation->filters(0)->stride )
            updateStride(input.columns);
        int idx= orientation->apply(input, k, support);
        for ( unsigned int j= 0; j < (*filters)(idx)->size(); ++j )
            output(n + j)= (*filters)(idx)->operator[](j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int idx= orientation->apply(input, n, support);
        for ( unsigned int i= 0; i < orientation->filters.size(); ++i )
            printf("%f ", orientation->filters[i]->apply(input, n, support));
        tprintf("%d\n", idx);
        float min= FLT_MAX, max= -FLT_MAX;
        for ( unsigned int i= 0; i < (*filters)(idx)->operator[](0)->size(); ++i )
        {
            if ( (*filters)(idx)->operator[](0)->operator[](i).second < min )
                min= (*filters)(idx)->operator[](0)->operator[](i).second;
            if ( (*filters)(idx)->operator[](0)->operator[](i).second > max )
                max= (*filters)(idx)->operator[](0)->operator[](i).second;
        }
        for ( unsigned int i= 0; i < (*filters)(idx)->operator[](0)->size(); ++i )
            input(n + (*filters)(idx)->operator[](0)->operator[](i).first)= (input(n + (*filters)(idx)->operator[](0)->operator[](i).first)*2 + ((*filters)(idx)->operator[](0)->operator[](i).second - min)/(max - min) * 255)/3;
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::dimensions()
    {
        return (*filters)(0)->size();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new GaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CorrelationGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleCCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CCorrelationGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleCCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedCorrelationGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedCCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedCCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedCCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedCCorrelationGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    /////////////////////////////////////////////

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new GaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleCCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, thetas, t + (theta1 - theta0), l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, l/sigma, t, l, psi, gamma));

        stringstream ss;
        ss << "RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas)
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2: public OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedMultiScaleFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1, lambda0, lambdas, lambda1, sigma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(l/sigma, t, l, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }
}

#endif // ORIENTATIONMULTISCALEFILTERFEATURE2S_H
