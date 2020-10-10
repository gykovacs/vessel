/**
 * @file Progress.h
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
 * The Progress class represents a way to monitor the progress of an algorithm
 */

#ifndef PROGRESS_H_INCLUDED
#define PROGRESS_H_INCLUDED

#include <cstdio>
#include <string>
#include <sstream>

namespace openip
{
    /**
     * Progress class to monitor progress of an algorithm in command line
     */
    class Progress
    {
    public:
        /**
         * The style of output
         */
        enum ProgressType
        {
            /**
             * progress bar
             */
            PROGRESSBAR,

            /**
             * percentage
             */
            PERCENTAGE,
        };

        /**
         * Progress constructor
         * @param min_ minimum value
         * @param cur_ starting value
         * @param step_ step in the progress
         * @param max_ maximum value
         * @param type progress type
         */
        Progress(float min_, float cur_, float step_, float max_, ProgressType type);

        /**
         * increases the state of the progress
         */
        void increase();

        /**
         * increases the state of the progress with step_
         * @param step_ the value to increase the state with
         */
        void increase(float step_);

        /**
         * sets the current state to cur_
         * @param cur_ the value to set the progress bar to
         */
        void set(float cur_);

        /**
         * prints the progress to standard output
         */
        void print();

        /**
         * current state of the progress
         */
        float current;

        /**
         * step of the progress
         */
        float step;

        /**
         * maximum value
         */
        float max;

        /**
         * minimum value
         */
        float min;

        /**
         * progress type
         */
        enum ProgressType type;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param p Progress object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const Progress& p);
}

#endif // PROGRESS_H_INCLUDED
