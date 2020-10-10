/**
 * @file PixelSequence2.h
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
 * The PixelSequence2 class represents a sequence of Pixels in two coordinate
 * representation and has several useful operators and member functions defined.
 * Used for representation of contours or binary objects.
 */

#ifndef _PIXELSEQUENCE2_H
#define	_PIXELSEQUENCE2_H

#include <vector>
#include <limits.h>
#include <algorithm>

#include <openipDS/Pixel.h>
#include <openipDS/Image.h>
#include <openipDS/PixelSequence.h>
#include <openipDS/Vector.h>

namespace openip
{
    /**
     * PixelSequence2 represents a sequence of pixels in two coordinate
     * representation
     */
    class PixelSequence2 : public PixelSequence<Pixel2>
    {
    public:
        /**
         * compute min, max, minX, maxX, minY, maxY values
         * @param stride stride of the image
         */
        void computeMinMax(int stride);

        /**
         * compute minX, maxX, minY, maxY values
         * @param stride stride of the image
         */
        void computeMinMaxXY(int stride);

        /**
         * compute min, max values
         * @param stride stride of the iamge
         */
        void computeMinMaxOnly(int stride);
    };
}

#endif	/* _PIXELSEQUENCE2_H */

