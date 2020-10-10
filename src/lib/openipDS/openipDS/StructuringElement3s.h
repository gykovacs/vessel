/* 
 * File:   StructuringElement3s.h
 * Author: gykovacs
 *
 * Created on July 19, 2011, 8:45 PM
 */

#ifndef STRUCTURINGELEMENT3S_H
#define	STRUCTURINGELEMENT3S_H

#include <openipDS/StructuringElement3.h>

namespace openip
{
    /**
     * method to generate disk structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param r radius of the disk shaped structuring element
     */
    void generateStructuringElementSphere(StructuringElement3& se, int r, int stride, int slicePitch);

    /**
     * method to generate square structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param size size of a side of the square
     */
    void generateStructuringElement3Rectangle(StructuringElement3& se, int s, int r, int c, int stride, int slicePitch);
    
    /**
     * method to generate square structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param size size of a side of the square
     */
    void generateStructuringElementCube(StructuringElement3& se, int size, int a, int stride, int slicePitch);

    /**
     * method to generate circle structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param r radius of the circle shaped structuring element
     */
    void generateStructuringElementBall(StructuringElement3& se, int r, int stride, int slicePitch);

    /**
     * method to generate circle structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param r radius of the circle shaped structuring element
     */
    void generateStructuringElementAxial(StructuringElement3& se, int r, int stride, int slicePitch);
    
    void generateStructuringElementLineSegment(StructuringElement3& se, int r, float a, float b, int stride, int slicePitch);
    

    /**
     * StructuringElementSquare is a square shaped structuring element
     */
    class StructuringElementSphere : public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        /**
         * constructor to generate square structuring element
         * @param stride stride of the image on which the element will be used
         * @param size of a side of the square
         */
        StructuringElementSphere(int size, int stride= 8000, int slicePitch= 6400000);
        
        StructuringElementSphere(const StructuringElementSphere& s);
        
        ~StructuringElementSphere();
    };

    /**
     * StructuringElementDisk is a disk shaped structuring element
     */
    class StructuringElement3Rectangle : public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        /**
         * constructor to generate square structuting element
         * @param stride stride of the image on which the element will be used
         * @param r radius of the disk
         */
        StructuringElement3Rectangle(int s, int r, int c, int stride= 8000, int slicePitch= 6400000);
        
        StructuringElement3Rectangle(const StructuringElement3Rectangle& s);
        
        ~StructuringElement3Rectangle();
    };

    class StructuringElementCube : public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementCube(int a, int stride= 8000, int slicePitch= 6400000);
        
        StructuringElementCube(const StructuringElementCube& s);
        
        ~StructuringElementCube();
    };

    class StructuringElementBall: public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementBall(int r, int stride= 8000, int slicePitch= 6400000);
        
        StructuringElementBall(const StructuringElementBall& s);
        
        ~StructuringElementBall();
	
	int r;
    };

    class StructuringElementAxial: public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementAxial(int r, int stride= 8000, int slicePitch= 6400000);

        StructuringElementAxial(const StructuringElementAxial& s);

        ~StructuringElementAxial();
    };
    
    class StructuringElementLineSegment: public StructuringElement3
    {
    public:
      using StructuringElement3::set;
      using StructuringElement3::operator=;
      using StructuringElement3::getProposedBorder;
      using StructuringElement3::operator+;
      
      StructuringElementLineSegment(int r, float a, float b, int stride= 1000, int slicePitch= 640000);
      
      StructuringElementLineSegment(Vector<int>& tmp, int stride= 1000, int slicePitch= 64000);
      
      StructuringElementLineSegment(const StructuringElementLineSegment& s);
      
      ~StructuringElementLineSegment();
    };
    
    class StructuringElementCS: public StructuringElement3
    {
    public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementCS(int r= 5, int stride= 8000, int slicePitch= 6400000);

        StructuringElementCS(const StructuringElementCS& s);

        ~StructuringElementCS();
    };
    
    class StructuringElementAC: public StructuringElement3
    {
      public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementAC(int r, int stride= 8000, int slicePitch= 6400000);

        StructuringElementAC(const StructuringElementAC& s);

        ~StructuringElementAC();
    };
    
    class StructuringElementAS: public StructuringElement3
    {
      public:
        using StructuringElement3::set;
        using StructuringElement3::operator=;
	using StructuringElement3::getProposedBorder;

        StructuringElementAS(int r, int stride= 8000, int slicePitch= 6400000);

        StructuringElementAS(const StructuringElementAS& s);

        ~StructuringElementAS();
    };
}

#endif	/* STRUCTURINGELEMENT3S_H */

