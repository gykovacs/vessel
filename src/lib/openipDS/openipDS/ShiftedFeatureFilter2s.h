#ifndef SHIFTEDFEATUREFILTER2S_H
#define SHIFTEDFEATUREFILTER2S_H

#include <openipDS/ShiftedFeatureFilter2.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/GaussGaborFilter2.h>

namespace openip
{
    /*template<typename INPUT, typename OUTPUT>
    class ShiftedGaborFilterR2: public ShiftedFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::~ShiftedGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaborFilterR2: public ShiftedCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::~ShiftedCorrelationGaborFilterR2()
    {
    }*/

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterSet2<INPUT, OUTPUT>::ShiftedGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCCorrelationGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::ShiftedCCorrelationGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedCCorrelationGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::ShiftedGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedGaussGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedGaussGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::ShiftedCorrelationGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaussGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCCorrelationGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::ShiftedCCorrelationGaussGaborFilterSet2(float angle, float dist, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "ShiftedCCorrelationGaussGaborFilterSet2 " << angle << " " << dist << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        for ( float t= -n*dist; t <= n*dist; t+= dist )
            this->push_back(new ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));
    }

    /*template<typename INPUT, typename OUTPUT>
    class ShiftedGaussianFilter2: public ShiftedFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaussianFilter2(float angle, float dist, float sigma, int size);

        ShiftedGaussianFilter2(const ShiftedGaussianFilter2<INPUT, OUTPUT>& s);

        ~ShiftedGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussianFilter2<INPUT, OUTPUT>::ShiftedGaussianFilter2(float angle, float dist, float sigma, int size)
        : ShiftedFilter2<INPUT, OUTPUT, float>(angle, dist, new GaussianFilter2<INPUT, OUTPUT>(sigma, size, 1, 1, 1, 0, 4000))
    {
        stringstream ss;
        ss << "ShiftedGaussianFilter2 " << sigma << " " << size;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussianFilter2<INPUT, OUTPUT>::ShiftedGaussianFilter2(const ShiftedGaussianFilter2<INPUT, OUTPUT>& f)
        : ShiftedFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussianFilter2<INPUT, OUTPUT>::~ShiftedGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaussianFilter2: public ShiftedCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaussianFilter2(float angle, float dist, float sigma, int size);

        ShiftedCorrelationGaussianFilter2(const ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& s);

        ~ShiftedCorrelationGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::ShiftedCorrelationGaussianFilter2(float angle, float dist, float sigma, int size)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(angle, dist, new GaussianFilter2<INPUT, OUTPUT>(sigma, size, 1, 1, 1, 0, 4000))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaussianFilter2 " << sigma << " " << size;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::ShiftedCorrelationGaussianFilter2(const ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& f)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::~ShiftedCorrelationGaussianFilter2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class MatchedMaxShiftedGaussianFilter2: public MatchedMaxFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMaxFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMaxFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMaxFilter2<INPUT, OUTPUT>::apply;
        using MatchedMaxFilter2<INPUT, OUTPUT>::descriptor;

        MatchedMaxShiftedGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size);

        MatchedMaxShiftedGaussianFilter2(const MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>& s);

        ~MatchedMaxShiftedGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>::MatchedMaxShiftedGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size)
        : MatchedMaxFilter2<INPUT, OUTPUT>()
    {
        stringstream ss;
        ss << "MatchedMaxShiftedGaussianFilter2 " << angle0 << " " << angles << " " << angle1 << " " << dist << " " << sigma << " " << size;
        descriptor= ss.str();

        for ( float a= angle0; a <= angle1; a+= angles)
            this->push_back(new ShiftedGaussianFilter2<INPUT, OUTPUT>(a, dist, sigma, size));
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>::MatchedMaxShiftedGaussianFilter2(const MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>& f)
        : MatchedMaxFilter2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>::~MatchedMaxShiftedGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxShiftedCorrelationGaussianFilter2: public MatchedMaxFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMaxFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMaxFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMaxFilter2<INPUT, OUTPUT>::apply;
        using MatchedMaxFilter2<INPUT, OUTPUT>::descriptor;

        MatchedMaxShiftedCorrelationGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size);

        MatchedMaxShiftedCorrelationGaussianFilter2(const MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& s);

        ~MatchedMaxShiftedCorrelationGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedMaxShiftedCorrelationGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size)
        : MatchedMaxFilter2<INPUT, OUTPUT>()
    {
        stringstream ss;
        ss << "MatchedMaxShiftedCorrelationGaussianFilter2 " << angle0 << " " << angles << " " << angle1 << " " << dist << " " << sigma << " " << size;
        descriptor= ss.str();

        for ( float a= angle0; a <= angle1; a+= angles)
            this->push_back(new ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>(a, dist, sigma, size));
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedMaxShiftedCorrelationGaussianFilter2(const MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& f)
        : MatchedMaxFilter2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::~MatchedMaxShiftedCorrelationGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinShiftedGaussianFilter2: public MatchedMinFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMinFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMinFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMinFilter2<INPUT, OUTPUT>::apply;
        using MatchedMinFilter2<INPUT, OUTPUT>::descriptor;

        MatchedMinShiftedGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size);

        MatchedMinShiftedGaussianFilter2(const MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>& s);

        ~MatchedMinShiftedGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>::MatchedMinShiftedGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size)
        : MatchedMinFilter2<INPUT, OUTPUT>()
    {
        stringstream ss;
        ss << "MatchedMinShiftedGaussianFilter2 " << angle0 << " " << angles << " " << angle1 << " " << dist << " " << sigma << " " << size;
        descriptor= ss.str();

        for ( float a= angle0; a <= angle1; a+= angles)
            this->push_back(new ShiftedGaussianFilter2<INPUT, OUTPUT>(a, dist, sigma, size));
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>::MatchedMinShiftedGaussianFilter2(const MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>& f)
        : MatchedMinFilter2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>::~MatchedMinShiftedGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinShiftedCorrelationGaussianFilter2: public MatchedMinFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMinFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMinFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMinFilter2<INPUT, OUTPUT>::apply;
        using MatchedMinFilter2<INPUT, OUTPUT>::descriptor;

        MatchedMinShiftedCorrelationGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size);

        MatchedMinShiftedCorrelationGaussianFilter2(const MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& s);

        ~MatchedMinShiftedCorrelationGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedMinShiftedCorrelationGaussianFilter2(float angle0, float angles, float angle1, float dist, float sigma, int size)
        : MatchedMinFilter2<INPUT, OUTPUT>()
    {
        stringstream ss;
        ss << "MatchedMinShiftedCorrelationGaussianFilter2 " << angle0 << " " << angles << " " << angle1 << " " << dist << " " << sigma << " " << size;
        descriptor= ss.str();

        for ( float a= angle0; a <= angle1; a+= angles)
            this->push_back(new ShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>(a, dist, sigma, size));
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedMinShiftedCorrelationGaussianFilter2(const MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>& f)
        : MatchedMinFilter2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>::~MatchedMinShiftedCorrelationGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaborFilterR2: public ShiftedFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::~ShiftedGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaborFilterR2: public ShiftedCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::~ShiftedCorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::apply;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::dimensions;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMin;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMax;

        ShiftedGaborFilterR2V(float angle, float step, float n, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedGaborFilterR2V(const ShiftedGaborFilterR2V& g);

        ~ShiftedGaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2V<INPUT, OUTPUT>::ShiftedGaborFilterR2V(float angle, float step, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= -n*step; t <= n*step; t+= step )
            this->push_back(new ShiftedGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));

        stringstream ss;
        ss << "ShiftedGaborFilterR2V " << angle << " " << step << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2V<INPUT, OUTPUT>::ShiftedGaborFilterR2V(const ShiftedGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2V<INPUT, OUTPUT>::~ShiftedGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::apply;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::dimensions;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMin;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMax;

        ShiftedCorrelationGaborFilterR2V(float angle, float step, float n, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCorrelationGaborFilterR2V(const ShiftedCorrelationGaborFilterR2V& g);

        ~ShiftedCorrelationGaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2V(float angle, float step, float n, float sigma, float theta, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= -n*step; t <= n*step; t+= step )
            this->push_back(new ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>(angle, t, sigma, theta, lambda, psi, gamma));

        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterR2V " << angle << " " << step << " " << n << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2V(const ShiftedCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::~ShiftedCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::apply;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::dimensions;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMin;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::min;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::max;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getDescriptor;

        RotatedShiftedGaborFilterR2V(float nstep, float n, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedShiftedGaborFilterR2V(const RotatedShiftedGaborFilterR2V& g);

        ~RotatedShiftedGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        Vector<ShiftedGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<GaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedShiftedGaborFilterR2V(float nstep, float n, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "RotatedShiftedGaborFilterR2V " << nstep << " " << n << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;

        for ( float t= theta0; t < theta1; t+= step )
        {
            filters.push_back(new ShiftedGaborFilterR2V<INPUT, OUTPUT>(t, nstep, n, sigma, t, lambda, psi, gamma));
            orientation.push_back(new GaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedShiftedGaborFilterR2V(const RotatedShiftedGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::~RotatedShiftedGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;
        int begin= this->getMin();
        int end= this->getMax();

        printf("apply: %d\n", n); fflush(stdout);
        for ( int i= -begin; i < input.n - end; ++i )
        {
            m= -FLT_MAX;
            mIdx= -1;
            for ( int j= 0; j < orientation.size(); ++j )
            {
                tmp= orientation(j)->apply(input, i, support);
                if ( m < tmp )
                {
                    m= tmp;
                    mIdx= j;
                }
            }
            for ( int j= 0; j < filters(mIdx)->size(); ++j )
                output(n + j)(i)= (*(filters(mIdx)))(j)->apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;

        m= -FLT_MAX;
        mIdx= -1;
        for ( int j= 0; j < orientation.size(); ++j )
        {
            tmp= orientation(j)->apply(input, k, support);
            if ( m < tmp )
            {
                m= tmp;
                mIdx= j;
            }
        }
        for ( int j= 0; j < filters(mIdx)->size(); ++j )
            output(n + j)= (*(filters(mIdx)))(j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->computeMinMax();
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->computeMinMax();
            filters(i)->computeMinMax();
        }
        this->min= 100000000000;
        this->max= -100000000000;

        for ( int i= 0; i < filters.size(); ++i )
        {
            if ( this->min > filters(i)->getMin() )
                this->min= filters(i)->getMin();
            if ( this->max < filters(i)->getMax() )
                this->max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::dimensions()
    {
        return filters(0)->size();
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( orientation(0)->stride == stride )
            return;
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->updateStride(stride);
            filters(i)->updateStride(stride);
        }
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedShiftedCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::apply;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::dimensions;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMin;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getMax;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::min;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::max;
        using VectorTransformFilterSet2<INPUT, OUTPUT, float>::getDescriptor;

        RotatedShiftedCorrelationGaborFilterR2V(float nstep, float n, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedShiftedCorrelationGaborFilterR2V(const RotatedShiftedCorrelationGaborFilterR2V& g);

        ~RotatedShiftedCorrelationGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        Vector<ShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<CorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedShiftedCorrelationGaborFilterR2V(float nstep, float n, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "RotatedShiftedCorrelationGaborFilterR2V " << nstep << " " << n << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;

        for ( float t= theta0; t < theta1; t+= step )
        {
            filters.push_back(new ShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>(t, nstep, n, sigma, t, lambda, psi, gamma));
            orientation.push_back(new CorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedShiftedCorrelationGaborFilterR2V(const RotatedShiftedCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::~RotatedShiftedCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;
        int begin= this->getMin();
        int end= this->getMax();

        printf("apply: %d\n", n); fflush(stdout);
        for ( int i= -begin; i < input.n - end; ++i )
        {
            m= -FLT_MAX;
            mIdx= -1;
            for ( int j= 0; j < orientation.size(); ++j )
            {
                tmp= orientation(j)->apply(input, i, support);
                if ( m < tmp )
                {
                    m= tmp;
                    mIdx= j;
                }
            }
            for ( int j= 0; j < filters(mIdx)->size(); ++j )
                output(n + j)(i)= (*(filters(mIdx)))(j)->apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;

        m= -FLT_MAX;
        mIdx= -1;
        for ( int j= 0; j < orientation.size(); ++j )
        {
            tmp= orientation(j)->apply(input, k, support);
            if ( m < tmp )
            {
                m= tmp;
                mIdx= j;
            }
        }
        for ( int j= 0; j < filters(mIdx)->size(); ++j )
            output(n + j)= (*(filters(mIdx)))(j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->computeMinMax();
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->computeMinMax();
            filters(i)->computeMinMax();
        }
        this->min= 100000000000;
        this->max= -100000000000;

        for ( int i= 0; i < filters.size(); ++i )
        {
            if ( this->min > filters(i)->getMin() )
                this->min= filters(i)->getMin();
            if ( this->max < filters(i)->getMax() )
                this->max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::dimensions()
    {
        return filters(0)->size();
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( orientation(0)->stride == stride )
            return;
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->updateStride(stride);
            filters(i)->updateStride(stride);
        }
        computeMinMax();
    }*/
}

#endif // SHIFTEDFEATUREFILTER2S_H
