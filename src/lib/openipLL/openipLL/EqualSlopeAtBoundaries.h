#ifndef _EQUAL_SLOPE_AT_BOUNDARIES_H_
#define _EQUAL_SLOPE_AT_BOUNDARIES_H_

#include <openipDS/Image.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StructuringElement2s.h>

#include <math.h>

int backgroundlength= 5;

namespace openip
{
  template<typename INPUT>
  class EqualSlopeAtBoundaries
  {
  public:
    EqualSlopeAtBoundaries(int r, int length, float slope, int width);

    EqualSlopeAtBoundaries(const EqualSlopeAtBoundaries& e);

    virtual ~EqualSlopeAtBoundaries();

    virtual void computeDescriptors(Image<unsigned char>& input, int n, Image<INPUT>& image, float& width, float& dyn, float& ri);
    
    virtual void apply(Image<unsigned char>& input, Image<unsigned char>& output, Image<INPUT>& image);

    virtual int apply(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float apply2(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float apply2all(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float dyn2(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float dyn2all(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual int swidth(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual int swidth2(Image<unsigned char>& input, int n, Image<unsigned char>& image, int f= 0);
    
    virtual int swidth(Image<unsigned char>& input, int n, int f= 0);
    
    virtual int swidth2(Image<unsigned char>& input, int n, int f= 0);
    
    virtual int swidthb(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float swidthc(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual float swidthb2(Image<unsigned char>& input, int n, int f= 0);
    
    virtual float diff(Image<unsigned char>& input, int n, Image<INPUT>& image, int f= 0);
    
    virtual void debug(Image<unsigned char>& input, Image<unsigned char>& output, Image<INPUT>& image);

    virtual int debug(Image<unsigned char>& input, int n, Image<INPUT>& image);

    virtual int updateStride(int stride);
    
    virtual int isOuterContour8(Image<unsigned char>& input, int i);
    
    virtual int isInnerContour8(Image<unsigned char>& input, int i);
    
    virtual int isInnerContour4(Image<unsigned char>& input, int i);
    
    virtual Border2 getProposedBorder();

    SobelFilterX<unsigned char, float> sx;
    SobelFilterY<unsigned char, float> sy;
    
    SobelFilterX<float, float> fsx;
    SobelFilterY<float, float> fsy;
    
    StructuringElementDisk sed;
    StructuringElementDisk sed2;
    StructuringElementSquare ses5;
    StructuringElementSquare ses3;

    int r;
    int length;
    int width;
    float slope;
    
    int stride;
    
    Vector<int> pos;
    
    Vector<Vector<int> > coordinates;
    Vector<Vector<int> > rows;
    Vector<Vector<int> > columns;
  };

  template<typename INPUT>
  EqualSlopeAtBoundaries<INPUT>::EqualSlopeAtBoundaries(int radius, int length, float slope, int width)
  {
    this->r= radius;
    this->length= length;
    this->slope= slope;
    this->width= width;
    this->stride= 0;
    
    generateStructuringElementDisk(sed, r, 4000);
    generateStructuringElementDisk(sed2, 2*r, 4000);
    generateStructuringElementSquare(ses5, 5, 4000);
    generateStructuringElementSquare(ses3, 3, 4000);
    
    int res= 10000, c, r;
    rows.resize(res);
    columns.resize(res);
    coordinates.resize(res);
    
    for ( int i= 0; i < res; ++i )
    {
      float angle= 2*M_PI/res*i - M_PI;
      
      for ( int j= 0; j < 4*length; ++j )
      {
	c= (cos(angle)*j*0.25);
	r= (sin(angle)*j*0.25);
	
	if ( rows(i).size() == 0 || rows(i)(rows(i).size()-1) != r || columns(i)(columns(i).size()-1) != c )
	{
	  rows(i).push_back(r);
	  columns(i).push_back(c);
	}
      }
    }
  }

  template<typename INPUT>
  EqualSlopeAtBoundaries<INPUT>::EqualSlopeAtBoundaries(const EqualSlopeAtBoundaries& e)
  {
    this->r= e.r;
    this->length= e.length;
    this->slope= e.slope;
    this->width= e.width;
    this->stride= e.stride;
    
    sed= e.sed;
  }

  template<typename INPUT>
  EqualSlopeAtBoundaries<INPUT>::~EqualSlopeAtBoundaries()
  {
  }

  template<typename INPUT>
  void EqualSlopeAtBoundaries<INPUT>::apply(Image<unsigned char>& /*input*/, Image<unsigned char>& /*output*/, Image<INPUT>& /*image*/)
  {
  }
  
  template<typename INPUT>
  void EqualSlopeAtBoundaries<INPUT>::debug(Image<unsigned char>& /*input*/, Image<unsigned char>& /*output*/, Image<INPUT>& /*image*/)
  {
  }

  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::apply(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& /*image*/, int /*flag*/)
  {
    
    return 0;
  }
  
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::apply2(Image<unsigned char>& input, int n, Image<INPUT>& image, int /*flag*/)
  {
    float gx, gy, t;
    float ats, atc, magn;
    int num;
    float sum;
    
    gx= gy= t= ats= atc= 0;
    num= 0;
    sum= 0;
    
    // computing average angles
    StructuringElementSquare ses(5);
    ses.updateStride(input.columns);
    for ( unsigned int i= 0; i < ses.size(); ++i )
    {
      if ( isOuterContour8(input, n + ses(i)) || isInnerContour8(input, n + ses(i)) )
      {
	++num;
	gx= sx.apply(input, n + ses(i));
	gy= sy.apply(input, n + ses(i));
	t= atan2(gy, gx);
	magn= sqrt(gx*gx + gy*gy);
	ats+= sin(t)*magn;
	atc+= cos(t)*magn;
	sum+= magn;
      }
    }
    if ( (ats == 0 && atc == 0) /*|| sum == 0*/ )
      return 0;
    
    ats/= sum;
    atc/= sum;
    
    t= atan2(atc, ats);
    
    // determining points of the line segment
    Vector<int> p;
    for ( int i= 0; i < length*4; ++i )
    {
      int c= cos(t)*i*0.25;
      int r= sin(t)*i*0.25;
      
      int m= r*input.columns + c;
      
      if ( p.size() == 0 || p(p.size()-1) != m )
	p.push_back(m);
    }
    
    // determining borders of the intersected vessel
    int a= -1, b= -1;
    for ( unsigned int i= 0; i < p.size(); ++i )
    {
      if ( a == -1 && input(n + p(i)) > 0 )
	a= i;
      if ( a != -1 && (input(n + p(i)) == 0 || i == p.size()-1) )
      {
	b= i;
	break;
      }
    }
    
    // finding minimum value covered by the line segment
    float minValue= 255;
    for ( int i= a; i < b; ++i )
      if ( input(n + p(i)) && image(n + p(i)) < minValue )
	minValue = image(n + p(i));
      
    float meanValue= 0;
    int nn= 0;
    //for ( int i= 1; i < p.size(); ++i )
    int m= 0;
    Vector<INPUT> background;
    for ( unsigned int i= 1; i < p.size() /*&& !input(n - p(i))*/&& int(background.size()) < backgroundlength; ++i )
    {
      if ( n - p(i) >= 0 && n - p(i) < int(input.n) && !input(n - p(i)) && !isOuterContour8(input, n - p(i)) ) 
      {
	meanValue+= image(n - p(i));
	background.push_back(image(n - p(i)));
	++nn;
	++m;
      }
    }
    
    meanValue/= nn;

    
    if ( meanValue == minValue )
      return 0;

    float slope= (image(n) - minValue)/(meanValue - minValue);
    
    if ( slope > 1 )
      return 1;
    if ( slope < 0 )
      return 0;
    
    return slope;

  }
  
  template<typename INPUT>
  void EqualSlopeAtBoundaries<INPUT>::computeDescriptors(Image<unsigned char>& input, int n, Image<INPUT>& image, float& width, float& dyn, float& ri)
  {
    float gx, gy, t;
    float ats, atc, magn;
    int num;
    float sum;
    
    gx= gy= t= ats= atc= 0;
    num= 0;
    sum= 0;
    
    // computing average angles
    for ( unsigned int i= 0; i < ses5.size(); ++i )
    {
      if ( (isOuterContour8(input, n + ses5(i)) || isInnerContour8(input, n + ses5(i))) )
      {
	++num;
	gx= sx.apply(input, n + ses5(i));
	gy= sy.apply(input, n + ses5(i));
	t= atan2(gy, gx);
	magn= sqrt(gx*gx + gy*gy);
	ats+= sin(t)*magn;
	atc+= cos(t)*magn;
	sum+= magn;
      }
    }
    if ( (ats == 0 && atc == 0) )
    {
      dyn= ri= 0;
      width= -1;
      return;
    }
    
    ats/= sum;
    atc/= sum;
    
    t= atan2(atc, ats);
    
    //printf("b: %f ", t);
    
    // determining idx of the line segment
    int idx= (int)floor((t + M_PI)*rows.size()/(2*M_PI) + 0.5);
    if ( idx < 0 )
      idx= 0;
    if ( idx >= int(rows.size()) )
      idx= rows.size()-1;
    /*for ( int i= 0; i < rows(idx).size(); ++i )
      printf("%d,%d ", rows(idx)(i), columns(idx)(i));
    printf("\n");*/
    
    // determining borders of the intersected vessel
    int a= -1, b= -1;
    for ( unsigned int i= 0; i < coordinates(idx).size(); ++i )
    {
      //if ( n + coordinates(idx)(i) >= 0 && n + coordinates(idx)(i) < input.n )
      {
	if ( a == -1 && input(n + coordinates(idx)(i)) > 0 )
	  a= i;
	if ( a != -1 && (input(n + coordinates(idx)(i)) == 0 || i == coordinates(idx).size()-1) )
	{
	  b= i;
	  break;
	}
      }
    }
    //printf("c: %d,%d\n", a, b);
    
    // determining width
    int ar= 0, ac= 0, br= 0, bc= 0;
    if ( a != -1 )
    {
      ar= rows(idx)(a);
      ac= columns(idx)(a);
    }
    
    if ( b != -1 )
    {
      br= rows(idx)(b);
      bc= columns(idx)(b);
    }
    
    width= (sqrt((ar - br)*(ar - br) + (ac - bc)*(ac - bc)));
    
    // finding minimum value covered by the line segment
    float minValue= 255;
    for ( int i= a; i < b; ++i )
      if ( input(n + coordinates(idx)(i)) && image(n + coordinates(idx)(i)) < minValue )
	minValue = image(n + coordinates(idx)(i));
      
    float meanValue= 0;
    int nn= 0;
    int bglength= width/2 < 5 ? 5 : width/2;
    //int bglength= backgroundlength;
    for ( unsigned int i= 1; i < coordinates(idx).size() && nn < int(bglength); ++i )
    {
      if ( n - coordinates(idx)(i) >= 0 && n - coordinates(idx)(i) < int(input.n) && !input(n - coordinates(idx)(i)) && !isOuterContour8(input, n - coordinates(idx)(i)) ) 
      {
	meanValue+= image(n - coordinates(idx)(i));
	++nn;
      }
    }
    if ( nn > 0 )
      meanValue/= nn;
    else
      meanValue= 0;

    // determining relative intensity
    if ( meanValue == minValue )
      ri= 0;
    else
      ri= (image(n) - minValue)/(meanValue - minValue);
    
    if ( ri > 1 )
      ri= 1;
    if ( ri < 0 )
      ri= 0;
    
    // determining dyn
    if ( meanValue <= minValue )
      dyn= 0;
    else
      dyn= meanValue - minValue;
    
    
    
    
    
    
    
    
  }
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::apply2all(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& /*image*/, int /*flag*/)
  {
    
      return 0;
  }
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::dyn2all(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& /*image*/, int /*flag*/)
  {
   
      return 0;
  }
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::dyn2(Image<unsigned char>& input, int n, Image<INPUT>& image, int /*flag*/)
  {
    float gx, gy, t;
    float ats, atc, magn;
    int num;
    float sum;
    
    gx= gy= t= ats= atc= 0;
    num= 0;
    sum= 0;
    
    // computing average angles
    StructuringElementSquare ses(5);
    ses.updateStride(input.columns);
    for ( unsigned int i= 0; i < ses.size(); ++i )
    {
      if ( isOuterContour8(input, n + ses(i)) || isInnerContour8(input, n + ses(i)) )
      {
	++num;
	gx= sx.apply(input, n + ses(i));
	gy= sy.apply(input, n + ses(i));
	t= atan2(gy, gx);
	magn= sqrt(gx*gx + gy*gy);
	ats+= sin(t)*magn;
	atc+= cos(t)*magn;
	sum+= magn;
      }
    }
    if ( (ats == 0 && atc == 0) /*|| sum == 0*/ )
      return 0;
    
    ats/= sum;
    atc/= sum;
    
    t= atan2(atc, ats);
    
    printf("b: %f ", t);
    
    // determining points of the line segment
    Vector<int> p;
    for ( int i= 0; i < length*4; ++i )
    {
      int c= cos(t)*i*0.25;
      int r= sin(t)*i*0.25;
      
      int m= r*input.columns + c;
      
      if ( p.size() == 0 || p(p.size()-1) != m )
      {
	printf("%d,%d ", r, c);
	p.push_back(m);
      }
    }
    
    // determining borders of the intersected vessel
    int a= -1, b= -1;
    for ( unsigned int i= 0; i < p.size(); ++i )
    {
      if ( a == -1 && input(n + p(i)) > 0 )
	a= i;
      if ( a != -1 && (input(n + p(i)) == 0 || i == p.size()-1) )
      {
	b= i;
	break;
      }
    }
    printf("c: %d,%d\n", a, b);
    // finding minimum value covered by the line segment
    float minValue= 255;
    for ( int i= a; i < b; ++i )
      if ( input(n + p(i)) && image(n + p(i)) < minValue )
	minValue = image(n + p(i));
      
    float meanValue= 0;
    int nn= 0;
    //for ( int i= 1; i < p.size(); ++i )
    int m= 0;
    Vector<INPUT> background;
    for ( unsigned int i= 1; i < p.size() /*&& !input(n - p(i))*/&& int(background.size()) < backgroundlength; ++i )
    {
      if ( n - p(i) >= 0 && n - p(i) < int(input.n) && !input(n - p(i)) && !isOuterContour8(input, n - p(i)) ) 
      {
	meanValue+= image(n - p(i));
	background.push_back(image(n - p(i)));
	++nn;
	++m;
      }
    }
    
    meanValue/= nn;

    if ( background.size() > 0 )
    {
      
    }
    else
    {
      //printf(".");
      //input(n)= 128;
      return 0;
    }
    
    //printf("%f:%f:%f ", image(n), minValue, meanValue);
    if ( meanValue <= minValue )
      return 0;
    /*return floor((meanValue - minValue) + 0.5);*/
    return meanValue - minValue;
    //return 0;
  }

  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::swidth2(Image<unsigned char>& /*input*/, int /*n*/, int )
  {
    
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::swidth(Image<unsigned char>& /*input*/, int /*n*/, int /*flag*/)
  {
    
    return 0;
  }
  
  
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::diff(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& /*image*/, int /*flag*/)
  {
    
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::swidth2(Image<unsigned char>& /*input*/, int /*n*/, Image<unsigned char>& /*image*/, int /*flag*/)
  {
    
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::swidth(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& , int /*flag*/)
  {
    
      return 0;
  }
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::swidthb2(Image<unsigned char>& input, int n, int )
  {
    float gx, gy, t;
    float ats, atc, magn;
    int num;
    float sum;
    
    gx= gy= t= ats= atc= 0;
    num= 0;
    sum= 0;
    
    StructuringElementSquare ses(5);
    ses.updateStride(input.columns);
    
    // computing average angles
    for ( unsigned int i= 0; i < ses.size(); ++i )
    {
      if ( isOuterContour8(input, n + ses(i)) || isInnerContour8(input, n + ses(i)) )
      {
	if ( n + ses(i) >= input.columns+1 && int(n + ses(i)) < int(input.n) - input.columns-1 && n + ses(i) >= input.columns+1 && int(n + ses(i)) < int(input.n) - input.columns-1 )
	{
	  ++num;
	  gx= sx.apply(input, n + ses(i));
	  gy= sy.apply(input, n + ses(i));
	  t= atan2(gy, gx);
	  /*if ( t < 0 )
	    t+= M_PI;
	  if ( t > M_PI )
	    t-= M_PI;*/
	  magn= sqrt(gx*gx + gy*gy);
	  ats+= sin(t)*magn;
	  atc+= cos(t)*magn;
	  sum+= magn;
	}
      }
    }

    //printf("%f ", sum); fflush(stdout);
    if ( (ats == 0 && atc == 0) /*|| sum == 0*/ )
      return -1;
    
    ats/= sum;
    atc/= sum;
    
    t= atan2(atc, ats);
    
    //printf("a:%f ", t);
    
    // determining points of the line segment
    Vector<int> pr;
    Vector<int> pc;
    for ( int i= 0; i < length*4; ++i )
    {
      int c= (cos(t)*i*0.25);
      int r= (sin(t)*i*0.25);
      
      //int m= (r*input.columns + c);
      
      if ( pr.size() == 0 || (pr(pr.size()-1) != r || pc(pc.size()-1) != c) )
      {
	pr.push_back(r);
	pc.push_back(c);
      }
    }
    
    /*for ( int i= 0; i < pr.size(); ++i )
      printf("%d,%d ", pr(i), pc(i));
    printf("\n");*/
    
    // determining borders of the intersected vessel
    int a= -1, b= -1;
    for ( unsigned int i= 0; i < pr.size(); ++i )
    {
      if ( n + pr(i)*input.columns + pc(i) >= 0 && n + pr(i)*input.columns + pc(i) < int(input.n) )
      {
	if ( a == -1 && input(n + pr(i)*input.columns + pc(i)) > 0 )
	  a= i;
	if ( a != -1 && (input(n + pr(i)*input.columns + pc(i)) == 0 || i == pr.size()-1) )
	{
	  b= i;
	  break;
	}
      }
      /*else
	break;*/
    }
    //printf("c: %d,%d\n", a, b);
    Region2 r;
    r.stride= input.columns;
    
    int ar, ac, br, bc;
    if ( a != -1 )
    {
	//r.getCoordinate2D(p(a), ar, ac);
      ar= pr(a);
      ac= pc(a);
    }
    else
      ar= ac= 0;
    if ( b != -1 )
    {
      //r.getCoordinate2D(p(b), br, bc);
      br= pr(b);
      bc= pc(b);
    }
    else
      br= bc= 0;
    //printf("%d,%d,%d,%d,%d,%d ", p(a),p(b),ar, ac, br, bc);
    float wid= sqrt((ar - br)*(ar - br) + (ac - bc)*(ac - bc));
    //printf("width: %f\n", wid);
    return (wid);
    
    /*if ( b == -1 || a == -1 )
      return 0;
    
    return (b-a);*/
    //return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::swidthb(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& , int /*flag*/)
  {
    
      return 0;
  }
  
  template<typename INPUT>
  float EqualSlopeAtBoundaries<INPUT>::swidthc(Image<unsigned char>& input, int n, Image<INPUT>& , int flag)
  {
    //float mean= 0;
    StructuringElementSquare ses(5);
    ses.updateStride(input.columns);
    
    //int nn= 0;
    Vector<float> w;
    
    
    
    for ( unsigned int i= 0; i < ses.size(); ++i )
      //if ( ses(i) == 0 )
      if ( isInnerContour8(input, n + ses(i)) || isOuterContour8(input, n + ses(i) ) )
      {
	int wth= swidthb2(input, n + ses(i), flag);
	if ( wth >= 0 )
	  w.push_back(wth);
      }
    
    if ( w.size() > 0 )
      return w.getMean();
    else
      return 0;
      
    
    return 0;
  }  
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::debug(Image<unsigned char>& /*input*/, int /*n*/, Image<INPUT>& /*image*/)
  {
   
    
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::isOuterContour8(Image<unsigned char>& input, int n)
  {
    if ( n < 0 || n >= int(input.n) || input(n) )
      return 0;
    
    for ( unsigned int i= 0; i < pos.size(); ++i )
      if ( n + pos(i) >= 0 && n + pos(i) < int(input.n) && input(n + pos(i)) )
	return 1;
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::isInnerContour8(Image<unsigned char>& input, int n)
  {
    if ( n < 0 || n >= int(input.n) || !input(n) )
      return 0;
    
    for ( unsigned int i= 0; i < pos.size(); ++i )
      if ( n + pos(i) >= 0 && n + pos(i) < int(input.n) && input(n + pos(i)) == 0 )
	return 1;
      
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::isInnerContour4(Image<unsigned char>& input, int n)
  {
    if ( n < 0 || n >= int(input.n) || !input(n) )
      return 0;
    
    if ( n + 1 >= 0 && n + 1 < int(input.n) && !input(n + 1) )
      return 1;
    if ( n - 1 >= 0 && n - 1 < int(input.n) && !input(n - 1) )
      return 1;
    if ( n + input.columns >= 0 && n + input.columns < int(input.n) && !input(n + input.columns) )
      return 1;
    if ( n + input.columns >= 0 && n + input.columns < int(input.n) && !input(n + input.columns) )
      return 1;
    
    return 0;
  }
  
  template<typename INPUT>
  int EqualSlopeAtBoundaries<INPUT>::updateStride(int stride)
  {
    this->stride= stride;
    
    sx.updateStride(stride);
    sy.updateStride(stride);
    fsx.updateStride(stride);
    fsy.updateStride(stride);
    sed.updateStride(stride);
    sed2.updateStride(stride);
    ses5.updateStride(stride);
    ses3.updateStride(stride);
    
    
    
    pos.clear();
    
    pos.push_back(1);
    pos.push_back(-1);
    pos.push_back(stride);
    pos.push_back(-stride);
    pos.push_back(1+stride);
    pos.push_back(1-stride);
    pos.push_back(-1+stride);
    pos.push_back(-1-stride);
    
    for ( unsigned int i= 0; i < rows.size(); ++i )
    {
      coordinates(i).resize(rows(i).size());
      for ( unsigned int j= 0; j < coordinates(i).size(); ++j )
	coordinates(i)(j)= rows(i)(j)*stride + columns(i)(j);
      
    }
    
    return 0;
  }
  
  template<typename INPUT>
  Border2 EqualSlopeAtBoundaries<INPUT>::getProposedBorder()
  {
    if ( r > length )
      return Border2(r, r, r, r);
    else
      return sx.getProposedBorder();
  }
}

#endif
