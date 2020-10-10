#ifndef _REGION2_OPERATIONS_H_
#define _REGION2_OPERATIONS_H_

#include <openipDS/Region2.h>
#include <openipDS/Region2Set.h>
#include <openipDS/Image.h>
#include <openipLL/ComponentLabeling.h>
#include <openipDS/Stopper.h>
#include <openipDS/StructuringElement2.h>
#include <climits>

namespace openip
{
  Region2Set getHoles(Region2& region);
  
  Region2Set getComponents(Region2& region);
  
  float circularity(Region2& region, Image<unsigned char>& input);
  
  Image<unsigned char> getImageOfRegion(Region2& region);
  
  int isConnective(Region2& region);
  
  void reduceToMinimumPoints(Region2& inputr, Image<unsigned char>& input/*, Region2& thinned*/, Region2& resulting);
  
  Region2 getEndPoints(Region2& region);
  
  int hasHoles(Region2& region);
  
  int numberOfJunctionPoints(Region2& region);
  
  Region2 getThinnedRegion(Region2& region);
  
  int getLengthOfRegion(Region2& region);
  
  int isConnected(int p, int q, Image<unsigned char>& input);
  
  int numberOfIntersectingComponents(int p, Image<unsigned char>& input, StructuringElement2& se);
  
  template<typename T>
  Region2 getThinnedMinimumRegion(Region2& region, Image<T>& colors)
  {
    int rows= region.getHeight();
    int cols= region.getWidth();
    
    int r, c;
    int rmin= INT_MAX, rmax= -INT_MAX, cmin= INT_MAX, cmax= -INT_MAX;
    for ( int i= 0; i < region.size(); ++i )
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
    
    for ( int i= 0; i < region.size(); ++i )
    {
      region.getCoordinate2D(region(i), r, c);
      tmp(r - rmin+1, c - cmin+1)= 255;
    }
    
    thinningCWSIminimum(tmp, colors, tmp2);
    
    Region2 oregion;
    oregion.stride= colors.columns;
    
    for ( int i= 0; i < tmp2.rows; ++i )
      for ( int j= 0; j < tmp2.columns; ++j )
	if ( tmp2(i,j) )
	  oregion.push_back((i + rmin - 1)*oregion.stride + j + cmin - 1);
    
    return oregion;
  }
  
  class AddAndConnectRegions
  {
  public:
    AddAndConnectRegions(int radius= 3, float corr= 0.8);
    AddAndConnectRegions(const AddAndConnectRegions& aacr);
    ~AddAndConnectRegions();

    void apply(Image<unsigned char>& input1, Image<unsigned char>& input2, Image<unsigned char>& input3, Image<unsigned char>& output);
    
    int r;
    float corr;
  };
  
  class ExtractSegments
  {
  public:
    ExtractSegments();
    
    ExtractSegments(const ExtractSegments& es);
    
    ~ExtractSegments();
    
    Region2Set apply(Image<unsigned char>& input);
  };
}

#endif