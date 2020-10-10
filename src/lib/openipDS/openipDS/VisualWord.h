/**
 * @file VisualWord.h
 * @author Janos Istvan Ivan <dzsekob@gmail.com>
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
 * VisualWord is a small image part, extracted from some images. It can be
 * used in matched filters.
 */

#ifndef VISUALWORD_H
#define	VISUALWORD_H

#include <openipDS/Filter2.h>
#include <openipDS/StructuringElement2s.h>

namespace openip
{
    /**
     * VisualWord is a small image part, extracted from some images. It can be
     * used in matched filters.
     */
    template<typename INPUT, typename OUTPUT>
    class VisualWordSquare: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::addElement;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
#ifdef USE_OPENCL
        using Filter2<INPUT, OUTPUT, float>::getOpenCLKernel;
#endif        
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::getRows;
        using Filter2<INPUT, OUTPUT, float>::getColumns;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::rotate90;
        using Filter2<INPUT, OUTPUT, float>::rotate180;
        using Filter2<INPUT, OUTPUT, float>::rotate270;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::resize;

        /**
         * constructor
         * @param s descriptor string (filename)
         */
        VisualWordSquare(std::string& s);

        /**
         * constructor, extracts visual word from image
         * @param image image to extract the visual word from
         * @param r row coordinate
         * @param c column coordinate
         * @param rows number of rows in the visual word
         * @param columns number of columns in the visual word
         */
        template<typename T>
        VisualWordSquare(Image<T>& image, int r, int c, int rows, int columns);

        /**
         * copy constructor
         * @param vw instance to copy
         */
        VisualWordSquare(const VisualWordSquare& vw);

        /**
         * destructor
         */
        ~VisualWordSquare();
    };

    template<typename INPUT, typename OUTPUT>
    class VisualWordDisk: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::addElement;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::dumpFilter;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
#ifdef USE_OPENCL        
        using Filter2<INPUT, OUTPUT, float>::getOpenCLKernel;
#endif        
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using Filter2<INPUT, OUTPUT, float>::getRows;
        using Filter2<INPUT, OUTPUT, float>::getColumns;
        using Filter2<INPUT, OUTPUT, float>::getProposedBorder;
        using Filter2<INPUT, OUTPUT, float>::rotate90;
        using Filter2<INPUT, OUTPUT, float>::rotate180;
        using Filter2<INPUT, OUTPUT, float>::rotate270;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::resize;

        /**
         * constructor
         * @param s descriptor string (filename)
         */
        VisualWordDisk(std::string& s);

        /**
         * constructor, extracts visual word from image
         * @param image image to extract the visual word from
         * @param r row coordinate
         * @param c column coordinate
         * @param rows number of rows in the visual word
         * @param columns number of columns in the visual word
         */
        template<typename T>
        VisualWordDisk(Image<T>& image, int r, int c, int radius);

        /**
         * copy constructor
         * @param vw instance to copy
         */
        VisualWordDisk(const VisualWordDisk& vw);

        /**
         * destructor
         */
        ~VisualWordDisk();
    };

    template<typename INPUT, typename OUTPUT> template<typename T>
    VisualWordSquare<INPUT, OUTPUT>::VisualWordSquare(Image<T>& image, int r, int c, int rows, int columns)
    : Filter2<INPUT, OUTPUT, float>()
    {
        int r2, c2;
        r2= rows/2;
        c2= columns/2;
        for ( int i= -r2; i <= r2; ++i )
            for ( int j= -c2; j <= c2; ++j )
                this->push_back(PositionWeightPair<float>(i*image.columns + j, image(r + i, c + j)));
        this->stride= image.columns;
        this->normalize();
        this->zeroMean();
        this->computeMinMax();

        stringstream ss;
        ss << "VisualWordSquare ";
        ss << this->stride;
        ss << " ";
        ss << this->size();
        ss << " ";
        for ( unsigned int i= 0; i < this->size(); ++i )
            ss << (*this)(i).first << " " << (*this)(i).second << " ";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSquare<INPUT, OUTPUT>::VisualWordSquare(std::string& s)
    : Filter2<INPUT, OUTPUT, float>()
    {
        if ( s.substr(0, 16).compare(string("VisualWordSquare")) == 0 )
        {
            std::string tmp= s.substr(16);
            stringstream ss(tmp);
            int size;
            ss >> this->stride;
            ss >> size;
            this->resize(size);
            for ( int i= 0; i < size; ++i )
            {
                ss >> (*this)(i).first;
                ss >> (*this)(i).second;
            }
        }
        this->computeMinMax();
        this->descriptor= s;
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSquare<INPUT, OUTPUT>::VisualWordSquare(const VisualWordSquare& v)
    : Filter2<INPUT, OUTPUT, float>(v)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSquare<INPUT, OUTPUT>::~VisualWordSquare()
    {
    }

    template<typename INPUT, typename OUTPUT> template<typename T>
    VisualWordDisk<INPUT, OUTPUT>::VisualWordDisk(Image<T>& image, int r, int c, int radius)
    : Filter2<INPUT, OUTPUT, float>()
    {
        StructuringElementDisk se(radius);
        se.updateStride(image.columns);

        int mina= 0, minb= 0;
        for ( int a= -4; a <= 4; ++a )
            for ( int b= -4; b<= 4; ++b )
            {
                if ( image(r + a, c + b) < image(r + mina, c + minb) )
                {
                    mina= a;
                    minb= b;
                }
            }

        for ( StructuringElementDisk::iterator it= se.begin(); it != se.end(); ++it )
        {
            this->push_back(PositionWeightPair<float> (*it, image((r + mina) * image.columns + (c + minb) + *it)));
        }

        this->stride= image.columns;
        stringstream ss;
        ss << "VisualWordDisk ";
        ss << this->stride;
        ss << " ";
        ss << this->size();
        ss << " ";
        for ( unsigned int i= 0; i < this->size(); ++i )
            ss << (*this)(i).first << " " << (*this)(i).second << " ";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordDisk<INPUT, OUTPUT>::VisualWordDisk(std::string& s)
    : Filter2<INPUT, OUTPUT, float>()
    {
        if ( s.substr(0, 14).compare(string("VisualWordDisk")) == 0 )
        {
            std::string tmp= s.substr(15);
            stringstream ss(tmp);
            int size;
            int stride;
            ss >> stride;
            ss >> size;
            this->stride= stride;
            this->resize(size);
            for ( int i= 0; i < size; ++i )
            {
                ss >> (*this)(i).first;
                ss >> (*this)(i).second;
            }
        }
        this->computeMinMax();
        this->descriptor= s;
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordDisk<INPUT, OUTPUT>::VisualWordDisk(const VisualWordDisk& v)
    : Filter2<INPUT, OUTPUT, float>(v)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordDisk<INPUT, OUTPUT>::~VisualWordDisk()
    {
    }
}

#endif	/* VISUALWORD_H */

