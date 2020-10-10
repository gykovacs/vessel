#ifndef ORIENTATIONFILTERFEATURE2S_H
#define ORIENTATIONFILTERFEATURE2S_H

#include <openipDS/OrientationFilterFeature2.h>
#include <openipDS/ShiftedFeatureFilter2s.h>
#include <openipDS/GaussGaborFilter2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class RotatedGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedGaborFilterSet2<INPUT, OUTPUT>::RotatedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new GaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedCCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CCorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedCCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedCorrelationGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedCCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedCCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedCCorrelationGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedCCorrelationGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedCCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedCCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedLambdaSpaceGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedLambdaSpaceGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedLambdaSpaceGaborFilterSet2<INPUT, OUTPUT>::RotatedLambdaSpaceGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new LambdaSpaceGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda0, lambdas, lambda1, psi, gamma));

        stringstream ss;
        ss << "RotatedLambdaSpaceGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedLambdaSpaceCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedLambdaSpaceCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedLambdaSpaceCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedLambdaSpaceCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new LambdaSpaceCorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda0, lambdas, lambda1, psi, gamma));

        stringstream ss;
        ss << "RotatedLambdaSpaceCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedLambdaSpaceCCorrelationGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedLambdaSpaceCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedLambdaSpaceCCorrelationGaborFilterSet2<INPUT, OUTPUT>::RotatedLambdaSpaceCCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new LambdaSpaceCCorrelationGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda0, lambdas, lambda1, psi, gamma));

        stringstream ss;
        ss << "RotatedLambdaSpaceCCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class SectionFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::dimensions;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::min;
        using FilterSet2<INPUT, OUTPUT, float>::max;
        using FilterSet2<INPUT, OUTPUT, float>::getDescriptor;

        SectionFilterSet2(float angle, float nstep, int n);

        SectionFilterSet2(const SectionFilterSet2& g);

        ~SectionFilterSet2();
    };

    template<typename INPUT, typename OUTPUT>
    SectionFilterSet2<INPUT, OUTPUT>::SectionFilterSet2(float angle, float nstep, int n)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= -n*nstep; t <= n*nstep; t+= nstep )
            this->push_back(new SamplingFilter2<INPUT, OUTPUT>(angle, t, 4000));

        stringstream ss;
        ss << "SectionFilterSet2 " << angle << " " << nstep << " " << n;
        descriptor= ss.str();
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    SectionFilterSet2<INPUT, OUTPUT>::SectionFilterSet2(const SectionFilterSet2& g)
        : FilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    SectionFilterSet2<INPUT, OUTPUT>::~SectionFilterSet2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class OrthogonalSectionFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        OrthogonalSectionFilterSet2(float nstep, float n, float theta0, float thetas, float theta1);
    };

    template<typename INPUT, typename OUTPUT>
    OrthogonalSectionFilterSet2<INPUT, OUTPUT>::OrthogonalSectionFilterSet2(float nstep, float n, float theta0, float thetas, float theta1)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new SectionFilterSet2<INPUT, OUTPUT>(t + M_PI/2, nstep, n));

        stringstream ss;
        ss << "OrthogonalSectionFilterSet2 " << nstep << " " << n << " " << theta0 << " " << thetas << " " << theta1;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new GaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedCCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
        public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new CCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, thetas, t + (theta1 - theta0), lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedCorrelationGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedCCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedShiftedCCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedShiftedCCorrelationGaussGaborFilterSet2(float dist, float n, float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new ShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(t, dist, n, sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedShiftedCCorrelationGaussGaborFilterSet2 " << dist << " " << n << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2: public OrientationBasedFilterSet2<INPUT, OUTPUT>
    {
    public:
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::apply;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::computeMinMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::updateStride;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMin;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getMax;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::descriptor;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::orientation;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::filters;
        using OrientationBasedFilterSet2<INPUT, OUTPUT>::getProposedBorder;

        RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : OrientationBasedFilterSet2<INPUT, OUTPUT>(5, theta0, thetas, theta1)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->filters->push_back(new PhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();

        computeMinMax();
    }

}

#endif // ORIENTATIONFILTERFEATURE2S_H
