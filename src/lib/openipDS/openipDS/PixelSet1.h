/**
 * @file PixelSet1.h
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
 * The PixelSet1 class represents a set of Pixels in row-continuous representation
 * and has several useful operators and member functions defined.
 */

#ifndef _PIXELSET1_H
#define	_PIXELSET1_H

#include <openipDS/Set.h>
#include <openipDS/Pixel.h>
#include <openipDS/Image.h>
#include <openipDS/PixelSet.h>

namespace openip
{
    /**
     * PixelSet1 represents a set of Pixel1 objects.
     */
    class PixelSet1 : public PixelSet<Pixel1>
    {
    public:
        using PixelSet<Pixel1>::set;

        PixelSet1();
        
        PixelSet1(const PixelSet1& p);
        
        ~PixelSet1();
        
        PixelSet1& operator=(const PixelSet1& p);

        PixelSet1 operator/(const PixelSet1& p) const;

        PixelSet1& operator/=(const PixelSet1& p);
        
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

        /**
         * updates stride
         * @param stride new stride
         */
        virtual int updateStride(int stride);

         /**
         * get number of rows in the filter elements
         * @return number of rows (height)
         */
        int getRows();

        /**
         * get number of columns in the filter elements
         * @return number of columns (width)
         */
        int getColumns();
    };
}
#endif	/* _PIXELSET1_H */

