/* 
 * File:   VoxelSet1.h
 * Author: gykovacs
 *
 * Created on July 20, 2011, 1:09 PM
 */

#ifndef VOXELSET1_H
#define	VOXELSET1_H

#include <openipDS/Set.h>
#include <openipDS/Voxel.h>
#include <openipDS/Image.h>
#include <openipDS/VoxelSet.h>

namespace openip
{
    /**
     * VoxelSet1 represents a set of Voxel1 objects.
     */
    class VoxelSet1 : public VoxelSet<Voxel1>
    {
    public:
        using VoxelSet<Voxel1>::set;
        using VoxelSet<Voxel1>::stride;
        using VoxelSet<Voxel1>::slicePitch;
	using VoxelSet<Voxel1>::operator+;
        
        VoxelSet1();
        
        VoxelSet1(const VoxelSet1& v);
        
        ~VoxelSet1();
        
        VoxelSet1& operator=(const VoxelSet1& v);
        
        /**
         * compute min, max, minX, maxX, minY, maxY values
         */
        void computeMinMax();

        /**
         * compute minX, maxX, minY, maxY values
         */
        void computeMinMaxXYZ();

        /**
         * compute min, max values
         */
        void computeMinMaxOnly();

        /**
         * updates stride
         * @param stride new stride
         */
        int updateStride(int stride, int slicePitch);
        
        void getCoordinates3D(int n, int& slice, int& row, int& column);

         /**
         * get number of rows in the filter elements
         * @return number of rows (height)
         */
        int getRows();

        /**
         * get number of columns in the filter elements
         * @return number of columns (width)
         */
        int getColumns();
        
        int getSlices();
        
        void getSlicesRowsColumns(int& slices, int& rows, int& columns);
	
	virtual Border3 getProposedBorder();
    };
}

#endif	/* VOXELSET1_H */

