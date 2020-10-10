/**
 * @file BinaryMaskSet2.h
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
 * The BinaryMaskSet2 class represents a set of binary masks.
 */

#ifndef _BINARYMASKSET2_H
#define	_BINARYMASKSET2_H

#include <float.h>
#include <string>
#include <vector>

#include <openipDS/Filter2.h>
#include <openipDS/Image.h>
#include <openipDS/Pixel.h>
#include <openipDS/BinaryMask2.h>
#include <openipDS/MaskSet2.h>

namespace openip
{
    /**
     * BinaryMaskSet represents a set of binary masks
     */
    class BinaryMaskSet2: public std::vector<BinaryMask2* >
    {
    public:
        /**
         * defines the logical connection between the masks in the set
         */
        enum MaskSetMode
        {
            /**
             * the mask set matches if any of the masks in it matches
             */
            MATCH_ANY= 1,

            /**
             * the mask set matches if all of the masks in it matches
             */
            MATCH_ALL,

            /**
             * the mask set matches if none of the masks in it matches
             */
            MATCH_NONE,
        };

        /**
         * constructor, creates an empty mask set
         * @param matches return value when matches
         * @param notMatches return value when not matches
         * @param mode matching mode
         */
        BinaryMaskSet2(unsigned char matches= 255, unsigned char notMatches= 0, int mode= MATCH_ANY);

        /**
         * copy constructor
         * @param bms instance to copy
         */
        BinaryMaskSet2(const BinaryMaskSet2& bms);

        /**
         * destructor
         */
        ~BinaryMaskSet2();

        /**
         * matches the masks in each position n. If any is matching, returns
         * matches value
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchAny(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in each position n. If any is matching, returns
         * matches value
         * @param input input image
         * @param output output pixel set
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchAny(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in each position n. If all is matching, returns
         * matches value
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchAll(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in each position n. If any is matching, returns
         * matches value
         * @param input input image
         * @param output output pixel set
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchAll(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in each position n. If none is matching, returns
         * matches value
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchNone(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in each position n. If any is matching, returns
         * matches value
         * @param input input image
         * @param output output pixel set
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void matchNone(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the environment of a pixel for any matching
         * @param env environment
         */
        unsigned char matchAny(unsigned int env);

        /**
         * matches the environment of a pixel for all matching
         * @param env environment
         */
        unsigned char matchAll(unsigned int env);

        /**
         * matches the environment of a pixel for no matching
         * @param env environment
         */
        unsigned char matchNone(unsigned int env);

        /**
         * extracts the environment of the pixel n in the input image
         * @param in input image
         * @param n position
         * @return environment
         */
        unsigned int extractEnvironment(Image<unsigned char>& in, int n);

        /**
         * applies the mask set to each position of image input and the
         * result get into image output
         * @param input image
         * @param output image
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void match(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask= NULL);

        /**
         * applies the mask set to each position of image input and the result
         * gets into output pixel set
         * @param input input image
         * @param output output pixel set
         * @param mask the operation is performed in the foreground positions (non 0) of the mask
         */
        void match(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the environment
         * @param env enviroment to match
         * @return matches or notMatches values
         */
        unsigned char match(unsigned int env);

        /**
         * matches the mask set in the nth position of the row-continuously stored image
         * @param input input image
         * @param n row-continuous coordinate
         * @return matches or notMatches values
         */
        unsigned char match(Image<unsigned char>& input, int n);

        /**
         * computes the value of min and max
         */
        void computeMinMax();

        /**
         * initializes the mask set
         */
        virtual void init(int stride= -1);

        /**
         * generates look up table
         */
        void generateLookUpTable();

        /**
         * returns the stride value of the binary masks in the set
         * if they differ, returns -1
         */
        int stride();

        /**
         * updates the stride of masks in the set
         * @param stride new stride
         */
        void updateStride(int stride);

        /**
         * this value is the output if a mask matches
         */
        unsigned char matches;

        /**
         * this value is the output if none of the masks matches
         */
        unsigned char notMatches;

        /**
         * smallest position in the masks
         */
        int min;

        /**
         * maximum position in the masks
         */
        int max;

        /**
         * mode of the mask set
         */
        int mode;

        /**
         * lookUpTable for fast processing
         */
        std::vector<unsigned char> lookUpTable;

        /**
         * look up table file name
         */
        std::string lookUpTableFileName;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param b BinaryMaskSet2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const BinaryMaskSet2& b);
}

#endif	/* _BINARYMASKSET2_H */

