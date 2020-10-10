/**
 * @file PixelSequence1.h
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
 * The PixelSequence1 class represents a sequence of Pixels in row-continuous
 * representation and has several useful operators and member functions defined.
 * Used for representation of contours or binary objects.
 */

#ifndef _PIXELSEQUENCE1_H
#define	_PIXELSEQUENCE1_H

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
     * PixelSequence1 represents a sequence of pixels in row-continuous
     * representation
     */
    class PixelSequence1 : public PixelSequence<Pixel1>
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

#endif	/* _PIXELSEQUENCE1_H */

