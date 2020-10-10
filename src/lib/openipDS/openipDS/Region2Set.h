#ifndef REGION2SET_H
#define REGION2SET_H

#include <openipDS/Region2.h>
#include <openipDS/Vector.h>

namespace openip
{
    class Region2Set: public Vector<Region2>
    {
    public:
        Region2Set();

        Region2Set(const Region2Set& r);

        ~Region2Set();

        Region2Set getLargestNRegion(int n);
	
	Region2Set getRegionsLargerThan(unsigned int size);
        
        Region2Set getRegionsSmallerThan(unsigned int size);
	
	Region2 getLargestRegion();

	void add(Region2Set& r);
	
	Region2Set getIntersectingRegions(Region2& r);
	
	Region2Set getIntersectingRegions(Region2Set& r);
	
	int getNumberOfRegionsLargerThan(unsigned int n);
	
	Region2Set getRegionsWithLabel(Image<unsigned char>& labels, unsigned char label);
	
	Region2 unifyToOneRegion();
	
	void sortRegionsInDirection(int dimension, int direction);
	
	int indexOfRegionWithSmallestCoordinate(int dimension, int direction);
	
	float distanceFromRegion(Region2& r);
	
	void removeRegion(Region2& r);
	
	template<typename T>
	void set(Image<T>& image, T color);
    }; 
    
    template<typename T>
    void Region2Set::set(Image<T>& image, T color)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
	(*this)(i).set(image, color);
    }
}

#endif // REGION2SET_H
