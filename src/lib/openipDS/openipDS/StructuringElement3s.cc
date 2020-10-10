#include <openipDS/StructuringElement3s.h>

namespace openip
{
    void generateStructuringElementSphere(StructuringElement3& se, int r, int stride, int slicePitch)
    {
        int rr= r*r;
        for ( int i= -r; i <= r; ++i )
            for ( int j= -r; j <= r; ++j )
                for ( int k= -r; k <= r; ++k )
                    if ( i*i + j*j + k*k <= rr )
                        se.push_back(k*slicePitch + i*stride + j);

        se.computeMinMaxOnly();
    }

    void generateStructuringElementRectangle(StructuringElement3& se, int s, int r, int c, int stride, int slicePitch)
    {
        for ( int i= -s/2; i <= s/2; ++i )
            for ( int j= -r/2; j <= r/2; ++j )
                for ( int k= -c/2; k <= c/2; ++k )
                    se.push_back(i * slicePitch + j * stride + k);
        se.computeMinMaxOnly();
    }

    void generateStructuringElementCube(StructuringElement3& se, int a, int stride, int slicePitch)
    {
        for ( int i= -a/2; i <= a/2; ++i )
            for ( int j= -a/2; j <= a/2; ++j )
                for ( int k= -a/2; k <= a/2; ++k )
                    se.push_back(i * slicePitch + j * stride + k);
        se.computeMinMaxOnly();
    }
    
    void generateStructuringElementBall(StructuringElement3& se, int r, int stride, int slicePitch)
    {
        int rr= r*r;
        for ( int i= -r; i <= r; ++i )
            for ( int j= -r; j <= r; ++j )
                for ( int k= -r; k <= r; ++k )
                    if ( i*i + j*j + k*k == rr )
                        se.push_back(k*slicePitch + i*stride + j);

        se.computeMinMaxOnly();
    }

    void generateStructuringElementAxial(StructuringElement3& se, int r, int /*stride*/, int slicePitch)
    {
        for ( int k= -r; k <= r; ++k )
            se.push_back(k*slicePitch);

        se.computeMinMaxOnly();
    }
    
    void generateStructuringElementCS(StructuringElement3& se, int r, int stride, int /*slicePitch*/)
    {
        for ( int k= -r; k <= r; ++k )
	  for ( int j= -r; j <= r; ++j )
	    if ( k*k + j*j <= r*r )
	      se.push_back(k*stride + j);

        se.computeMinMaxOnly();
    }
    
    void generateStructuringElementAC(StructuringElement3& se, int r, int stride, int slicePitch)
    {
        for ( int k= -r; k <= r; ++k )
	  for ( int j= -r; j <= r; ++j )
	    if ( k*k + j*j <= r*r )
	      se.push_back(k*slicePitch + j*stride);

        se.computeMinMaxOnly();
    }
    
    void generateStructuringElementAS(StructuringElement3& se, int r, int /*stride*/, int slicePitch)
    {
        for ( int k= -r; k <= r; ++k )
	  for ( int j= -r; j <= r; ++j )
	    if ( k*k + j*j <= r*r )
	      se.push_back(k*slicePitch + j);

        se.computeMinMaxOnly();
    }
    
    void generateStructuringElementLineSegment(StructuringElement3& se, int r, float a, float b, int stride, int slicePitch)
    {
      float ax= r*cos(b);
      float s= r*sin(b)*sin(a);
      float c= r*sin(b)*cos(a);
      
      float da= ax/2000.0;
      float ds= s/2000.0;
      float dc= c/2000.0;
      
      float aa, ss, cc;
      aa= ss= cc= 0;
      //printf("%f %f %f\n", da, ds, dc);
      for ( int i= 0; i < 2000; ++i )
      {
	se.push_back(int(aa)*slicePitch + int(ss)*stride + int(cc));
	aa+= da;
	ss+= ds;
	cc+= dc;
      }
	  
      sort(se.begin(), se.end());
      se.resize(std::distance(se.begin(), std::unique(se.begin(), se.end())));
      /*printf("%d\n", se.size());
      int s0, r0, c0;
      for ( int i= 0; i < se.size(); ++i )
      {
	se.getCoordinates3D(se(i), s0, r0, c0);
	printf("(%d,%d,%d,%d) ", se(i), s0, r0, c0);
      }
      printf("\n");*/
      se.computeMinMaxOnly();
    }

    StructuringElementSphere::StructuringElementSphere(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementSphere(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }
    
    StructuringElementSphere::StructuringElementSphere(const StructuringElementSphere& s)
    : StructuringElement3(s)
    {
    }
    
    StructuringElementSphere::~StructuringElementSphere()
    {
    }
    
    StructuringElement3Rectangle::StructuringElement3Rectangle(int s, int r, int c, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementRectangle(*this, s, r, c, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }
    
    StructuringElement3Rectangle::StructuringElement3Rectangle(const StructuringElement3Rectangle& s)
    : StructuringElement3(s)
    {
    }
    
    StructuringElement3Rectangle::~StructuringElement3Rectangle()
    {
    }

    StructuringElementCube::StructuringElementCube(int a, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementCube(*this, a, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }
    
    StructuringElementCube::StructuringElementCube(const StructuringElementCube& s)
    : StructuringElement3(s)
    {
    }
    
    StructuringElementCube::~StructuringElementCube()
    {
    }
    
    StructuringElementBall::StructuringElementBall(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementBall(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
	this->r= r;
    }
    
    StructuringElementBall::StructuringElementBall(const StructuringElementBall& s)
    : StructuringElement3(s)
    {
    }
    
    StructuringElementBall::~StructuringElementBall()
    {
    }

    StructuringElementAxial::StructuringElementAxial(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementAxial(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }

    StructuringElementAxial::StructuringElementAxial(const StructuringElementAxial& s)
    : StructuringElement3(s)
    {
    }

    StructuringElementAxial::~StructuringElementAxial()
    {
    }
    
    StructuringElementCS::StructuringElementCS(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementCS(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }

    StructuringElementCS::StructuringElementCS(const StructuringElementCS& s)
    : StructuringElement3(s)
    {
    }

    StructuringElementCS::~StructuringElementCS()
    {
    }
    
    StructuringElementAC::StructuringElementAC(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementAC(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }

    StructuringElementAC::StructuringElementAC(const StructuringElementAC& s)
    : StructuringElement3(s)
    {
    }

    StructuringElementAC::~StructuringElementAC()
    {
    }
    
    StructuringElementAS::StructuringElementAS(int r, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementAS(*this, r, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }

    StructuringElementAS::StructuringElementAS(const StructuringElementAS& s)
    : StructuringElement3(s)
    {
    }

    StructuringElementAS::~StructuringElementAS()
    {
    }
    
    StructuringElementLineSegment::StructuringElementLineSegment(int r, float a, float b, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
        generateStructuringElementLineSegment(*this, r, a, b, stride, slicePitch);
        this->stride= stride;
        this->slicePitch= slicePitch;
        this->computeMinMaxOnly();
    }

    StructuringElementLineSegment::StructuringElementLineSegment(Vector<int>& tmp, int stride, int slicePitch)
    : StructuringElement3(stride, slicePitch)
    {
      for ( unsigned int i= 0; i < tmp.size(); ++i )
	this->push_back(tmp(i));
      this->computeMinMaxOnly();
    }
    
    StructuringElementLineSegment::StructuringElementLineSegment(const StructuringElementLineSegment& s)
    : StructuringElement3(s)
    {
    }

    StructuringElementLineSegment::~StructuringElementLineSegment()
    {
    }
}
