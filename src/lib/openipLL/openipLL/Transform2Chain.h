/**
 * @file Transform2Chain.h
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
 * Transform2Chain represents a chain of image transforms.
 */

#ifndef TRANSFORMCHAIN_H
#define	TRANSFORMCHAIN_H

#include <openipLL/Transform2Set.h>
#include <openipLL/Scaling.h>

namespace openip
{
    /**
     * Transform2Chain represents a chain of image transforms.
     */
    template<typename INPUT, typename OUTPUT>
    class Transform2Chain: public Transform2Set<INPUT, OUTPUT>, public Transform2<INPUT, OUTPUT>
    {
    public:
        //using Transform2Set<INPUT, OUTPUT>::add;

        /**
         * default constructor
         */
        Transform2Chain();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Transform2Chain(const Transform2Chain& f);

        /**
         * destructor
         */
        ~Transform2Chain();

        /**
         * using the proposed border, the each transform can be applied to each position of an image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();

        /**
         * applies the transform chain
         * @param input input image
         * @param output output image
         * @param mask the transforms are applied only in the foreground (non-zero) region of the mask
         * @param roi the transforms use only the foreground (non-zero) region of the roi
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * vector of transforms
         */
        Vector<VectorTransform2<OUTPUT, OUTPUT>* > tvoo;

        Image<OUTPUT> tmp;
        Image<OUTPUT> tmp2;

        virtual std::string getDescriptor(char separator=' ', int i= 0);

        Transform2Chain<INPUT, OUTPUT>& operator=(const Transform2Chain<INPUT, OUTPUT>& t);
    };

    template<typename INPUT, typename OUTPUT>
    Transform2Chain<INPUT, OUTPUT>::Transform2Chain()
    : Transform2Set<INPUT, OUTPUT>(), Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2Chain<INPUT, OUTPUT>::Transform2Chain(const Transform2Chain& f)
    : Transform2Set<INPUT, OUTPUT>(f), Transform2<INPUT, OUTPUT>(f), tvoo(f.tvoo)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2Chain<INPUT, OUTPUT>::~Transform2Chain()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Transform2Chain<INPUT, OUTPUT>& Transform2Chain<INPUT, OUTPUT>::operator=(const Transform2Chain<INPUT, OUTPUT>& t)
    {
        this->tvoo= t.tvoo;
        return *this;
    }

    template<typename INPUT, typename OUTPUT>
    void Transform2Chain<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        tmp.resizeImage(input);
        tmp= input;

        for ( unsigned int i= 0; i < tvoo.size(); ++i )
        {
            output= 0;
            tvoo(i)->apply(tmp, output, roi, support);

            if ( tmp.rows != input.rows || tmp.columns != input.columns )
            {
                bilinearResize(input, tmp.rows, tmp.columns, tmp.topBorder, tmp.bottomBorder, tmp.leftBorder, tmp.rightBorder);
            }
            
            if (  i == tvoo.size() - 1 )
                return;
            
            tmp= 0;
            tvoo(i+1)->apply(output, tmp, roi, support);

            if ( tmp.rows != input.rows || tmp.columns != input.columns )
            {
                bilinearResize(input, tmp.rows, tmp.columns, tmp.topBorder, tmp.bottomBorder, tmp.leftBorder, tmp.rightBorder);
            }

            ++i;
        }
        output= tmp;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 Transform2Chain<INPUT, OUTPUT>::getProposedBorder()
    {
        int w= 0;
        for ( unsigned int i= 0; i < tvoo.size(); ++i )
            if ( tvoo(i)->getProposedBorder().topBorder > w )
                w= tvoo(i)->getProposedBorder().topBorder;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    std::string Transform2Chain<INPUT, OUTPUT>::getDescriptor(char separator, int)
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

#endif	/* TRANSFORMCHAIN_H */

