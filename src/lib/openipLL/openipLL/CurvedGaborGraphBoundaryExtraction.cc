#include <openipLL/CurvedGaborGraphBoundaryExtraction.h>

namespace openip
{
    FittedCurveSegmentGraph::FittedCurveSegmentGraph()
        : magnitude(0), r(0), c(0), r1(0), c1(0), r2(0), c2(0), sigma(0), theta(0), lambda(0), psi(0), gamma(0)
    {
    }

    FittedCurveSegmentGraph::FittedCurveSegmentGraph(float magnitude, float r, float c, float r1, float c1, float r2, float c2)
    {
        this->magnitude= magnitude;
        this->r= r;
        this->c= c;
        this->r1= r1;
        this->c1= c1;
        this->r2= r2;
        this->c2= c2;
    }

    FittedCurveSegmentGraph::FittedCurveSegmentGraph(float magnitude, float r, float c, float r1, float c1, float r2, float c2, CurvedGaborFilter2<float, float>* filter)
    {
        this->magnitude= magnitude;
        this->r= r;
        this->c= c;
        this->r1= r1;
        this->c1= c1;
        this->r2= r2;
        this->c2= c2;

        this->curv= filter->curv;
        this->sigma= filter->sigma;
        this->theta= filter->angle;
        this->lambda= filter->lambda;
        this->psi= filter->psi;
        this->gamma= filter->gamma;
    }

    FittedCurveSegmentGraph::FittedCurveSegmentGraph(const FittedCurveSegmentGraph& fcs)
        : magnitude(fcs.magnitude), r(fcs.r), c(fcs.c), r1(fcs.r1), c1(fcs.c1), r2(fcs.r2), c2(fcs.c2), curv(fcs.curv), sigma(fcs.sigma), theta(fcs.theta), lambda(fcs.lambda), psi(fcs.psi), gamma(fcs.gamma)
    {
    }

    FittedCurveSegmentGraph::~FittedCurveSegmentGraph()
    {
    }

    FittedCurveSegmentGraph& FittedCurveSegmentGraph::operator=(const FittedCurveSegmentGraph& b)
    {
        this->magnitude= b.magnitude;
        this->r= b.r;
        this->c= b.c;
        this->r1= b.r1;
        this->c1= b.c1;
        this->r2= b.r2;
        this->c2= b.c2;
        this->curv= b.curv;
        this->sigma= b.sigma;
        this->theta= b.theta;
        this->lambda= b.lambda;
        this->psi= b.psi;
        this->gamma= b.gamma;
        
        return (*this);
    }

    float FittedCurveSegmentGraph::dist21(FittedCurveSegmentGraph& b)
    {
        return sqrt((r2 - b.r1)*(r2 - b.r1) + (c2 - b.c1)*(c2 - b.c1));
    }

    float FittedCurveSegmentGraph::angleHalf(FittedCurveSegmentGraph& b)
    {
        float vr= r-r2;
        float vc= c-c2;
        float wr= b.r - b.r1;
        float wc= b.c - b.c1;
        float mv= sqrt(vr*vr + vc*vc);
        float mw= sqrt(wr*wr + wc*wc);

        return acos((vr*wr + vc*wc)/(mv*mw));
    }

    FittedCurveGraph::FittedCurveGraph()
        : Vector<FittedCurveSegmentGraph>()
    {
    }

    FittedCurveGraph::FittedCurveGraph(const FittedCurveGraph& fc)
        : Vector<FittedCurveSegmentGraph>(fc)
    {
    }

    FittedCurveGraph::~FittedCurveGraph()
    {
    }
}
