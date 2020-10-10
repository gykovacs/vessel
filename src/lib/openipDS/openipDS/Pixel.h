/**
 * @file Pixel.h
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
 * The Pixel1 class represents a pixel in row-continuous representation.
 * The Pixel2 class represents a pixel in two coordinate representation.
 */

#ifndef _PIXEL_H_
#define _PIXEL_H_

#include <openipDS/Vector2.h>

using namespace openip;

namespace openip
{
    /**
     * Pixel1 represents a pixel in row-continuous representation
     */
    typedef int Pixel1;

    /**
     * Pixel2 represents a pixel in two coordinate representation
     */
    class Pixel2 : public Vector2<int>
    {
    public:
        /**
         * default constructor
         */
        Pixel2();

        /**
         * copy constructor
         * @param p_ instance to copy
         */
        Pixel2(const Pixel2& p_);

        /**
         * constructor with initial values
         * @param x row index of the pixel
         * @param y column index of the pixel
         */
        Pixel2(int x, int y);

        /**
         * destructor
         */
        ~Pixel2();

        int r() const;

        int c() const;

        int& r();

        int& c();

        /**
         * returns the row-continuous representation index
         * @param stride stride of the image
         * @return Pixel1 object
         */
        Pixel1 getPixel1(int stride);
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param p Pixel2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const Pixel2& p);
}

#endif
