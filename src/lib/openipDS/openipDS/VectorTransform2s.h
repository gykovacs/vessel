#ifndef VECTORTRANSFORM2S_H
#define VECTORTRANSFORM2S_H

#include <openipDS/VectorTransform2.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/Filter2s.h>
#include <openipML/GramSchmidtOrthogonalization.h>

namespace openip
{
    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class VectorTransformFilterSet2: public VectorTransform2<INPUT, OUTPUT>, public Vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >
    {
    public:
        VectorTransformFilterSet2();

        VectorTransformFilterSet2(const VectorTransformFilterSet2& vt);

        virtual ~VectorTransformFilterSet2();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int begin= 0, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual int dimensions();

        virtual Border2 getProposedBorder();

        virtual void orthogonalize();

        virtual void updateStride(int stride);

        virtual int getMin();

        virtual int getMax();

        virtual void computeMinMax();

        virtual std::string getDescriptor(char space= ' ', int i= 0);

        int min;

        int max;

        std::string descriptor;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::VectorTransformFilterSet2()
        : VectorTransform2<INPUT, OUTPUT>(), Vector<Filter2<INPUT, OUTPUT, WEIGHTS>*>()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::VectorTransformFilterSet2(const VectorTransformFilterSet2& vt)
        : VectorTransform2<INPUT, OUTPUT>(vt), Vector<Filter2<INPUT, OUTPUT, WEIGHTS>*>(vt)
    {
        this->descriptor= vt.descriptor;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::~VectorTransformFilterSet2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        for ( int i= 0; i < this->size(); ++i )
        {
            (*this)(i)->apply(input, output(n+i), roi, support);
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>&, Image<OUTPUT>&, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    Border2 VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::getProposedBorder()
    {
        Border2 b;
        Border2 maxb;
        for ( int i= 0; i < this->size(); ++i )
        {
            b= (*this)(i)->getProposedBorder();
            maxb= maximizeBorders(b, maxb);
        }
        return maxb;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::dimensions()
    {
        return this->size();
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::string VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::getDescriptor(char space, int i)
    {
        if ( i == 0 )
        {
            std::string a= this->descriptor;
            for ( unsigned int i= 0; i < a.size(); ++i )
                if ( a[i] == ' ' )
                    a[i]= space;
            return a;
        }
        else
            return std::string("padding");
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::orthogonalize()
    {
        GramSchmidtOrthogonalization<WEIGHTS> gso;
        Vector<Vector<WEIGHTS> > input;
        input.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->getWeights(input(i));
        Vector<Vector<WEIGHTS> > output;
        gso.apply(input, output);
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->setWeights(output(i));
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::updateStride(int stride)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->computeMinMax();
        this->min= (*this)(0)->getMin();
        this->max= (*this)(0)->getMax();

        for ( int i= 0; i < this->size(); ++i )
        {
            cout << (*this)(i)->getDescriptor() << " " << (*this)(i)->getMin() << " " << (*this)(i)->getMax() << endl;
            if ( this->min > (*this)(i)->getMin() )
                this->min= (*this)(i)->getMin();
            if ( this->max < (*this)(i)->getMax() )
                this->max= (*this)(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    int VectorTransformFilterSet2<INPUT, OUTPUT, WEIGHTS>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    class GaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        GaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        GaborFilterR2V(const GaborFilterR2V& g);

        ~GaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2V<INPUT, OUTPUT>::GaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new GaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "GaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2V<INPUT, OUTPUT>::GaborFilterR2V(const GaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2V<INPUT, OUTPUT>::~GaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        CorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        CorrelationGaborFilterR2V(const CorrelationGaborFilterR2V& g);

        ~CorrelationGaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2V<INPUT, OUTPUT>::CorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "CorrelationGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2V<INPUT, OUTPUT>::CorrelationGaborFilterR2V(const CorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2V<INPUT, OUTPUT>::~CorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaborFilterPhasesR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        CorrelationGaborFilterPhasesR2V(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);

        CorrelationGaborFilterPhasesR2V(const CorrelationGaborFilterPhasesR2V& g);

        ~CorrelationGaborFilterPhasesR2V();
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>::CorrelationGaborFilterPhasesR2V(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= psi0; t <= psi1; t+= psis )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, t, gamma));

        stringstream ss;
        ss << "CorrelationGaborFilterPhasesR2V " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>::CorrelationGaborFilterPhasesR2V(const CorrelationGaborFilterPhasesR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>::~CorrelationGaborFilterPhasesR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class GaborFilterPhasesR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        GaborFilterPhasesR2V(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);

        GaborFilterPhasesR2V(const GaborFilterPhasesR2V& g);

        ~GaborFilterPhasesR2V();
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilterPhasesR2V<INPUT, OUTPUT>::GaborFilterPhasesR2V(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= psi0; t <= psi1; t+= psis )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, t, gamma));

        stringstream ss;
        ss << "GaborFilterPhasesR2V " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterPhasesR2V<INPUT, OUTPUT>::GaborFilterPhasesR2V(const GaborFilterPhasesR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterPhasesR2V<INPUT, OUTPUT>::~GaborFilterPhasesR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class OrthogonalGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        OrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        OrthogonalGaborFilterR2V(const OrthogonalGaborFilterR2V& g);

        ~OrthogonalGaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    OrthogonalGaborFilterR2V<INPUT, OUTPUT>::OrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "OrthogonalGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->orthogonalize();
    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalGaborFilterR2V<INPUT, OUTPUT>::OrthogonalGaborFilterR2V(const OrthogonalGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalGaborFilterR2V<INPUT, OUTPUT>::~OrthogonalGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class OrthogonalCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        OrthogonalCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        OrthogonalCorrelationGaborFilterR2V(const OrthogonalCorrelationGaborFilterR2V& g);

        ~OrthogonalCorrelationGaborFilterR2V();
    };

    template<typename INPUT, typename OUTPUT>
    OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::OrthogonalCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "OrthogonalCorrelationGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->orthogonalize();
    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::OrthogonalCorrelationGaborFilterR2V(const OrthogonalCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::~OrthogonalCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class OrthogonalLineSegmentFilter2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        OrthogonalLineSegmentFilter2V(int rows, int columns, float sigma, float theta0, float thetas, float theta1, float alpha, float beta, float delta, float gamma, int stride);

        OrthogonalLineSegmentFilter2V(const OrthogonalLineSegmentFilter2V& g);

        ~OrthogonalLineSegmentFilter2V();
    };

    template<typename INPUT, typename OUTPUT>
    OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::OrthogonalLineSegmentFilter2V(int rows, int columns, float sigma, float theta0, float step, float theta1, float alpha, float beta, float delta, float gamma, int stride)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeLineSegmentFilter<INPUT, OUTPUT>(rows, columns, sigma, t, alpha, beta, delta, gamma, stride));

        stringstream ss;
        ss << "OrthogonalLineSegmentFilter2V " << rows << " " << columns << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
        this->orthogonalize();
    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::OrthogonalLineSegmentFilter2V(const OrthogonalLineSegmentFilter2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {

    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::~OrthogonalLineSegmentFilter2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedOrthogonalLineSegmentFilter2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalLineSegmentFilter2V(int rows, int columns, float sigma, float theta0, float step, float theta1, float alpha, float beta, float delta, float gamma, int stride);

        RotatedOrthogonalLineSegmentFilter2V(const RotatedOrthogonalLineSegmentFilter2V& g);

        ~RotatedOrthogonalLineSegmentFilter2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeLineSegmentFilter<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::RotatedOrthogonalLineSegmentFilter2V(int rows, int columns, float sigma, float theta0, float step, float theta1, float alpha, float beta, float delta, float gamma, int stride)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeLineSegmentFilter<INPUT, OUTPUT>(rows, columns, sigma, theta0 + t, alpha, beta, delta, gamma, stride));

        stringstream ss;
        ss << "RotatedOrthogonalLineSegmentFilter2V " << rows << " " << columns << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>(rows, columns, sigma, theta0+t, step, theta1+t, alpha, beta, delta, gamma, stride));
            orientation.push_back(new FixedSizeLineSegmentFilter<INPUT, OUTPUT>(rows, columns, sigma, theta0 + t, alpha, beta, delta, gamma, stride));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::RotatedOrthogonalLineSegmentFilter2V(const RotatedOrthogonalLineSegmentFilter2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::~RotatedOrthogonalLineSegmentFilter2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;
        int begin= this->getMin();
        int end= this->getMax();

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
    void RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedOrthogonalGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedOrthogonalGaborFilterR2V(const RotatedOrthogonalGaborFilterR2V& g);

        ~RotatedOrthogonalGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedOrthogonalGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi, gamma));
            orientation.push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalGaborFilterR2V(const RotatedOrthogonalGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::~RotatedOrthogonalGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedOrthogonalCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedOrthogonalCorrelationGaborFilterR2V(const RotatedOrthogonalCorrelationGaborFilterR2V& g);

        ~RotatedOrthogonalCorrelationGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedOrthogonalCorrelationGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationGaborFilterR2V(const RotatedOrthogonalCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::~RotatedOrthogonalCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class MaxPhasedOrthogonalGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        MaxPhasedOrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        MaxPhasedOrthogonalGaborFilterR2V(const MaxPhasedOrthogonalGaborFilterR2V& g);

        ~MaxPhasedOrthogonalGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        //Vector<OrthogonalGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<OrthogonalGaborFilterR2V<INPUT, OUTPUT>* > filtersA;
        Vector<OrthogonalGaborFilterR2V<INPUT, OUTPUT>* > filtersB;
        Vector<FixedSizeGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::MaxPhasedOrthogonalGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        stringstream ss;
        ss << "MaxPhasedOrthogonalGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float t= theta0; t <= theta1; t+= step )
        {
            //filters.push_back(new OrthogonalGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi, gamma));
            filtersA.push_back(new OrthogonalGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi + M_PI/2, gamma));
            filtersB.push_back(new OrthogonalGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t + M_PI, step, theta1 + t + M_PI, lambda, psi + M_PI/2, gamma));
            orientation.push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::MaxPhasedOrthogonalGaborFilterR2V(const MaxPhasedOrthogonalGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::~MaxPhasedOrthogonalGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i)->computeMinMax();
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->computeMinMax();
            //filters(i)->computeMinMax();
            filtersA(i)->computeMinMax();
            filtersB(i)->computeMinMax();
        }
        this->min= 100000000000;
        this->max= -100000000000;

        for ( int i= 0; i < this->size(); ++i )
        {
            if ( this->min > filtersA(i)->getMin() )
                this->min= filtersA(i)->getMin();
            if ( this->max < filtersA(i)->getMax() )
                this->max= filtersA(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
    {
        if ( orientation(0)->stride == stride )
            return;
        for ( int i= 0; i < orientation.size(); ++i )
        {
            orientation(i)->updateStride(stride);
            //filters(i)->updateStride(stride);
            filtersA(i)->updateStride(stride);
            filtersB(i)->updateStride(stride);
        }
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    int MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::dimensions()
    {
        return (this->size());
    }

    template<typename INPUT, typename OUTPUT>
    void MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;
        int begin= this->getMin();
        int end= this->getMax();
        float a, b;

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
            for ( int j= 0; j < this->size(); ++j )
            {
                //output(n + j)(i)= (*(filters(mIdx)))(j)->apply(input, i, support);
                a= (*(filtersA(mIdx)))(j)->apply(input, i, support);
                b= (*(filtersB(mIdx)))(j)->apply(input, i, support);
                output(n + j)(i)= a > b ? a : b;
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    void MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;

        m= -FLT_MAX;
        mIdx= -1;
        float a, b;
        for ( int j= 0; j < orientation.size(); ++j )
        {
            tmp= orientation(j)->apply(input, k, support);
            if ( m < tmp )
            {
                m= tmp;
                mIdx= j;
            }
        }
        int fa= 0;
        if ( (*(filtersA(mIdx)))(0)->apply(input, k, support) > (*(filtersB(mIdx)))(0)->apply(input, k, support))
            fa= 1;
        for ( int j= 0; j < this->size(); ++j )
        {

            if ( fa )
                output(n + j)= (*(filtersA(mIdx)))(j)->apply(input, k, support);
            else
                output(n + j)= (*(filtersB(mIdx)))(j)->apply(input, k, support);
            //output(n + j + this->size())=
            //output(n + j + 2*(this->size()))=
        }
    }


    template<typename INPUT, typename OUTPUT>
    class RotatedOrthogonalCorrelationScalesGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalCorrelationScalesGaborFilterR2V(float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedOrthogonalCorrelationScalesGaborFilterR2V(const RotatedOrthogonalCorrelationScalesGaborFilterR2V& g);

        ~RotatedOrthogonalCorrelationScalesGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationScalesGaborFilterR2V(float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma1, t, lambda, psi, gamma));

        stringstream ss;
        ss << "RotatedOrthogonalCorrelationScalesGaborFilterR2V " << sigma0 << " " << sstep << " " << sigma1 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float s= sigma0; s <= sigma1; s+= sstep )
        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>(s, theta0 + t, step, theta1 + t, lambda, psi, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(s, theta0 + t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationScalesGaborFilterR2V(const RotatedOrthogonalCorrelationScalesGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::~RotatedOrthogonalCorrelationScalesGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedOrthogonalCorrelationLambdasGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalCorrelationLambdasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        RotatedOrthogonalCorrelationLambdasGaborFilterR2V(const RotatedOrthogonalCorrelationLambdasGaborFilterR2V& g);

        ~RotatedOrthogonalCorrelationLambdasGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationLambdasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda1, psi, gamma));

        stringstream ss;
        ss << "RotatedOrthogonalCorrelationLambdasGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float l= lambda0; l <= lambda1; l+= lambdas )
        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, l, psi, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, l, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationLambdasGaborFilterR2V(const RotatedOrthogonalCorrelationLambdasGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::~RotatedOrthogonalCorrelationLambdasGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedOrthogonalCorrelationPhasesGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedOrthogonalCorrelationPhasesGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);

        RotatedOrthogonalCorrelationPhasesGaborFilterR2V(const RotatedOrthogonalCorrelationPhasesGaborFilterR2V& g);

        ~RotatedOrthogonalCorrelationPhasesGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationPhasesGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));

        stringstream ss;
        ss << "RotatedOrthogonalCorrelationPhasesGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float p= psi0; p <= psi1; p+= psis )
        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new OrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, p, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, p, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::RotatedOrthogonalCorrelationPhasesGaborFilterR2V(const RotatedOrthogonalCorrelationPhasesGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::~RotatedOrthogonalCorrelationPhasesGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedCorrelationLambdasGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedCorrelationLambdasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        RotatedCorrelationLambdasGaborFilterR2V(const RotatedCorrelationLambdasGaborFilterR2V& g);

        ~RotatedCorrelationLambdasGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<CorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationLambdasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda1, psi, gamma));

        stringstream ss;
        ss << "RotatedCorrelationLambdasGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float l= lambda0; l <= lambda1; l+= lambdas )
        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new CorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, l, psi, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, l, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationLambdasGaborFilterR2V(const RotatedCorrelationLambdasGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::~RotatedCorrelationLambdasGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedCorrelationGammasGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedCorrelationGammasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma0, float gammas, float gamma1);

        RotatedCorrelationGammasGaborFilterR2V(const RotatedCorrelationGammasGaborFilterR2V& g);

        ~RotatedCorrelationGammasGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<CorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationGammasGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma0, float gammas, float gamma1)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t <= theta1; t+= step )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma1));

        stringstream ss;
        ss << "RotatedCorrelationGammasGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma0 << " " << gammas << " " << gamma1;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float g= gamma0; g <= gamma1; g+= gammas )
        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new CorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi, g));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, psi, g));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationGammasGaborFilterR2V(const RotatedCorrelationGammasGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::~RotatedCorrelationGammasGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        RotatedCorrelationGaborFilterR2V(const RotatedCorrelationGaborFilterR2V& g);

        ~RotatedCorrelationGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual Border2 getProposedBorder();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        Vector<CorrelationGaborFilterR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "RotatedCorrelationGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        this->orthogonalize();

        for ( float t= 0; t <= theta1 - theta0; t+= step )
        {
            filters.push_back(new CorrelationGaborFilterR2V<INPUT, OUTPUT>(sigma, theta0 + t, step, theta1 + t, lambda, psi, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0 + t, lambda, psi, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationGaborFilterR2V(const RotatedCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::~RotatedCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
            if ( this->min > orientation(i)->getMin() )
                this->min= orientation(i)->getMin();
            if ( this->max < orientation(i)->getMax() )
                this->max= orientation(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 m;
        for ( int i= 0; i < orientation.size(); ++i )
        {
            Border2 b= this->filters(i)->getProposedBorder();
            m= maximizeBorders(m, b);
            Border2 c= this->orientation(i)->getProposedBorder();
            m= maximizeBorders(m, c);
        }
        return m;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::dimensions()
    {
        return this->filters(0)->size();
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);

        float m, tmp;
        int mIdx;
        int begin= this->getMin();
        int end= this->getMax();

        for ( int i= -begin; i < input.n - end; ++i )
        {
            m= -FLT_MAX;
            mIdx= -1;

            if ( !roi || (*roi)(i) )
            {
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
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class RotatedCorrelationPhaseShiftedGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedCorrelationPhaseShiftedGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);

        RotatedCorrelationPhaseShiftedGaborFilterR2V(const RotatedCorrelationPhaseShiftedGaborFilterR2V& g);

        ~RotatedCorrelationPhaseShiftedGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationPhaseShiftedGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= psi0; t <= psi1; t+= psis )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, lambda, t, gamma));

        stringstream ss;
        ss << "RotatedCorrelationPhaseShiftedGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;

        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));
            orientation.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedCorrelationPhaseShiftedGaborFilterR2V(const RotatedCorrelationPhaseShiftedGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::~RotatedCorrelationPhaseShiftedGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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

        //printf("%f ", m); fflush(stdout);

        for ( int j= 0; j < filters(mIdx)->size(); ++j )
            output(n + j)= (*(filters(mIdx)))(j)->apply(input, k, support);
    }

    template<typename INPUT, typename OUTPUT>
    class RotatedPhaseShiftedGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        RotatedPhaseShiftedGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma);

        RotatedPhaseShiftedGaborFilterR2V(const RotatedPhaseShiftedGaborFilterR2V& g);

        ~RotatedPhaseShiftedGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<GaborFilterPhasesR2V<INPUT, OUTPUT>* > filters;
        Vector<FixedSizeGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedPhaseShiftedGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= psi0; t <= psi1; t+= psis )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, lambda, t, gamma));

        stringstream ss;
        ss << "RotatedPhaseShiftedGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
        this->orthogonalize();

        for ( float t= theta0; t <= theta1; t+= step )
        {
            filters.push_back(new GaborFilterPhasesR2V<INPUT, OUTPUT>(sigma, t, lambda, psi0, psis, psi1, gamma));
            orientation.push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::RotatedPhaseShiftedGaborFilterR2V(const RotatedPhaseShiftedGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::~RotatedPhaseShiftedGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    void RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    class FittedPhaseCorrelationGaborFilterR2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        FittedPhaseCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma, int res);

        FittedPhaseCorrelationGaborFilterR2V(const FittedPhaseCorrelationGaborFilterR2V& g);

        ~FittedPhaseCorrelationGaborFilterR2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        Vector<CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>* > filters;
        Vector<CorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::FittedPhaseCorrelationGaborFilterR2V(float sigma, float theta0, float step, float theta1, float lambda, float psi0, float psis, float psi1, float gamma, int res)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t < theta1; t+= (theta1 - theta0)/res + 0.01 )
            this->push_back(new CorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));

        stringstream ss;
        ss << "FittedPhaseCorrelationGaborFilterR2V " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma << " " << res;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;

        //for ( float p= psi0; p < psi1; p+= psis )
            for ( float t= theta0; t < theta1; t+= step )
            {
                filters.push_back(new CorrelationGaborFilterPhasesR2V<INPUT, OUTPUT>(sigma, t, lambda, psi0, (psi1 - psi0)/res + 0.01, psi1, gamma));
                orientation.push_back(new CorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));
            }
    }

    template<typename INPUT, typename OUTPUT>
    FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::FittedPhaseCorrelationGaborFilterR2V(const FittedPhaseCorrelationGaborFilterR2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::~FittedPhaseCorrelationGaborFilterR2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    void FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::computeMinMax()
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
    int FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>::updateStride(int stride)
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
    class SectionFilter2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        SectionFilter2V(float angle, float nstep, int n);

        SectionFilter2V(const SectionFilter2V& g);

        ~SectionFilter2V();
    };

    template<typename INPUT, typename OUTPUT>
    SectionFilter2V<INPUT, OUTPUT>::SectionFilter2V(float angle, float nstep, int n)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= -n*nstep; t <= n*nstep; t+= nstep )
            this->push_back(new SamplingFilter2<INPUT, OUTPUT>(angle, t, 4000));

        stringstream ss;
        ss << "SectionFilter2V " << angle << " " << nstep << " " << n;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;
    }

    template<typename INPUT, typename OUTPUT>
    SectionFilter2V<INPUT, OUTPUT>::SectionFilter2V(const SectionFilter2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    SectionFilter2V<INPUT, OUTPUT>::~SectionFilter2V()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class OrthogonalSectionFilter2V: public VectorTransformFilterSet2<INPUT, OUTPUT, float>
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

        OrthogonalSectionFilter2V(float nstep, int n, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        OrthogonalSectionFilter2V(const OrthogonalSectionFilter2V& g);

        ~OrthogonalSectionFilter2V();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support);

        virtual void updateStride(int stride);

        virtual void computeMinMax();

        virtual int getMin();

        virtual int getMax();

        virtual int dimensions();

        Vector<SectionFilter2V<INPUT, OUTPUT>* > filters;
        Vector<CorrelationGaborFilterR2<INPUT, OUTPUT>* > orientation;
    };

    template<typename INPUT, typename OUTPUT>
    OrthogonalSectionFilter2V<INPUT, OUTPUT>::OrthogonalSectionFilter2V(float nstep, int n, float sigma, float theta0, float step, float theta1, float lambda, float psi0, float gamma)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>()
    {
        stringstream ss;
        ss << "OrthogonalSectionFilter2V " << nstep << " " << n << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi0 <<  " " << gamma;
        descriptor= ss.str();
        cout << getDescriptor() << std::endl;

        for ( float t= theta0; t < theta1; t+= step )
        {
            filters.push_back(new SectionFilter2V<INPUT, OUTPUT>(t + M_PI/2, nstep, n));
            orientation.push_back(new CorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi0, gamma));
        }
    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalSectionFilter2V<INPUT, OUTPUT>::OrthogonalSectionFilter2V(const OrthogonalSectionFilter2V& g)
        : VectorTransformFilterSet2<INPUT, OUTPUT, float>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    OrthogonalSectionFilter2V<INPUT, OUTPUT>::~OrthogonalSectionFilter2V()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void OrthogonalSectionFilter2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    void OrthogonalSectionFilter2V<INPUT, OUTPUT>::apply(Image<INPUT>& input, int k, Vector<OUTPUT>& output, int n, Image<unsigned char>* support)
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
    void OrthogonalSectionFilter2V<INPUT, OUTPUT>::computeMinMax()
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
            if ( this->min > orientation(i)->getMin() )
                this->min= orientation(i)->getMin();
            if ( this->max < orientation(i)->getMax() )
                this->max= orientation(i)->getMax();
            if ( this->min > filters(i)->getMin() )
                this->min= filters(i)->getMin();
            if ( this->max < filters(i)->getMax() )
                this->max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int OrthogonalSectionFilter2V<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int OrthogonalSectionFilter2V<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void OrthogonalSectionFilter2V<INPUT, OUTPUT>::updateStride(int stride)
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
    int OrthogonalSectionFilter2V<INPUT, OUTPUT>::dimensions()
    {
        return filters(0)->size();
    }*/
}

#endif // VECTORTRANSFORM2S_H
