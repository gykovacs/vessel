/**
 * @file VectorFeature2s.h
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
 * This file contains some simple vector features.
 */

#ifndef FEATURE2VECTORS_H
#define	FEATURE2VECTORS_H

#include <vector>

#include <openipDS/Feature2Generator.h>
#include <openipDS/VectorFeature2.h>
#include <openipDS/StructuringElement2s.h>

namespace openip
{
    /**
     * Identical feature
     */
    template<typename INPUT, typename OUTPUT>
    class IdenticalVectorFeature2: public VectorTransform2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using VectorTransform2<INPUT, OUTPUT>::descriptor;
        /**
         * constructor
         * @param radius radius of the feature
         */
        IdenticalVectorFeature2(int radius);

        /**
         * copy constructor
         * @param i instance to copy
         */
        IdenticalVectorFeature2(const IdenticalVectorFeature2& i);

        /**
         * destructor
         */
        ~IdenticalVectorFeature2();

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n row-major position
         * @param output output vector
         */
        virtual void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int dimensions();
    };

    template<typename INPUT, typename OUTPUT>
    IdenticalVectorFeature2<INPUT, OUTPUT>::IdenticalVectorFeature2(int radius)
    : VectorTransform2<INPUT, OUTPUT>(), StructuringElementDisk(radius)
    {
        std::stringstream ss;
        ss << "IdenticalVectorFeature2 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    IdenticalVectorFeature2<INPUT, OUTPUT>::IdenticalVectorFeature2(const IdenticalVectorFeature2& i)
    : VectorTransform2<INPUT, OUTPUT>(i), StructuringElementDisk(i)
    {
    }

    template<typename INPUT, typename OUTPUT>
    IdenticalVectorFeature2<INPUT, OUTPUT>::~IdenticalVectorFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void IdenticalVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>&, int, Vector<OUTPUT>&)
    {
        /*output.resize(this->size);
        this->updateStride(input.columns);
        this->computeMinMax();

        if ( n < -this->min || n > input.n - this->max )
            output= 0;

        for ( int i= -this->min; i < input.n - this->max; ++i )
        {
            output(i + this->min)= input(n + i);
        }*/
    }

    template<typename INPUT, typename OUTPUT>
    void IdenticalVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>*, Image<unsigned char>*)
    {
        this->updateStride(input.columns);


        PixelSet1::iterator pit;
        //printf("%s %d %d %d %d\n", this->descriptor.c_str(), this->getMin(), input.n, this->getMax(), this->size()); fflush(stdout);
        int k;
#pragma omp parallel for private(k,pit)
        for ( int i= -int(this->getMin()); i < int(input.n) - int(this->getMax()); ++i )
        {
            k= n;
            for ( pit= this->begin(); pit != this->end(); ++pit )
            {
                output(k)(i)= input(i + *pit);
                ++k;
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    Border2 IdenticalVectorFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int w= this->getRows() / 2 + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    int IdenticalVectorFeature2<INPUT, OUTPUT>::dimensions()
    {
        return this->size();
    }



    template<typename INPUT, typename OUTPUT>
    class LocalBinaryPatternVectorFeature2: public VectorTransform2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using VectorTransform2<INPUT, OUTPUT>::descriptor;
        /**
         * constructor
         * @param radius radius of the feature
         */
        LocalBinaryPatternVectorFeature2(int radius);

        /**
         * copy constructor
         * @param i instance to copy
         */
        LocalBinaryPatternVectorFeature2(const LocalBinaryPatternVectorFeature2& i);

        /**
         * destructor
         */
        ~LocalBinaryPatternVectorFeature2();

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n row-major position
         * @param output output vector
         */
        virtual void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int dimensions();
    };

    template<typename INPUT, typename OUTPUT>
    LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::LocalBinaryPatternVectorFeature2(int radius)
    : VectorTransform2<INPUT, OUTPUT>(), StructuringElementDisk(radius)
    {
        std::stringstream ss;
        ss << "LocalBinaryPatternVectorFeature2 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::LocalBinaryPatternVectorFeature2(const LocalBinaryPatternVectorFeature2& i)
    : VectorTransform2<INPUT, OUTPUT>(i), StructuringElementDisk(i)
    {
    }

    template<typename INPUT, typename OUTPUT>
    LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::~LocalBinaryPatternVectorFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>&, int, Vector<OUTPUT>&)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>*, Image<unsigned char>*)
    {
        this->updateStride(input.columns);

        PixelSet1::iterator pit;

        int k;
        double sum;
        #pragma omp parallel for private(k,pit)
        for ( int i= -int(this->getMin()); i < int(input.n) - int(this->getMax()); ++i )
        {
            sum= 0;
            for ( pit= this->begin(); pit != this->end(); ++pit )
            {
                sum+= input(i + *pit);
            }
            sum/= this->size();

            k= n;
            for ( pit= this->begin(); pit != this->end(); ++pit )
            {
                if ( input(i + *pit) > sum )
                    output(k)(i)= 1;
                else
                    output(k)(i)= -1;
                ++k;
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    Border2 LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int w= this->getRows() / 2 + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    int LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>::dimensions()
    {
        return this->size();
    }
















    /**
     * Difference from center intensity feature
     * @param radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceFromCenterVectorFeature2: public VectorFeature2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        /**
         * constructor
         * @param radius radius of the feature
         */
        DifferenceFromCenterVectorFeature2(int radius);

        /**
         * copy constructor
         * @param i instance to copy
         */
        DifferenceFromCenterVectorFeature2(const DifferenceFromCenterVectorFeature2& i);

        /**
         * destructor
         */
        ~DifferenceFromCenterVectorFeature2();

        /**
         * computes the feature in a given row-major position of the image
         * @param input input image
         * @param n row-major position
         * @param output vector
         */
        void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output);
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceFromCenterVectorFeature2<INPUT, OUTPUT>::DifferenceFromCenterVectorFeature2(int radius)
    : StructuringElementDisk(radius)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFromCenterVectorFeature2<INPUT, OUTPUT>::DifferenceFromCenterVectorFeature2(const DifferenceFromCenterVectorFeature2& i)
    : VectorFeature2<INPUT, OUTPUT>(i), StructuringElementDisk(i)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFromCenterVectorFeature2<INPUT, OUTPUT>::~DifferenceFromCenterVectorFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void DifferenceFromCenterVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output)
    {
        output.resize(this->size());
        this->updateStride(input.columns);
        this->computeMinMax();

        if ( n < -this->min || n > input.n - this->max )
            output= 0;
        else
        {
            for ( int i= 0; i < this->size(); ++i )
            {
                output(i)= input(n) - input(n + (*this)(i));
            }
        }
    }
}

#endif	/* FEATURE2VECTORS_H */

