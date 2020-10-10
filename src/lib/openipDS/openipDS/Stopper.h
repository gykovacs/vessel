/**
 * @file Stopper.h
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
 * The Stopper class represents a simple tool for measuring time intervals.
 */

#ifndef _STOPPER_H_
#define _STOPPER_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace openip
{
    /** timed standard output */
    int tprintf(const char* fmt, ...);

    int setprintf(const char* fmt, ...);

    int ftprintf(FILE* f, const char* fmt, ...);

    int eprintf(const char* fmt, ...);

    int feprintf(FILE* f, const char* fmt, ...);

    int wprintf(const char* fmt, ...);

    int weprintf(FILE* f, const char* fmt, ...);

    int etprintf(const char* fmt, ...);

    int fetprintf(FILE* f, const char* fmt, ...);

    int wtprintf(const char* fmt, ...);

    int fwtprintf(FILE* f, const char* fmt, ...);
    
    std::string getTimeString(char separator='_');

    /**
      * Stopper is a simple tool to measure time intervals
      */
    class Stopper
    {
    public:
        /**
          * starts the stopper
          */
        virtual void start();

        /**
          * stops the stopper
          */
        virtual void stop();

        /**
          * converts to time
          */
        virtual void convertToTime();

        /**
         * returns the time difference as string
         * @return the time difference as string
         */
        virtual std::string getElapsedTime();

        /**
          * hours
          */
        unsigned int h;

        /**
          * minutes
          */
        unsigned int m;

        /**
          * seconds
          */
        unsigned int s;

        /**
          * milliseconds
          */
        unsigned int ms;
    };
}
#endif
