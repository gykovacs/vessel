/**
 * @file VisualWordExtractor.h
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
 * VisualWordExtractor extracts visual words from images.
 */

#ifndef VISUALWORDEXTRACTOR_H
#define	VISUALWORDEXTRACTOR_H

#include <openipDS/VisualWord.h>
#include <openipDS/VisualWordSet.h>

namespace openip
{
    /**
     * VisualWordExtractor extracts visual words from images
     */
    class VisualWordExtractor
    {
    public:
        /**
         * constructor
         * @param n number of visual words to extract
         */
        VisualWordExtractor(int n= 50);

        /**
         * copy constructor
         * @param v instance to copy
         */
        VisualWordExtractor(const VisualWordExtractor& v);

        /**
         * destructor
         */
        ~VisualWordExtractor();

        /**
         * extracts through database descriptor parameter file
         * @param filename database descriptor parameter file
         */
        void extract(char* filename);

        /**
         * number of visual words to extract
         */
        int n;
        /**
         * vector of visual word set
         */
        Vector<VisualWordSet<unsigned char, float> > vws;
    };
}

#endif	/* VISUALWORDEXTRACTOR_H */

