/**
 * @file configIO.h
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
 * A set of constants related to image IO.
 */

#ifndef _CONFIG_H
#define	_CONFIG_H

#define USE_BMP
#define USE_PGM
#define USE_FLOAT
//#define USE_MINC2

namespace openip
{
    /**
     * Image IO error codes
     */
    enum ImageIOError
    {
        SUCCESSFUL= 0,
        NOT_SUPPORTED_IMAGE_FORMAT,
        NO_SUCH_FILE,
        CANNOT_OPEN_FILE,
        NOT_SUPPORTED_INNER_IMAGE_FORMAT,
        UNKNOWN_EXTENSION,
    };

    /**
     * channel identifiers, when reading in only one intensity image
     */
    enum readChannelType
    {
        READ_CHANNEL_RED= 1,
        READ_CHANNEL_GREEN= 2,
        READ_CHANNEL_BLUE= 3,
        READ_CHANNEL_GRAYSCALE= 4,
    };
}

#endif	/* _CONFIG_H */

