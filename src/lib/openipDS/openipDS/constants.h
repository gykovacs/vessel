/**
 * @file constants.h
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
 * The constants of openipDS are defined in this file.
 */

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

namespace openip
{
    /** filter mode: weighted sum*/
    #define FILTER_MODE_WEIGHTED_SUM 1
    /** filter mode: matching*/
    #define FILTER_MODE_MATCH 2
    /** filter mode: distance*/
    #define FILTER_MODE_DISTANCE 4
    /** filter mode: correlation*/
    #define FILTER_MODE_CORRELATION 8
    /** filter mode: other*/
    #define FILTER_MODE_OWN 16

    /** filter set mode: maximum weighted sum*/
    #define FILTER_SET_MODE_MAX_WEIGHTED_SUM 1
    /** filter set mode: minimum weighted sum*/
    #define FILTER_SET_MODE_MIN_WEIGHTED_SUM 2
    /** filter set mode: maximum match*/
    #define FILTER_SET_MODE_MAX_MATCH 3
    /** filter set mode: minimum match*/
    #define FILTER_SET_MODE_MIN_MATCH 4
    /** filter set mode: maximum distance*/
    #define FILTER_SET_MODE_MAX_DISTANCE 5
    /** filter set mode: minimum distance*/
    #define FILTER_SET_MODE_MIN_DISTANCE 6
    /** filter set mode: maximum correlation*/
    #define FILTER_SET_MODE_MAX_CORRELATION 7
    /** filter set mode: minimum correlation*/
    #define FILTER_SET_MODE_MIN_CORRELATION 8
    /** filter set mode: maximum own*/
    #define FILTER_SET_MODE_MAX_OWN 9
    /** filter set mode: minimum own*/
    #define FILTER_SET_MODE_MIN_OWN 10

    /** filter system mode: maximum weighted sum*/
    #define FILTER_SYSTEM_MODE_MAX_WEIGHTED_SUM 1
    /** filter system mode: minimum weighted sum*/
    #define FILTER_SYSTEM_MODE_MIN_WEIGHTED_SUM 2
    
    /** filter system mode: maximum match*/
    #define FILTER_SYSTEM_MODE_MAX_MATCH 3
    /** filter system mode: minimum match*/
    #define FILTER_SYSTEM_MODE_MIN_MATCH 4
    /** filter system mode: maximum distance*/
    #define FILTER_SYSTEM_MODE_MAX_DISTANCE 5
    /** filter system mode: minimum distance*/
    #define FILTER_SYSTEM_MODE_MIN_DISTANCE 6
    /** filter system mode maximum correlation*/
    #define FILTER_SYSTEM_MODE_MAX_CORRELATION 7
    /** filter system mode minimum correlation*/
    #define FILTER_SYSTEM_MODE_MIN_CORRELATION 8
    /** filter system mode maximum own*/
    #define FILTER_SYSTEM_MODE_MAX_OWN 9
    /** filter system mode minimum own*/
    #define FILTER_SYSTEM_MODE_MIN_OWN 10

    /**
     * set border pixels to zero
     */
    #define BORDER_MODE_ZERO 1

    /**
     * mirror the image to the border
     */
    #define BORDER_MODE_MIRRORED 2

    /**
     * set the image to be periodic
     */
    #define BORDER_MODE_PERIODIC 4

    #define BORDER_MODE_ONES 16

    #define BORDER_MODE_255 8
    
    /**
     * set image size to optimal for Fourier-transform
     */
    #define BORDER_MODE_FOURIER_OPTIMAL 32
    
    #define BORDER_MODE_LASTREPEATED 64

    /**1 unsigned char typed object*/
    #define CACHE_UCHAR_1 0x0000000000000001llu

    /**1 short typed object*/
    #define CACHE_SHORT_1 0x0000000000000100llu

    /**1 int typed object*/
    #define CACHE_INT_1 0x0000000000010000llu

    /**1 float typed object*/
    #define CACHE_FLOAT_1 0x0000000001000000llu

    /**1 double typed object*/
    #define CACHE_DOUBLE_1 0x0000000100000000llu
    
    /**1 complex float typed object*/
    #define CACHE_CFLOAT_1 0x0000010000000000llu

    /**1 complex double typed object*/
    #define CACHE_CDOUBLE_1 0x0001000000000000llu

    /**number of unsigned char typed object mask*/
    #define CACHE_UCHAR_MASK 0x00000000000000FFllu
    /**number of int typed object mask*/
    #define CACHE_SHORT_MASK 0x000000000000FF00llu
    /**number of float typed object mask*/
    #define CACHE_INT_MASK 0x0000000000FF0000llu
    /**number of double typed object mask*/
    #define CACHE_FLOAT_MASK 0x00000000FF000000llu
    /**number of complex float typed object mask*/
    #define CACHE_DOUBLE_MASK 0x000000FF00000000llu
    /**number of complex double typed object mask*/
    #define CACHE_CFLOAT_MASK 0x0000FF0000000000llu
    /**number of complex double typed object*/
    #define CACHE_CDOUBLE_MASK 0x00FF000000000000llu

    /**
     * usual FOREGROUND color
     */
    #define FOREGROUND 255

    /**
     * usual BACKGROUND color
     */
    #define BACKGROUND 0

    /**
     * XXX what the f*ck is this?
     */
    #define BOOSTBLURFACTOR 90.0

    /**
    * nth POWER of 2
    */
    #define POWER(n) (1<<n)

    /**
     * 0th power of 2
     */
    #define POWER0 POWER(0)

    /**
     * 1st power of 2
     */
    #define POWER1 POWER(1)

    /**
     * 2nd power of 2
     */
    #define POWER2 POWER(2)

    /**
     * 3rd power of 2
     */
    #define POWER3 POWER(3)

    /**
     * 4th power of 2
     */
    #define POWER4 POWER(4)

    /**
     * 5th power of 2
     */
    #define POWER5 POWER(5)

    /**
     * 6th power of 2
     */
    #define POWER6 POWER(6)

    /**
     * 7th power of 2
     */
    #define POWER7 POWER(7)

    /**
     * 8th power of 2
     */
    #define POWER8 POWER(8)

    /**
     * 9th power of 2
     */
    #define POWER9 POWER(9)

    /**
     * 10th power of 2
     */
    #define POWER10 POWER(10)

    /**
     * 11th power of 2
     */
    #define POWER11 POWER(11)

    /**
     * 12th power of 2
     */
    #define POWER12 POWER(12)

    /**
     * 13th power of 2
     */
    #define POWER13 POWER(13)

    /**
     * 14th power of 2
     */
    #define POWER14 POWER(14)

    /**
     * 15th power of 2
     */
    #define POWER15 POWER(15)

    /**
     * 16th power of 2
     */
    #define POWER16 POWER(16)

    /**
     * 17th power of 2
     */
    #define POWER17 POWER(17)

    /**
     * 18th power of 2
     */
    #define POWER18 POWER(18)

    /**
     * 19th power of 2
     */
    #define POWER19 POWER(19)

    /**
     * 20th power of 2
     */
    #define POWER20 POWER(20)

    /**
     * 21th power of 2
     */
    #define POWER21 POWER(21)

    /**
     * 22th power of 2
     */
    #define POWER22 POWER(22)

    /**
     * 23th power of 2
     */
    #define POWER23 POWER(23)

    /**
     * 24th power of 2
     */
    #define POWER24 POWER(24)

    /**
     * 25th power of 2
     */
    #define POWER25 POWER(25)

    /**
     * 26th power of 2
     */
    #define POWER26 POWER(26)

    /**
     * 27th power of 2
     */
    #define POWER27 POWER(27)

    /**
     * 28th power of 2
     */
    #define POWER28 POWER(28)

    /**
     * 29th power of 2
     */
    #define POWER29 POWER(29)

    /**
     * 30th power of 2
     */
    #define POWER30 POWER(30)

    /**
     * 31th power of 2
     */
    #define POWER31 POWER(31)
}

#endif
