/* 
 * File:   FourierMatchedFilter2s.h
 * Author: gykovacs
 *
 * Created on 2011. június 7., 23:45
 */

#ifndef FOURIERMATCHEDFILTER2S_H
#define	FOURIERMATCHEDFILTER2S_H

#include <openipLL/FourierFilterSet2.h>
#include <openipDS/Transform2.h>
#include <openipDS/MatchedGaborFilter2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class FourierMatchedLineSegmentFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        FourierMatchedLineSegmentFilter2(const FourierMatchedLineSegmentFilter2& f);

        ~FourierMatchedLineSegmentFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::FourierMatchedLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedLineSegmentFilter2<INPUT, OUTPUT>* m= new MatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma, angle0, angleStep, angle1, alpha, beta, delta, gamma);
        this->addFilterSet2(m);
        std::stringstream ss;
        ss << "FourierMatchedLineSegmentFilter2 " << rows << " " << columns << " " << sigma << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::FourierMatchedLineSegmentFilter2(const FourierMatchedLineSegmentFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::~FourierMatchedLineSegmentFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedCorrelationLineSegmentFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        //using FourierFilterSet2<INPUT, OUTPUT>::applyMin;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        FourierMatchedCorrelationLineSegmentFilter2(const FourierMatchedCorrelationLineSegmentFilter2& f);

        ~FourierMatchedCorrelationLineSegmentFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedLineSegmentFilter2<INPUT, OUTPUT>* m= new MatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma, angle0, angleStep, angle1, alpha, beta, delta, gamma);
        this->addFilterSet2(m, true);
        std::stringstream ss;
        ss << "FourierMatchedCorrelationLineSegmentFilter2 " << rows << " " << columns << " " << sigma << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        (*this).Transform2<INPUT, OUTPUT>::descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationLineSegmentFilter2(const FourierMatchedCorrelationLineSegmentFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::~FourierMatchedCorrelationLineSegmentFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }


    template<typename INPUT, typename OUTPUT>
    class FourierMatchedGaussianFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        //using FourierFilterSet2<INPUT, OUTPUT>::applyMin;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedGaussianFilter2(int kernel, float alpha, float beta, float delta, float gamma, float sigma0, float step, float sigma1);

        FourierMatchedGaussianFilter2(const FourierMatchedGaussianFilter2& f);

        ~FourierMatchedGaussianFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaussianFilter2<INPUT, OUTPUT>::FourierMatchedGaussianFilter2(int kernel, float alpha, float beta, float delta, float gamma, float sigma0, float step, float sigma1)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedGaussianFilter2<INPUT, OUTPUT>* m= new MatchedGaussianFilter2<INPUT, OUTPUT>(kernel, sigma0, alpha, beta, delta, gamma, 4000);
        m->setMatchType(sigma0, step, sigma1, MATCHED_GAUSSIAN_FILTER_SIGMA);
        this->addFilterSet2(m);
        std::stringstream ss;
        ss << "FourierMatchedGaussianFilter2 " << kernel << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << sigma0 << " " << step << " " << sigma1;
        (*this).Transform2<INPUT, OUTPUT>::descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaussianFilter2<INPUT, OUTPUT>::FourierMatchedGaussianFilter2(const FourierMatchedGaussianFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaussianFilter2<INPUT, OUTPUT>::~FourierMatchedGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedGaussianFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedCorrelationGaussianFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        //using FourierFilterSet2<INPUT, OUTPUT>::applyMin;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedCorrelationGaussianFilter2(int kernel, float sigma0, float step, float sigma1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        FourierMatchedCorrelationGaussianFilter2(const FourierMatchedCorrelationGaussianFilter2& f);

        ~FourierMatchedCorrelationGaussianFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationGaussianFilter2(int kernel, float alpha, float beta, float delta, float gamma, float sigma0, float step, float sigma1 )
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedGaussianFilter2<INPUT, OUTPUT>* m= new MatchedGaussianFilter2<INPUT, OUTPUT>(kernel, sigma0, alpha, beta, delta, gamma, 4000);
        m->setMatchType(sigma0, step, sigma1, MATCHED_GAUSSIAN_FILTER_SIGMA);
        this->addFilterSet2(m, true);
        std::stringstream ss;
        ss << "FourierMatchedCorrelationGaussianFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        (*this).Transform2<INPUT, OUTPUT>::descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationGaussianFilter2(const FourierMatchedCorrelationGaussianFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::~FourierMatchedCorrelationGaussianFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedLoGFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        //using FourierFilterSet2<INPUT, OUTPUT>::applyMin;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1);

        FourierMatchedLoGFilter2(const FourierMatchedLoGFilter2& f);

        ~FourierMatchedLoGFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLoGFilter2<INPUT, OUTPUT>::FourierMatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedLoGFilter2<INPUT, OUTPUT>* m= new MatchedLoGFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1);
        this->addFilterSet2(m);
        std::stringstream ss;
        ss << "FourierMatchedLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1;
        (*this).Transform2<INPUT, OUTPUT>::descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLoGFilter2<INPUT, OUTPUT>::FourierMatchedLoGFilter2(const FourierMatchedLoGFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedLoGFilter2<INPUT, OUTPUT>::~FourierMatchedLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedLoGFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedCorrelationLoGFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        //using FourierFilterSet2<INPUT, OUTPUT>::applyMin;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1);

        FourierMatchedCorrelationLoGFilter2(const FourierMatchedCorrelationLoGFilter2& f);

        ~FourierMatchedCorrelationLoGFilter2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        MatchedLoGFilter2<INPUT, OUTPUT>* m= new MatchedLoGFilter2<INPUT, OUTPUT>(kernel, sigma0, step, sigma1);
        this->addFilterSet2(m, true);
        std::stringstream ss;
        ss << "FourierMatchedCorrelationLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1;
        (*this).Transform2<INPUT, OUTPUT>::descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationLoGFilter2(const FourierMatchedCorrelationLoGFilter2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::~FourierMatchedCorrelationLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class FourierGaborFilter2: public FourierFilter2<INPUT, OUTPUT>
    {
    public:
        using FourierFilter2<INPUT, OUTPUT>::apply;
        using FourierFilter2<INPUT, OUTPUT>::mft;
        using FourierFilter2<INPUT, OUTPUT>::tmp;
        using FourierFilter2<INPUT, OUTPUT>::filter;
        using FourierFilter2<INPUT, OUTPUT>::correlation;
        using FourierFilter2<INPUT, OUTPUT>::data;
        using FourierFilter2<INPUT, OUTPUT>::rows;
        using FourierFilter2<INPUT, OUTPUT>::columns;

        FourierGaborFilter2();
        /**
         * constructor
         * @param rows rows of the image on which the filter will be applied
         * @param columns columns of the image on which the filter will be applied
         * @param f filter in image space from which the Fourier filter is generated
         * @param c Cache2 object to make the operation faster
         */
        FourierGaborFilter2(float sigma, float theta, float lambda, float psi, float gamma, bool correlation= false);

        /**
         * copy constructor
         * @param ff instance to copy
         */
        FourierGaborFilter2(const FourierGaborFilter2& ff);

        /**
         * destructor
         */
        ~FourierGaborFilter2();

        void setFilter(float sigma, float theta, float lambda, float psi, float gamma, bool correlation= false);

        virtual void updateSize(int rows, int columns);

        float sigma;
        float theta;
        float lambda;
        float psi;
        float gamma;

        GaborFilterR2<INPUT, OUTPUT>* gfr;
        GaborFilterI2<INPUT, OUTPUT>* gfi;
    };

    template<typename INPUT, typename OUTPUT>
    void FourierGaborFilter2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        if ( this->rows != rows || this->columns != columns )
        {
            Image<std::complex<double> > im;

            mft.updateSize(rows, columns);
            tmp.resizeImage(rows, columns);
            im.resizeImage(rows, columns);
            this->resizeImage(rows, columns);

            im= 0;
            int x, y;

            int i= 0;
            for ( typename Filter2<INPUT, OUTPUT, float>::fIt fit= gfr->begin(); fit != gfr->end(); ++fit )
            {
                x= fit->first/gfr->stride;
                if ( abs(fit->first%gfr->stride) > gfr->stride/2 )
                    x= (fit->first > 0) ? x+1 : x-1;

                y= fit->first%gfr->stride;
                if ( abs(y) > gfr->stride/2 )
                    y+= y > 0 ? - gfr->stride : gfr->stride;

                if ( x < 0 )
                    x= im.columns+x;
                if ( y < 0 )
                    y= im.rows+y;

                im(y,x)= std::complex<double>(fit->second, (*gfi)(i).second);
                ++i;
            }

            mft.transform(im.data(), this->data());
            if ( correlation )
                for ( unsigned int i= 0; i < this->n; ++i )
                    (*this)(i)= std::complex<double>(real((*this)(i)), -imag((*this)(i)));
        }
    }

    template<typename INPUT, typename OUTPUT>
    FourierGaborFilter2<INPUT, OUTPUT>::FourierGaborFilter2()
    : FourierFilter2<INPUT, OUTPUT>(NULL)
    {
        ///fake
        gfr= NULL;
        gfi= NULL;
    }

    template<typename INPUT, typename OUTPUT>
    FourierGaborFilter2<INPUT, OUTPUT>::FourierGaborFilter2(float sigma, float theta, float lambda, float psi, float gamma, bool correlation)
    : FourierFilter2<INPUT, OUTPUT>(NULL)
    {
        this->correlation= correlation;
        this->sigma= sigma;
        this->theta= theta;
        this->lambda= lambda;
        this->psi= psi;
        this->gamma= gamma;

        gfr= new GaborFilterR2<INPUT, OUTPUT> (sigma, theta, lambda, psi, gamma);
        gfi= new GaborFilterI2<INPUT, OUTPUT> (sigma, theta, lambda, psi, gamma);
    }

    template<typename INPUT, typename OUTPUT>
    FourierGaborFilter2<INPUT, OUTPUT>::FourierGaborFilter2(const FourierGaborFilter2& ff)
    : FourierFilter2<INPUT, OUTPUT>(ff)
    {
        this->mft= ff.mft;
        this->tmp= ff.tmp;
        this->correlation= ff.correlation;
        this->sigma= ff.sigma;
        this->theta= ff.theta;
        this->lambda= ff.lambda;
        this->psi= ff.psi;
        this->gamma= ff.gamma;

        this->gfr= ff.gfr;
        this->gfi= ff.gfi;
    }

    template<typename INPUT, typename OUTPUT>
    FourierGaborFilter2<INPUT, OUTPUT>::~FourierGaborFilter2()
    {
        delete gfr;
        delete gfi;
    }

    template<typename INPUT, typename OUTPUT>
    void FourierGaborFilter2<INPUT, OUTPUT>::setFilter(float sigma, float theta, float lambda, float psi, float gamma, bool correlation)
    {
        *gfr= GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);
        *gfi= GaborFilterI2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);
        rows= columns= -1;
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedGaborFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using FourierFilterSet2<INPUT, OUTPUT>::input;
        using FourierFilterSet2<INPUT, OUTPUT>::output;
        //using FourierFilterSet2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        FourierMatchedGaborFilter2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param ffs instance to copy
         */
        FourierMatchedGaborFilter2(const FourierMatchedGaborFilter2& ffs);

        /**
         * destructor
         */
        ~FourierMatchedGaborFilter2();

        /**
         * computes the maximum weighted sum in each position of the input image,
         * the output gets into the output image
         * @param input input image
         * @param output output image
         * @param c Cache2 objecto to make the operation faster
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        void updateSize(int rows, int columns);

        /**
         * this filter is instantiated when an image space filter set is used
         */
        FourierGaborFilter2<INPUT, OUTPUT> ff2;
        Image<std::complex<double> > tmpImage;

        Image<OUTPUT> tmp;

        MatchedGaborFilter2Magnitude<INPUT, OUTPUT> gmf;
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilter2<INPUT, OUTPUT>::FourierMatchedGaborFilter2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        for ( float tmp= theta0; tmp <= theta1; tmp+= step )
            gmf.push_back(new GaborFilter2Magnitude<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
        
        std::stringstream ss;
        ss << "FourierMatchedGaborFilter2 " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilter2<INPUT, OUTPUT>::FourierMatchedGaborFilter2(const FourierMatchedGaborFilter2& ffs)
    : FourierFilterSet2<INPUT, OUTPUT>(ffs)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilter2<INPUT, OUTPUT>::~FourierMatchedGaborFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedGaborFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        /*Image<INPUT> input;
        Image<OUTPUT> output;*/

        int cmin= in.columns, cmax= 0;
        int rmin= in.rows, rmax= 0;
        if ( roi )
        {
            for ( int i= 0; i < roi->rows; ++i )
                for ( int j= 0; j < roi->columns; ++j )
                {
                    if ( (*roi)(i,j) )
                    {
                        if ( i < rmin )
                            rmin= i;
                        if ( i > rmax )
                            rmax= i;
                        if ( j < cmin )
                            cmin= j;
                        if ( j > cmax )
                            cmax= j;
                    }
                }
        }

        if ( !roi )
        {
            input.resizeImage(in);
            output.resizeImage(out);
            input= in;
        }
        else
        {
            Border2 b= this->getProposedBorder();
            rmax= rmax + b.bottomBorder < in.rows ? rmax + b.bottomBorder : in.rows - 1;
            rmin= rmin - b.topBorder >= 0 ? rmin - b.topBorder : 0;
            cmax= cmax + b.rightBorder < in.columns ? cmax + b.rightBorder : in.columns - 1;
            cmin= cmin - b.leftBorder >= 0 ? cmin - b.leftBorder : 0;

            
            input.resizeImage(rmax - rmin + 1, cmax - cmin + 1);

            fitToFourier(b, input);
            input.resizeBorder(b);
            output.resizeImage(input);
            for ( int i= rmin; i <= rmax; ++i )
                for ( int j= cmin; j <= cmax; ++j )
                    input(b.topBorder + i - rmin, b.leftBorder + j - cmin)= in(i,j);
            //char filename[12];
            /*printf("border: %d %d %d %d\n", input.leftBorder, input.rightBorder, input.topBorder, input.bottomBorder);
            sprintf(filename, "test%d.png", rand()%6);
            writeImage(filename, input);*/
        }

        ff2.correlation= false;
        this->updateSize(input.rows, input.columns);

        if ( gmf.begin() != gmf.end() )
        {
            ff2.gfr= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >(((gmf)[0]))->real);
            ff2.gfi= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >(((gmf)[0]))->imag);
            ff2.rows= ff2.columns= -1;
            ff2.updateSize(input.rows, input.columns);
            ff2.mft.transform(input.data(), tmpImage.data());
            ff2.tmp= tmpImage;
            ff2.tmp*= ff2;
            ff2.mft.inverseTransform(ff2.tmp.data(), output.data());
            //ff2.apply(input, output);

            for ( typename FilterSet2<INPUT, OUTPUT, float>::iterator fit= gmf.begin() + 1; fit != gmf.end(); ++fit )
            {
                ff2.gfr= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((*fit))->real);
                ff2.gfi= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((*fit))->imag);
                ff2.rows= ff2.columns= -1;
                ff2.updateSize(input.rows, input.columns);
                ff2.tmp= tmpImage;
                ff2.tmp*= ff2;
                ff2.mft.inverseTransform(ff2.tmp.data(), tmp.data());
                //ff2.apply(input, tmp);
                for ( unsigned int i= 0; i < output.n; ++i )
                    if ( tmp(i) > output(i) )
                        output(i)= (tmp)(i);
            }
        }

        if ( !roi )
            out= output;
        else
        {
            for ( int i= input.topBorder; i <= input.topBorder + rmax - rmin + 1; ++i )
                for ( int j= input.leftBorder; j <= input.leftBorder + cmax - cmin + 1; ++j )
                    out(i - input.topBorder + rmin, j - input.leftBorder + cmin)= output(i,j);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedGaborFilter2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        tmp.resizeImage(rows, columns);
        tmpImage.resizeImage(rows, columns);

        /*for ( typename FourierMatchedGaborFilter2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateSize(rows, columns);
        }*/
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedCorrelationGaborFilter2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;
        using FourierFilterSet2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        FourierMatchedCorrelationGaborFilter2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param ffs instance to copy
         */
        FourierMatchedCorrelationGaborFilter2(const FourierMatchedCorrelationGaborFilter2& ffs);

        /**
         * destructor
         */
        ~FourierMatchedCorrelationGaborFilter2();

        /**
         * computes the maximum weighted sum in each position of the input image,
         * the output gets into the output image
         * @param input input image
         * @param output output image
         * @param c Cache2 objecto to make the operation faster
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        void updateSize(int rows, int columns);

        /**
         * this filter is instantiated when an image space filter set is used
         */
        FourierGaborFilter2<INPUT, OUTPUT> ff2;
        Image<std::complex<double> > tmpImage;

        Image<OUTPUT> tmp;

        MatchedGaborFilter2Magnitude<INPUT, OUTPUT> gmf;
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationGaborFilter2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        for ( float tmp= theta0; tmp <= theta1; tmp+= step )
            gmf.push_back(new GaborFilter2Magnitude<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));

        std::stringstream ss;
        ss << "FourierMatchedCorrelationGaborFilter2 " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::FourierMatchedCorrelationGaborFilter2(const FourierMatchedCorrelationGaborFilter2& ffs)
    : FourierFilterSet2<INPUT, OUTPUT>(ffs)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::~FourierMatchedCorrelationGaborFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        ff2.correlation= true;
        this->updateSize(input.rows, input.columns);

        if ( gmf.begin() != gmf.end() )
        {
            ff2.gfr= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((gmf[0]))->real);
            ff2.gfi= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((gmf[0]))->imag);
            ff2.rows= ff2.columns= -1;
            ff2.apply(input, output);

            for ( typename FilterSet2<INPUT, OUTPUT, float>::iterator fit= gmf.begin() + 1; fit != gmf.end(); ++fit )
            {
                ff2.gfr= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((*fit))->real);
                ff2.gfi= (dynamic_cast<GaborFilter2Magnitude<INPUT, OUTPUT>* >((*fit))->imag);
                ff2.rows= ff2.columns= -1;
                ff2.apply(input, tmp);
                for ( unsigned int i= 0; i < output.n; ++i )
                    if ( (tmp)(i) > output(i) )
                        output(i)= (tmp)(i);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        tmp.resizeImage(rows, columns);
        tmpImage.resizeImage(rows, columns);

        /*for ( typename FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            (*fit)->updateSize(rows, columns);
        }*/
    }

    template<typename INPUT, typename OUTPUT>
    class FourierMatchedGaborFilterR2: public FourierFilterSet2<INPUT, OUTPUT>
    {
    public:
        using FourierFilterSet2<INPUT, OUTPUT>::apply;
        using FourierFilterSet2<INPUT, OUTPUT>::addFilterSet2;
        using FourierFilterSet2<INPUT, OUTPUT>::descriptor;

        FourierMatchedGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        FourierMatchedGaborFilterR2(const FourierMatchedGaborFilterR2& f);

        ~FourierMatchedGaborFilterR2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilterR2<INPUT, OUTPUT>::FourierMatchedGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : FourierFilterSet2<INPUT, OUTPUT>()
    {
        //MatchedGaborFilterR2<INPUT, OUTPUT>* m= new MatchedGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma);
        //this->addFilterSet2(m);
        std::stringstream ss;
        ss << "FourierMatchedGaborFilterR2 " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilterR2<INPUT, OUTPUT>::FourierMatchedGaborFilterR2(const FourierMatchedGaborFilterR2& f)
    : FourierFilterSet2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FourierMatchedGaborFilterR2<INPUT, OUTPUT>::~FourierMatchedGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void FourierMatchedGaborFilterR2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateSize(input.rows, input.columns);
        this->FourierFilterSet2<INPUT, OUTPUT>::apply(input, output, roi, support);
    }
}

#endif	/* FOURIERMATCHEDFILTER2S_H */

