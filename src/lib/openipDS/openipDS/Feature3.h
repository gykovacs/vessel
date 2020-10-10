/* 
 * File:   Feature3.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 8:20 AM
 */

#ifndef FEATURE3_H
#define	FEATURE3_H

#include <openipDS/Volume.h>
#include <openipDS/Transform3.h>

namespace openip
{
    /**
    * Feature2 represents a feature extractable from images
    */
    template<typename INPUT, typename OUTPUT>
    class Feature3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::apply;
        using Transform3<INPUT, OUTPUT>::descriptor;
        using Transform3<INPUT, OUTPUT>::getProposedBorder;
#ifdef USE_OPENCL
        using Transform3<INPUT, OUTPUT>::enqueueKernel;
        using Transform3<INPUT, OUTPUT>::getOpenCLKernel;
        using Transform3<INPUT, OUTPUT>::applyGPU;
#endif
        /**
         * constructor
         */
        Feature3();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Feature3(const Feature3<INPUT, OUTPUT>& f);

        /**
         * destructor
         */
        virtual ~Feature3();

	/**
	* extracts the feature from image input at position n
	* @param input input image
	* @param n position
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual OUTPUT apply(Volume<INPUT>& input, int n, Volume<unsigned char>* support= NULL);

	/**
	* extracts the feature in all positions of the image input into image output in the foreground (non 0) region of the mask
	* @param input input image
	* @param output output image
	* @param mask the operation is performed in the foreground region
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride, int slicePitch);

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
    int Feature3<INPUT, OUTPUT>::getMin()
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int Feature3<INPUT, OUTPUT>::getMax()
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    Feature3<INPUT, OUTPUT>::Feature3()
    : Transform3<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Feature3<INPUT, OUTPUT>::Feature3(const Feature3<INPUT, OUTPUT>& f)
    : Transform3<INPUT, OUTPUT>(f)
    {
        this->descriptor= f.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    Feature3<INPUT, OUTPUT>::~Feature3()
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
    std::ostream& operator<<(std::ostream& o, const Feature3<INPUT, OUTPUT>& f)
    {
        o << f.descriptor;

        return o;
    }

    template<typename INPUT, typename OUTPUT>
    void Feature3<INPUT, OUTPUT>::computeMinMax()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int Feature3<INPUT, OUTPUT>::updateStride(int, int)
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT Feature3<INPUT, OUTPUT>::apply(Volume<INPUT>&,  int, Volume<unsigned char>*)
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void Feature3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        this->updateStride(input.columns, input.sliceSize);
        this->computeMinMax();

        int begin= -(int)(this->getMin());
        int end= input.n - (int)(this->getMax());

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= begin; i < end; ++i )
            {
                output(i)= apply(input, i, support);
            }
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

#endif	/* FEATURE3_H */

