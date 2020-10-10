#ifndef ORIENTATIONESTIMATORS_H
#define ORIENTATIONESTIMATORS_H

#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/Filter2s.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/GradientVectorFlow.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class CorrelationGaborOrientationIndexFeature2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        CorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual int getMin();

        virtual int getMax();

        virtual Border2 getProposedBorder();

        virtual void computeMinMax();

        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > filters;

        float theta0;
        float thetas;
        float theta1;
        int min;
        int max;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::CorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        std::stringstream ss;
        ss << "CorrelationGaborOrientationIndexFeature2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        for ( float t= theta0; t < theta1; t+= thetas )
            filters.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        this->theta0= theta0;
        this->thetas= thetas;
        this->theta1= theta1;

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int k= 0;
        int maxIdx= 0;
        float maxCorr= -1;
        float c;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( k == filters.size() )
                return maxIdx;
            c= filters(k)->apply(input, n, support);
            if ( c > maxCorr )
            {
                maxCorr= c;
                maxIdx= k;
            }
            ++k;
        }

        return maxIdx;
    }

    template<typename INPUT, typename OUTPUT>
    int CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        {
            this->stride= stride;
            for ( int i= 0; i < filters.size(); ++i )
                filters(i)->updateStride(stride);
        }
        computeMinMax();
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        min= 1000000;
        max= -1000000;
        for ( int i= 0; i < filters.size(); ++i )
        {
            if ( filters(i)->getMin() < min )
                min= filters(i)->getMin();
            if ( filters(i)->getMax() > max )
                max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 CorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 m;
        for ( int i= 0; i < filters.size(); ++i )
        {
            Border2 b= filters(i)->getProposedBorder();
            m= maximizeBorders(b, m);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    class MultiScaleCorrelationGaborOrientationIndexFeature2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MultiScaleCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual int getMin();

        virtual int getMax();

        virtual Border2 getProposedBorder();

        virtual void computeMinMax();

        Vector<FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>* > filters;

        float theta0;
        float thetas;
        float theta1;
        float lambda0;
        float lambdas;
        float lambda1;
        int min;
        int max;
    };

    template<typename INPUT, typename OUTPUT>
    MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::MultiScaleCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        std::stringstream ss;
        ss << "MultiScaleCorrelationGaborOrientationIndexFeature2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        cout << descriptor << endl;

        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            filters.push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(l/sigma, t, l, psi, gamma));

        for ( int i= 0; i < filters.size(); ++i )
            cout << filters(i)->size() << " ";
        cout << endl;

        this->theta0= theta0;
        this->thetas= thetas;
        this->theta1= theta1;
        this->lambda0= lambda0;
        this->lambdas= lambdas;
        this->lambda1= lambda1;

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int k= 0;
        int maxIdx= 0;
        float maxCorr= -1;
        float c;
        for ( int i= 0; i < filters.size(); ++i )
        //for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( k == filters.size() )
                return maxIdx;
            c= filters(k)->apply(input, n, support);
            if ( c > maxCorr )
            {
                maxCorr= c;
                maxIdx= k;
            }
            ++k;
        }

        return maxIdx;
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        {
            this->stride= stride;
            for ( int i= 0; i < filters.size(); ++i )
                filters(i)->updateStride(stride);
        }
        computeMinMax();
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        min= 1000000;
        max= -1000000;
        for ( int i= 0; i < filters.size(); ++i )
        {
            if ( filters(i)->getMin() < min )
                min= filters(i)->getMin();
            if ( filters(i)->getMax() > max )
                max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 MultiScaleCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 m;
        for ( int i= 0; i < filters.size(); ++i )
        {
            Border2 b= filters(i)->getProposedBorder();
            m= maximizeBorders(b, m);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    class CCorrelationGaborOrientationIndexFeature2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        CCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual int getMin();

        virtual int getMax();

        virtual Border2 getProposedBorder();

        virtual void computeMinMax();

        Vector<FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>* > filters;

        float theta0;
        float thetas;
        float theta1;
        int min;
        int max;
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::CCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        std::stringstream ss;
        ss << "CCorrelationGaborOrientationIndexFeature2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        for ( float t= theta0; t < theta1; t+= thetas )
            filters.push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));

        this->theta0= theta0;
        this->thetas= thetas;
        this->theta1= theta1;

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int k= 0;
        int maxIdx= 0;
        float maxCorr= -1;
        float c;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( k == int(filters.size()) )
                return maxIdx;
            c= filters(k)->apply(input, n, support);
            if ( c > maxCorr )
            {
                maxCorr= c;
                maxIdx= k;
            }
            ++k;
        }

        return maxIdx;
    }

    template<typename INPUT, typename OUTPUT>
    int CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        {
            this->stride= stride;
            for ( unsigned int i= 0; i < filters.size(); ++i )
                filters(i)->updateStride(stride);
        }
        computeMinMax();
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        min= 1000000;
        max= -1000000;
        for ( unsigned int i= 0; i < filters.size(); ++i )
        {
            if ( filters(i)->getMin() < min )
                min= filters(i)->getMin();
            if ( filters(i)->getMax() > max )
                max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 CCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 m;
        for ( unsigned int i= 0; i < filters.size(); ++i )
        {
            Border2 b= filters(i)->getProposedBorder();
            m= maximizeBorders(b, m);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    class MultiScaleCCorrelationGaborOrientationIndexFeature2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MultiScaleCCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        virtual int updateStride(int stride);

        virtual int getMin();

        virtual int getMax();

        virtual Border2 getProposedBorder();

        virtual void computeMinMax();

        Vector<FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>* > filters;

        float theta0;
        float thetas;
        float theta1;
        float lambda0;
        float lambdas;
        float lambda1;
        int min;
        int max;
    };

    template<typename INPUT, typename OUTPUT>
    MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::MultiScaleCCorrelationGaborOrientationIndexFeature2(float sigma, float theta0, float thetas, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        std::stringstream ss;
        ss << "MultiscaleCCorrelationGaborOrientationIndexFeature2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();

        cout << descriptor << endl;

        for ( float l= lambda0; l < lambda1; l+= lambdas )
        for ( float t= theta0; t < theta1; t+= thetas )
            //for ( float p= psi - 3*M_PI/6; p <= psi + 3*M_PI/6 + 0.1; p += M_PI/6 )
            filters.push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(l/sigma, t, l, psi, gamma));
                //filters.push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(l/sigma, t, l, p, gamma));

        for ( unsigned int i= 0; i < filters.size(); ++i )
            cout << filters(i)->size() << " ";
        cout << endl;

        this->theta0= theta0;
        this->thetas= thetas;
        this->theta1= theta1;
        this->lambda0= lambda0;
        this->lambdas= lambdas;
        this->lambda1= lambda1;

        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        int k= 0;
        int maxIdx= 0;
        float maxCorr= -FLT_MAX;
        float c;
        for ( unsigned int i= 0; i < filters.size(); ++i )
        //for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( k == int(filters.size()) )
                return maxIdx;
            c= filters(k)->apply(input, n, support);
            if ( c > maxCorr )
            {
                maxCorr= c;
                maxIdx= k;
            }
            ++k;
        }

        return maxIdx;
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        {
            this->stride= stride;
            for ( unsigned int i= 0; i < filters.size(); ++i )
                filters(i)->updateStride(stride);
        }
        computeMinMax();
	return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        min= 1000000;
        max= -1000000;
        for ( unsigned int i= 0; i < filters.size(); ++i )
        {
            if ( filters(i)->getMin() < min )
                min= filters(i)->getMin();
            if ( filters(i)->getMax() > max )
                max= filters(i)->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMin()
    {
        return min;
    }

    template<typename INPUT, typename OUTPUT>
    int MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getMax()
    {
        return max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 MultiScaleCCorrelationGaborOrientationIndexFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 m;
        for ( unsigned int i= 0; i < filters.size(); ++i )
        {
            Border2 b= filters(i)->getProposedBorder();
            m= maximizeBorders(b, m);
        }

        return m;
    }

    template<typename INPUT, typename OUTPUT>
    class PCADirectionFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        PCADirectionFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    PCADirectionFeature2<INPUT, OUTPUT>::PCADirectionFeature2(int r, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "PCADirectionFeature2 " << r << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT PCADirectionFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
        int tmpr, tmpc;
        float meanr, meanc;
        float a, b, c, d;
        meanr= meanc= 0;
        float sum= 0;
        INPUT intensity;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            this->getXY((*this)(i), tmpr, tmpc);
            intensity= -input(n + (*this)(i));
            //printf("%d,%d\n", tmpr, tmpc);
            meanr+= intensity * tmpr;
            meanc+= intensity * tmpc;
            sum+= intensity;
        }
        meanr/= sum;
        meanc/= sum;
        //printf("%f,%f,%f\n", meanr, meanc, sum); fflush(stdout);
        a= b= c= d= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            this->getXY((*this)(i), tmpr, tmpc);
            intensity= -input(n + (*this)(i));
            a+= intensity*(tmpr - meanr)*(tmpr - meanr);
            d+= intensity*(tmpc - meanc)*(tmpc - meanc);
            b+= intensity*(tmpr - meanr)*(tmpc - meanc);
        }
        c= b;

        a/= sum;
        b/= sum;
        c/= sum;
        d/= sum;

        float l1/*, l2*/;
        float discr= (a + d)*(a + d) - 4*(a*d - b*c);
        if ( discr < 0 )
            return 0;
        l1= (a + d + sqrt(discr))/2;
        //l2= (a + d - sqrt(discr))/2;

        float v1r= 1;
        float v1c= c / (l1 - d);
        //float v2r= 1;
        //float v2c= c / (l2 - d);

        return atan2(v1r, v1c);
    }

    template<typename INPUT, typename OUTPUT>
    class AveragePCADirectionFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        AveragePCADirectionFeature2(int r, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        PCADirectionFeature2<INPUT, OUTPUT>* pcad;
    };

    template<typename INPUT, typename OUTPUT>
    AveragePCADirectionFeature2<INPUT, OUTPUT>::AveragePCADirectionFeature2(int r, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r + 1, stride)
    {
        std::stringstream ss;
        ss << "AveragePCADirectionFeature2 " << r << " " << stride;
        descriptor= ss.str();
        pcad= new PCADirectionFeature2<INPUT, OUTPUT>(r, stride);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT AveragePCADirectionFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float tmpr= 0;
        float tmpc= 0;

        float angle;
        angle= pcad->apply(input, n - input.columns, support);
        tmpr+= sin(angle);
        tmpc+= cos(angle);
        angle= pcad->apply(input, n + input.columns, support);
        tmpr+= sin(angle);
        tmpc+= cos(angle);
        angle= pcad->apply(input, n - 1, support);
        tmpr+= sin(angle);
        tmpc+= cos(angle);
        angle= pcad->apply(input, n + 1, support);
        tmpr+= sin(angle);
        tmpc+= cos(angle);
        angle= pcad->apply(input, n, support);
        tmpr+= sin(angle);
        tmpc+= cos(angle);
        tmpr/= 5;
        tmpc/= 5;

        return atan2(tmpr, tmpc);
    }

    template<typename INPUT, typename OUTPUT>
    class PCAOrientationIndexFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        PCAOrientationIndexFeature2(int r, float theta0, float thetas, float theta1, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float theta0;
        float theta1;
        float thetas;

        AveragePCADirectionFeature2<INPUT, OUTPUT>* pcad;
    };

    template<typename INPUT, typename OUTPUT>
    PCAOrientationIndexFeature2<INPUT, OUTPUT>::PCAOrientationIndexFeature2(int r, float theta0, float thetas, float theta1, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "PCAOrientationIndexFeature2 " << r << " " << theta0 << " " << thetas << " " << theta1 << " " << stride;
        descriptor= ss.str();
        this->theta0= theta0;
        this->theta1= theta1;
        this->thetas= thetas;
        pcad= new AveragePCADirectionFeature2<INPUT, OUTPUT>(r, stride);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT PCAOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float angle= pcad->apply(input, n, support);
        int k= 0;
        if ( angle < theta0 )
            angle+= M_PI;
        if ( angle > theta1 )
            angle-= M_PI;

        if ( angle < theta0 )
            return 0;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( t <= angle && angle < t + thetas )
                return k;
            ++k;
        }
        return k-1;
    }

    template<typename INPUT, typename OUTPUT>
    class AveragePCAOrientationIndexFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        AveragePCAOrientationIndexFeature2(int r, float theta0, float thetas, float theta1, int stride= 4000);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float theta0;
        float theta1;
        float thetas;

        AveragePCADirectionFeature2<INPUT, OUTPUT>* pcad;
    };

    template<typename INPUT, typename OUTPUT>
    AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>::AveragePCAOrientationIndexFeature2(int r, float theta0, float thetas, float theta1, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "AveragePCAOrientationIndexFeature2 " << r << " " << theta0 << " " << thetas << " " << theta1 << " " << stride;
        descriptor= ss.str();
        this->theta0= theta0;
        this->theta1= theta1;
        this->thetas= thetas;
        pcad= new AveragePCADirectionFeature2<INPUT, OUTPUT>(r, stride);
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT AveragePCAOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float angle= pcad->apply(input, n, support);
        int k= 0;
        while ( angle < theta0 )
            angle+= M_PI;
        while ( angle > theta1 )
            angle-= M_PI;

        if ( angle < theta0 )
            return 0;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( t <= angle && angle < t + thetas )
                return k;
            ++k;
        }
        return k-1;
    }

    template<typename INPUT, typename OUTPUT>
    class StructureTensorOrientationIndexFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        StructureTensorOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride= 4000);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float computeGx(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float computeGy(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float theta0;
        float theta1;
        float thetas;
        float sigma;

        Vector<float> weights;
    };

    template<typename INPUT, typename OUTPUT>
    StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>::StructureTensorOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "StructureTensorOrientationIndexFeature2 " << r << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << stride;
        descriptor= ss.str();
        this->theta0= theta0;
        this->theta1= theta1;
        this->thetas= thetas;

        int tmpr, tmpc;
        weights.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
        {
            this->getXY((*this)(i), tmpr, tmpc);
            weights(i)= gauss2(tmpr, tmpc, sigma, 1, 1, 1, 0);
        }
    }

    template<typename INPUT, typename OUTPUT>
    float StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>::computeGy(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float res= 0;
        if ( !support || (*support)(n - input.columns - 1) )
            res+= -input(n - input.columns - 1);
        if ( !support || (*support)(n - input.columns) )
            res+= -2*input(n - input.columns);
        if ( !support || (*support)(n - input.columns + 1) )
            res+= -input(n - input.columns + 1);
        if ( !support || (*support)(n + input.columns - 1) )
            res+= input(n + input.columns - 1);
        if ( !support || (*support)(n + input.columns) )
            res+= 2*input(n + input.columns);
        if ( !support || (*support)(n + input.columns + 1) )
            res+= input(n + input.columns + 1);
        return res;
    }

    template<typename INPUT, typename OUTPUT>
    float StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>::computeGx(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float res= 0;
        if ( !support || (*support)(n - input.columns - 1) )
            res+= -input(n - input.columns - 1);
        if ( !support || (*support)(n - 1) )
            res+= -2*input(n - 1);
        if ( !support || (*support)(n + input.columns - 1) )
            res+= -input(n + input.columns - 1);

        if ( !support || (*support)(n - input.columns + 1) )
            res+= input(n - input.columns + 1);
        if ( !support || (*support)(n + 1) )
            res+= 2*input(n + 1);
        if ( !support || (*support)(n + input.columns + 1) )
            res+= input(n + input.columns + 1);

        return res;
    }

    template<typename INPUT, typename OUTPUT>
    void StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        this->computeMinMax();

        int begin= -(int)(this->getMin());
        int end= input.n - (int)(this->getMax());
        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT StructureTensorOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float gx, gy;
        float gx2, gy2, gxgy;
        float sum;
        float length;
        gx2= gy2= gxgy= sum= 0;

        for ( int i= 0; i < this->size(); ++i )
        {
            gx= computeGx(input, n + (*this)(i), support);
            gy= computeGy(input, n + (*this)(i), support);
            length= sqrt(gx*gx + gy*gy);
            /*gx2+= weights(i)*gx*gx*length*length;
            gy2+= weights(i)*gy*gy*length*length;
            gxgy+= weights(i)*gx*gy*length*length;
            sum+= weights(i)*length*length;*/
            gx2+= gx*gx*length*length*length;
            gy2+= gy*gy*length*length*length;
            gxgy+= gx*gy*length*length*length;
            sum+= length*length*length;
        }

        gx2/= sum;
        gy2/= sum;
        gxgy/= sum;

        float angle;

        float trace= gx2 + gy2;
        float det= gx2*gy2 - gxgy*gxgy;

        float d= sqrt(max(0.0, 0.25*trace*trace - det));
        float lmax= max(0.0, 0.5*trace + d);
        float lmin= max(0.0, 0.5*trace - d);

        float v11= lmax - gy2; float v21= gxgy;
        float l1= sqrt(v11*v11 + v21*v21);
        v11/= l1;
        v21/= l1;
        float v12= lmin - gy2; float v22= gxgy;
        float l2= sqrt(v12*v12 + v22*v22);
        v12/= l2;
        v22/= l2;

        if ( fabs(lmax) > fabs(lmin) )
            angle= atan2(v11, v21);
        else
            angle= atan2(v12, v22);

        //angle+= M_PI/2;

        int k= 0;
        if ( angle < theta0 )
            angle+= M_PI;
        if ( angle < theta0 )
            angle+= M_PI;
        if ( angle > theta1 )
            angle-= M_PI;
        if ( angle > theta1 )
            angle-= M_PI;
        angle+= M_PI/2;

        if ( angle < theta0 )
            return 0;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( t <= angle && angle < t + thetas )
                return (float)k;
            ++k;
        }
        return (float)(k-1);
    }

    template<typename INPUT, typename OUTPUT>
    class MaxGradientOrientationIndexFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        MaxGradientOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride= 4000);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float computeGx(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float computeGy(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float theta0;
        float theta1;
        float thetas;
        float sigma;

        Vector<float> weights;
    };

    template<typename INPUT, typename OUTPUT>
    MaxGradientOrientationIndexFeature2<INPUT, OUTPUT>::MaxGradientOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "MaxGradientOrientationIndexFeature2 " << r << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << stride;
        descriptor= ss.str();
        this->theta0= theta0;
        this->theta1= theta1;
        this->thetas= thetas;

        int tmpr, tmpc;
        weights.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
        {
            this->getXY((*this)(i), tmpr, tmpc);
            weights(i)= gauss2(tmpr, tmpc, sigma, 1, 1, 1, 0);
        }
    }

    template<typename INPUT, typename OUTPUT>
    float MaxGradientOrientationIndexFeature2<INPUT, OUTPUT>::computeGy(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float res= 0;
        if ( !support || (*support)(n - input.columns - 1) )
            res+= -input(n - input.columns - 1);
        if ( !support || (*support)(n - input.columns) )
            res+= -2*input(n - input.columns);
        if ( !support || (*support)(n - input.columns + 1) )
            res+= -input(n - input.columns + 1);
        if ( !support || (*support)(n + input.columns - 1) )
            res+= input(n + input.columns - 1);
        if ( !support || (*support)(n + input.columns) )
            res+= 2*input(n + input.columns);
        if ( !support || (*support)(n + input.columns + 1) )
            res+= input(n + input.columns + 1);
        return res;
    }

    template<typename INPUT, typename OUTPUT>
    float MaxGradientOrientationIndexFeature2<INPUT, OUTPUT>::computeGx(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float res= 0;
        if ( !support || (*support)(n - input.columns - 1) )
            res+= -input(n - input.columns - 1);
        if ( !support || (*support)(n - 1) )
            res+= -2*input(n - 1);
        if ( !support || (*support)(n + input.columns - 1) )
            res+= -input(n + input.columns - 1);

        if ( !support || (*support)(n - input.columns + 1) )
            res+= input(n - input.columns + 1);
        if ( !support || (*support)(n + 1) )
            res+= 2*input(n + 1);
        if ( !support || (*support)(n + input.columns + 1) )
            res+= input(n + input.columns + 1);

        return res;
    }

    template<typename INPUT, typename OUTPUT>
    void MaxGradientOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        this->computeMinMax();

        int begin= -(int)(this->getMin());
        int end= input.n - (int)(this->getMax());
        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MaxGradientOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float gx, gy;
        float gx2, gy2, gxgy;
        float sum;
        float length;
        gx2= gy2= gxgy= sum= 0;

        float angleMax= 0;
        float magnMax= 0;
        for ( int i= 0; i < this->size(); ++i )
        {
            gx= computeGx(input, n + (*this)(i), support);
            gy= computeGy(input, n + (*this)(i), support);
            length= sqrt(gx*gx + gy*gy);
            if ( length > magnMax )
            {
                magnMax= length;
                angleMax= atan2(gy, gx);
            }
        }
        /*gx= computeGx(input, n, support);
        gy= computeGy(input, n, support);
        angleMax= atan2(gy, gx);*/

        float angle= angleMax;
        angle+= M_PI/2;

        int k= 0;
        if ( angle < theta0 )
            angle+= M_PI;
        if ( angle < theta0 )
            angle+= M_PI;
        if ( angle > theta1 )
            angle-= M_PI;
        if ( angle > theta1 )
            angle-= M_PI;

        if ( angle < theta0 )
            return 0;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( t <= angle && angle < t + thetas )
                return (float)k;
            ++k;
        }
        return (float)(k-1);
    }

    template<typename INPUT, typename OUTPUT>
    class GVFOrientationIndexFeature2: public StatisticalFeature2<INPUT, OUTPUT>
    {
    public:
        using StatisticalFeature2<INPUT, OUTPUT>::descriptor;
        using StatisticalFeature2<INPUT, OUTPUT>::getMin;
        using StatisticalFeature2<INPUT, OUTPUT>::getMax;
        using StatisticalFeature2<INPUT, OUTPUT>::stride;
        using StatisticalFeature2<INPUT, OUTPUT>::updateStride;
        using StatisticalFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using StatisticalFeature2<INPUT, OUTPUT>::apply;
        using StatisticalFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        GVFOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride= 4000);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        float theta0;
        float theta1;
        float thetas;
        float sigma;
        Image<INPUT>* pointer;
        Image<OUTPUT> angles;
    };

    template<typename INPUT, typename OUTPUT>
    GVFOrientationIndexFeature2<INPUT, OUTPUT>::GVFOrientationIndexFeature2(int r, float sigma, float theta0, float thetas, float theta1, int stride)
        : StatisticalFeature2<INPUT, OUTPUT>(r, stride)
    {
        std::stringstream ss;
        ss << "GVFOrientationIndexFeature2 " << r << " " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << stride;
        descriptor= ss.str();
        this->theta0= theta0;
        this->theta1= theta1;
        this->thetas= thetas;
        this->sigma= sigma;
        this->pointer= NULL;
    }

    template<typename INPUT, typename OUTPUT>
    void GVFOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        this->computeMinMax();

        int begin= -(int)(this->getMin());
        int end= input.n - (int)(this->getMax());
        if ( roi == NULL )
        {
            for ( int i= begin; i < end; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            for ( int i= begin; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= apply(input, i, support);
        }
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GVFOrientationIndexFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        if ( &input != pointer )
        {
            pointer= &input;
            angles.resizeImage(input);
            GradientVectorFlow<INPUT, OUTPUT> gvf(sigma);
            gvf.apply(input, angles, support, support);
        }

        float angle= angles(n);
        angle= angle + M_PI/2;

        int k= 0;
        while ( angle < theta0 )
            angle+= M_PI;
        while ( angle > theta1 )
            angle-= M_PI;

        if ( angle < theta0 )
            return 0;
        for ( float t= theta0; t < theta1; t+= thetas)
        {
            if ( t <= angle && angle < t + thetas )
                return (float)k;
            ++k;
        }
        return (float)(k-1);
    }
}

#endif // ORIENTATIONESTIMATORS_H
