#ifndef REGION3SET_H
#define REGION3SET_H

#include <openipDS/Vector.h>
#include <openipDS/Region3.h>

namespace openip
{
    class Region3Set: public Vector<Region3>
    {
    public:
        Region3Set();

        Region3Set(const Region3Set& r);

        ~Region3Set();

        Region3Set getLargestNRegion(int n);
	
	Region3 getLargestRegion();

        Region3Set getOrderedNRegion(int n);
	
	Region3 getLongestRegion(int dim);
	
	Region3Set getRegionsLargerThan(unsigned int rsize);

        void processRow(Vector<Vector<float> >& centers, Vector<Vector<float> >& oneRow, Vector<int>& result, float dcf);

        void processSlice(Vector<Vector<float> >& centers, Vector<Vector<float> >& oneRow, Vector<int>& result, float drf, float dcf);
	
	void sortRegionsInDirection(int dimension, int direction);
	
	int indexOfRegionWithSmallestCoordinate(int dimension, int direction);
	
	void getMinMaxCoordinates(int& smin, int& smax, int& rmin, int& rmax, int& cmin, int& cmax);
	
	int minAxialCoordinate();
	
	int maxAxialCoordinate();
	
	void add(Region3Set& r);
	
	template<typename T>
	void set(Volume<T>& input, T label);
    };

    Vector<Vector<float> > getCentersOfGravity(Region3Set& rs);
    
    struct VectorCoordinate0Comparator
    {
    public:
        bool operator() (const Vector<float>& a, const Vector<float>& b);
    };

    struct VectorCoordinate1Comparator
    {
    public:
        bool operator() (const Vector<float>& a, const Vector<float>& b);
    };

    struct VectorCoordinate2Comparator
    {
    public:
        bool operator() (const Vector<float>& a, const Vector<float>& b);
    };
    
    template<typename T>
    void Region3Set::set(Volume<T>& input, T label)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
	(*this)(i).set(input, label);
    }
}

#endif // REGION3SET_H
