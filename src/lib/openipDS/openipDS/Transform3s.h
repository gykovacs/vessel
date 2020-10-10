/* 
 * File:   Transform3s.h
 * Author: gykovacs
 *
 * Created on July 20, 2011, 5:09 PM
 */

#ifndef TRANSFORM3S_H
#define	TRANSFORM3S_H

#include <openipDS/Transform3.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class NormalizationTransform3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::apply;
        using Transform3<INPUT, OUTPUT>::descriptor;
        using Transform3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         */
        NormalizationTransform3(OUTPUT a= 0, OUTPUT b= 1);

        /**
         * copy constructor
         * @param f instance to copy
         */
        NormalizationTransform3(const NormalizationTransform3<INPUT, OUTPUT>& f);

        /**
         * destructor
         */
        ~NormalizationTransform3();

	/**
	* extracts the feature in all positions of the image input into image output in the foreground (non 0) region of the mask
	* @param input input image
	* @param output output image
	* @param mask the operation is performed in the foreground region
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

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
    NormalizationTransform3<INPUT, OUTPUT>::NormalizationTransform3(OUTPUT a, OUTPUT b)
    : Transform3<INPUT, OUTPUT>()
    {
        this->a= a;
        this->b= b;
        stringstream ss;
        ss << "NormalizationTransform3 " << a << " " << b;
        descriptor= ss.str();
    }
    
    template<typename INPUT, typename OUTPUT>
    NormalizationTransform3<INPUT, OUTPUT>::NormalizationTransform3(const NormalizationTransform3<INPUT, OUTPUT>& n)
    : Transform3<INPUT, OUTPUT>(n)
    {
        descriptor= n.descriptor;
        this->a= n.a;
        this->b= n.b;
    }
    
    template<typename INPUT, typename OUTPUT>
    NormalizationTransform3<INPUT, OUTPUT>::~NormalizationTransform3()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void NormalizationTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
    {
        INPUT min= input(0), max= input(0);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( input(i) < min )
                min= input(i);
            if ( input(i) > max )
                max= input(i);
        }
        
        if ( min == max )
        {
            output= a;
            return;
        }
        
        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= (input(i) - min)/float(max - min) * (b - a) + a;
        
        min= output(0); max= output(0);
        for ( unsigned int i= 0; i < output.n; ++i )
        {
            if ( output(i) < min )
                min= output(i);
            if ( output(i) > max )
                max= output(i);
        }
        
        return;
    }
    
    template<typename INPUT, typename OUTPUT>
    int NormalizationTransform3<INPUT, OUTPUT>::getMin()
    {
        return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    int NormalizationTransform3<INPUT, OUTPUT>::getMax()
    {
        return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    class IdenticalTransform3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::apply;
        using Transform3<INPUT, OUTPUT>::descriptor;
        using Transform3<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         */
        IdenticalTransform3();

        /**
         * copy constructor
         * @param f instance to copy
         */
        IdenticalTransform3(const IdenticalTransform3<INPUT, OUTPUT>& f);

        /**
         * destructor
         */
        ~IdenticalTransform3();

	/**
	* extracts the feature in all positions of the image input into image output in the foreground (non 0) region of the mask
	* @param input input image
	* @param output output image
	* @param mask the operation is performed in the foreground region
        * @param roi if not NULL, only the foreground (non-zero) region of the roi image is used
	*/
        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

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
    IdenticalTransform3<INPUT, OUTPUT>::IdenticalTransform3()
    : Transform3<INPUT, OUTPUT>()
    {
        stringstream ss;
        ss << "IdenticalTransform3 ";
        descriptor= ss.str();
    }
    
    template<typename INPUT, typename OUTPUT>
    IdenticalTransform3<INPUT, OUTPUT>::IdenticalTransform3(const IdenticalTransform3<INPUT, OUTPUT>& n)
    : Transform3<INPUT, OUTPUT>(n)
    {
        descriptor= n.descriptor;
    }
    
    template<typename INPUT, typename OUTPUT>
    IdenticalTransform3<INPUT, OUTPUT>::~IdenticalTransform3()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void IdenticalTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* /*roi*/, Volume<unsigned char>* /*support*/)
    {
        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= input(i);
        
        return;
    }
    
    template<typename INPUT, typename OUTPUT>
    int IdenticalTransform3<INPUT, OUTPUT>::getMin()
    {
        return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    int IdenticalTransform3<INPUT, OUTPUT>::getMax()
    {
        return 0;
    }
    
    
}
#endif	/* TRANSFORM3S_H */

