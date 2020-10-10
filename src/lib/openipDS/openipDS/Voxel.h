/**
 * @file Voxel.h
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
 * The Voxel class represents the primitive element of Volumes.
 */

#ifndef _VOXEL_H
#define	_VOXEL_H

#include <openipDS/Vector3.h>

using namespace openip;

namespace openip
{
    /**
     * Voxel1 represents a voxel in row-continuous representation
     */
    typedef int Voxel1;

    /**
     * Voxel2 represents a voxel in two coordinate representation
     */
    class Voxel3
    {
    public:
        /**
         * default constructor
         */
        Voxel3();

        /**
         * copy constructor
         * @param p_ instance to copy
         */
        Voxel3(const Voxel3& p_);

        /**
         * constructor with initial values
         * @param s slice index of the voxel
         * @param r row index of the voxel
         * @param c column index of the voxel
         */
        Voxel3(int s, int r, int c);

        /**
         * destructor
         */
        ~Voxel3();

        /**
         * returns the row-continuous representation index
         * @param columns columns of the volume
         * @param rows rows of the volume
         * @return Pixel1 object
         */
        Voxel1 getVoxel1(int rows, int columns);

        /** slice index */
        int s;
        /** row index */
        int r;
        /** column index */
        int c;
    };



    /**
    * << operator
    *
    * @param o output stream object
    * @param v Voxel3 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const Voxel3& v);
}

#endif	/* _VOXEL_H */

