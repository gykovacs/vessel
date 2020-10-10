/**
 * @file ImageInfo.h
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
 * The ImageInfo class represents the general properties of an image.
 */

#ifndef IMAGEINFO_H
#define	IMAGEINFO_H

namespace openip
{
    /**
     * enumeration of possible color types
     */
    enum ColorType
    {
        COLOR_TYPE_INDEXED= 1,
        COLOR_TYPE_GRAYSCALE= 2,
        COLOR_TYPE_RGB= 3,
        COLOR_TYPE_GRAYSCALE_ALPHA= 4,
        COLOR_TYPE_RGB_ALPHA= 5,
        COLOR_TYPE_YCbCr= 6,
        COLOR_TYPE_CMYK= 7,
        COLOR_TYPE_YCCK= 8,
    };

    /**
     * ImageInfo class represents the general properties of an image
     */
    class ImageInfo
    {
    public:
        /** color type */
        int colorType;
        /** number of rows */
        int rows;
        /** number of columns */
        int columns;
        /** bit depth */
        int bitDepth;
    };
}

#endif	/* IMAGEINFO_H */

