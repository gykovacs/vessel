/* 
 * File:   Transform3Set.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 4:17 PM
 */

#ifndef TRANSFORM3SET_H
#define	TRANSFORM3SET_H

#include <string>

#include <openipDS/Image.h>
//#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature3s.h>
//#include <openipDS/RatioFeature2s.h>
//#include <openipDS/DifferenceFeature2s.h>
#include <openipDS/Transform3.h>
#include <openipDS/Volume.h>
#include <openipDS/VolumeVector.h>
#include <openipDS/ParameterFile.h>
#include <openipDS/Stopper.h>

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class Transform3Generator;

    /**
     * Transform2Set represents a set of image transforms.
     */
    template<typename INPUT, typename OUTPUT>
    class Transform3Set: public Vector<VectorTransform3<INPUT, OUTPUT>* >
    {
    public:
        using Vector<VectorTransform3<INPUT, OUTPUT>* >::push_back;
        using Vector<VectorTransform3<INPUT, OUTPUT>* >::operator();

        /**
         * default constructor
         */
        Transform3Set();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Transform3Set(const Transform3Set& f);

        /**
         * constructor, creates transform set from feature descriptor file
         * @param fdf feature descriptor file
         * @param keyword parameter name
         */
        /*Transform2Set(std::string fdf, std::string keyword);

        Transform2Set(Vector<std::string>& descriptors);*/
        /**
         * destructor
         */
        ~Transform3Set();

        /**
         * applies the transform to the input image
         * @param input input image
         * @param output output image vector
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         * @param roi the operation uses only the foreground (non-zero) region of the roi
         */
        void apply(Volume<INPUT>& input, VolumeVector<OUTPUT>& output, Volume<unsigned char>* mask= NULL, Volume<unsigned char>* roi= NULL);

        /**
         * adds transform to the transform set
         * @param f transform to add
         */
        void add(VectorTransform3<INPUT, OUTPUT>* f);

        /**
         * using the proposed border, the each transform can be applied to each position of an image
         * @return the proposed border
         */
        virtual Border3 getProposedBorder();

        int dimensions();
    };

    template<typename INPUT, typename OUTPUT>
    Transform3Set<INPUT, OUTPUT>::Transform3Set()
    : Vector<VectorTransform3<INPUT, OUTPUT>* >()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform3Set<INPUT, OUTPUT>::Transform3Set(const Transform3Set& f)
    : Vector<VectorTransform3<INPUT, OUTPUT>* >(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform3Set<INPUT, OUTPUT>::~Transform3Set()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Transform3Set<INPUT, OUTPUT>::apply(Volume<INPUT>& input, VolumeVector<OUTPUT>& output, Volume<unsigned char>* mask, Volume<unsigned char>* roi)
    {
        unsigned int d= this->dimensions();
	int j= 0;
        for ( unsigned int i= 0; i < d; )
        {
            tprintf("applying %s\n", (*this)(j)->descriptor.c_str()); fflush(stdout);
            (*this)(j)->apply(input, output, i, mask, roi);
            i+= (*this)(j)->dimensions();
	    ++j;
        }
    }

    template<typename INPUT, typename OUTPUT>
    void Transform3Set<INPUT, OUTPUT>::add(VectorTransform3<INPUT, OUTPUT>* f)
    {
        this->push_back(f);
    }

    template<typename INPUT, typename OUTPUT>
    Border3 Transform3Set<INPUT, OUTPUT>::getProposedBorder()
    {
        Border3 b;
        
        if ( this->size() > 0 )
                b= (*this)(0)->getProposedBorder();
        
        for ( unsigned int i= 1; i < this->size(); ++i )
        {
            b= Border3(b, (*this)(i)->getProposedBorder());
        }
        
        
        return b;
    }

    template<typename INPUT, typename OUTPUT>
    int Transform3Set<INPUT, OUTPUT>::dimensions()
    {
        int d= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
            d+= (*this)(i)->dimensions();
        return d;
    }

}

#endif	/* TRANSFORM3SET_H */

