/**
 * @file VectorFeature2.h
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
 * VectorFeature2 is the base class of all features that map a vector to a pixel.
 */

#ifndef Vector2Feature_H
#define	Vector2Feature_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/ImageVector.h>
#include <openipDS/VectorTransform2.h>

namespace openip
{
    /**
     * Vector Feature2 is the base class of all features that map a vector to a pixel.
     */
    template<typename INPUT, typename OUTPUT>
    class VectorFeature2: public VectorTransform2<INPUT, OUTPUT>
    {
    public:
        /**
         * default constructor
         */
        VectorFeature2();

        /**
         * copy constructor
         * @param f instance to copy
         */
        VectorFeature2(const VectorFeature2& f);

        /**
         * destructor
         */
        ~VectorFeature2();

        /**
         * computes the features in the nth row-major position of the input image
         * @param input input image
         * @param n row-major position
         * @param output computed features
         */
        virtual void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VectorFeature2<INPUT, OUTPUT>::VectorFeature2()
    : VectorTransform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorFeature2<INPUT, OUTPUT>::VectorFeature2(const VectorFeature2& f)
    : VectorTransform2<INPUT, OUTPUT>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorFeature2<INPUT, OUTPUT>::~VectorFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output, Image<unsigned char>* support)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
    }
}


#endif	/* Vector2Feature_H */

