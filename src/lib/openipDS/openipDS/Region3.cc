#include <openipDS/Region3.h>

#include <climits>

namespace openip
{
    Region3::Region3()
        :VoxelSet1()
    {
    }

    Region3::Region3(const Region3& r)
        :VoxelSet1(r)
    {
        this->stride= r.stride;
        this->slicePitch= r.slicePitch;
    }

    Region3::~Region3()
    {
    }

    int Region3::getVolume()
    {
      return this->size();
    }
    
    int Region3::getSurface()
    {
      int n= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( !(this->contains((*this)(i)+1)) || !(this->contains((*this)(i)-1)) || !(this->contains((*this)(i)+stride)) || !(this->contains((*this)(i)-stride)) || !(this->contains((*this)(i)+slicePitch)) || !(this->contains((*this)(i)-slicePitch)) )
	  ++n;
      return n;
    }
    
    void Region3::getCoordinate3D(int n, int& s, int& r, int& c)
    {
        int tmp;
        s= n / slicePitch;
        tmp= n % slicePitch;
        c= tmp % this->stride;
        r= tmp / stride;
    }

    void Region3::getMinMaxCoordinates(int& smin, int& smax, int& rmin, int& rmax, int& cmin, int& cmax)
    {
        smax= rmax= cmax= 0;
        smin= rmin= cmin= INT_MAX;
        int s, r, c;
        for ( unsigned int i= 0; i < size(); ++i )
        {
            //printf("%d,%d ", i, (*this)(i)); fflush(stdout);
            this->getCoordinate3D((*this)(i), s, r, c);
            if ( s < smin )
                smin= s;
            if ( s > smax )
                smax = s;
            if ( r < rmin )
                rmin= r;
            if ( r > rmax )
                rmax= r;
            if ( c < cmin )
                cmin= c;
            if ( c > cmax )
                cmax= c;
        }
    }

    bool Region3SizeComparator::operator() (const Region3& a, const Region3& b)
    {
        return a.size() < b.size();
    }

    Vector<float> Region3::getCenterOfGravity()
    {
        Vector<float> res;
        res.resize(3);
        res= 0;
        int s, r, c;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            getCoordinate3D((*this)(i), s, r, c);
            res(0)+= s;
            res(1)+= r;
            res(2)+= c;
        }

        res/= this->size();
        return res;
    }

    Vector<float> Region3::getCenterOfBoundingBox()
    {
        int smin, smax, rmin, rmax, cmin, cmax;
        this->getMinMaxCoordinates(smin, smax, rmin, rmax, cmin, cmax);

        Vector<float> res;
        res.resize(3);
        res(0)= (smax + smin)/2.0;
        res(1)= (rmax + rmin)/2.0;
        res(2)= (cmax + cmin)/2.0;

        return res;
    }
    
    int Region3::getIndexOfClosestVoxel(int j, Region3& r)
    {
      int k;
      float dist;
      
      getIndexAndDistanceOfClosestVoxel(j, r, k, dist);
      return k;
    }
	
    float Region3::getDistanceOfClosestVoxel(int j, Region3& r)
    {
      int k;
      float dist;
      
      getIndexAndDistanceOfClosestVoxel(j, r, k, dist);
      return dist;
    }
    
    void Region3::getIndexAndDistanceOfClosestVoxel(int j, Region3& region, int& k, float& dist)
    {
      int s, r, c;
      int ss, rr, cc;
      float minDist= FLT_MAX;
      float minIdx= 0;
      float tmp;
      
      getCoordinate3D(j, s, r, c);
      
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	getCoordinate3D(region(i), ss, rr, cc);
	tmp= (s-ss)*(s-ss) + (r-rr)*(r-rr) + (c-cc)*(c-cc);
	if ( tmp < minDist )
	{
	  minDist= tmp;
	  minIdx= i;
	}
      }
      
      k= minIdx;
      dist= sqrt(minDist);
    }
    
    float Region3::getMeanDistance(Region3& region)
    {
      float meanDistance= 0;
      int n= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	meanDistance+= getDistanceOfClosestVoxel((*this)(i), region);
	++n;
      }
      
      for ( unsigned int i= 0; i < region.size(); ++i )
      {
	meanDistance+= region.getDistanceOfClosestVoxel(region(i), *this);
	++n;
      }
      
      return meanDistance/= n;
    }
    
    int Region3::lengthSlices()
    {
      int smax, smin, rmax, rmin, cmax, cmin;
      getMinMaxCoordinates(smin, smax, rmin, rmax, cmin, cmax);
      
      return smax - smin + 1;
    }
	
    int Region3::lengthRows()
    {
      int smax, smin, rmax, rmin, cmax, cmin;
      getMinMaxCoordinates(smin, smax, rmin, rmax, cmin, cmax);
      
      return rmax - rmin + 1;
    }
    
    int Region3::lengthColumns()
    {
      int smax, smin, rmax, rmin, cmax, cmin;
      getMinMaxCoordinates(smin, smax, rmin, rmax, cmin, cmax);
      
      return cmax - cmin + 1;
    }
    
    int Region3::length(int dim)
    {
      if ( dim == 0 )
	return lengthSlices();
      if ( dim == 1 )
	return lengthRows();
      if ( dim == 2 )
	return lengthColumns();
      
      return 0;
    }
    
    void Region3::convertAxialRegion2To3(Region2& r, int slice, int slicePitch)
    {
      this->clear();
      this->slicePitch= slicePitch;
      this->stride= r.stride;
      
      for ( unsigned int i= 0; i < r.size(); ++i )
      {
	this->push_back(slice*slicePitch + r(i));
      }
    }
}
