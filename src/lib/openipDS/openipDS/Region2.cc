#include <openipDS/Region2.h>
#include <openipDS/StructuringElement2s.h>

#include <math.h>
#include <limits.h>

namespace openip
{
    
  
    Region2::Region2()
    : PixelSet1()
    {
    }

    Region2::Region2(const Region2& r)
    : PixelSet1(r)
    {
    }

    Region2::~Region2()
    {
    }

    Region2::Region2(int stride)
    : PixelSet1()
    {
      this->stride= stride;
    }
    
    void Region2::getCoordinate2D(int n, int& r, int& c)
    {
	c= n % stride;
	r= n / stride;
	
	{
	    /*r= n / this->stride;
	    if ( abs(n % this->stride) > this->stride/2 )
		r= (n > 0) ? r+1 : r-1;

	    c= n % this->stride;
	    if ( abs(c) > this->stride/2 )
		c+= c > 0 ? -this->stride : this->stride;


	    //printf("%d %d %d %d\n", x, y, stride, *pit);
	    //printf(".");
	    printf("%d,%d,%d,%d ", n, r, c, r*stride + c);*/
	}
    }
    
    void Region2::getCenter(int& rr, int& cc)
    {
      float mr= 0;
      float mc= 0;
      int r, c;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	mr+= r;
	mc+= c;
	
      }
      rr= mr/this->size();
      cc= mc/this->size();
      printf("%d %d %f %f %zu\n", rr, cc, mr, mc, this->size());
    }
    
    int Region2::width()
    {
      int wmin= 1000000, wmax= 0;
      int r, c;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	if ( c < wmin )
	  wmin= c;
	if ( c > wmax )
	  wmax= c;
      }
      
      return wmax - wmin + 1;
    }
    
    int Region2::height()
    {
      int hmin= 1000000, hmax= 0;
      int r, c;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	if ( r < hmin )
	  hmin= r;
	if ( r > hmax )
	  hmax= r;
      }
      
      return hmax - hmin + 1;
    }
    
    int Region2::area()
    {
        return this->size();
    }

    int Region2::perimeter()
    {
        int p= 0;
        int coord;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            coord= (*this)(i) + 1;
            if ( find(coord) == end() )
            {
                ++p;
                continue;
            }
            coord= (*this)(i) - 1;
            if ( find(coord) == end() )
            {
                ++p;
                continue;
            }
            coord= (*this)(i) - stride;
            if ( find(coord) == end() )
            {
                ++p;
                continue;
            }
            coord= (*this)(i) + stride;
            if ( find(coord) == end() )
            {
                ++p;
                continue;
            }
        }

        return p;
    }
    
    int Region2::perimeterLength()
    {
        int coord;
	Vector<int> outerContour;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            coord= (*this)(i) + 1;
            if ( find(coord) == end() )
            {
                //++p;
		//continue;
	      outerContour.push_back(coord);
            }
            coord= (*this)(i) - 1;
            if ( find(coord) == end() )
            {
                //++p;
		//continue;
	      outerContour.push_back(coord);
            }
            coord= (*this)(i) - stride;
            if ( find(coord) == end() )
            {
                //++p;
		//continue;
	      outerContour.push_back(coord);
            }
            coord= (*this)(i) + stride;
            if ( find(coord) == end() )
            {
                //++p;
		//continue;
	      outerContour.push_back(coord);
            }
        }

        sort(outerContour.begin(), outerContour.end());
	Vector<int>::iterator it= unique(outerContour.begin(), outerContour.end());
	outerContour.resize(std::distance(outerContour.begin(), it));
        
        return outerContour.size();
    }

    float Region2::circularity()
    {
        float a= area();
        float p2= perimeterLength();
        p2*= p2;

	float c= a*4*M_PI/p2;
        return c;
    }
    
    int Region2::isCircularitySmallerThan(float /*circ*/)
    {
	return 0;
    }

    int Region2::isOuterContour4(Pixel1 p)
    {
        if ( !contains(p) &&
             (contains(p + 1) ||
              contains(p - 1) ||
              contains (p + stride) ||
              contains(p - stride)) )
            return 1;
        return 0;
    }

    int Region2::isOuterContour8(Pixel1 p)
    {
        if ( !contains(p) &&
             (contains(p + 1) ||
              contains(p - 1) ||
              contains(p + stride) ||
              contains(p - stride) ||
              contains(p + stride + 1) ||
              contains(p + stride - 1) ||
              contains(p - stride + 1) ||
              contains(p - stride - 1)) )
            return 1;
        return 0;
    }

    int Region2::isInnerContour4(Pixel1 p)
    {
        if ( contains(p) &&
             (!contains(p + 1) ||
              !contains(p - 1) ||
              !contains (p + stride) ||
              !contains(p - stride)) )
            return 1;
        return 0;
    }

    int Region2::isInnerContour8(Pixel1 p)
    {
        if ( contains(p) &&
             (!contains(p + 1) ||
              !contains(p - 1) ||
              !contains(p + stride) ||
              !contains(p - stride) ||
              !contains(p + stride + 1) ||
              !contains(p + stride - 1) ||
              !contains(p - stride + 1) ||
              !contains(p - stride - 1)) )
            return 1;
        return 0;
    }

    Region2 Region2::getInnerContour4()
    {
        Region2 result;
        result.stride= stride;

        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( isInnerContour4((*this)(i)) )
                result.push_back((*this)(i));
        return result;
    }

    Region2 Region2::getOuterContour4()
    {
        Region2 result;
        result.stride= stride;

        /*for ( int i= 0; i < this->size(); ++i )
            if ( isOuterContour4((*this)(i)) )
                result.push_back((*this)(i));*/
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            if ( !this->contains((*this)(i) + 1) )
                result.insert((*this)(i) + 1);
            if ( !this->contains((*this)(i) - 1) )
                result.insert((*this)(i) - 1);
            if ( !this->contains((*this)(i) + stride) )
                result.insert((*this)(i) + stride);
            if ( !this->contains((*this)(i) - stride) )
                result.insert((*this)(i) - stride);
        }
        return result;
    }

    Region2 Region2::getInnerContour8()
    {
        Region2 result;
        result.stride= stride;

        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            if ( !this->contains((*this)(i) + 1) )
                result.insert((*this)(i) + 1);
            if ( !this->contains((*this)(i) - 1) )
                result.insert((*this)(i) - 1);
            if ( !this->contains((*this)(i) + stride) )
                result.insert((*this)(i) + stride);
            if ( !this->contains((*this)(i) - stride) )
                result.insert((*this)(i) - stride);
            if ( !this->contains((*this)(i) + 1 + stride) )
                result.insert((*this)(i) + 1 + stride);
            if ( !this->contains((*this)(i) - 1 - stride) )
                result.insert((*this)(i) - 1 - stride);
            if ( !this->contains((*this)(i) + stride - 1) )
                result.insert((*this)(i) + stride - 1);
            if ( !this->contains((*this)(i) - stride + 1) )
                result.insert((*this)(i) - stride + 1);
        }
        return result;
    }

    Region2 Region2::getOuterContour8()
    {
        Region2 result;
        result.stride= stride;

        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( isOuterContour8((*this)(i)) )
                result.push_back((*this)(i));
        return result;
    }
    
    bool Region2SizeComparator::operator() (const Region2& a, const Region2& b)
    {
        return a.size() < b.size();
    }
    
    int Region2::isIntersecting(Region2& r)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( r.find((*this)(i)) != r.end() )
	  return 1;
      return 0;
    }
    
    int Region2::isNeighboring(Region2& r)
    {
      for ( unsigned int i= 0; i < r.size(); ++i )
      {
	if ( find(r(i)) != end() || find(r(i) + r.stride) != end() || find(r(i) - r.stride) != end() || find(r(i) + 1) != end() || find(r(i) - 1) != end() )
	  return 1;
      }
      return 0;
    }
    
    int Region2::getIndexOfFarestPixel(int p)
    {
      int r, c;
      int r0, c0;
      int maxIdx= 0;
      int maxDist= 0;
      
      this->getCoordinate2D(p, r0, c0);
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	this->getCoordinate2D((*this)(i), r, c);
	if ( (r-r0)*(r-r0) + (c-c0)*(c-c0) > maxDist )
	{
	  maxDist= (r-r0)*(r-r0) + (c-c0)*(c-c0);
	  maxIdx= i;
	}
      }
      
      return (*this)(maxIdx);
    }
    
    void Region2::getCoordinatesOfClosestPixel(int r0, int c0, int& r, int& c)
    {
      int r1, c1;
      int bestR1= 0, bestC1=0;
      int minDist= INT_MAX;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	this->getCoordinate2D((*this)(i), r1, c1);
	if ( (r1-r0)*(r1-r0) + (c1-c0)*(c1-c0) < minDist )
	{
	  minDist= (r1-r0)*(r1-r0) + (c1-c0)*(c1-c0);
	  bestR1= r1;
	  bestC1= c1;
	}
      }
      
      r= bestR1;
      c= bestC1;
    }
    
    void Region2::getCoordinateOfFirstPixel(int dim, int dir, int& r, int& c)
    {
      int r0, c0;
      int minR0, minC0;
      
      if ( dir == 0 )
      {
	minR0= INT_MAX;
	minC0= INT_MAX;
      }
      else
      {
	minR0= -INT_MAX;
	minC0= -INT_MAX;
      }
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	this->getCoordinate2D((*this)(i), r0, c0);
	if ( dim == 0 )
	{
	  if ( dir == 0 )
	  {
	    if ( r0 < minR0 )
	    {
	      minR0= r0;
	      minC0= c0;
	    }
	  }
	  else
	  {
	    if ( r0 > minR0 )
	    {
	      minR0= r0;
	      minC0= c0;
	    }
	  }
	}
	else if ( dim == 1 )
	{
	  if ( dir == 0 )
	  {
	    if ( c0 < minC0 )
	    {
	      minR0= r0;
	      minC0= c0;
	    }
	  }
	  else
	  {
	    if ( c0 > minC0 )
	    {
	      minR0= r0;
	      minC0= c0;
	    }
	  }
	}
      }
      
      r= minR0;
      c= minC0;
    }
    
    void Region2::removeDuplicates()
    {
      sort( this->begin(), this->end() );
      this->erase( unique( this->begin(), this->end() ), this->end() );
    }
    
    Region2 Region2::connectSuccessivePixels(int resolution)
    {
      Region2 output;
      output.stride= this->stride;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	int r0, c0, r1, c1, r, c, lastr= 0, lastc= 0;
	float dr, dc;
	this->getCoordinate2D((*this)(i), r0, c0);
	this->getCoordinate2D((*this)((i+1)%(this->size())), r1, c1);
	
	dr= (r0 - r1)/float(resolution);
	dc= (c0 - c1)/float(resolution);
	
	for ( int j= 0; j < resolution; ++j )
	{
	  r= dr*j + r1;
	  c= dc*j + c1;
	  
	  if ( r != lastr || c != lastc )
	  {
	    output.push_back(r*output.stride + c);
	    //printf("(%d,%d %d,%d %d,%d) ", r0, c0, r1, c1, r, c);
	  }
	  
	  lastr= r;
	  lastc= c;
	}
      }
	
      output.removeDuplicates();
      return output;
    }
    
    Region2 Region2::connectPixels(int resolution)
    {
      Region2 output;
      output.stride= this->stride;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	int r0, c0, r1, c1, r, c, lastr= 0, lastc= 0;
	float dr, dc;
	this->getCoordinate2D((*this)(i), r0, c0);
	
	for ( unsigned int j= i + 1; j < this->size(); ++j )
	{
	  this->getCoordinate2D((*this)(j), r1, c1);
	  
	  dr= (r0 - r1)/float(resolution);
	  dc= (c0 - c1)/float(resolution);
	  
	  for ( int k= 0; k < resolution; ++k )
	  {
	    r= dr*k + r1;
	    c= dc*k + c1;
	    
	    if ( r != lastr || c != lastc )
	      output.push_back(r*output.stride + c);
	    
	    lastr= r;
	    lastc= c;
	  }
	}
      }
	
      output.removeDuplicates();
      return output;
    }
    
    void Region2::add(Region2& r)
    {
      this->stride= r.stride;
      for ( unsigned int i= 0; i < r.size(); ++i )
	this->push_back(r(i));
    }
    
    float Region2::getDistanceFromRegion(Region2& region)
    {
      float dist= FLT_MAX, tmp;
      
      int r, c, r0, c0;
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	for ( unsigned int j= 0; j < this->size(); ++j )
	{
	  this->getCoordinate2D((*this)(j), r0, c0);
	  tmp= sqrt((r-r0)*(r-r0) + (c-c0)*(c-c0));
	  if ( tmp < dist )
	    dist= tmp;
	}
      }
      
      return dist;
    }
    
    float Region2::getDistanceFromRegion(int n)
    {
      float dist= FLT_MAX, tmp;
      
      int r, c, r0, c0;
      this->getCoordinate2D(n, r, c);
      for ( unsigned int j= 0; j < this->size(); ++j )
      {
	this->getCoordinate2D((*this)(j), r0, c0);
	tmp= sqrt((r-r0)*(r-r0) + (c-c0)*(c-c0));
	if ( tmp < dist )
	  dist= tmp;
      }
      
      return dist;
    }
    
    float Region2::getMeanDistanceFromRegion(Region2& region)
    {
      float dist= FLT_MAX, tmp;
      float mean= 0;
      
      int r, c, r0, c0;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	dist= FLT_MAX;
	for ( unsigned int j= 0; j < region.size(); ++j )
	{
	  region.getCoordinate2D(region(j), r0, c0);
	  tmp= sqrt((r-r0)*(r-r0) + (c-c0)*(c-c0));
	  if ( tmp < dist )
	    dist= tmp;
	}
	mean+= dist;
      }
      return mean/this->size();
    }
    
    int Region2::getNumberOfPixelsCloserThan(Region2& region, float dist0)
    {
      float dist= FLT_MAX, tmp;
      int n= 0;
      
      int r, c, r0, c0;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	dist= FLT_MAX;
	for ( unsigned int j= 0; j < region.size(); ++j )
	{
	  region.getCoordinate2D(region(j), r0, c0);
	  tmp= sqrt((r-r0)*(r-r0) + (c-c0)*(c-c0));
	  if ( tmp < dist )
	    dist= tmp;
	}
	if ( dist < dist0 )
	  ++n;
      }
      return n;
    }
    
    void Region2::getCoordinatesOfFarestPixels(int& r0, int& c0, int& r1, int& c1)
    {
      int a0, b0, a1, b1;
      float maxDist= 0;
      r0= c0= r1= c1= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	this->getCoordinate2D((*this)(i), a0, b0);
	for ( unsigned int j= i+1; j < this->size(); ++j )
	{
	  this->getCoordinate2D((*this)(j), a1, b1);
	  float dist;
	  dist= (a0-a1)*(a0-a1) + (b0-b1)*(b0-b1);
	  if ( dist > maxDist )
	  {
	    r0= a0;
	    r1= a1;
	    c0= b0;
	    c1= b1;
	    maxDist= dist;
	  }
	}
      }
      
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
    
    float Region2::getDistanceOfFarestPixels()
    {
      int a0, b0, a1, b1;
      getCoordinatesOfFarestPixels(a0, b0, a1, b1);
      
      return sqrt(float((a0-a1)*(a0-a1) + (b0-b1)*(b0-b1)));
    }
    
    float Region2::distanceOfCenters(Region2& r)
    {
      int r0, c0, r1, c1;
      getCenter(r0, c0);
      r.getCenter(r1, c1);
      
      return sqrt((r0-r1)*(r0-r1) + (c0-c1)*(c0-c1));
    }
    
    Vector<float> Region2::getCenterOfGravity()
    {
      int r, c;
      getCenter(r, c);
      Vector<float> result;
      result.push_back(r);
      result.push_back(c);
      
      return result;
    }
    
    float Region2::rectangularity()
    {
      Region2 convexHull= this->getConvexHull();
      
      return float(this->size())/convexHull.size();
    }
    
    Region2 Region2::getConvexHull()
    {
      Region2 output;
      Region2 input= *this;
      if ( input.size() < 3 )
	{
	  output= input;
	  //printf("ccc0.5\n"); fflush(stdout);
	  return output;
	}
	
	Vector<int> rows;
	Vector<int> columns;
	
	Vector<int> result;
	result.resize(input.size());
	result= -1;
	
	int r, c;
	for ( unsigned int i= 0; i < input.size(); ++i )
	{
	  input.getCoordinate2D(input(i), r, c);
	  rows.push_back(r);
	  columns.push_back(c);
	}
	
	// find leftmost point
	int leftmost= 0;
	for ( unsigned int i= 0; i < columns.size(); ++i )
	  if ( columns(i) < columns(leftmost) )
	    leftmost= i;
	
	int p, q;
	p= leftmost;
	do
	{
	  q= (p+1)%input.size();
	  
	  for ( unsigned int i= 0; i < input.size(); ++i )
	    if ( orientation(rows(p), columns(p), rows(i), columns(i), rows(q), columns(q)) == 2 )
	      q= i;
	    
	  result(p)= q;
	  p= q;
	} while ( p != leftmost );
	
	output.clear();
	
	p= leftmost;
	//printf("fff\n"); fflush(stdout);
	do
	{
	  output.push_back(input(p));
	  p= result(p);
	} while ( p != leftmost );
	
	output.stride= input.stride;
	//printf("ddd %d\n", output.size()); fflush(stdout);
	Region2 contour;
	contour= output.connectSuccessivePixels(100);
	//printf("eee %d\n", contour.size()); fflush(stdout);
	Region2 res;
	res= contour.connectPixels(100);
	
	output= res;
	return output;
    }
    
    int Region2::orientation(int pr, int pc, int qr, int qc, int rr, int rc)
    {
      int val= (qr - pr) * (rc - qc) - (qc - pc) * (rr - qr);
      if ( val == 0 ) return 0;
      return (val > 0) ? 1 : 2;
    }
    
    float Region2::correlation()
    {
      Vector<int> rs;
      Vector<int> cs;
      
      int r, c;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	rs.push_back(r);
	cs.push_back(c);
      }
      
      float meanr, meanc, stdr, stdc, cov;
      meanr= rs.getMean();
      meanc= cs.getMean();
      stdr= rs.getStandardDeviation();
      stdc= cs.getStandardDeviation();
      
      cov= 0;
      for ( unsigned int i= 0; i < rs.size(); ++i )
      {
	cov+= (rs(i) - meanr)*(cs(i) - meanc);
      }
      cov/= this->size();
      cov/= stdr*stdc;
      
      return cov;
    }
    
    Vector<int> Region2::getEndPointCoordinates()
    {
      Vector<int> result;
      StructuringElementSquare ses(3);
      ses.updateStride(stride);
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	int n= 0;
	for ( unsigned int j= 0; j < ses.size(); ++j )
	  if ( ses(j) != 0 && this->contains((*this)(i) + ses(j)) )
	    ++n;
	if ( n == 1 )
	  result.push_back(i);
      }
      return result;
    }
    
    float Region2::distanceOfPoints(int a, int b)
    {
      int r1, c1, r2, c2;
      getCoordinate2D((a), r1, c1);
      getCoordinate2D((b), r2, c2);
      
      return sqrt((r1-r2)*(r1-r2) + (c1-c2)*(c1-c2));
    }
    
    float Region2::correlationNearEndPoint(int endPointCoordinate, int radius)
    {
      //printf("aaa\n"); fflush(stdout);
      Vector<float> rs;
      Vector<float> cs;
      
      int r, c;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( distanceOfPoints((*this)(endPointCoordinate), (*this)(i)) < radius )
	{
	  getCoordinate2D((*this)(i), r, c);
	  rs.push_back(r);
	  cs.push_back(c);
	}
      }
      //printf("bbb\n"); fflush(stdout);
      float meanr, meanc, stdr, stdc, cov;
      meanr= rs.getMean();
      meanc= cs.getMean();
      stdr= rs.getStandardDeviation();
      stdc= cs.getStandardDeviation();
      
      //printf("%f %f %f %f\n", meanr, meanc, stdr, stdc);
      
      if ( stdr == 0 || stdc == 0 )
	return 1;
      
      cov= 0;
      for ( unsigned int i= 0; i < rs.size(); ++i )
      {
	cov+= (rs(i) - meanr)*(cs(i) - meanc);
      }
      cov/= rs.size();
      cov/= stdr*stdc;
      
      return cov;
    }
    
    void Region2::drdcNearEndPoint(int endPointCoordinate, int radius, float& dr, float& dc)
    {
      int r0, c0, r1, c1;
      getCoordinate2D((*this)(endPointCoordinate), r0, c0);
      
      float maxDist= 0;
      int farestPoint= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( distanceOfPoints((*this)(endPointCoordinate), (*this)(i)) < radius &&  distanceOfPoints((*this)(endPointCoordinate), (*this)(i)) > maxDist )
	{
	  maxDist= distanceOfPoints((*this)(endPointCoordinate), (*this)(i));
	  farestPoint= i;
	}
      }
      
      getCoordinate2D((*this)(farestPoint), r1, c1);
      dr= r0-r1;
      dc= c0-c1;
    }
    
    int Region2::numberOfPixelsWith1Neighbor(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      int n= 0;
      int tmp= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	{
	  tmp= 0;
	  for ( unsigned int j= 0; j < ses.size(); ++j )
	  {
	    if ( ses(j) != 0 && input((*this)(i) + ses(j)) )
	      ++tmp;
	  }
	  if ( tmp == 1 )
	    ++n;
	}
      }
      return n;
    }
    
    int Region2::numberOfPixelsWith2Neighbor(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      int n= 0;
      int tmp= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	{
	  tmp= 0;
	  for ( unsigned int j= 0; j < ses.size(); ++j )
	  {
	    if ( ses(j) != 0 && input((*this)(i) + ses(j)) )
	      ++tmp;
	  }
	  if ( tmp == 2 )
	    ++n;
	}
      }
      return n;
    }
    
    int Region2::numberOfPixelsWith3Neighbor(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      int n= 0;
      int tmp= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	{
	  tmp= 0;
	  for ( unsigned int j= 0; j < ses.size(); ++j )
	  {
	    if ( ses(j) != 0 && input((*this)(i) + ses(j)) )
	      ++tmp;
	  }
	  if ( tmp == 3 )
	    ++n;
	}
      }
      return n;
    }
    
    int Region2::numberOfPixelsWith4Neighbor(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      int n= 0;
      int tmp= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	{
	  tmp= 0;
	  for ( unsigned int j= 0; j < ses.size(); ++j )
	  {
	    if ( ses(j) != 0 && input((*this)(i) + ses(j)) )
	      ++tmp;
	  }
	  if ( tmp == 4 )
	    ++n;
	}
      }
      return n;
    }
    
    int Region2::numberOfPixelsCovered(Image<unsigned char>& input)
    {
      int n= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	    ++n;
      }
      return n;
    }
    
    int Region2::numberOfCrossingPixels(Image<unsigned char>& input)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(input.columns);
      int n= 0;
      int tmp= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( input((*this)(i)) )
	{
	  tmp= 0;
	  for ( unsigned int j= 0; j < ses.size(); ++j )
	  {
	    if ( ses(j) != 0 && input((*this)(i) + ses(j)) )
	      ++tmp;
	  }
	  if ( tmp > 2 )
	    ++n;
	}
      }
      return n;
    }
 
    int Region2::numberOfNeighbors(int n)
    {
      StructuringElementSquare ses(3);
      ses.updateStride(stride);
      int result= 0;
      
      for ( unsigned int i= 0; i < ses.size(); ++i )
	if ( ses(i) != 0 && this->find(n + ses(i)) != this->end() )
	  result++;
      return result;
    }
 
    int Region2::isConnective(Vector<int>& endPoints)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( endPoints.find((*this)(i)) == endPoints.end() && numberOfNeighbors((*this)(i)) < 2 )
	  return 0;
      }
      return 1;
    }
 
    
    
    int Region2::getWidth()
    {
      int r, c;
      int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	if ( r < rmin )
	  rmin= r;
	if ( r > rmax )
	  rmax= r;
	if ( c < cmin )
	  cmin= c;
	if ( c > cmax )
	  cmax= c;
      }
      
      return cmax - cmin + 1;
    }
    
    int Region2::getHeight()
    {
      int r, c;
      int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	if ( r < rmin )
	  rmin= r;
	if ( r > rmax )
	  rmax= r;
	if ( c < cmin )
	  cmin= c;
	if ( c > cmax )
	  cmax= c;
      }
      
      return rmax - rmin + 1;
    }
    
    int Region2::isOuterContour4(Image<unsigned char>& input, Pixel1 p)
    {
	if ( !input(p) && (input(p + 1) || input(p - 1) || input(p + input.columns) || input(p - input.columns)))
            return 1;
        return 0;
    }

    int Region2::isOuterContour8(Image<unsigned char>& input, Pixel1 p)
    {
	if ( !input(p) && (input(p + 1) || input(p-1) || input(p + input.columns) || input(p - input.columns) || input(p + input.columns + 1) || input(p + input.columns - 1) || input(p - input.columns + 1) || input(p - input.columns - 1)))
            return 1;
        return 0;
    }

    int Region2::isInnerContour4(Image<unsigned char>& input, Pixel1 p)
    {
	if ( input(p) && (!input(p + 1) || !input(p - 1) || !input(p + input.columns) || !input(p - input.columns)))
            return 1;
        return 0;
    }

    int Region2::isInnerContour8(Image<unsigned char>& input, Pixel1 p)
    {
	if ( input(p) && (!input(p + 1) || !input(p - 1) || !input(p + input.columns) || ! input(p - input.columns) || !input(p + input.columns + 1) || !input(p + input.columns - 1) || !input(p - input.columns + 1) || !input(p - input.columns-1)))
            return 1;
        return 0;
    }
    
    Region2 Region2::translate(float dr, float dc, float d)
    {
      Region2 result;
      result.stride= this->stride;
      
      int r, c;
      
      float length= sqrt(dr*dr + dc*dc);
      dr/= length;
      dc/= length;
      dr*= d;
      dc*= d;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	r+= int(dr);
	c+= int(dc);
	result.push_back(r*stride + c);
      }
      
      return result;
    }
    
    Region2 Region2::rotate(float rr, float rc, float angle)
    {
      Region2 result;
      result.stride= this->stride;
      
      int r, c;
      int r0, c0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinate2D((*this)(i), r, c);
	
	r0= rr + (r - rr)*cos(angle) - (c - rc)*sin(angle);
	c0= rc + (r - rr)*sin(angle) + (c - rc)*cos(angle);
	
	result.push_back(r0*stride + c0);
      }
      
      return result;
    }
    
    Region2 Region2::getIntersection(Region2& r)
    {
      Region2 result;
      result.stride= r.stride;
      
      for ( unsigned int i= 0; i < r.size(); ++i )
	if ( this->find(r(i)) != this->end() )
	{
	  result.push_back(r(i));
	  continue;
	}
	
      return result;
    }
}
