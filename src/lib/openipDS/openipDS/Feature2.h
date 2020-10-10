/**
 * @file Feature2.h
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
 * The Feature2 class is the base class of all feature extractable
 * in a given pixel.
 */

#ifndef FEATURE2_H
#define	FEATURE2_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/Transform2.h>

namespace openip
{
    /**
    * Feature2 represents a feature extractable from images
    */
    template<typename INPUT, typename OUTPUT>
    class Feature2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;
#ifdef USE_OPENCL
        using Transform2<INPUT, OUTPUT>::enqueueKernel;
        using Transform2<INPUT, OUTPUT>::getOpenCLKernel;
        using Transform2<INPUT, OUTPUT>::applyGPU;
#endif
        /**
         * constructor
         */
        Feature2();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Feature2(const Feature2<INPUT, OUTPUT>& f);

        /**
         * destructor
         */
        virtual ~Feature2();

	/**
	* extracts the feature from image input at position n
	* @param input input image
	* @param n position
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

	/**
	* extracts the feature in all positions of the image input into image output in the foreground (non 0) region of the mask
	* @param input input image
	* @param output output image
	* @param mask the operation is performed in the foreground region
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * computes the minimum and maximum positions, in row-major representation, in which the feature can be computed
         */
        virtual void computeMinMax();

        /**
         * returns the minimum position, in row-major representation, in which the feature can be computed
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position, in row-major representation, in which the feature can be computed
         * @return
         */
        virtual int getMax();
    };

    template<typename INPUT, typename OUTPUT>
    int Feature2<INPUT, OUTPUT>::getMin()
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int Feature2<INPUT, OUTPUT>::getMax()
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    Feature2<INPUT, OUTPUT>::Feature2()
    : Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Feature2<INPUT, OUTPUT>::Feature2(const Feature2<INPUT, OUTPUT>& f)
    : Transform2<INPUT, OUTPUT>(f)
    {
        this->descriptor= f.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    Feature2<INPUT, OUTPUT>::~Feature2()
    {
    }

    /**
    * << operator
    *
    * @param o output stream object
    * @param f Feature2 object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const Feature2<INPUT, OUTPUT>& f)
    {
        o << f.descriptor;

        return o;
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2<INPUT, OUTPUT>::computeMinMax()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int Feature2<INPUT, OUTPUT>::updateStride(int)
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT Feature2<INPUT, OUTPUT>::apply(Image<INPUT>&,  int, Image<unsigned char>*)
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        this->computeMinMax();

        int begin= -(int)(this->getMin());
        int end= input.n - (int)(this->getMax());
        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                output(i)= apply(input, i, support);
        }
        else
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
                if ( (*roi)(i) > 0 )
                    output(i)= apply(input, i, support);
        }
    }
}


#endif	/* FEATURE2_H */
