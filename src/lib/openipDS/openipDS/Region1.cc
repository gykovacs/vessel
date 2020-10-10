#include <openipDS/Region1.h>

namespace openip
{
  Region1::Region1()
  : Vector<int>()
  {
  }

  Region1::Region1(const Region1& r)
  : Vector<int>(r)
  {
  }

  Region1::~Region1()
  {
  }

  Region1& Region1::operator=(const Region1& r)
  {
    this->clear();
    for ( unsigned int i= 0; i < r.size(); ++i )
      this->push_back(r(i));
    return (*this);
  }

  int Region1::indexOfLargestNeighboringRegion(Vector<Region1>& r)
  {
    int idx= -1;
    unsigned int size= 0;
    for ( unsigned int i= 0; i < r.size(); ++i )
    {
      for ( unsigned int j= 0; j < r(i).size(); ++j )
	if ( this->contains(r(i)(j)) )
	  if ( r(i).size() > size )
	  {
	    idx= i;
	    size= r(i).size();
	  }
    }
    
    return idx;
  }

  int Region1::indexOfLargestRegion(Vector<Region1>& r)
  {
    int idx= -1;
    unsigned int size= 0;
    for ( unsigned int i= 0; i < r.size(); ++i )
    {
      for ( unsigned int j= 0; j < r(i).size(); ++j )
	  if ( r(i).size() > size )
	  {
	    idx= i;
	    size= r(i).size();
	  }
    }
    
    return idx;
  }

  void extractHorizontalRegions(Image<unsigned char>& input, int line, Vector<Region1>& results)
  {
    int begin= -1, end= -1;
    
    for ( int i= 0; i < int(input.columns)-1; ++i )
    {
      if ( input(line, i) && begin == -1 )
      {
	begin= i;
	continue;
      }
      if ( !input(line, i+1) && begin != -1 && end == -1 )
      {
	end= i;
	Region1 r;
	for ( int j= begin; j <= end; ++j )
	{
	  r.push_back(j);
	}
	results.push_back(r);
	begin= -1;
	end= -1;
	continue;
      }
    }
  }
  
}