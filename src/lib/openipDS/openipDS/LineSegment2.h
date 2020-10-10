#ifndef _LINE_SEGMENT_2_H_
#define _LINE_SEGMENT_2_H_

#include <openipDS/Image.h>
#include <openipDS/Region2.h>

namespace openip
{
  class LineSegment2
  {
  public:
    LineSegment2();

    LineSegment2(const LineSegment2& ls);
    
    LineSegment2(Region2& r);

    LineSegment2(int r0, int c0, int r1, int c1);

    ~LineSegment2();
    
    LineSegment2& operator=(const LineSegment2& ls);

    void orderCoordinates();

    void enlargeTowardsTheOrigin(float times);

    void enlargeOutwardsTheOrigin(float times);
    
    void enlarge(float times);

    Region2 convertToRegion(int stride);
    
    template<typename T>
    Region2 convertToRegion(Image<T>& img);

    template<typename T>
    void set(Image<T>& input, T color);

    float length();

    int r0, c0, r1, c1;
  };

  template<typename T>
  void LineSegment2::set(Image<T>& input, T color)
  {
    Region2 r= convertToRegion(input.columns);
    for ( int i= 0; i < r.size(); ++i )
      if ( r(i) > 0 && r(i) < input.n )
	input(r(i))= color;
  }

  LineSegment2 extrapolateLineSegments(Vector<LineSegment2>& y, Vector<int>& x, int i);
  
  template<typename T>
  Region2 LineSegment2::convertToRegion(Image<T>& img)
  {
    Region2 r;
    r.stride= img.columns;
    
    int n= 10000.0;

    float dr= float(r0 - r1)/n;
    float dc= float(c0 - c1)/n;

    for ( int i= 0; i < n; ++i )
    {
      int tr= (r1 + int(i*dr));
      int tc= c1 + int(i*dc);
      if ( tr >= 0 && tr < img.rows && tc >= 0 && tc < img.columns )
	r.push_back(tr*img.columns + tc);
    }

    r.removeDuplicates();
    return r;
  }
}

#endif