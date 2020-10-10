#include <openipLL/Region2Operations.h>

#include <openipLL/thinning.h>
#include <openipDS/Stopper.h>
#include <omp.h>

namespace openip
{
  float circularity(Region2& region, Image<unsigned char>& input)
  {
    float area= region.size();
    float perimeter= 0;

    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      if ( region(i) - input.columns - 1 >= 0 && region(i) + input.columns + 1 < int(input.n) )
      {
	if ( !input(region(i) + 1) )
	  input(region(i)+1)= 128;
	if ( !input(region(i) - 1) )
	  input(region(i)-1)= 128;
	if ( !input(region(i) + input.columns) )
	  input(region(i)+input.columns)= 128;
	if ( !input(region(i) - input.columns) )
	  input(region(i)-input.columns)= 128;
	/*if ( !input(region(i) + 1 + input.columns) )
	  p++;
	if ( !input(region(i) + 1 - input.columns) )
	  p++;
	if ( !input(region(i) - 1 + input.columns) )
	  p++;
	if ( !input(region(i) - 1 - input.columns) )
	  p++;*/
      }
    }
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      if ( region(i) - input.columns - 1 >= 0 && region(i) + input.columns + 1 < int(input.n) )
      {
	if ( input(region(i) + 1) == 128 )
	{
	  input(region(i)+1)= 0;
	  perimeter+= 1.0;
	}
	if ( input(region(i) - 1) == 128 )
	{
	  input(region(i)-1)= 0;
	  perimeter+= 1.0;
	}
	if ( input(region(i) + input.columns) == 128 )
	{
	  input(region(i)+input.columns)= 0;
	  perimeter+= 1.0;
	}
	if ( input(region(i) - input.columns) == 128 )
	{
	  input(region(i)-input.columns)= 0;
	  perimeter+= 1.0;
	}
	/*if ( !input(region(i) + 1 + input.columns) )
	  p++;
	if ( !input(region(i) + 1 - input.columns) )
	  p++;
	if ( !input(region(i) - 1 + input.columns) )
	  p++;
	if ( !input(region(i) - 1 - input.columns) )
	  p++;*/
      }
    }
    
    float c= area*4*M_PI/(perimeter*perimeter);
    return c;
  }
  
  Image<unsigned char> getImageOfRegion(Region2& region)
  {
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      if ( r < rmin )
	rmin= r;
      if ( r > rmax )
	rmax= r;
      if ( c < cmin )
	cmin= c;
      if ( c > cmax )
	cmax= c;
    }
    
    Image<unsigned char> tmp(rows, cols);
    tmp= 0;
    //printf("a"); fflush(stdout);
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin, c - cmin)= 255;
    }
    
    return tmp;
  }
  
    Region2Set getHoles(Region2& region)
    {
      ExtractRegions er(1);
      int rows= region.getHeight();
      int cols= region.getWidth();
      
      int r, c;
      int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	if ( r < rmin )
	  rmin= r;
	if ( r > rmax )
	  rmax= r;
	if ( c < cmin )
	  cmin= c;
	if ( c > cmax )
	  cmax= c;
      }
      
      Image<unsigned char> tmp(rows, cols);
      tmp= 0;
      //printf("a"); fflush(stdout);
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	tmp(r - rmin, c - cmin)= 255;
      }
      //printf("b"); fflush(stdout);
      //writeImage("tmp.bmp", tmp);
      Region2Set rs;
      if ( tmp.rows == 1 || tmp.columns == 1 )
	return rs;
      int num= 0;
      for ( unsigned int i= 0; i < tmp.n; ++i )
	if ( tmp(i) )
	  num++;
      if ( num == int(tmp.n) )
	  return rs;
      er.apply(tmp, rs);
      
      
      
      //printf("c"); fflush(stdout);
      Region2Set result;
      for ( unsigned int i= 0; i < rs.size(); ++i )
      {
	int flag= 0;
	for ( unsigned int j= 0; j < rs(i).size() && flag == 0; ++j )
	{
	  rs(i).getCoordinate2D(rs(i)(j), r, c);
	  if ( r == 0 || r == tmp.rows || c == 0 || c == tmp.columns )
	    flag= 1;
	}
	if ( !flag )
	  result.push_back(rs(i));
      }
      //printf("d"); fflush(stdout);
      return result;
    }
    
    Region2Set getComponents(Region2& region)
    {
      ExtractRegions er;
      int rows= region.getHeight();
      int cols= region.getWidth();
      
      int r, c;
      int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	if ( r < rmin )
	  rmin= r;
	if ( r > rmax )
	  rmax= r;
	if ( c < cmin )
	  cmin= c;
	if ( c > cmax )
	  cmax= c;
      }
      
      Image<unsigned char> tmp(rows+2, cols+2);
      tmp= 0;
      
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	tmp(r - rmin + 1, c - cmin + 1)= 255;
      }
      Region2Set rs;
      er.apply(tmp, rs);
      
      Region2Set finalRegions;
      for ( unsigned int i= 0; i < rs.size(); ++i )
      {
	Region2 finalRegion;
	finalRegion.stride= region.stride;
	for ( unsigned int j= 0; j < rs(i).size(); ++j )
	{
	  rs(i).getCoordinate2D(rs(i)(j), r, c);
	  r= r - 1 + rmin;
	  c= c - 1 + cmin;
	  finalRegion.push_back(r*region.stride + c);
	}
	finalRegions.push_back(finalRegion);
      }

      return finalRegions;
    }
  
    int isConnective(Region2& region)
    {
      ExtractRegions er(1);
      int rows= region.getHeight();
      int cols= region.getWidth();
      
      int r, c;
      int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	if ( r < rmin )
	  rmin= r;
	if ( r > rmax )
	  rmax= r;
	if ( c < cmin )
	  cmin= c;
	if ( c > cmax )
	  cmax= c;
      }
      
      Image<unsigned char> tmp(rows + 1, cols + 1);
      tmp= 0;
      
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	region.getCoordinate2D(region(i), r, c);
	tmp(r - rmin, c - cmin)= 255;
      }
      
      Region2Set rs;
      if ( tmp.getSum() == 0 )
	return 0;
      er.apply(tmp, rs);
      
      return rs.size() == 1;
    }
  
  void reduceToMinimumPoints(Region2& inputr, Image<unsigned char>& input, /*Region2& thinned,*/ Region2& resulting)
  {
    Region2 result;
    result= inputr;
    Vector<int> endPoints;
    
    /*for ( int i= 0; i < thinned.size(); ++i )
      if ( thinned.numberOfNeighbors(thinned(i)) == 1 )
	endPoints.push_back(thinned(i));*/
    
    for ( unsigned int i= 0; i < inputr.size(); ++i )
      if ( input.isLocalMinimum8(inputr(i)) )
	endPoints.push_back(inputr(i));
    
    
    
      
    float bestMean= FLT_MAX;
    Region2 bestRegion;
    bestRegion.stride= inputr.stride;
    result.stride= inputr.stride;
    resulting.clear();
    resulting.stride= inputr.stride;
    
    float initMean= inputr.getMean(input);
    for ( int l= 0; l < 8; ++l )
    {
      int flag= 0;
      result= inputr;
      float mean= initMean;
      for ( unsigned int j= 0; j < 2*inputr.size() && !flag; ++j )
      {
	if ( result.size() <= 1 )
	{
	  flag=1;
	  continue;
	}
	
	int r= rand()%result.size();
	
	int k= 0;
	while ( !((input(result(r))) >= mean && !endPoints.contains(result(r)) ) )
	{
	  r= rand()%result.size();
	  ++k;
	  if ( k == 100 )
	  {
	    flag= 1;
	    break;
	  }
	}
	if ( flag == 1 )
	  continue;
	int tmp= result(r);
	result.erase(result.begin() + r);
	
	if ( !isConnective(result) )
	  result.push_back(tmp);
	else
	{
	  mean*= (result.size()+1);
	  mean-= input(tmp);
	  mean/= (result.size());
	}
      }
      
      if ( result.size() <= 2 )
	continue;
      
      mean= result.getMean(input);
      if ( mean < bestMean )
      {
	bestMean= mean;
	bestRegion= result;
      }
    }
    
    resulting= bestRegion;
  }
  
  Region2 getEndPoints(Region2& region)
  {
    Region2 result;
    result.stride= region.stride;
    
    for ( unsigned int i= 0; i < region.size(); ++i )
      if ( region.numberOfNeighbors(region(i)) == 1 )
	result.push_back(region(i));
      
    return result;
  }
  
  AddAndConnectRegions::AddAndConnectRegions(int r, float corr)
  {
    this->r= r;
    this->corr= corr;
  }
  
  AddAndConnectRegions::AddAndConnectRegions(const AddAndConnectRegions& a)
  {
    this->r= a.r;
    this->corr= a.corr;
  }
  
  AddAndConnectRegions::~AddAndConnectRegions()
  {
  }
  
  void AddAndConnectRegions::apply(Image<unsigned char>& input1, Image<unsigned char>& input2, Image<unsigned char>& input3, Image<unsigned char>& output)
  {
    ExtractRegions er;
    Region2Set regions;

    Image<unsigned char> tmp;
    tmp.resizeImage(input1);
    tmp= 0;
    Image<unsigned char> thinned;
    thinned.resizeImage(input1);
    
    thinningCWSI(input2, thinned);
    er.apply(thinned, regions);
    
    output= input1;
    int added= 0, removed= 0;
    Region2Set nextRound;
    int step= 0;
    
    while ( 1 )
    {
      ++step;
      
      if ( step != 1 && regions.size() == nextRound.size() )
	break;
      if ( step != 1 )
      {
	regions= nextRound;
	nextRound.clear();
      }
      
      for ( unsigned int m= 0; m < regions.size(); ++m )
      {
	  Vector<int> endPoints= regions(m).getEndPointCoordinates();
	  //printf("endpoints.size(): %d %d\n", endPoints.size(), regions(m).size());

	  float dr, dc;
	  int r0, c0;
	  Vector<Region2> finalPaths;
	  for ( unsigned int j= 0; j < endPoints.size(); ++j )
	  {
	      if ( fabs(regions(m).correlationNearEndPoint((endPoints(j)), 9)) > corr )
	      {
		regions(m).drdcNearEndPoint((endPoints(j)), 9, dr, dc);
		regions(m).getCoordinate2D(regions(m)(endPoints(j)), r0, c0);
		
		Vector<Region2> paths;
		
		for ( float vr= -0.1; vr <= 0.11; vr+= 0.1 )
		  for ( float vc= -0.1; vc <= 0.11; vc+= 0.1 )
		  {
		    
		    Region2 reg;
		    reg.stride= input1.columns;
		    float dr1, dc1;
		    dr1= dr;
		    dc1= dc;
		    
		    float resolution= 100.0;
		    float length= sqrt(dr1*dr1 + dc1*dc1);
		    dr1/= length;
		    dc1/= length;
		    
		    dr1= dr+vr;
		    dc1= dc+vc;
		    length= sqrt(dr1*dr1 + dc1*dc1);
		    dr1/= length;
		    dc1/= length;
		    dr1/= resolution;
		    dc1/= resolution;
		    
		    for ( int k= 0; k < resolution*(this->r); ++k )
		    {
		      if ( !output(int(r0 + k*dr1), int(c0 + k*dc1)) )
		      {
			reg.push_back(int((r0 + k*dr1))*input1.columns + int(c0 + k*dc1));
		      }
		      else
		      {
			reg.removeDuplicates();
			paths.push_back(reg);
			break;
		      }
		    }
		  }
		if ( paths.size() == 0 )
		  continue;
		else
		{
		  int minIdx= 0;
		  for ( unsigned int k= 0; k < paths.size(); ++k )
		    if ( paths(k).getMean(input3) < paths(minIdx).getMean(input3) )
		      minIdx= k;
		  if ( paths.size() > 0 )
		    finalPaths.push_back(paths(minIdx));  
		}
	    }
	    else
	    {
	      //printf("correlation near endpoint is too small\n");
	      removed++;
	    }
	  }
	for ( unsigned int l= 0; l < finalPaths.size(); ++l )
	{
	  finalPaths(l).set(output, (unsigned char)255);
	  finalPaths(l).set(tmp, (unsigned char)255);
	}
	if ( finalPaths.size() == 0 )
	{
	  nextRound.push_back(regions(m));
	}
	else
	{
	  /*for ( int j= 0; j < regions(m).size(); ++j )
	    if ( thinned(regions(m)(j)) )
	      output(regions(m)(j))= 255;
	    else
	      output(regions(m)(j))= 0;*/
	  regions(m).set(output, (unsigned char)255);
	  added++;
	}
      }
    }
    tprintf("a/r: %d %d\n", added, removed);

    for ( unsigned int i= 0; i < regions.size(); ++i )
      regions(i).set(output, (unsigned char)255);
  }
  
  
  
  ExtractSegments::ExtractSegments()
  {
  }
    
  ExtractSegments::ExtractSegments(const ExtractSegments& /*es*/)
  {
  }
  
  ExtractSegments::~ExtractSegments()
  {
  }
  
  Region2Set ExtractSegments::apply(Image<unsigned char>& input)
  {
    Image<unsigned char> thinned;
    Image<unsigned char> tmp;
    thinned.resizeImage(input);
    thinned= 0;
    tmp.resizeImage(input);
    tmp= 0;
    
    thinningCWSI(input, thinned);
    
    Vector<int> pos;
    pos.push_back(1);
    pos.push_back(-input.columns + 1);
    pos.push_back(-input.columns);
    pos.push_back(-input.columns - 1);
    pos.push_back(-1);
    pos.push_back(input.columns - 1);
    pos.push_back(input.columns);
    pos.push_back(input.columns + 1);
    
    for ( unsigned int i= 0; i < thinned.n; ++i )
      if ( thinned(i) > 128 )
	thinned(i)= 255;
      else
	thinned(i)= 0;
    
    while (1)
    {
      int n= 0;
      for ( int i= input.columns + 1; i < int(input.n) - input.columns-1; ++i )
      {
	if ( thinned(i) && thinned(i + pos(0)) && !thinned(i + pos(1)) && thinned(i + pos(2)) /*&& !thinned(i + pos(3))*/ && !thinned(i + pos(4)) && !thinned(i + pos(5)) && !thinned(i + pos(6)) /*&& !thinned(i + pos(7))*/ )
	{
	  thinned(i)= 0;
	  ++n;
	  //printf("a");
	}
	else if ( thinned(i) && !thinned(i + pos(0)) /*&& !thinned(i + pos(1))*/ && thinned(i + pos(2)) && !thinned(i + pos(3)) && thinned(i + pos(4)) /*&& !thinned(i + pos(5))*/ && !thinned(i + pos(6)) && !thinned(i + pos(7)) )
	{
	  thinned(i)= 0;
	  ++n;
	  //printf("b");
	}
	else if ( thinned(i) && !thinned(i + pos(0)) && !thinned(i + pos(1)) && !thinned(i + pos(2)) /*&& !thinned(i + pos(3))*/ && thinned(i + pos(4)) && !thinned(i + pos(5)) && thinned(i + pos(6)) /*&& !thinned(i + pos(7))*/ )
	{
	  thinned(i)= 0;
	  ++n;
	  //printf("c");
	}
	else if ( thinned(i) && thinned(i + pos(0)) /*&& !thinned(i + pos(1))*/ && !thinned(i + pos(2)) && !thinned(i + pos(3)) && !thinned(i + pos(4)) /*&& !thinned(i + pos(5))*/ && thinned(i + pos(6)) && !thinned(i + pos(7)) )
	{
	  thinned(i)= 0;
	  ++n;
	  //printf("d");
	}
      }
      //printf("%d\n", n);
      if ( n == 0 )
	break;
    }
    
    StructuringElementSquare ses(3);
    ses.updateStride(thinned.columns);
    
    /*Region2Set segments1;
    ExtractRegions er0;
    er0.apply(thinned, segments1);
    
    for ( int i= 0; i < segments1.size(); ++i )
      for ( int j= 0; j < segments1(i).size(); ++j )
	if ( segments1(i).numberOfNeighbors(segments1(i)(j)) > */
    
    Vector<int> crosses;
    for ( int i= -ses.getMin(); i < int(thinned.n) - ses.getMax(); ++i )
    {
      int n= 0;
      for ( unsigned int j= 0; j < ses.size(); ++j )
	if ( ses(j) != 0 && thinned(i + ses(j)) )
	  ++n;
      if ( n > 2 )
	crosses.push_back(i);
    }
    
    for ( unsigned int i= 0; i < crosses.size(); ++i )
    {
      for ( unsigned int j= 0; j < ses.size(); ++j )
	thinned(crosses(i) + ses(j))= 0;
    }
    
    ExtractRegions er;
    Region2Set segments0;
    er.apply(thinned, segments0);
    
    Image<int> labels;
    labels.resizeImage(input);
    
    labels= 0;
    for ( unsigned int i= 0; i < segments0.size(); ++i )
      for ( unsigned int j= 0; j < segments0(i).size(); ++j )
	labels(segments0(i)(j))= i+1;
    
    StructuringElementDisk sed(5);
    sed.updateStride(input.columns);
    
    for ( unsigned int i= 0; i < input.n; ++i )
    {
      float minDist= FLT_MAX;
      int minLabel= 0;
      int r, c, r0, c0;
      input.getCoordinate2D(i, r, c);
      if ( input(i) )
      {
	for ( unsigned int j= 0; j < sed.size(); ++j )
	  if ( labels(i + sed(j)) )
	  {
	    input.getCoordinate2D(i + sed(j), r0, c0);
	    if ( (r0-r)*(r0-r) + (c0-c)*(c0-c) < minDist )
	    {
	      minDist= (r0-r)*(r0-r) + (c0-c)*(c0-c);
	      minLabel= labels(i + sed(j));
	    }
	  }
	//printf("%d ", minLabel); fflush(stdout);
	if ( minLabel != 0 )
	  segments0(minLabel-1).push_back(i);
      }
    }
    
    Image<unsigned char> control;
    control.resizeImage(input);
    
    control= 0;
    for ( unsigned int i= 0; i < segments0.size(); ++i )
      segments0(i).set(control, (unsigned char)255);
    
    /*writeImage("thinned.bmp", thinned);
    writeImage("tmp.bmp", tmp);
    writeImage("control.bmp", control);*/
    
    //Region2Set rs;
    return segments0;
  }
  
  int hasHoles(Region2& region)
  {
    ExtractRegions er(1);
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      if ( r < rmin )
	rmin= r;
      if ( r > rmax )
	rmax= r;
      if ( c < cmin )
	cmin= c;
      if ( c > cmax )
	cmax= c;
    }
    
    Image<unsigned char> tmp(rows, cols);
    tmp= 255;
    
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin, c - cmin)= 0;
    }
    
    Region2Set rs;
    er.apply(tmp, rs);
    
    Region2Set result;
    for ( unsigned int i= 0; i < rs.size(); ++i )
    {
      int flag= 0;
      for ( unsigned int j= 0; j < rs(i).size() && flag == 0; ++j )
      {
	rs(i).getCoordinate2D(rs(i)(j), r, c);
	
	if ( r == 0 || r == tmp.rows-1 || c == 0 || c == tmp.columns-1 )
	{
	  flag= 1;
	}
      }
      
      if ( !flag && rs(i).size() != 0 )
	return 1;
    }
    
    return 0;
  }
  
  int numberOfJunctionPoints(Region2& region)
  {
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      if ( r < rmin )
	rmin= r;
      if ( r > rmax )
	rmax= r;
      if ( c < cmin )
	cmin= c;
      if ( c > cmax )
	cmax= c;
    }
    
    Image<unsigned char> tmp(rows+2, cols+2), tmp2(rows+2, cols+2);
    tmp= 0;
    tmp2= 0;
    
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin+1, c - cmin+1)= 255;
    }
    
    thinningCWSI(tmp, tmp2);
    
    
    
    ExtractRegions er;
    Region2Set regions;
    er.apply(tmp2, regions);
    if ( regions.size() == 0 )
      return 0;
    int n= 0;
    n= regions(0).numberOfCrossingPixels(tmp2);
    
    return n;
  }
  
  Region2 getThinnedRegion(Region2& region)
  {
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      if ( r < rmin )
	rmin= r;
      if ( r > rmax )
	rmax= r;
      if ( c < cmin )
	cmin= c;
      if ( c > cmax )
	cmax= c;
    }
    
    Image<unsigned char> tmp(rows+2, cols+2), tmp2(rows+2, cols+2);
    tmp= 0;
    tmp2= 0;
    
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin+1, c - cmin+1)= 255;
    }
    
    thinningCWSI(tmp, tmp2);
    
    ExtractRegions er;
    Region2Set regions;
    er.apply(tmp2, regions);
    
    return regions(0);
  }
  
  int getLengthOfRegion(Region2& region)
  {
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      if ( r < rmin )
	rmin= r;
      if ( r > rmax )
	rmax= r;
      if ( c < cmin )
	cmin= c;
      if ( c > cmax )
	cmax= c;
    }
    
    Image<unsigned char> tmp(rows+2, cols+2), tmp2(rows+2, cols+2);
    tmp= 0;
    tmp2= 0;
    
    for ( unsigned int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin+1, c - cmin+1)= 255;
    }
    
    thinningCWSI(tmp, tmp2);
    
    ExtractRegions er;
    Region2Set regions;
    er.apply(tmp2, regions);
    
    return regions(0).size();
  }
  
  int isConnected(int p, int q, Image<unsigned char>& input)
  {
    StructuringElementSquare ses(3);
    ses.updateStride(input.columns);
    
    Vector<int> queue;
    queue.push_back(p);
    Vector<int> toRestore;
    
    int r0, c0, r1, c1;
    input.getCoordinate2D(p, r0, c0);
    
    while ( queue.size() != 0 )
    {
      int pix= queue(0);
      if ( pix == q )
	break;
      toRestore.push_back(pix);
      queue.erase(queue.begin());
      
      for ( unsigned int i= 0; i < ses.size(); ++i )
	if ( ses(i) != 0 )
	  if ( input(pix + ses(i)) == 255 )
	  {
	    input.getCoordinate2D(pix + ses(i), r1, c1);
	    if ( sqrt((r0 - r1)*(r0 - r1) + (c0 - c1)*(c0 - c1)) < 19 )
	    {
	      queue.push_back(pix + ses(i));
	      input(pix + ses(i))= 128;
	    }
	  }
    }
    
    for ( unsigned int i= 0; i < toRestore.size(); ++i )
      input(toRestore(i))= 255;
    for ( unsigned int i= 0; i < queue.size(); ++i )
      input(queue(i))= 255;
    if ( queue.size() != 0 )
      return 1;
    
    return 0;
  }
  
  int numberOfIntersectingComponents(int p, Image<unsigned char>& input, StructuringElement2& se)
  {
    Region2 region;
    region.stride= input.columns;
    
    for ( unsigned int i= 0; i < se.size(); ++i )
      if ( p + se(i) >= 0 && p + se(i) < int(input.n) )
      {
	if ( input(p + se(i)) )
	{
	  if ( isConnected(p, p + se(i), input) )
	    region.push_back(p + se(i));
	}
      }
    //printf("r%d ", region.size());
    if ( region.size() == 0 )
      return 0;
    return getComponents(region).size();
  }
}
