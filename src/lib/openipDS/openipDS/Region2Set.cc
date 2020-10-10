#include <openipDS/Region2Set.h>

namespace openip
{
    Region2Set::Region2Set()
        : Vector<Region2>()
    {
    }

    Region2Set::Region2Set(const Region2Set& r)
        : Vector<Region2>(r)
    {
    }

    Region2Set::~Region2Set()
    {
    }

    int Region2Set::getNumberOfRegionsLargerThan(unsigned int n)
    {
      int res= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*this)(i).size() > n )
	  ++res;
	
      return res;
    }
    
    void Region2Set::removeRegion(Region2& r)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( (*this)(i).size() == r.size() )
	{
	  unsigned int j;
	  for ( j= 0; j < (*this)(i).size(); ++j )
	    if ( !r.contains((*this)(i)(j)) )
	      break;
	  if ( j == (*this)(i).size() )
	  {
	    Region2Set rs;
	    rs= (*this);
	    rs.erase(rs.begin() + i);
	    *this= rs;
	    return;
	  }
	}
      }
    }
    
    Region2Set Region2Set::getLargestNRegion(int n)
    {
        Region2Set result;

        Region2SizeComparator r;

        sort(begin(), end(), r);

        //tprintf("t2 %d %d\n", this->size(), n);
        int s= this->size();
        int lrange= s-n >= 0 ? s-n : 0;
        for ( int i= s - 1; i >= lrange; --i )
        {
            //printf("."); fflush(stdout);
            result.push_back((*this)(i));
        }

        //tprintf("r.size %d\n", result.size());
        return result;
    }

    Region2Set Region2Set::getRegionsLargerThan(unsigned int rsize)
    {
      Region2Set result;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*this)(i).size() > rsize )
	  result.push_back((*this)(i));
      return result;
    }
    
    Region2Set Region2Set::getRegionsSmallerThan(unsigned int rsize)
    {
      Region2Set result;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*this)(i).size() < rsize )
	  result.push_back((*this)(i));
      return result;
    }
    
    void Region2Set::add(Region2Set& r)
    {
      for ( unsigned int i= 0; i < r.size(); ++i )
	this->push_back(r(i));
    } 
    
    Region2Set Region2Set::getIntersectingRegions(Region2& r)
    {
      Region2Set result;
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( r.isIntersecting((*this)(i)) )
	  result.push_back((*this)(i));
      return result;
    }
    
    Region2Set Region2Set::getIntersectingRegions(Region2Set& r)
    {
      Region2Set result;
      for ( unsigned int i= 0; i < this->size(); ++i )
	for ( unsigned int j= 0; j < r.size(); ++j )
	{
	  if ( r(j).isIntersecting((*this)(i)) )
	  {
	    result.push_back((*this)(i));
	    break;
	  }
	}
      return result;
    }
    
    Region2 Region2Set::getLargestRegion()
    {
      int maxIdx= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*this)(i).size() > (*this)(maxIdx).size() )
	  maxIdx= i;
      return (*this)(maxIdx);
    }
    
    Region2Set Region2Set::getRegionsWithLabel(Image<unsigned char>& labels, unsigned char label)
    {
      Region2Set rs;
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( labels((*this)(i)(0)) == label )
	  rs.push_back((*this)(i));
      return rs;
    }
    
    Region2 Region2Set::unifyToOneRegion()
    {
      Region2 r;
      r.stride= (*this)(0).stride;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
	r.add((*this)(i));
      return r;
    }
    
    int Region2Set::indexOfRegionWithSmallestCoordinate(int dimension, int direction)
    {
      int idx= 0;
      float coordinate;
      
      float tmpcoord;
      Vector<float> tmp;
      
      tmp= (*this)(0).getCenterOfGravity();
      coordinate= tmp(dimension);
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	tmp= (*this)(i).getCenterOfGravity();
	tmpcoord= tmp(dimension);
	
	if ( direction == 0 )
	  if ( tmpcoord < coordinate )
	  {
	    coordinate= tmpcoord;
	    idx= i;
	  }
	if ( direction == 1 )
	  if ( tmpcoord > coordinate )
	  {
	    coordinate= tmpcoord;
	    idx= i;
	  }
      }
      
      return idx;
    }
    
    void Region2Set::sortRegionsInDirection(int dimension, int direction)
    {
      Region2Set tmp;
      int idx;
      
      while ( this->size() != 0 )
      {
	idx= this->indexOfRegionWithSmallestCoordinate(dimension, direction);
	tmp.push_back((*this)(idx));
	this->erase(this->begin() + idx);
      }
      
      (*this)= tmp;
    }
    
    float Region2Set::distanceFromRegion(Region2& r)
    {
      float tmp= FLT_MAX;
      float minDist= FLT_MAX;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	tmp= (*this)(i).getDistanceFromRegion(r);
	if ( tmp < minDist )
	  minDist= tmp;
      }
      return minDist;
    }
}
