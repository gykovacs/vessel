#ifndef _REGION_1_H_
#define _REGION_1_H_

#include <openipDS/Vector.h>
#include <openipDS/Image.h>

namespace openip
{

  class Region1 : public Vector<int>
  {
  public:
    Region1();
    
    Region1(const Region1& r);
    
    ~Region1();
    
    int indexOfLargestNeighboringRegion(Vector<Region1>& r);
    
    int indexOfLargestRegion(Vector<Region1>& r);
    
    Region1& operator=(const Region1& r);
  };

  void extractHorizontalRegions(Image<unsigned char>& input, int line, Vector<Region1>& results);
}

#endif