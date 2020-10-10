#ifndef _CURVEDGABORGRAPHBOUNDARYEXTRACTION_H
#define _CURVEDGABORGRAPHBOUNDARYEXTRACTION_H

#include <openipDS/CurvedGaborFilter2.h>
#include <openipDS/GaborFilter2.h>
#include <openipLL/imageIO.h>
#include <openipDS/Stopper.h>
#include <openipML/SparseAdjacencyMatrix.h>
#include <openipML/DijkstraClosedPath.h>

#include <set>

namespace openip
{
    class FittedCurveSegmentGraph
    {
    public:
        FittedCurveSegmentGraph();

        FittedCurveSegmentGraph(float magnitude, float r, float c, float r1, float c1, float r2, float c2);

        FittedCurveSegmentGraph(float magnitude, float r, float c, float r1, float c1, float r2, float c2, CurvedGaborFilter2<float, float>* filter);

        FittedCurveSegmentGraph(const FittedCurveSegmentGraph& fcs);

        ~FittedCurveSegmentGraph();

        FittedCurveSegmentGraph& operator=(const FittedCurveSegmentGraph& b);

        template<typename INPUT>
        void drawIntoImage(Image<INPUT>& image, INPUT color= 0.0);

        float dist21(FittedCurveSegmentGraph& b);

        float angleHalf(FittedCurveSegmentGraph& b);

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

    class FittedCurveGraph: public Vector<FittedCurveSegmentGraph>
    {
    public:
        FittedCurveGraph();

        FittedCurveGraph(const FittedCurveGraph& fc);

        ~FittedCurveGraph();

        template<typename INPUT>
        void drawIntoImage(Image<INPUT>& image, INPUT color= 0.0);
    };

    template<typename INPUT>
    class CurvedGaborGraphBoundaryExtraction
    {
    public:
        CurvedGaborGraphBoundaryExtraction();

        CurvedGaborGraphBoundaryExtraction(const CurvedGaborGraphBoundaryExtraction& cgbe);

        ~CurvedGaborGraphBoundaryExtraction();

        void init(float curv0, float curvstep, float curv1, float sigma0, float sigmastep, float sigma1, float theta0, float thetastep, float theta1, float lambda, float psi, float gamma);

        void doBE(Image<INPUT>& input, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        void extractFittedCurveSegments();

        void extractFittedCurves();

        void initializeMaps();

        void buildAdjacencyMatrix();

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

        Vector<MatchedCurvedGaborFilter2RMagnitude<float, float>* > filters;
        MatchedCurvedGaborFilter2RMagnitude<float, float>* prefilters;
        Vector<Image<float>* > magnitude;
        Image<float> prefiltered;
        Image<unsigned char> thresholded;
        Vector<Image<int>* > indices;
        Image<int> ptovmap;
        Vector<int> vtopmap;
        Image<unsigned char>* roi;
        Image<unsigned char>* support;
        Vector<FittedCurveSegmentGraph> fcs;
        Vector<unsigned char> fcsMask;
        Vector<FittedCurveGraph> fc;
        SparseAdjacencyMatrix sam;
    };

    template<typename INPUT>
    void FittedCurveSegmentGraph::drawIntoImage(Image<INPUT>& image, INPUT color)
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
    void FittedCurveGraph::drawIntoImage(Image<INPUT>& image, INPUT color)
    {
        for ( int i= 0; i < this->size(); ++i )
            (*this)(i).drawIntoImage(image, color);
    }

    template<typename INPUT>
    CurvedGaborGraphBoundaryExtraction<INPUT>::CurvedGaborGraphBoundaryExtraction()
        : curv0(0), curvstep(0), curv1(0), sigma0(0), sigmastep(0), sigma1(0), theta0(0), thetastep(0), theta1(0), lambda(0), psi(0), gamma(0), roi(NULL), support(NULL)
    {
    }

    template<typename INPUT>
    CurvedGaborGraphBoundaryExtraction<INPUT>::CurvedGaborGraphBoundaryExtraction(const CurvedGaborGraphBoundaryExtraction &cgbe)
        : curv0(cgbe.curv0), curvstep(cgbe.curvstep), curv1(cgbe.curv1), sigma0(cgbe.sigma0), sigmastep(cgbe.sigmastep), sigma1(cgbe.sigma1), theta0(cgbe.theta0), thetastep(cgbe.thetastep), theta1(cgbe.theta1), lambda(cgbe.lambda), psi(cgbe.psi), gamma(cgbe.gamma)
    {
    }

    template<typename INPUT>
    CurvedGaborGraphBoundaryExtraction<INPUT>::~CurvedGaborGraphBoundaryExtraction()
    {
    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::init(float curv0, float curvstep, float curv1, float sigma0, float sigmastep, float sigma1, float theta0, float thetastep, float theta1, float lambda, float psi, float gamma)
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

        float stmp= sigma0;
        /*while ( stmp <= sigma1 )
        {
            filters.push_back(new MatchedCurvedGaborFilter2RMagnitude<float, float>(curv0, curvstep, curv1, stmp, sigmastep, stmp+0.1, theta0, thetastep, theta1, lambda, psi, gamma));
            stmp+= sigmastep;
        }*/
        filters.push_back(new MatchedCurvedGaborFilter2RMagnitude<float, float>(curv0, curvstep, curv1, stmp, sigmastep, stmp+0.1, theta0, thetastep, theta1, lambda, psi, gamma));
        prefilters= new MatchedCurvedGaborFilter2RMagnitude<float, float>(0.0, 1.0, 0.0, sigma0, 1.0, sigma0, theta0, thetastep, theta1, lambda, psi, gamma);
    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::doBE(Image<INPUT>& input, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        magnitude.resize(filters.size());
        indices.resize(filters.size());
        for ( int i= 0; i < filters.size(); ++i )
        {
            magnitude(i)= new Image<float>();
            indices(i)= new Image<int>();
            magnitude(i)->resizeImage(input);
            indices(i)->resizeImage(input);
        }
        prefiltered.resizeImage(input);
        thresholded.resizeImage(input);

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

        tprintf("prefiltering...\n");
        prefilters->apply(input, prefiltered, this->roi, this->support);

        for ( int i= 0; i < prefilters->size(); ++i )
        {
            CurvedGaborFilter2<float, float>* tmp;
            tmp= dynamic_cast<CurvedGaborFilter2<float, float>*>((*prefilters)[i]);
            Image<float> tmpimage;
            char name[100];
            sprintf(name, "mf-%d.bmp", i);
            tmp->dumpFilter(tmpimage);
            tmpimage.normalize(0, 255);
            writeImage(name, tmpimage);
        }

        tprintf("thresholding...\n");
        float threshold= prefiltered.getMax() * 0.3;

        for(int i= 0; i < prefiltered.n; ++i )
            if ( prefiltered(i) > threshold )
                thresholded(i)= 255;
            else
                thresholded(i)= 0;

        writeImage("thresholded.bmp", thresholded);

        tprintf("building adjacency matrix\n");

        for ( int i= 0; i < filters.size(); ++i )
        {
            tprintf("%d\n", i);
            filters(i)->apply(input, *(magnitude(i)), *(indices(i)), &thresholded, this->support);
        }

        tprintf("save magnitude images\n");
        for ( int i= 0; i < magnitude.size(); ++i )
        {
            char name[100];
            sprintf(name, "magnitude%d.bmp", i);
            magnitude(i)->normalize(0, 255);
            writeImage(name,*(magnitude(i)));
        }

        tprintf("extract curve segments\n");
        extractFittedCurveSegments();

        Image<unsigned char> img;
        img.resizeImage(thresholded);
        for ( int i= 0; i < fcs.size(); ++i )
            if ( i % 50 == 0 )
            fcs(i).drawIntoImage(img, (unsigned char)255);

        writeImage("img.bmp", img);

        tprintf("initializeMaps\n");
        initializeMaps();

        tprintf("build adjacency matrix\n");
        buildAdjacencyMatrix();
        tprintf("built\n");

        DijkstraClosedPath dcp;

        Vector<int> result;
        result= dcp.extractPath(sam);

        for ( int i= 0; i < result.size(); ++i )
            printf("%d ", result(i));
        fflush(stdout);

        //Vector<int> cpath= disjkstra(adj);

    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::extractFittedCurveSegments()
    {
        tprintf("extract fitted curve segments\n");
        float r1, r2, c1, c2;

        for ( int k= 0; k < magnitude.size(); ++k )
            for ( int i= 0; i < magnitude(k)->rows; ++i )
            for ( int j= 0; j < magnitude(k)->columns; ++j )
            {
                if ( (thresholded)(i,j) )
                {
                    CurvedGaborFilter2<float, float>* cgf= dynamic_cast<CurvedGaborFilter2<float, float>*>((*(filters(k)))[indices(k)->operator()(i,j)]);
                    cgf->getBaselineEndCoordinates(i, j, r1, c1, r2, c2);
                    printf("%f %f %f %f\n", r1, c1, r2, c2);
                    fcs.push_back(FittedCurveSegmentGraph(magnitude(k)->operator()(i,j), i, j, r1, c1, r2, c2, cgf));
                }
            }
        getchar();
        fcsMask.resize(fcs.size());
        fcsMask= 1;
    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::initializeMaps()
    {
        ptovmap.resizeImage(thresholded);
        ptovmap= -1;
        for ( int i= 0; i < fcs.size(); ++i )
        {
            ptovmap(int(fcs(i).r1)*thresholded.columns + int(fcs(i).c1))= -2;
            ptovmap(int(fcs(i).r2)*thresholded.columns + int(fcs(i).c2))= -2;
        }

        int n= 0;
        for ( int i= 0; i < ptovmap.n; ++i )
            if ( ptovmap(i) == -2 )
                ++n;

        tprintf("number of vertices: %d\n", n);
        vtopmap.resize(n);
        int j= 0;
        for ( int i= 0; i < ptovmap.n; ++i )
            if ( ptovmap(i) == -2 )
            {
                ptovmap(i)= j;
                vtopmap(j++)= i;
            }
    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::buildAdjacencyMatrix()
    {
        sam.clear();
        sam.resize(vtopmap.size());

        for ( int i= 0; i < fcs.size(); ++i )
        {
            int a= int(fcs(i).r1)*thresholded.columns + int(fcs(i).c1);
            int b= int(fcs(i).r2)*thresholded.columns + int(fcs(i).c2);
            printf("%d,%d ", a, b);
            sam(ptovmap(a)).push_back(VertexWeightPair(ptovmap(b), fcs(i).magnitude));
            //sam(ptovmap(b)).push_back(VertexWeightPair(ptovmap(a), fcs(i).magnitude));
        }
    }

    template<typename INPUT>
    void CurvedGaborGraphBoundaryExtraction<INPUT>::extractFittedCurves()
    {
        /*tprintf("extract fitted curves\n");
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
            FittedCurveSegmentGraph last= fctmp(fctmp.size() - 1);
            int closestIdx= -1;

            for ( int i= 0; i < fcs.size(); ++i )
                if ( fcsMask(i) != 2 && last.dist21(fcs(i)) < 2.0 && (closestIdx == -1 || fcs(i).magnitude > fcs(closestIdx).magnitude) && fabs(last.angleHalf(fcs(i))) > M_PI/2)
                    closestIdx= i;

            if ( closestIdx == -1 || fctmp.size() > 100 )
                break;

            fcsMask(closestIdx)= 2;
            tprintf("attachement found\n");
            fctmp.push_back(fcs(closestIdx));
        }

        fc.push_back(fctmp);*/
    }
}

#endif // _CURVEDGABORBOUNDARYEXTRACTION_H
