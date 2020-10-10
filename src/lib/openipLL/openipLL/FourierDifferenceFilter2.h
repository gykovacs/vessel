/* 
 * File:   FourierDifferenceFilter2.h
 * Author: gykovacs
 *
 * Created on 2011. június 8., 10:12
 */

#ifndef FOURIERDIFFERENCEFILTER2_H
#define	FOURIERDIFFERENCEFILTER2_H

#include <openipLL/FourierFilterSet2.h>
#include <openipLL/FourierMatchedFilter2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierFilterSet2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * default constructor
         */
        DifferenceFourierFilterSet2();

        /**
         * constructor, to set the features whose difference will be computed at
         * a given pixel
         * @param f1 feature 1
         * @param f2 feature 2
         */
        DifferenceFourierFilterSet2(FourierFilterSet2<INPUT, OUTPUT>* f1, FourierFilterSet2<INPUT, OUTPUT>* f2);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierFilterSet2(const DifferenceFourierFilterSet2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierFilterSet2();

        /**
         * computes the difference of features f1 and f2 in the image input where the mask contains
         * non-zero elements, if no mask is set, the features are computed in the whole image
         * @param input input image
         * @param output output image containing the result
         * @param mask the operation is performed in the foreground (non-zero) region
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * f1-f2 is computed
         */
        FourierFilterSet2<INPUT, OUTPUT>* f1;

        /**
         * f1-f2 is computed
         */
        FourierFilterSet2<INPUT, OUTPUT>* f2;
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierFilterSet2<INPUT, OUTPUT>::DifferenceFourierFilterSet2()
    : f1(NULL), f2(NULL)
    {
    }

    /**
     * constructor, to set the features whose difference will be computed at
     * a given pixel
     * @param f1 feature 1
     * @param f2 feature 2
     */
    template<typename INPUT, typename OUTPUT>
    DifferenceFourierFilterSet2<INPUT, OUTPUT>::DifferenceFourierFilterSet2(FourierFilterSet2<INPUT, OUTPUT>* f1, FourierFilterSet2<INPUT, OUTPUT>* f2)
    {
        this->f1= f1;
        this->f2= f2;
    }

    /**
     * copy constructor
     * @param df feature to copy
     */
    template<typename INPUT, typename OUTPUT>
    DifferenceFourierFilterSet2<INPUT, OUTPUT>::DifferenceFourierFilterSet2(const DifferenceFourierFilterSet2<INPUT, OUTPUT>& df)
    {
        this->f1= df.f1;
        this->f2= df.f2;
    }

    /**
     * destructor
     */
    template<typename INPUT, typename OUTPUT>
    DifferenceFourierFilterSet2<INPUT, OUTPUT>::~DifferenceFourierFilterSet2()
    {
        if ( f1 )
            delete f1;
        if ( f2 )
            delete f2;
    }

    /**
     * computes the difference of features f1 and f2 in the image input where the mask contains
     * non-zero elements, if no mask is set, the features are computed in the whole image
     * @param input input image
     * @param output output image containing the result
     * @param mask the operation is performed in the foreground (non-zero) region
     */
    template<typename INPUT, typename OUTPUT>
    void DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        Image<OUTPUT> tmp;
        tmp.resizeImage(output);
        f1->apply(input, tmp, roi, support);
        f2->apply(input, output, roi, support);
        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= tmp(i) - output(i);
    }


    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedLineSegmentFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedLineSegmentFilter2(int rows, int columns, float sigma0, float sigma1, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedLineSegmentFilter2(const DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedLineSegmentFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedLineSegmentFilter2(int rows, int columns, float sigma0, float sigma1, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma0, angle0, angleStep, angle1, alpha, beta, delta, gamma), new FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma1, angle0, angleStep, angle1, alpha, beta, delta, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedLineSegmentFilter2 " << rows << " " << columns << " " << sigma0 << " " << sigma1 << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedLineSegmentFilter2(const DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedLineSegmentFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedCorrelationLineSegmentFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma0, float sigma1, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedCorrelationLineSegmentFilter2(const DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedCorrelationLineSegmentFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma0, float sigma1, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma0, angle0, angleStep, angle1, alpha, beta, delta, gamma), new FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma1, angle0, angleStep, angle1, alpha, beta, delta, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedCorrelationLineSegmentFilter2 " << rows << " " << columns << " " << sigma0 << " " << sigma1 << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationLineSegmentFilter2(const DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedCorrelationLineSegmentFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedGaussianFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedGaussianFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedGaussianFilter2(const DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedGaussianFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1, float alpha, float beta, float delta, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedGaussianFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1, alpha, beta, delta, gamma), new FourierMatchedGaussianFilter2<INPUT, OUTPUT>(kernel, sigmab0, stepb, sigmab1, alpha, beta, delta, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedGaussianFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1 << " " << sigmab0 << " " << stepb << " " << sigmab1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedGaussianFilter2(const DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedCorrelationGaussianFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedCorrelationGaussianFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedCorrelationGaussianFilter2(const DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedCorrelationGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationGaussianFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1, float alpha, float beta, float delta, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1, alpha, beta, delta, gamma), new FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(kernel, sigmab0, stepb, sigmab1, alpha, beta, delta, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedCorrelationGaussianFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1 << " " << sigmab0 << " " << stepb << " " << sigmab1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationGaussianFilter2(const DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedCorrelationGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedLoGFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedLoGFilter2(const DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedLoGFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedLoGFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1), new FourierMatchedLoGFilter2<INPUT, OUTPUT>(kernel, sigmab0, stepb, sigmab1))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1 << " " << sigmab0 << " " << stepb << " " << sigmab1;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedLoGFilter2(const DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedCorrelationLoGFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedCorrelationLoGFilter2(const DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedCorrelationLoGFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1, float sigmab0, float stepb, float sigmab1)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1), new FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>(kernel, sigmab0, stepb, sigmab1))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1 << " " << sigmab0 << " " << stepb << " " << sigmab1;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationLoGFilter2(const DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedCorrelationLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedGaborFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedGaborFilter2(float sigma0, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedGaborFilter2(const DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedGaborFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedGaborFilter2(float sigma0, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedGaborFilter2<INPUT, OUTPUT>(sigma1, theta0, step, theta1, lambda, psi, gamma), new FourierMatchedGaborFilter2<INPUT, OUTPUT>(sigma0, theta0, step, theta1, lambda, psi, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedGaborFilter2 " << sigma0 << " " << sigma1 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedGaborFilter2(const DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedGaborFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceFourierMatchedCorrelationGaborFilter2: public DifferenceFourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::apply;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f1;
        using DifferenceFourierFilterSet2<INPUT, OUTPUT>::f2;

        /**
         * default constructor
         */
        DifferenceFourierMatchedCorrelationGaborFilter2(float sigma0, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFourierMatchedCorrelationGaborFilter2(const DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFourierMatchedCorrelationGaborFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationGaborFilter2(float sigma0, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(new FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>(sigma0, theta0, step, theta1, lambda, psi, gamma), new FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>(sigma1, theta0, step, theta1, lambda, psi, gamma))
    {
        std::stringstream ss;
        ss << "DifferenceFourierMatchedCorrelationGaborFilter2 " << sigma0 << " " << sigma1 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::DifferenceFourierMatchedCorrelationGaborFilter2(const DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>& df)
    : DifferenceFourierFilterSet2<INPUT, OUTPUT>(df)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::~DifferenceFourierMatchedCorrelationGaborFilter2()
    {
    }
}
#endif	/* FOURIERDIFFERENCEFILTER2_H */

