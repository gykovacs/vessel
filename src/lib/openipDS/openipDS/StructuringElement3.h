/* 
 * File:   StructuringElement33.h
 * Author: gykovacs
 *
 * Created on July 19, 2011, 8:45 PM
 */

#ifndef STRUCTURINGELEMENT3_H
#define	STRUCTURINGELEMENT3_H

#include <vector>
#include <algorithm>

#include <openipDS/Image.h>
#include <openipDS/VoxelSet1.h>
//#include <openipDS/VoxelSet3.h>

using namespace std;

namespace openip
{
    /**
     * StructuringElement3 represents a structuring element, that is a set
     * of pixels relative to a center pixel
     */
    class StructuringElement3 : public VoxelSet1
    {
    public:
        using VoxelSet1::set;
        using VoxelSet1::updateStride;
        using VoxelSet1::getCoordinates3D;
        using VoxelSet1::getSlices;
        using VoxelSet1::getRows;
        using VoxelSet1::getColumns;
        using VoxelSet1::operator=;
	using VoxelSet1::getProposedBorder;
	using VoxelSet1::operator+;

        /**
         * default constructor
         */
        StructuringElement3(int stride= 4000, int slicePitch= 640000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        StructuringElement3(const StructuringElement3& s);
        
        /**
         * destructor
         */
        ~StructuringElement3();

        VoxelSet1& operator=(const VoxelSet1& v);
        
        /**
         * computes the row and column coordinates of a row-major coordinate
         * @param n row-major coordinate
         * @param row output parameter, row coordinate
         * @param column output parameter, column coordinate
         */
        

        /**
         * computes the sum of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applySum(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the max of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMax(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the min of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMin(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the mean of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMean(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the variance of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyVariance(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the standard deviation of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyStandardDeviation(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * computes the median of the values in image in in the positions of the pixels
         * of the structuring elements, and puts the values in image out.
         * @param in input
         * @param out output
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         * @param roi if not NULL, the operation uses only the foreground (non-zero) region of the image
         */
        template<typename INPUT, typename OUTPUT>
        void applyMedian(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param s StructuringElement3 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const StructuringElement3& s);

    /*class StructuringElement32: public VoxelSet2
    {
    public:
        StructuringElement32();

        StructuringElement32(StructuringElement3& se);

        StructuringElement32(const StructuringElement32& se);

        ~StructuringElement32();
    };*/
}

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applySum(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getSum(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getSum(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyMax(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMax(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMax(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyMin(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMin(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMin(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyMean(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMean(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMean(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyVariance(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);

        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getVariance(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getVariance(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyStandardDeviation(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);
        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getStandardDeviation(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getStandardDeviation(in, i, support));
    }

    template<typename INPUT, typename OUTPUT>
    void StructuringElement3::applyMedian(Volume<INPUT>& in, Volume<OUTPUT>& out, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(in.columns, in.sliceSize);
        
        int start= -min;
        int end= in.n - max;

        if ( roi == NULL )
            for ( int i= start; i < end; ++i )
                out(i)= (OUTPUT)(this->getMedian(in, i, support));
        else
            for ( int i= start; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    out(i)= (OUTPUT)(this->getMedian(in, i, support));
    }
}


#endif	/* STRUCTURINGELEMENT3_H */

