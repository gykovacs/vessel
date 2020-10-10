#ifndef _CURVEDGABORBOUNDARYEXTRACTION_H
#define _CURVEDGABORBOUNDARYEXTRACTION_H

#include <openipDS/CurvedGaborFilter2.h>
#include <openipLL/imageIO.h>
#include <openipDS/Stopper.h>

namespace openip
{
    class FittedCurveSegment
    {
    public:
        FittedCurveSegment();

        FittedCurveSegment(float magnitude, float r, float c, float r1, float c1, float r2, float c2);

        FittedCurveSegment(float magnitude, float r, float c, float r1, float c1, float r2, float c2, CurvedGaborFilter2<float, float>* filter);

        FittedCurveSegment(const FittedCurveSegment& fcs);

        ~FittedCurveSegment();

        FittedCurveSegment& operator=(const FittedCurveSegment& b);

        template<typename INPUT>
        void drawIntoImage(Image<INPUT>& image, INPUT color= 0.0);

        float dist21(FittedCurveSegment& b);

        float angleHalf(FittedCurveSegment& b);

        float magnitude;
        float r;
        float c;
        float r1;
        float c1;
        float r2;
        float c2;

        float curv;
        float sigma;
        float theta;
        float lambda;
        float psi;
        float gamma;
    };

    class FittedCurve: public Vector<FittedCurveSegment>
    {
    public:
        FittedCurve();

        FittedCurve(const FittedCurve& fc);

        ~FittedCurve();

        template<typename INPUT>
        void drawIntoImage(Image<INPUT>& image, INPUT color= 0.0);
    };

    template<typename INPUT>
    class CurvedGaborBoundaryExtraction
    {
    public:
        CurvedGaborBoundaryExtraction();

        CurvedGaborBoundaryExtraction(const CurvedGaborBoundaryExtraction& cgbe);

        ~CurvedGaborBoundaryExtraction();

        void init(float curv0, float curvstep, float curv1, float sigma0, float sigmastep, float sigma1, float theta0, float thetastep, float theta1, float lambda, float psi, float gamma);

        void doBE(Image<INPUT>& input, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        void extractFittedCurveSegments();

        void extractFittedCurves();

        float curv0;
        float curvstep;
        float curv1;
        float sigma0;
        float sigmastep;
        float sigma1;
        float theta0;
        float thetastep;
        float theta1;
        float lambda;
        float psi;
        float gamma;

        MatchedCurvedGaborFilter2RMagnitude<float, float>* filters;
        Image<float> magnitude;
        Image<int> indices;
        Image<unsigned char>* roi;
        Image<unsigned char>* support;
        Vector<FittedCurveSegment> fcs;
        Vector<unsigned char> fcsMask;
        Vector<FittedCurve> fc;
    };

    template<typename INPUT>
    void FittedCurveSegment::drawIntoImage(Image<INPUT>& image, INPUT color)
    {
        float width= GWIDTH*sigma;
        float x, y, x1, y1;
        for ( int i= -width; i <= width; ++i )
        {
            x= i;
            y= i*i*curv;

            x1= cos(theta)*x + sin(theta)*y;
            y1= -sin(theta)*x + cos(theta)*y;

            if ( x1*x1 + y1*y1 < width*width )
            image(r + y1, c + x1)= color;
        }
    }

    template<typename INPUT>
    void FittedCurve::drawIntoImage(Image<INPUT>& image, INPUT color)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i).drawIntoImage(image, color);
    }

    template<typename INPUT>
    CurvedGaborBoundaryExtraction<INPUT>::CurvedGaborBoundaryExtraction()
        : curv0(0), curvstep(0), curv1(0), sigma0(0), sigmastep(0), sigma1(0), theta0(0), thetastep(0), theta1(0), lambda(0), psi(0), gamma(0), roi(NULL), support(NULL)
    {
    }

    template<typename INPUT>
    CurvedGaborBoundaryExtraction<INPUT>::CurvedGaborBoundaryExtraction(const CurvedGaborBoundaryExtraction &cgbe)
        : curv0(cgbe.curv0), curvstep(cgbe.curvstep), curv1(cgbe.curv1), sigma0(cgbe.sigma0), sigmastep(cgbe.sigmastep), sigma1(cgbe.sigma1), theta0(cgbe.theta0), thetastep(cgbe.thetastep), theta1(cgbe.theta1), lambda(cgbe.lambda), psi(cgbe.psi), gamma(cgbe.gamma)
    {
    }

    template<typename INPUT>
    CurvedGaborBoundaryExtraction<INPUT>::~CurvedGaborBoundaryExtraction()
    {
    }

    template<typename INPUT>
    void CurvedGaborBoundaryExtraction<INPUT>::init(float curv0, float curvstep, float curv1, float sigma0, float sigmastep, float sigma1, float theta0, float thetastep, float theta1, float lambda, float psi, float gamma)
    {
        this->curv0= curv0;
        this->curvstep= curvstep;
        this->curv1= curv1;
        this->sigma0= sigma0;
        this->sigmastep= sigmastep;
        this->sigma1= sigma1;
        this->theta0= theta0;
        this->thetastep= thetastep;
        this->theta1= theta1;
        this->lambda= lambda;
        this->psi= psi;
        this->gamma= gamma;

        filters= new MatchedCurvedGaborFilter2RMagnitude<float, float>(curv0, curvstep, curv1, sigma0, sigmastep, sigma1, theta0, thetastep, theta1, lambda, psi, gamma);
    }

    template<typename INPUT>
    void CurvedGaborBoundaryExtraction<INPUT>::doBE(Image<INPUT>& input, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        magnitude.resizeImage(input);
        indices.resizeImage(input);

        if ( roi != NULL )
            this->roi= roi;
        else if ( this->roi != NULL )
        {
            this->roi->resizeImage(input);
            this->roi->setContentTo((unsigned char)255);
        }
        else
        {
            this->roi= new Image<unsigned char>();
            this->roi->resizeImage(input);
            this->roi->setContentTo((unsigned char)255);
        }

        if ( support != NULL )
            this->support= support;
        else if ( this->support != NULL )
        {
            this->support->resizeImage(input);
            this->support->setContentTo((unsigned char)255);
        }
        else
        {
            this->support= new Image<unsigned char>();
            this->support->resizeImage(input);
            this->support->setContentTo((unsigned char)255);
        }


        filters->apply(input, magnitude, indices, this->roi, this->support);

        fcs.clear();

        extractFittedCurveSegments();

        for ( int i= 0; i < 10; ++i )
            extractFittedCurves();
    }

    template<typename INPUT>
    void CurvedGaborBoundaryExtraction<INPUT>::extractFittedCurveSegments()
    {
        tprintf("extract fitted curve segments\n");
        float r1, r2, c1, c2;

        for ( int i= 0; i < magnitude.rows; ++i )
            for ( int j= 0; j < magnitude.columns; ++j )
            {
                if ( (*roi)(i,j) )
                {
                    dynamic_cast<CurvedGaborFilter2<float, float>*>((*filters)[indices(i,j)])->getBaselineEndCoordinates(i, j, r1, c1, r2, c2);
                    fcs.push_back(FittedCurveSegment(magnitude(i,j), i, j, r1, c1, r2, c2, dynamic_cast<CurvedGaborFilter2<float, float>*>((*filters)[indices(i,j)])));
                }
            }
        fcsMask.resize(fcs.size());
        fcsMask= 1;
    }

    template<typename INPUT>
    void CurvedGaborBoundaryExtraction<INPUT>::extractFittedCurves()
    {
        tprintf("extract fitted curves\n");
        // find filter with highets response
        int maxIdx= 0;
        float maxResponse= -FLT_MAX;
        for ( int i= 0; i < fcs.size(); ++i )
            if ( fcsMask(i) && fcs(i).magnitude > maxResponse )
            {
                maxIdx= i;
                maxResponse= fcs(i).magnitude;
            }

        fcsMask(maxIdx)= 0;

        FittedCurve fctmp;
        fctmp.push_back(fcs(maxIdx));

        tprintf("find attached responses\n");
        //find attached responses with highest magnitudes
        while ( 1 )
        {
            FittedCurveSegment last= fctmp(fctmp.size() - 1);
            int closestIdx= -1;

            for ( int i= 0; i < fcs.size(); ++i )
                if ( fcsMask(i) != 2 && last.dist21(fcs(i)) < 1.0 && (closestIdx == -1 || fcs(i).magnitude > fcs(closestIdx).magnitude) && fabs(last.angleHalf(fcs(i))) > M_PI/2)
                    closestIdx= i;

            if ( closestIdx == -1 || fctmp.size() > 40 )
                break;

            fcsMask(closestIdx)= 2;
            tprintf("attachement found\n");
            fctmp.push_back(fcs(closestIdx));
        }

        fc.push_back(fctmp);
    }
}

#endif // _CURVEDGABORBOUNDARYEXTRACTION_H
