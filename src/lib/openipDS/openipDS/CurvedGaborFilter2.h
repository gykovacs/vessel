#ifndef CURVEDGABORFILTER2_H
#define CURVEDGABORFILTER2_H

#include <float.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <openipDS/Filter2.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/FilterSet2.h>
#include <openipLL/imageIO.h>
#include <openipDS/drawing.h>
#include <math.h>

#define WIDTHRATIO 2
#define GWIDTH 2.0*sqrt(2.0*log(WIDTHRATIO))

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class CurvedGaborFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;

        CurvedGaborFilter2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CurvedGaborFilter2(float curv, float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CurvedGaborFilter2(const CurvedGaborFilter2& gf);

        /**
        * destructor
        */
        ~CurvedGaborFilter2();

        void updateCurvAndAngle(float curv, float angle);

        void dumpFilter(Image<float>& image);

        void getBaselineEndCoordinates(int r, int ca, float& r1, float& c1, float& r2, float& c2);

        void drawBaseline(Image<INPUT>& input, int r, int c, INPUT color= 0);

        void drawFilter(Image<INPUT>& input, int r, int c);

        /** angle of the filter */
        float angle;

        float curv;

        float sigma;

        float lambda;

        float psi;

        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    CurvedGaborFilter2<INPUT, OUTPUT>::CurvedGaborFilter2()
        : Filter2<INPUT, OUTPUT, float>()
    {
        angle= curv= sigma= lambda= psi= gamma= 0;
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborFilter2<INPUT, OUTPUT>::CurvedGaborFilter2(float curv, float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        this->sigma= sigma;
        this->lambda= lambda;
        this->psi= psi;
        this->gamma= gamma;

        updateCurvAndAngle(curv, theta);
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborFilter2<INPUT, OUTPUT>::CurvedGaborFilter2(const CurvedGaborFilter2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
        this->angle= gf.angle;
        this->curv= gf.curv;
        this->sigma= gf.sigma;
        this->lambda= gf.lambda;
        this->psi= gf.psi;
        this->gamma= gf.gamma;
        this->descriptor= gf.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborFilter2<INPUT, OUTPUT>::~CurvedGaborFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborFilter2<INPUT, OUTPUT>::updateCurvAndAngle(float curv, float angle)
    {
        this->curv= curv;
        this->angle= angle;

        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 2;
        float xmax= max(fabs(nstds*sigma_x*cos(angle)), fabs(nstds*sigma_y*sin(angle)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(angle)), fabs(nstds*sigma_y*cos(angle)));
        ymax= ceil(max(1.0f, ymax));
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_angle;
        float y_angle;

        for ( int i= xmin*2; i <= xmax*2; ++i )
            for ( int j= ymin*2; j <= ymax*2; ++j )
            {

                x_angle= i*cos(angle) + j*sin(angle);
                y_angle= -i*sin(angle) + j*cos(angle);

                x_angle= y_angle*y_angle * curv - x_angle;

                weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_angle*x_angle/(sigma_x*sigma_x)
                        + y_angle*y_angle/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_angle + psi);
                /*if ( curv*j*j + i >= xmin && curv*j*j + i <= xmax  && j >= ymin && j <= ymax )
                    this->addElement(curv*j*j+i, j, weight);*/
                if ( i >= xmin && i <= xmax  && j >= ymin && j <= ymax )
                                    this->addElement(i, j, weight);
            }
        this->computeMinMax();
        this->zeroMean();
        std::stringstream ss;
        ss << "CurvedGaborFilter2" << " " << curv << " " << sigma << " " << angle << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborFilter2<INPUT, OUTPUT>::dumpFilter(Image<float>& image)
    {
        Filter2<INPUT, OUTPUT, float>::dumpFilter(image);

        int rc= image.rows/2;
        int cc= image.columns/2;

        //drawCircle(image, rc, cc, int(GWIDTH*sigma), image.getMax(), false);

        float r1, c1, r2, c2;
        this->getBaselineEndCoordinates(rc, cc, r1, c1, r2, c2);
        //printf("%f %f %f %f\n", r1, c1, r2, c2);

        //image(r1, c1)= image.getMin();
        //image(r2, c2)= image.getMin();

        //this->drawBaseline(image, rc, cc);
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborFilter2<INPUT, OUTPUT>::getBaselineEndCoordinates(int r, int ca, float& r1, float& c1, float& r2, float& c2)
    {
        float y1, y2;
        float x1, x2;
        float rad= GWIDTH*sigma;
        float c= curv;

        //printf("%f %f\n", c, rad);
        if ( c != 0 )
        {
            y1= (-1.0/c + sqrt(1.0/(c*c) + 4.0*rad*rad))/2.0;
            y2= y1;
            x1= sqrt(y1/c);
            x2= -x1;
        }
        else
        {
            x1= -rad;
            x2= rad;
            y1= y2= 0;
        }

        c1= cos(angle)*x1 + sin(angle)*y1;
        r1= -sin(angle)*x1 + cos(angle)*y1;

        c2= cos(angle)*x2 + sin(angle)*y2;
        r2= -sin(angle)*x2 + cos(angle)*y2;

        c1+= ca;
        r1+= r;
        c2+= ca;
        r2+= r;
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborFilter2<INPUT, OUTPUT>::drawFilter(Image<INPUT>& input, int r, int c)
    {
        Image<float> filter;
        Filter2<INPUT, OUTPUT, float>::dumpFilter(filter);

        int rc= filter.rows/2;
        int cc= filter.columns/2;

        float radius= GWIDTH*sigma;
        for ( int i= -rc; i <= rc; ++i )
            for ( int j= -cc; j <= cc; ++j )
                if ( i*i + j*j <= radius*radius )
                {
                    input(r + i, c + j)= filter(i + rc, j + cc);
                }
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborFilter2<INPUT, OUTPUT>::drawBaseline(Image<INPUT>& input, int r, int c, INPUT color)
    {
        float width= GWIDTH*sigma;
        Image<float> filter;
        Filter2<INPUT, OUTPUT, float>::dumpFilter(filter);
        float x, y, x1, y1;
        for ( int i= -width; i <= width; ++i )
        {
            x= i;
            y= i*i*curv;

            x1= cos(angle)*x + sin(angle)*y;
            y1= -sin(angle)*x + cos(angle)*y;

            if ( x1*x1 + y1*y1 < width*width )
            input(r + y1, c + x1)= color;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCurvedGaborFilter2RMagnitude: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCurvedGaborFilter2RMagnitude();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCurvedGaborFilter2RMagnitude(float curv0, float cstep, float curv1, float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda= 0, float psi= 0, float gamma= 0);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCurvedGaborFilter2RMagnitude(const MatchedCurvedGaborFilter2RMagnitude& gf);

        /**
        * destructor
        */
        ~MatchedCurvedGaborFilter2RMagnitude();

        OUTPUT apply(Image<INPUT>& in, Image<int>& out, int n, Image<unsigned char>* support);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& labels, Image<unsigned char>* roi, Image<unsigned char>* support);
    };


    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborFilter2RMagnitude()
    : FilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborFilter2RMagnitude(float curv0, float cstep, float curv1, float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        char f[1000];
        float tmps= sigma0;
        while ( tmps <= sigma1 )
        {
            float tmpc= curv0;
            while ( tmpc <= curv1 )
            {
                float tmp= theta0;
                while ( tmp <= theta1 )
                {
                    //printf("%f %f %f %f %f %f\n", tmpc, sigma, tmp, lambda, psi, gamma);
                    CurvedGaborFilter2<INPUT, OUTPUT>* ft= new CurvedGaborFilter2<INPUT, OUTPUT>(tmpc, tmps, tmp, lambda, psi, gamma);
                    this->push_back(ft);
                    Image<float> output;
                    ft->dumpFilter(output);
                    //ft->drawBaseline(output, output.rows/2, output.columns/2, output.getMin());

                    sprintf(f, "f-%f-%f-%f-%f-%f-%f.bmp", tmpc, tmps, tmp, lambda, psi, gamma);
                    output.normalize(0, 255);
                    writeImage(f, output);
                    printf("%s\n", f);
                    tmp+= step;
                }
                tmpc+= cstep;
            }
        tmps+= sstep;
        }

        std::stringstream ss;
        ss << "MatchedCurvedGaborFilter2RMagnitude" << " " << curv0 << " " << cstep << " " << curv1 << " " << sigma0 << " " << sstep << " " << sigma1 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborFilter2RMagnitude(const MatchedCurvedGaborFilter2RMagnitude& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::~MatchedCurvedGaborFilter2RMagnitude()
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::apply(Image<INPUT>& in, Image<int>& out, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;

        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= (*this)[i]->apply(in, n, support);
            if ( tmp > max )
            {
                max= tmp;
                out(n)= i;
            }
        }

        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCurvedGaborFilter2RMagnitude<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& labels, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
            {
                output(i)= apply(input, labels, i, support);
            }
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, labels, i, support);
                    //++n;
                }
        }
    }




    template<typename INPUT, typename OUTPUT>
    class CurvedGaborCorrelationFilter2: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using CorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using CorrelationFilter2<INPUT, OUTPUT, float>::descriptor;
        using CorrelationFilter2<INPUT, OUTPUT, float>::stride;
        using CorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using CorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using CorrelationFilter2<INPUT, OUTPUT, float>::getMin;
        using CorrelationFilter2<INPUT, OUTPUT, float>::getMax;

        CurvedGaborCorrelationFilter2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CurvedGaborCorrelationFilter2(float curv, float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CurvedGaborCorrelationFilter2(const CurvedGaborCorrelationFilter2& gf);

        /**
        * destructor
        */
        ~CurvedGaborCorrelationFilter2();

        void updateCurvAndAngle(float curv, float angle);

        void dumpFilter(Image<float>& image);

        void getBaselineEndCoordinates(int r, int ca, float& r1, float& c1, float& r2, float& c2);

        void drawBaseline(Image<INPUT>& input, int r, int c, INPUT color= 0);

        void drawFilter(Image<INPUT>& input, int r, int c);

        /** angle of the filter */
        float angle;

        float curv;

        float sigma;

        float lambda;

        float psi;

        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::CurvedGaborCorrelationFilter2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        angle= curv= sigma= lambda= psi= gamma;
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::CurvedGaborCorrelationFilter2(float curv, float sigma, float theta, float lambda, float psi, float gamma)
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        this->sigma= sigma;
        this->lambda= lambda;
        this->psi= psi;
        this->gamma= gamma;

        updateCurvAndAngle(curv, theta);
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::CurvedGaborCorrelationFilter2(const CurvedGaborCorrelationFilter2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
        this->angle= gf.angle;
        this->curv= gf.curv;
        this->sigma= gf.sigma;
        this->lambda= gf.lambda;
        this->psi= gf.psi;
        this->gamma= gf.gamma;
        this->descriptor= gf.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::~CurvedGaborCorrelationFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::updateCurvAndAngle(float curv, float angle)
    {
        this->curv= curv;
        this->angle= angle;

        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 2;
        float xmax= max(fabs(nstds*sigma_x*cos(angle)), fabs(nstds*sigma_y*sin(angle)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(angle)), fabs(nstds*sigma_y*cos(angle)));
        ymax= ceil(max(1.0f, ymax));
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_angle;
        float y_angle;

        for ( int i= xmin*2; i <= xmax*2; ++i )
            for ( int j= ymin*2; j <= ymax*2; ++j )
            {

                x_angle= i*cos(angle) + j*sin(angle);
                y_angle= -i*sin(angle) + j*cos(angle);

                x_angle= y_angle*y_angle * curv - x_angle;

                weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_angle*x_angle/(sigma_x*sigma_x)
                        + y_angle*y_angle/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_angle + psi);
                /*if ( curv*j*j + i >= xmin && curv*j*j + i <= xmax  && j >= ymin && j <= ymax )
                    this->addElement(curv*j*j+i, j, weight);*/
                if ( i >= xmin && i <= xmax  && j >= ymin && j <= ymax )
                                    this->addElement(i, j, weight);
            }
        this->computeMinMax();
        this->zeroMean();
        std::stringstream ss;
        ss << "CurvedGaborFilter2" << " " << curv << " " << sigma << " " << angle << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::dumpFilter(Image<float>& image)
    {
        CorrelationFilter2<INPUT, OUTPUT, float>::dumpFilter(image);

        int rc= image.rows/2;
        int cc= image.columns/2;

        drawCircle(image, rc, cc, int(GWIDTH*sigma), image.getMax(), false);

        float r1, c1, r2, c2;
        this->getBaselineEndCoordinates(rc, cc, r1, c1, r2, c2);
        //printf("%f %f %f %f\n", r1, c1, r2, c2);

        image(r1, c1)= image.getMin();
        image(r2, c2)= image.getMin();

        this->drawBaseline(image, rc, cc);
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::getBaselineEndCoordinates(int r, int ca, float& r1, float& c1, float& r2, float& c2)
    {
        float y1, y2;
        float x1, x2;
        float rad= GWIDTH*sigma;
        float c= curv;

        //printf("%f %f\n", c, rad);
        if ( c != 0 )
        {
            y1= (-1.0/c + sqrt(1.0/(c*c) + 4.0*rad*rad))/2.0;
            y2= y1;
            x1= sqrt(y1/c);
            x2= -x1;
        }
        else
        {
            x1= -rad;
            x2= rad;
            y1= y2= 0;
        }

        c1= cos(angle)*x1 + sin(angle)*y1;
        r1= -sin(angle)*x1 + cos(angle)*y1;

        c2= cos(angle)*x2 + sin(angle)*y2;
        r2= -sin(angle)*x2 + cos(angle)*y2;

        c1+= ca;
        r1+= r;
        c2+= ca;
        r2+= r;
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::drawFilter(Image<INPUT>& input, int r, int c)
    {
        Image<float> filter;
        CorrelationFilter2<INPUT, OUTPUT, float>::dumpFilter(filter);

        int rc= filter.rows/2;
        int cc= filter.columns/2;

        float radius= GWIDTH*sigma;
        for ( int i= -rc; i <= rc; ++i )
            for ( int j= -cc; j <= cc; ++j )
                if ( i*i + j*j <= radius*radius )
                {
                    input(r + i, c + j)= filter(i + rc, j + cc);
                }
    }

    template<typename INPUT, typename OUTPUT>
    void CurvedGaborCorrelationFilter2<INPUT, OUTPUT>::drawBaseline(Image<INPUT>& input, int r, int c, INPUT color)
    {
        float width= GWIDTH*sigma;
        Image<float> filter;
        CorrelationFilter2<INPUT, OUTPUT, float>::dumpFilter(filter);
        float x, y, x1, y1;
        for ( int i= -width; i <= width; ++i )
        {
            x= i;
            y= i*i*curv;

            x1= cos(angle)*x + sin(angle)*y;
            y1= -sin(angle)*x + cos(angle)*y;

            if ( x1*x1 + y1*y1 < width*width )
            input(r + y1, c + x1)= color;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCurvedGaborCorrelationFilter2RMagnitude: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCurvedGaborCorrelationFilter2RMagnitude();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCurvedGaborCorrelationFilter2RMagnitude(float curv0, float cstep, float curv1, float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda= 0, float psi= 0, float gamma= 0);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCurvedGaborCorrelationFilter2RMagnitude(const MatchedCurvedGaborCorrelationFilter2RMagnitude& gf);

        /**
        * destructor
        */
        ~MatchedCurvedGaborCorrelationFilter2RMagnitude();

        OUTPUT apply(Image<INPUT>& in, Image<int>& out, int n, Image<unsigned char>* support);

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& labels, Image<unsigned char>* roi, Image<unsigned char>* support);
    };


    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborCorrelationFilter2RMagnitude()
    : FilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborCorrelationFilter2RMagnitude(float curv0, float cstep, float curv1, float sigma0, float sstep, float sigma1, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        char f[1000];
        float tmps= sigma0;
        while ( tmps <= sigma1 )
        {
            float tmpc= curv0;
            while ( tmpc <= curv1 )
            {
                float tmp= theta0;
                while ( tmp <= theta1 )
                {
                    //printf("%f %f %f %f %f %f\n", tmpc, sigma, tmp, lambda, psi, gamma);
                    CurvedGaborCorrelationFilter2<INPUT, OUTPUT>* ft= new CurvedGaborCorrelationFilter2<INPUT, OUTPUT>(tmpc, tmps, tmp, lambda, psi, gamma);
                    this->push_back(ft);
                    Image<float> output;
                    ft->dumpFilter(output);
                    ft->drawBaseline(output, output.rows/2, output.columns/2, output.getMin());

                    sprintf(f, "f-%f-%f-%f-%f-%f.bmp", tmpc, tmps, tmp, lambda, psi, gamma);
                    output.normalize(0, 255);
                    writeImage(f, output);
                    tmp+= step;
                }
                tmpc+= cstep;
            }
        tmps+= sstep;
        }

        std::stringstream ss;
        ss << "MatchedCurvedGaborCorrelationFilter2RMagnitude" << " " << curv0 << " " << cstep << " " << curv1 << " " << sigma0 << " " << sstep << " " << sigma1 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::MatchedCurvedGaborCorrelationFilter2RMagnitude(const MatchedCurvedGaborCorrelationFilter2RMagnitude& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::~MatchedCurvedGaborCorrelationFilter2RMagnitude()
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::apply(Image<INPUT>& in, Image<int>& out, int n, Image<unsigned char>* support)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;

        for ( int i= 0; i < this->size(); ++i )
        {
            tmp= (*this)[i]->apply(in, n, support);
            if ( tmp > max )
            {
                max= tmp;
                out(n)= i;
            }
        }

        return max;
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCurvedGaborCorrelationFilter2RMagnitude<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& labels, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        updateStride(input.columns);
        computeMinMax();

        output= 0;

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                output(i)= apply(input, labels, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->min; i < (int)(input.n) - this->max; ++i )
                if ( (*roi)(i) > 0 )
                {
                    output(i)= apply(input, labels, i, support);
                    //++n;
                }
        }
    }
}

#endif // CURVEDGABORFILTER2_H
