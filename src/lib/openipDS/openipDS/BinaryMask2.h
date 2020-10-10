/**
 * @file BinaryMask2.h
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
 * The BinaryMask2 class represents an arbitrary binary mask in 2 dimensions.
 */

#ifndef _BINARYMASK_H
#define	_BINARYMASK_H

#include <openipDS/Mask2.h>
#include <openipDS/PixelSet1.h>

namespace openip
{
    /**
     * BinaryMask2 class represents an arbitrary binary mask in 2 dimension
     */
    class BinaryMask2: public Mask2<unsigned char, unsigned char, unsigned char>
    {
    public:

        /**
         * BinaryMask2 constructor
         * @param stride stride of the images on which the mask will be used
         * @param matches return value where the mask matches
         * @param notMatches return value where the mask not matches
         */
        BinaryMask2(int stride= 4000, unsigned char matches= 255, unsigned char notMatches= 0);

        /**
         * copy constructor
         * @param b instance to copy
         */
        BinaryMask2(const BinaryMask2& b);

        /**
         * destructor
         */
        ~BinaryMask2();

        /**
         * generates unsigned int mask and pattern, for maximum 5x5 mask size
         */
        void generateMaskAndPattern();

        /**
         * matches the parameter binary environment, represented in an unsigned int
         * @param env environment to match
         * @return matches value if matches, notMatches value if not matches
         */
        unsigned char match(unsigned int env);

        /**
         * matches the mask in each position of the input image, the result is in the
         * output image
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void match(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the mask in each position of the input image, the result is in the
         * output pixelset
         * @param input input image
         * @param output output pixel set
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void match(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask= NULL);

        /**
         * rotates the mask with 90 degrees counter-clockwise
         * @return this object
         */
        BinaryMask2& rotate90();

        /**
         * rotates the mask with 180 degrees
         * @return this object
         */
        BinaryMask2& rotate180();

        /**
         * rotates the mask with 270 degrees
         * @return this object
         */
        BinaryMask2& rotate270();

        /**
         * mask, contains 1s in the positions of the mask elements
         */
        unsigned int mask;

        /**
         * pattern, contains the binary value of the mask in the appropriate position
         */
        unsigned int pattern;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param b BinaryMask2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const BinaryMask2& b);
}


#endif	/* _BINARYMASK_H */

