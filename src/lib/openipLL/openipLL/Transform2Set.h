/**
 * @file Transform2Set.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Transform2Set represents a set of image transforms.
 */

#ifndef TRANSFORM2SET_H
#define	TRANSFORM2SET_H

#include <string>

#include <openipDS/Image.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/RatioFeature2s.h>
#include <openipDS/DifferenceFeature2s.h>
#include <openipDS/Transform2.h>
#include <openipDS/Volume.h>
#include <openipDS/ImageVector.h>
#include <openipDS/ParameterFile.h>
#include <openipDS/Stopper.h>

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class Transform2Generator;

    /**
     * Transform2Set represents a set of image transforms.
     */
    template<typename INPUT, typename OUTPUT>
    class Transform2Set: public Vector<VectorTransform2<INPUT, OUTPUT>* >
    {
    public:
        using Vector<VectorTransform2<INPUT, OUTPUT>* >::push_back;
        using Vector<VectorTransform2<INPUT, OUTPUT>* >::operator();

        /**
         * default constructor
         */
        Transform2Set();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Transform2Set(const Transform2Set& f);

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
        virtual ~Transform2Set();

        /**
         * applies the transform to the input image
         * @param input input image
         * @param output output image vector
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         * @param roi the operation uses only the foreground (non-zero) region of the roi
         */
        void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* mask= NULL, Image<unsigned char>* roi= NULL);

        /**
         * adds transform to the transform set
         * @param f transform to add
         */
        void add(VectorTransform2<INPUT, OUTPUT>* f);

        /**
         * using the proposed border, the each transform can be applied to each position of an image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();

        int dimensions();
    };

    template<typename INPUT, typename OUTPUT>
    Transform2Set<INPUT, OUTPUT>::Transform2Set()
    : Vector<VectorTransform2<INPUT, OUTPUT>* >()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2Set<INPUT, OUTPUT>::Transform2Set(const Transform2Set& f)
    : Vector<VectorTransform2<INPUT, OUTPUT>* >(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2Set<INPUT, OUTPUT>::~Transform2Set()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Transform2Set<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* mask, Image<unsigned char>* roi)
    {
        unsigned int d= this->dimensions();
        int n= 0;
        int i= 0;
        for ( n= 0; n < int(d); )
        {
//            printf("applying transform %d %d\n", n, output.size()); fflush(stdout);
            (*this)(i)->apply(input, output, n, mask, roi);
            n+= (*this)(i)->dimensions();
            ++i;
        }
//        tprintf("transforms applied\n");
    }

    template<typename INPUT, typename OUTPUT>
    void Transform2Set<INPUT, OUTPUT>::add(VectorTransform2<INPUT, OUTPUT>* f)
    {
        this->push_back(f);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 Transform2Set<INPUT, OUTPUT>::getProposedBorder()
    {
	Border2 btmp, bmax;
	
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
	  btmp= (*this)(i)->getProposedBorder();
	  bmax= maximizeBorders(btmp, bmax);
        }
        return bmax;
    }

    template<typename INPUT, typename OUTPUT>
    int Transform2Set<INPUT, OUTPUT>::dimensions()
    {
        int d= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
            d+= (*this)(i)->dimensions();
        return d;
    }

}

#endif	/* TRANSFORM2SET_H */

