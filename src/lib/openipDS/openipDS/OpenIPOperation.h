/**
 * @file OpenIPOperation.h
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
 * OpenIPOperation is a general operation applicable on images.
 */

#ifndef _OPENIPOPERATION_H
#define	_OPENIPOPERATION_H

#include <openipDS/Image.h>

namespace openip
{
    /**
    * OpenIPOperation is a general wrapper for image processing algorithms
    */
    template<typename INPUT, typename OUTPUT>
    class OpenIPOperation
    {
    public:
	/**
	* default constructor
	*/
        OpenIPOperation();

	/**
	* copy constructor
	* @param o object to copy
	*/
        OpenIPOperation(const OpenIPOperation& o);

	/**
	* destructor
	*/
        ~OpenIPOperation();

	/**
	* applies the operation to vector input into vector output in the foreground (non 0) region of the mask
	* @param input input vector
	* @param output output vector
	* @param mask the operation is performed in the foreground region of the mask
	*/
        virtual void apply(Vector<INPUT>& input, Vector<OUTPUT>& output, Vector<unsigned char>* mask= NULL)= 0;

	/**
	* applies the operation to vector input into vector output in the foreground (non 0) region of the mask, using temporary objects from cache c
	* @param input input vector
	* @param c Cache2 object
	* @param output output vector
	* @param mask the operation is performed in the foreground region of the mask
	*/
        //virtual void apply(Vector<INPUT>& input, Cache2& c, Vector<OUTPUT>& output, Vector<unsigned char>* mask= NULL)= 0;
    };
}

#endif	/* _OPENIPOPERATION_H */

