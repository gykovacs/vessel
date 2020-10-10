/* 
 * File:   ComponentLabeling.h
 * Author: gykovacs
 *
 * Created on 2011. április 13., 20:09
 */

#ifndef COMPONENTLABELING_H
#define	COMPONENTLABELING_H

#include <float.h>

#include <openipDS/Image.h>
#include <openipDS/Transform2.h>
#include <openipDS/Transform3.h>
#include <openipLL/imageIO.h>
#include <openipLL/ROIDetector.h>
#include <openipDS/Region2.h>
#include <openipDS/Volume.h>
#include <openipDS/VoxelSet1.h>
#include <openipDS/Region2Set.h>
#include <openipDS/Region3Set.h>
#include <queue>

namespace openip
{
    class ComponentLabeling: public Transform2<unsigned char, int>
    {
    public:
        /**
         * constructor
         */
        ComponentLabeling(int neighborhood= 2);

        /**
         * copy constructor
         * @param c instance to copy
         */
        ComponentLabeling(const ComponentLabeling& c);

        /**
         * destructor
         */
        ~ComponentLabeling();

        /**
         * labels the input image in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<unsigned char>& input, Image<int>& output, Image<unsigned char>* mask= NULL);
	
	int neighborhood;
    };

    class ExtractRegions
    {
    public:
        ExtractRegions(int neighborhood= 2);
        
        ExtractRegions(const ExtractRegions& e);
        
        ~ExtractRegions();
        
        virtual void apply(Image<unsigned char>& input, Vector<Region2>& result, bool foreground= true);
	
	int neighborhood;
    };

    class RecursiveComponentLabeling3: public Transform3<unsigned char, int>
    {
    public:
        RecursiveComponentLabeling3();

        RecursiveComponentLabeling3(const RecursiveComponentLabeling3& r);

        ~RecursiveComponentLabeling3();

        virtual void apply(Volume<unsigned char>& input, Volume<int>& output, Volume<unsigned char>* roi= NULL);
	
	virtual void apply(Volume<unsigned char>& input, Volume<int>& output, Region3& roi);

        void labeling(Volume<unsigned char>& input, Volume<unsigned char>& mask, Volume<int>& output, int label);

        std::queue<int> slices;
        std::queue<int> rows;
        std::queue<int> columns;
    };

    class ExtractRegions3
    {
    public:
        ExtractRegions3();

        ExtractRegions3(const ExtractRegions3& e);

        ~ExtractRegions3();

        virtual void apply(Volume<unsigned char>& input, Region3Set& result, Volume<unsigned char>* roi= NULL, bool foreground= true);
	
	virtual void apply(Volume<unsigned char>& input, Region3Set& result, Region3& roi, bool foreground= true);
    };
}

#endif	/* COMPONENTLABELING_H */

