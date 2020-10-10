/**
 * @file BinaryMaskSystem2.h
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
 * The BinaryMaskSystem2 represents a set of BinaryMaskSet2s. Using this abstraction,
 * a system of binary masks can be defined with arbitrary logical connection
 * among them.
 */

#ifndef _BINARYMASKSYSTEM2_H
#define	_BINARYMASKSYSTEM2_H

#include <string>

#include <openipDS/BinaryMaskSet2.h>

namespace openip
{
    /**
     * BinaryMaskSystem represents a set of BinaryMaskSets. Using this abstraction,
     * a system of binary masks can be defined with arbitrary logical connection
     * among them
     */
    class BinaryMaskSystem2: public vector<BinaryMaskSet2* >
    {
    public:
        /**
         * constructor
         * @param matches return value when the filter matches a position
         * @param notMatches return value when the filter does not match a position
         */
        BinaryMaskSystem2(unsigned char matches= 255, unsigned char notMatches= 0);

        /**
         * copy constructor
         * @param bms instance to copy
         */
        BinaryMaskSystem2(const BinaryMaskSystem2& bms);

        /**
         * destructor
         */
        ~BinaryMaskSystem2();

        /**
         * matches the mask in each position of the input image
         * @param in input image
         * @param out output image
         * @param mask foreground (not 0) where to apply the mask
         */
        void match(Image<unsigned char>& in, Image<unsigned char>& out, Image<unsigned char>* mask= NULL);

        /**
         * matches the mask in each position of the input image, the result is
         * in the input image
         * @param in input image
         * @param c Cache2 object
         * @param mask foreground (not 0) where to apply the mask
         */
        /*oid match(Image<unsigned char>& in, Cache2* c, Image<unsigned char>* mask= NULL);*/

        /**
         * matches the mask in each position of the input image
         * @param in input image
         * @param out output matched pixelset
         * @param mask foreground (not 0) where to apply the mask
         */
        void match(Image<unsigned char>& in, PixelSet1& out, Image<unsigned char>* mask= NULL);

        /**
         * matches the environment
         * @param env environment to match
         * @return matches if the mask matches, notMatches if the mask does not match
         */
        unsigned char match(unsigned int env);

        /**
         * extracts the environment of pixel n in row-continuous representation
         * @param in input image
         * @param n pixel position
         * @return the extracted binary environment
         */
        unsigned int extractEnvironment(Image<unsigned char>& in, int n);

        /**
         * returns the stride of masks in the sets if they are the same, in the
         * case of differing, returns -1;
         * @return stride of masks in the sets
         */
        int stride();

        /**
         * updates the stride in the masks in the sets
         * @param stride new stride
         */
        void updateStride(int stride);

        /**
         * initializes the mask system
         * @param stride stride of the image on which the mask system will be used
         */
        virtual void init(int stride= -1);

        /**
         * generates lookup table to fasten the processing
         */
        void generateLookUpTable();

        /**
         * computes min, max values
         */
        void computeMinMax();

        /**
         * lookup table to fasten the processing
         */
        std::vector<unsigned char> lookUpTable;

        /**
         * matches value
         */
        unsigned char matches;

        /**
         * does not match value
         */
        unsigned char notMatches;

        /**
         * minimum pixel position
         */
        int min;

        /**
         * maximum pixel position
         */
        int max;

        /**
         * lookup table file name, the lookup table is saved automatically
         * into a file with this name; if the file is present, the table is not
         * generated, only read in
         */
        std::string lookUpTableFileName;
    };

   /**
    * << operator
    *
    * @param o output stream object
    * @param b BinaryMaskSystem2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const BinaryMaskSystem2& b);
}

#endif	/* _BINARYMASKSYSTEM2_H */

