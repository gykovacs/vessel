/**
 * @file PixelSet2.h
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
 * The PixelSet2 class represents a set of Pixel2 objects and has several
 * useful operators and member functions defined.
 */

#ifndef _PIXELSET2_H
#define	_PIXELSET2_H

#include <openipDS/Set.h>
#include <openipDS/Pixel.h>
#include <openipDS/Image.h>
#include <openipDS/PixelSet.h>

namespace openip
{
    /**
     * PixelSet2 represents a set of Pixel2 objects.
     */
    class PixelSet2 : public PixelSet<Pixel2>
    {
    public:
        /**
         * compute min, max, minX, maxX, minY, maxY values
         */
        void computeMinMax();

        /**
         * compute minX, maxX, minY, maxY values
         */
        void computeMinMaxXY();

        /**
         * compute min, max values
         */
        void computeMinMaxOnly();
    };
}
#endif	/* _PixelSet2_H */

