/* 
 * File:   Region.h
 * Author: gykovacs
 *
 * Created on 2011. március 4., 18:29
 */

#ifndef REGION_H
#define	REGION_H

#include <openipDS/PixelSet1.h>

namespace openip
{
    class Region2: public PixelSet1
    {
    public:
        Region2();

        Region2(const Region2& r);
	
	Region2(int stride);

        ~Region2();

        int area();

        int perimeter();
	
	int perimeterLength();

        float circularity();
	
	int isCircularitySmallerThan(float circ);
	
	float rectangularity();
	
	int width();
	
	int height();

        int isOuterContour4(Pixel1 p);

        int isOuterContour8(Pixel1 p);

        int isInnerContour4(Pixel1 p);

        int isInnerContour8(Pixel1 p);

        Region2 getInnerContour4();

        Region2 getOuterContour4();

        Region2 getInnerContour8();

        Region2 getOuterContour8();

	void getCoordinate2D(int n, int& r, int &c);
	
	void getCenter(int& r, int &c);
	
	Vector<float> getCenterOfGravity();
	
	int isIntersecting(Region2& r);
	
	int isNeighboring(Region2& r);
	
	Region2 getIntersection(Region2& r);
	
	int getIndexOfFarestPixel(int p);
	
	void getCoordinatesOfClosestPixel(int r0, int c0, int& r, int& c);
	
	float getDistanceFromRegion(Region2& region);
	
	float getDistanceFromRegion(int i);
	
	float getMeanDistanceFromRegion(Region2& region);
	
	int getNumberOfPixelsCloserThan(Region2& region, float dist);
	
	void removeDuplicates();
	
	Region2 connectSuccessivePixels(int resolution= 1000);
	
	Region2 connectPixels(int resolution= 1000);
	
	void add(Region2& r);
	
	void getCoordinatesOfFarestPixels(int& r0, int& c0, int& r1, int& c1);
	
	float getDistanceOfFarestPixels();
	
	void getCoordinateOfFirstPixel(int dim, int dir, int& r, int& c);

	float distanceOfCenters(Region2& r);
	
	Region2 getConvexHull();
	
	int orientation(int pr, int pc, int qr, int qc, int rr, int rc);
	
	float correlation();
	
	Vector<int> getEndPointCoordinates();
	
	float correlationNearEndPoint(int endPointCoordinate, int radius);
	
	float distanceOfPoints(int a, int b);
	
	void drdcNearEndPoint(int endPointCoordinate, int radius, float& dr, float& dc);
	
	Region2 translate(float dr, float dc, float d);
	
	Region2 rotate(float rr, float rc, float angle);
	
	int numberOfPixelsWith1Neighbor(Image<unsigned char>& input);
	int numberOfPixelsWith2Neighbor(Image<unsigned char>& input);
	int numberOfPixelsWith3Neighbor(Image<unsigned char>& input);
	int numberOfPixelsWith4Neighbor(Image<unsigned char>& input);
	int numberOfPixelsCovered(Image<unsigned char>& input);
	int numberOfCrossingPixels(Image<unsigned char>& input);
	
	static int isInnerContour4(Image<unsigned char>& input, int p);
	static int isInnerContour8(Image<unsigned char>& input, int p);
	static int isOuterContour4(Image<unsigned char>& input, int p);
	static int isOuterContour8(Image<unsigned char>& input, int p);
	
	int numberOfNeighbors(int n);
	
	int isConnective(Vector<int>& endPoints);
	
	int getWidth();
	
	int getHeight();
    };
    
    struct Region2SizeComparator
    {
    public:
        bool operator() (const Region2& a, const Region2& b);
    };
    
    template<typename T>
    void connectRegions(Region2& region1, Region2& region2, Image<T>& img, T color)
    {
      float resolution= 1000, dr, dc;
      int r0, c0, r1, c1;
      for ( unsigned int i= 0; i < region1.size(); ++i )
      {
	region1.getCoordinate2D(region1(i), r0, c0);
	
	for ( unsigned int j= 0; j < region2.size(); ++j )
	{
	  region2.getCoordinate2D(region2(j), r1, c1);
	  if ( r0 >= 0 && r0 < img.rows && r1 >= 0 && r1 < img.rows && c0 >= 0 && c0 < img.columns && c1 >= 0 && c1 < img.columns)
	  {
	    if ( r0 > 1000 || r1 > 1000 )
	    {
	      printf("%d %d %d %d, ", r0, c0, r1, c1); fflush(stdout);
	      region1.getCoordinate2D(region1(i), r0, c0);
	      region2.getCoordinate2D(region2(j), r1, c1);
	      printf("%d %d %d %d, ", r0, c0, r1, c1); fflush(stdout);
	    }
	    dr= (r0-r1)/resolution;
	    dc= (c0-c1)/resolution;
	    for ( int k= 0; k < resolution; ++k )
	    {
	      img(r1 + dr*k, c1 + dc*k)= color;
	    }
	  }
	}
      }
    }
}

#endif	/* REGION_H */

