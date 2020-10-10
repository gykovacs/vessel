/**
 * @file DigitalAlphabet.h
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
 * The DigitalAlphabet class represents a the letters and digits of
 * the english alphabet. Using this class it is able to print texts on Images
 * using the methods in writing.h
 */

#ifndef _DIGITAL_ALPHABET_H_
#define _DIGITAL_ALPHABET_H_

#include <vector>
#include <iostream>

namespace openip
{
    /**
     * DigitalAlphabet contains binary patterns of letters and digits. The
     * signs can be written on images using the methods of writing.h
     */
    class DigitalAlphabet
    {
    public:
        /**
         * default constructor
         */
        DigitalAlphabet();

        /**
         * initializes the binary pattern of the letter p with
         * the pattern in the string str
         * @param p letter vector
         * @param str row-continuous binary pattern
         */
        void init(std::vector<unsigned char>& p, const char* str);

        /**
         * width of letters
         */
        unsigned int width;
        /**
         * height of letters
         */
        unsigned int height;

        /**binary pattern for letter a*/
        std::vector<unsigned char> a;
        /**binary pattern for letter b*/
        std::vector<unsigned char> b;
        /**binary pattern for letter c*/
        std::vector<unsigned char> c;
        /**binary pattern for letter d*/
        std::vector<unsigned char> d;
        /**binary pattern for letter e*/
        std::vector<unsigned char> e;
        /**binary pattern for letter f*/
        std::vector<unsigned char> f;
        /**binary pattern for letter g*/
        std::vector<unsigned char> g;
        /**binary pattern for letter h*/
        std::vector<unsigned char> h;
        /**binary pattern for letter i*/
        std::vector<unsigned char> i;
        /**binary pattern for letter j*/
        std::vector<unsigned char> j;
        /**binary pattern for letter k*/
        std::vector<unsigned char> k;
        /**binary pattern for letter l*/
        std::vector<unsigned char> l;
        /**binary pattern for letter m*/
        std::vector<unsigned char> m;
        /**binary pattern for letter n*/
        std::vector<unsigned char> n;
        /**binary pattern for letter o*/
        std::vector<unsigned char> o;
        /**binary pattern for letter p*/
        std::vector<unsigned char> p;
        /**binary pattern for letter q*/
        std::vector<unsigned char> q;
        /**binary pattern for letter r*/
        std::vector<unsigned char> r;
        /**binary pattern for letter s*/
        std::vector<unsigned char> s;
        /**binary pattern for letter t*/
        std::vector<unsigned char> t;
        /**binary pattern for letter u*/
        std::vector<unsigned char> u;
        /**binary pattern for letter v*/
        std::vector<unsigned char> v;
        /**binary pattern for letter w*/
        std::vector<unsigned char> w;
        /**binary pattern for letter x*/
        std::vector<unsigned char> x;
        /**binary pattern for letter y*/
        std::vector<unsigned char> y;
        /**binary pattern for letter z*/
        std::vector<unsigned char> z;

        /**binary pattern for digit 0*/
        std::vector<unsigned char> d0;
        /**binary pattern for digit 1*/
        std::vector<unsigned char> d1;
        /**binary pattern for digit 2*/
        std::vector<unsigned char> d2;
        /**binary pattern for digit 3*/
        std::vector<unsigned char> d3;
        /**binary pattern for digit 4*/
        std::vector<unsigned char> d4;
        /**binary pattern for digit 5*/
        std::vector<unsigned char> d5;
        /**binary pattern for digit 6*/
        std::vector<unsigned char> d6;
        /**binary pattern for digit 7*/
        std::vector<unsigned char> d7;
        /**binary pattern for digit 8*/
        std::vector<unsigned char> d8;
        /**binary pattern for digit 9*/
        std::vector<unsigned char> d9;

        /**binary pattern for comma*/
        std::vector<unsigned char> sc;
        /**binary pattern for dot*/
        std::vector<unsigned char> sd;
        /**binary pattern for semicolon*/
        std::vector<unsigned char> ss;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param d DigitalAlphabet object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    std::ostream& operator<<(std::ostream& o, const DigitalAlphabet& d);
}

#endif
