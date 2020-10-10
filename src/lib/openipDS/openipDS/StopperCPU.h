/**
 * @file StopperCPU.h
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
 * The StopperCPU class represents a simple tool for measuring time intervals using time.h.
 */

#ifndef _STOPPER_CPU_H_
#define _STOPPER_CPU_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#include <openipDS/Stopper.h>

namespace openip
{
    /**
      * StopperCPU is a simple tool to measure time intervals
      */
    class StopperCPU: public Stopper
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
         * stores the start tick of the stopper
         */
        clock_t begin;

        /**
         * stores the stop tick of the stopper
         */
        clock_t end;
    };
}
#endif
