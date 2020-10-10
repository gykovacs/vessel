/* 
 * File:   RegionTransform2Set.h
 * Author: gykovacs
 *
 * Created on September 30, 2011, 4:06 PM
 */

#ifndef REGIONTRANSFORM2SET_H
#define	REGIONTRANSFORM2SET_H

#include <openipDS/RegionFeature2.h>

namespace openip
{
    template<typename INPUT>
    class RegionsTransform2Generator;

    /**
     * Transform2Set represents a set of image transforms.
     */
    template<typename INPUT>
    class RegionFeature2Set: public Vector<RegionFeature2<INPUT>* >
    {
    public:
        using Vector<RegionFeature2<INPUT>* >::push_back;
        using Vector<RegionFeature2<INPUT>* >::operator();

        /**
         * default constructor
         */
        RegionFeature2Set();

        /**
         * copy constructor
         * @param f instance to copy
         */
        RegionFeature2Set(const RegionFeature2Set& f);

        /**
         * constructor, creates transform set from feature descriptor file
         * @param fdf feature descriptor file
         * @param keyword parameter name
         */
        /*Transform2Set(std::string fdf, std::string keyword);

        Transform2Set(Vector<std::string>& descriptors);*/
        /**
         * destructor
         */
        ~RegionFeature2Set();

        /**
         * applies the transform to the input image
         * @param input input image
         * @param output output image vector
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         * @param roi the operation uses only the foreground (non-zero) region of the roi
         */
        void apply(Image<INPUT>& input, Region2& r, Vector<float>& output);

        /**
         * adds transform to the transform set
         * @param f transform to add
         */
        void add(RegionFeature2<INPUT>* f);

        /**
         * using the proposed border, the each transform can be applied to each position of an image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();

        int dimensions();
    };

    template<typename INPUT>
    RegionFeature2Set<INPUT>::RegionFeature2Set()
    : Vector<RegionFeature2<INPUT>* >()
    {
    }

    template<typename INPUT>
    RegionFeature2Set<INPUT>::RegionFeature2Set(const RegionFeature2Set& f)
    : Vector<RegionFeature2<INPUT>* >(f)
    {
    }

    template<typename INPUT>
    RegionFeature2Set<INPUT>::~RegionFeature2Set()
    {
    }

    template<typename INPUT>
    void RegionFeature2Set<INPUT>::apply(Image<INPUT>& input, Region2& r, Vector<float>& output)
    {
        //unsigned int d= this->dimensions();
        unsigned int actual= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            //tprintf("applying %s\n", (*this)(i)->descriptor.c_str()); fflush(stdout);
            (*this)(i)->apply(input, r, actual, output);
            //output(i)= (*this)(i)->apply(input, r);
            actual+= (*this)(i)->dimensions();
        }
    }

    template<typename INPUT>
    void RegionFeature2Set<INPUT>::add(RegionFeature2<INPUT>* f)
    {
        this->push_back(f);
    }

    template<typename INPUT>
    Border2 RegionFeature2Set<INPUT>::getProposedBorder()
    {
        int w= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            /*if ( (*this)(i)->getProposedBorder().topBorder > w )
                w= (*this)(i)->getProposedBorder().topBorder;*/
        }
        return Border2(w, w, w, w);
    }

    template<typename INPUT>
    int RegionFeature2Set<INPUT>::dimensions()
    {
        int d= 0;
        for ( unsigned int i= 0; i < this->size(); ++i )
            d+= (*this)(i)->dimensions();
        return d;
    }

}

#endif	/* REGIONTRANSFORM2SET_H */

