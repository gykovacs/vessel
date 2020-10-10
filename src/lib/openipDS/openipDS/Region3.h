#ifndef REGION3_H
#define REGION3_H

#include <openipDS/VoxelSet1.h>
#include <openipDS/Region2.h>

namespace openip
{
    class Region3: public VoxelSet1
    {
    public:
        using VoxelSet1::set;

        Region3();

        Region3(const Region3& r);

        ~Region3();

        void getMinMaxCoordinates(int& front, int& back, int& top, int& bottom, int& left, int& right);

        void getCoordinate3D(int n, int& s, int& r, int& c);
	
	int getVolume();
	
	int getSurface();
	
	int getIndexOfClosestVoxel(int j, Region3& r);
	
	float getDistanceOfClosestVoxel(int j, Region3& r);
	
	void getIndexAndDistanceOfClosestVoxel(int j, Region3& r, int& k, float& dist);
	
	float getMeanDistance(Region3& r);
	
	int lengthSlices();
	
	int lengthRows();
	
	int lengthColumns();
	
	int length(int dim);

        Vector<float> getCenterOfGravity();

        Vector<float> getCenterOfBoundingBox();
	
	void convertAxialRegion2To3(Region2& r, int slice, int slicePitch);
	
	template<typename T>
	void set(Volume<T>& input, T color);
    };

    
    // TODO: replace with comparator
    template<typename T>
    void Region3::set(Volume<T>& input, T color)
    {
      for ( unsigned int i= 0; i < this->size(); ++i )
	input((*this)(i))= color;
    }

    struct Region3SizeComparator
    {
    public:
        bool operator() (const Region3& a, const Region3& b);
    };
}

#endif // REGION3_H
