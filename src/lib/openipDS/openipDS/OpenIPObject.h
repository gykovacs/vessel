/**
 * @file OpenIPObject.h
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
 * The OpenIPObject class is a general base of all the classes defined
 * in the openIP libraries.
 */

/*! \mainpage openIP packages
 *
 * \section intro_sec Introduction
 *
 * The aim of openIP open source image processing library is to provide
 * an open source interoperable c++ library for research and development.
 *
 * With any problems or commands contact Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 *
 */

#ifndef _OPENIPOBJECT_H
#define	_OPENIPOBJECT_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include <openipDS/Stopper.h>
#include <openipDS/constants.h>

using namespace std;

namespace openip
{
    /**
     * OpenIPObject is the general base of all the classes in the
     * openIP library
     */
    class OpenIPObject
    {
    public:
        /**
         * general toString method for object description
         * @return script description
         */
        virtual string toString() const= 0;
    };

    /**
     * ostream << operator for streaming OpenIPObjects
     * @param o ostream object
     * @param v OpenIPObject based object for streaming object description
     * @return the parameter ostream object
     */
    std::ostream& operator<<(std::ostream& o, const OpenIPObject& v);
}

#endif	/* _OPENIPOBJECT_H */

