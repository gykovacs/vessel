#include <openipLL/convexHull.h>
#include <openipDS/Stopper.h>

namespace openip
{
      ConvexHull::ConvexHull(int mode)
      {
	this->mode= mode;
      }
      
      ConvexHull::ConvexHull(const ConvexHull& /*c*/)
      {
      }
      
      ConvexHull::~ConvexHull()
      {
      }
      
      int ConvexHull::orientation(int pr, int pc, int qr, int qc, int rr, int rc)
      {
	int val= (qr - pr) * (rc - qc) - (qc - pc) * (rr - qr);
	if ( val == 0 ) return 0;
	return (val > 0) ? 1 : 2;
      }
      
      void ConvexHull::apply(Region2Set& input, Region2& output)
      {
	//printf("bbb0\n"); fflush(stdout);
	if ( mode == 0 || (mode == 1 && input.size() == 1))
	{
	  Region2 tmp;
	  
	  for ( unsigned int i= 0; i < input.size(); ++i )
	    tmp.add(input(i));
	    
	  tmp.stride= input(0).stride;
	  output.stride= input(0).stride;
	  apply(tmp, output);
	}
	else if ( mode == 1 )
	{
	  Region2 tmp;
	  tmp.stride= input(0).stride;
	  Region2 subresult;
	  subresult.stride= input(0).stride;
	  output.stride= input(0).stride;
	  output.clear();
	  
	  for ( unsigned int i= 0; i < input.size(); ++i )
	    for ( unsigned int j= i+1; j < input.size(); ++j )
	    {
	      //printf("%d + %d; %d + %d\n", i, j, input(i).size(), input(j).size());
	      tmp.clear();
	      tmp.add(input(i));
	      tmp.add(input(j));
	      
	      apply(tmp, subresult);
	      output.add(subresult);
	    }
	}
	else
	{
	  printf("unsupported convex hull mode");
	}
	//printf("bbb1\n"); fflush(stdout);
      }
      
      void ConvexHull::apply(Region2& input, Region2& output)
      {
	//printf("ccc0\n"); fflush(stdout);
	if ( input.size() < 3 )
	{
	  output= input;
	  //printf("ccc0.5\n"); fflush(stdout);
	  return;
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
	contour= output.connectSuccessivePixels(1000);
	//printf("eee %d\n", contour.size()); fflush(stdout);
	Region2 res;
	res= contour.connectPixels(1000);
	
	output= res;
	
	//printf("ccc1 %d\n", res.size()); fflush(stdout);
      }
      
      
      ConvexHullOfPoints::ConvexHullOfPoints(int mode)
      {
	this->mode= mode;
      }
      
      ConvexHullOfPoints::ConvexHullOfPoints(const ConvexHullOfPoints& c)
      {
	this->mode= mode;
	this->convexHull= c.convexHull;
      }
      
      ConvexHullOfPoints::~ConvexHullOfPoints()
      {
      }
      
      int ConvexHullOfPoints::orientation(float pr, float pc, float qr, float qc, float rr, float rc)
      {
	float val= (qr - pr) * (rc - qc) - (qc - pc) * (rr - qr);
	if ( val == 0 ) return 0;
	return (val > 0) ? 1 : 2;
      }
      
      void ConvexHullOfPoints::apply(Vector<Vector<float> >& points, Vector<Vector<float> >& ch)
      {
	if ( points.size() < 3 )
	{
	  convexHull= points;
	  return;
	}
	
	Vector<float> rows;
	Vector<float> columns;
	
	Vector<int> result;
	result.resize(points.size());
	result= -1;
	
	for ( unsigned int i= 0; i < points.size(); ++i )
	{
	  rows.push_back(points(i)(0));
	  columns.push_back(points(i)(1));
	}
	
	// find leftmost point
	int leftmost= 0;
	for ( unsigned int i= 0; i < columns.size(); ++i )
	  if ( columns(i) < columns(leftmost) )
	    leftmost= i;
	
	tprintf("leftmost point: %d -- %f, %f\n", leftmost, points(leftmost)(0), points(leftmost)(1));
	  
	int p, q;
	p= leftmost;
	do
	{
	  q= (p+1)%points.size();
	  
	  for ( unsigned int i= 0; i < points.size(); ++i )
	    if ( orientation(rows(p), columns(p), rows(i), columns(i), rows(q), columns(q)) == 2 )
	      q= i;
	    
	  result(p)= q;
	  p= q;
	  tprintf("p= %d\n", q);
	} while ( p != leftmost );
	
	convexHull.clear();
	
	p= leftmost;
	
	tprintf("traversing convex hull\n");
	do
	{
	  convexHull.push_back(points(p));
	  tprintf("%d,%f,%f\n", p, convexHull(convexHull.size()-1)(0), convexHull(convexHull.size()-1)(1)); 
	  p= result(p);
	} while ( p != leftmost );

	ch= convexHull;
      }
      
      float ConvexHullOfPoints::getArea()
      {
	float sum= 0;
	for ( unsigned int i= 0; i < convexHull.size(); ++i )
	{
	  int j= (i+1)%convexHull.size();
	  float r= convexHull(i)(0)*convexHull(j)(1) - convexHull(i)(1)*convexHull(j)(0);
	  sum+= r;
	}
	return fabs(sum)/2;
      }
}