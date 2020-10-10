#include <openipDS/LineSegment2.h>

namespace openip
{
    LineSegment2::LineSegment2()
    {
      r0= c0= r1= c1= 0;
    }

    LineSegment2::LineSegment2(const LineSegment2& ls)
    {
      r0= ls.r0;
      c0= ls.c0;
      r1= ls.r1;
      c1= ls.c1;
    }

    LineSegment2::LineSegment2(Region2& r)
    {
      r.getCoordinatesOfFarestPixels(r0, c0, r1, c1);
    }
    
    LineSegment2::LineSegment2(int r0, int c0, int r1, int c1)
    {
      this->r0= r0;
      this->c0= c0;
      this->r1= r1;
      this->c1= c1;
    }

    LineSegment2::~LineSegment2()
    {
    }

    LineSegment2& LineSegment2::operator=(const LineSegment2& ls)
    {
      this->r0= ls.r0;
      this->c0= ls.c0;
      this->r1= ls.r1;
      this->c1= ls.c1;
      
      return *this;
    }
    
    float LineSegment2::length()
    {
      return sqrt((r0 - r1)*(r0 - r1) + (c0 - c1)*(c0 - c1));
    }

    void LineSegment2::orderCoordinates()
    {
      if ( r1 < r0 )
      {
	int tmp= r1;
	r1= r0;
	r0= tmp;
	tmp= c1;
	c1= c0;
	c0= tmp;
      }
    }

    void LineSegment2::enlargeTowardsTheOrigin(float times)
    {
      orderCoordinates();

      float vr= (r0 - r1);
      float vc= (c0 - c1);

      r0= r0 + int(times * vr);
      c0= c0 + int(times * vc);
    }

    void LineSegment2::enlargeOutwardsTheOrigin(float times)
    {
      orderCoordinates();

      float vr= (r0 - r1);
      float vc= (c0 - c1);

      r1= r1 - int(times * vr);
      c1= c1 - int(times * vc);
    }
    
    void LineSegment2::enlarge(float times)
    {
      float vr= (r1 - r0);
      float vc= (c1 - c0);
      
      r1= r1 + int(times*vr);
      c1= c1 + int(times*vc);
    }

    Region2 LineSegment2::convertToRegion(int stride)
    {
      Region2 r;
      r.stride= stride;
      
      int n= 10000.0;

      float dr= float(r0 - r1)/n;
      float dc= float(c0 - c1)/n;

      for ( int i= 0; i < n; ++i )
      {
	int tr= (r1 + int(i*dr));
	int tc= c1 + int(i*dc);
	r.push_back(tr*stride + tc);
      }

      r.removeDuplicates();
      return r;
    }

    float extrapolateNumberSeries(Vector<int>& yy, Vector<int>& xx, int vx)
    {
      Vector<int> x;
      Vector<int> y;

      if ( xx.size() <= 2 )
      {
	x= xx;
	y= yy;
      }
      else
      {
	for ( unsigned int i= xx.size()/2; i < xx.size(); ++i )
	{
	  x.push_back(xx(i));
	  y.push_back(yy(i));
	}
      }
      
      float beta;
      float alpha;
      
      float meanx= x.getMean();
      float meany= y.getMean();
      float varx= x.getVariance()*x.size();
      float covar= 0;
      
      for ( unsigned int i= 0; i < x.size(); ++i )
	covar+= (x(i) - meanx)*(y(i) - meany);
      
      beta= covar/varx;
      alpha= meany - beta*meanx;

      return float(vx*beta + alpha);
    }

    LineSegment2 extrapolateLineSegments(Vector<LineSegment2>& yvalues, Vector<int>& xvalues, int idx)
    {
      LineSegment2 result;
      
      Vector<int> x;
      Vector<int> r0;
      Vector<int> c0;
      Vector<int> r1;
      Vector<int> c1;

      for ( unsigned int i= 0; i < yvalues.size(); ++i )
      {
	x.push_back(xvalues(i));
	r0.push_back(yvalues(i).r0);
	r1.push_back(yvalues(i).r1);
	c0.push_back(yvalues(i).c0);
	c1.push_back(yvalues(i).c1);
      }
      
      result.r0= extrapolateNumberSeries(r0, x, idx);
      result.c0= extrapolateNumberSeries(c0, x, idx);
      result.r1= extrapolateNumberSeries(r1, x, idx);
      result.c1= extrapolateNumberSeries(c1, x, idx);
      
      return result;
    }
}