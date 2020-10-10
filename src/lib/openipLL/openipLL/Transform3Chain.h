/* 
 * File:   Transform3Chain.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 4:18 PM
 */

#ifndef TRANSFORM3CHAIN_H
#define	TRANSFORM3CHAIN_H

#include <openipLL/Transform3Set.h>

namespace openip
{
    /**
     * Transform2Chain represents a chain of image transforms.
     */
    template<typename INPUT, typename OUTPUT>
    class Transform3Chain: public Transform3Set<INPUT, OUTPUT>, public Transform3<INPUT, OUTPUT>
    {
    public:
        //using Transform2Set<INPUT, OUTPUT>::add;

        /**
         * default constructor
         */
        Transform3Chain();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Transform3Chain(const Transform3Chain& f);

        /**
         * destructor
         */
        ~Transform3Chain();

        /**
         * using the proposed border, the each transform can be applied to each position of an image
         * @return the proposed border
         */
        virtual Border3 getProposedBorder();

        /**
         * applies the transform chain
         * @param input input image
         * @param output output image
         * @param mask the transforms are applied only in the foreground (non-zero) region of the mask
         * @param roi the transforms use only the foreground (non-zero) region of the roi
         */
        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        /**
         * vector of transforms
         */
        Vector<VectorTransform3<OUTPUT, OUTPUT>* > tvoo;

        Volume<OUTPUT> tmp;

        virtual std::string getDescriptor(char separator=' ', int i= 0);

        Transform3Chain<INPUT, OUTPUT>& operator=(const Transform3Chain<INPUT, OUTPUT>& t);
    };

    template<typename INPUT, typename OUTPUT>
    Transform3Chain<INPUT, OUTPUT>::Transform3Chain()
    : Transform3Set<INPUT, OUTPUT>(), Transform3<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform3Chain<INPUT, OUTPUT>::Transform3Chain(const Transform3Chain& f)
    : Transform3Set<INPUT, OUTPUT>(f), Transform3<INPUT, OUTPUT>(f), tvoo(f.tvoo)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform3Chain<INPUT, OUTPUT>::~Transform3Chain()
    {
        for ( unsigned int i= 0; i < tvoo.size(); ++i )
            delete tvoo[i];
        tvoo.clear();
    }

    template<typename INPUT, typename OUTPUT>
    Transform3Chain<INPUT, OUTPUT>& Transform3Chain<INPUT, OUTPUT>::operator=(const Transform3Chain<INPUT, OUTPUT>& t)
    {
        this->tvoo= t.tvoo;
        return *this;
    }

    template<typename INPUT, typename OUTPUT>
    void Transform3Chain<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        tmp.resizeVolume(input);
        tmp= input;

        for ( unsigned int i= 0; i < tvoo.size(); ++i )
        {
            output= 0;
            tvoo(i)->apply(tmp, output, roi, support);
            if (  i == tvoo.size() - 1 )
                return;
            tmp= 0;
            tvoo(i+1)->apply(output, tmp, roi, support);
            ++i;
        }
        output= tmp;
    }

    template<typename INPUT, typename OUTPUT>
    Border3 Transform3Chain<INPUT, OUTPUT>::getProposedBorder()
    {
        Border3 b;
        
        if ( tvoo.size() > 0 )
                b= tvoo(0)->getProposedBorder();
        
        for ( unsigned int i= 1; i < tvoo.size(); ++i )
        {
            b= Border3(b, tvoo(i)->getProposedBorder());
        }
        
        return b;
    }

    template<typename INPUT, typename OUTPUT>
    std::string Transform3Chain<INPUT, OUTPUT>::getDescriptor(char separator, int)
    {
        stringstream ss;
        for ( unsigned int i= 0; i < this->tvoo.size(); ++i )
        {
            ss << "chain";
            ss << separator;
            ss << (*this).tvoo(i)->getDescriptor(separator);
            ss << separator;
        }
        return ss.str();
    }
}

#endif	/* TRANSFORM3CHAIN_H */

