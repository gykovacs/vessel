/* 
 * File:   Transform2s.h
 * Author: gykovacs
 *
 * Created on February 10, 2011, 9:15 AM
 */

#ifndef TRANSFORM2S_H
#define	TRANSFORM2S_H

#include <openipDS/Transform2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class Normalization2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         */
        Normalization2(OUTPUT a, OUTPUT b);

        /**
         * copy constructor
         * @param f instance to copy
         */
        Normalization2(const Normalization2<INPUT, OUTPUT>& f);

        /**
         * destructor
         */
        ~Normalization2();

	/**
	* extracts the feature in all positions of the image input into image output in the foreground (non 0) region of the mask
	* @param input input image
	* @param output output image
	* @param mask the operation is performed in the foreground region
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

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

        OUTPUT a;
        OUTPUT b;
    };

    template<typename INPUT, typename OUTPUT>
    Normalization2<INPUT, OUTPUT>::Normalization2(OUTPUT a, OUTPUT b)
    : Transform2<INPUT, OUTPUT>()
    {
        this->a= a;
        this->b= b;

        std::stringstream ss;
        ss << "Normalization2 " << a << " " << b;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    Normalization2<INPUT, OUTPUT>::Normalization2(const Normalization2& n)
    : Transform2<INPUT, OUTPUT>(n)
    {

    }

    template<typename INPUT, typename OUTPUT>
    Normalization2<INPUT, OUTPUT>::~Normalization2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Normalization2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);
        for ( unsigned int i= 0; i < input.size(); ++i )
        {
            output(i)= (OUTPUT)(float(float(input(i)) - min)/float(max - min)*(b - a) + a);
        }
    }

    template<typename INPUT, typename OUTPUT>
    int Normalization2<INPUT, OUTPUT>::getMin()
    {
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    int Normalization2<INPUT, OUTPUT>::getMax()
    {
        return 0;
    }
}

#endif	/* TRANSFORM2S_H */

