/**
 * @file Feature1.h
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
 * The Feature1 class is the base classes for all feature extractable
 * in a given position of a vector.
 */

#ifndef _FEATURE1_H
#define	_FEATURE1_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/Transform2.h>

namespace openip
{
    /**
    * Feature1 represents a feature extractable from vectors
    */
    template<typename INPUT, typename OUTPUT>
    class Feature1
    {
    public:
	/**
	* exracts the feature from vector input at position n
	* @param input input vector
	* @param n position
	*/
        virtual OUTPUT apply(Vector<INPUT>& input, int n, Vector<unsigned char>* support= NULL)= 0;

	/**
	* extracts the feature in all positions of vector input into vector output in the foreground (non 0) region of the mask
	* @param input input vector
	* @param output output vector
	* @param mask the operation is performed in the foreground region
	*/
        virtual void apply(Vector<INPUT>& input, Vector<OUTPUT>& output, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL)= 0;

        /**
         * computes the minimum and maximum positions in row-major representation, where the features can be applied
         */
        virtual void computeMinMax();

        /**
         * the minimum position
         */
        virtual int getMin();

        /**
         * the maximum position
         */
        virtual int getMax();
    };

    template<typename INPUT, typename OUTPUT>
    void Feature1<INPUT, OUTPUT>::computeMinMax()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int Feature1<INPUT, OUTPUT>::getMin()
    {
      return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    int Feature1<INPUT, OUTPUT>::getMax()
    {
      return 0;
    }

}

#endif	/* _FEATURE_H */

