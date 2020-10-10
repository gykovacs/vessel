/**
 * @file MaskSet2.h
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
 * The MaskSet2 is a set of mask objects. The effect of applying a mask set
 * to an image is that in each position of the image the distance of the mask is
 * measured or its the matching is checked
 */

#ifndef _MASK_SET_H_
#define _MASK_SET_H_

#include <float.h>
#include <string>

#include <openipDS/Mask2.h>
#include <openipDS/Image.h>
#include <openipDS/Pixel.h>

namespace openip
{
    /**
     * MaskSet represents a set of filters
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class MaskSet2: public vector<Mask2<INPUT, OUTPUT, WEIGHTS>* >
    {
    public:

        /**
         * vector<Mask2*>::iterator typdef
         */
        typedef typename vector<Mask2<INPUT, OUTPUT, WEIGHTS>* >::iterator msIt;

        /**
         * vector<Mask2*>::const_iterator typdef
         */
        typedef typename vector<Mask2<INPUT, OUTPUT, WEIGHTS>* >::const_iterator const_msIt;

        /**
         * mask set mode
         */
        enum MaskSetMode
        {
            /**
             * the mask set matches if any of the masks matches
             */
            MATCH_ANY= 1,

            /**
             * the mask set matches if all of the masks matches
             */
            MATCH_ALL,

            /**
             * the mask set matches if noneof the masks matches
             */
            MATCH_NONE,
        };

        /**
         * constructor, creates an empty filter set
         */
        MaskSet2(OUTPUT matches= 255, OUTPUT notMatches= 0, int mode= MATCH_ANY);

        /**
         * returns the maximum weighted sum after applying each filter
         * in the position n
         * @param input input image
         * @param n the position to compute maximum weighted sum in
         * @return returns the maximum weighted sum in position n
         */
        OUTPUT getMaxDistance(Image<INPUT>& input, int n);

        /**
         * applies the filter set in each position of the image
         * @param input input image
         * @param output output image
         * @param mask the mask is applied in the foreground (non 0) positions of
         * the mask
         */
        void applyMaxDistance(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * applies the filter set in each position of the image, where the mask
         * pixel foreground is
         * @param input input image
         * @param c cache2 object
         * @param mask the mask set is applied in the foreground (non 0) positions of
         * the mask
         */
        //void applyMaxDistance(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask= NULL);

        /**
         * returns the minimum weighted sum after applying each filter
         * in the position n
         * @param input input image
         * @param n the position to compute maximum weighted sum in
         * @return returns the maximum weighted sum in position n
         */
        OUTPUT getMinDistance(Image<INPUT>& input, int n);

        /**
         * applies the filter set in each position of the image
         * @param input input image
         * @param output output image
         * @param mask the mask set is applied in the foreground (non 0) positions of
         * the mask
         */
        void applyMinDistance(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * applies the mask set in each position of the image, the minimum value
         * is returned
         * @param input input image
         * @param c Cache2 object
         * @param mask the mask set is applied in the foreground (non 0) positions of
         * the mask
         */
        //void applyMinDistance(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask= NULL);

        /**
         * applies the mask set in each position of the image, matches if any of
         * the masks matches
         * @param input input image
         * @param output output image
         * @param mask the mask set is applied in the position of foreground pixels of the mask
         */
        void matchAny(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches the masks in position n. If any is matching, returns
         * matches value
         * @param input input image
         * @param n position to check match in
         * @return matches or notMatches
         */
        OUTPUT matchAny(Image<INPUT>& input, int n);

        /**
         * matches the mask set in each position of the image, matches if all of
         * the masks matches
         * @param input input image
         * @param output output image
         * @param mask the mask set is applied in the position of foreground pixels of the mask
         */
        void matchAll(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches any of the masks in position n. If all is matching, returns
         * matches value
         * @param input input image
         * @param n position to check match in
         * @return matches or notMatches
         */
        OUTPUT matchAll(Image<INPUT>& input, int n);

        /**
         * matches the mask set in each position of the image, matches if none of
         * the masks matches
         * @param input input image
         * @param output output image
         * @param mask the mask set is applied in the position of foreground pixels of the mask
         */
        void matchNone(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * matches any of the masks in position n. If none is matching, returns
         * matches value
         * @param input input image
         * @param n position to check match in
         * @return matches or notMatches
         */
        OUTPUT matchNone(Image<INPUT>& input, int n);

        /**
         * applies the mask set to each position of image input and the
         * result get into image output
         * @param input image
         * @param output image
         * @param mask the mask set is matched in the foreground positions of the mask
         */
        void match(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask= NULL);

        /**
         * applies the mask set to each position of image input and the
         * result get into image output
         * @param input image
         * @param c Cache2 object
         * @param mask the mask set is matched in the foreground positions of the mask
         */
        //void match(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask= NULL);

        /**
         * computes the value of min and max
         */
        void computeMinMax();

        /**
         * this value is the output if a mask matches
         */
        OUTPUT matches;

        /**
         * this value is the output if none of the masks matches
         */
        OUTPUT notMatches;

        /**
         * smallest position in the masks
         */
        int min;

        /**
         * maximum position in the masks
         */
        int max;

        /**
         * mode of the mask set
         */
        int mode;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaskSet2<INPUT, OUTPUT, WEIGHTS>::MaskSet2(OUTPUT matches, OUTPUT notMatches, int mode)
    : vector<Filter<INPUT, OUTPUT, WEIGHTS>* > ()
    {
        this->matches= matches;
        this->notMatches= notMatches;
        this->mode= mode;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        int min, max;
        min= INT_MAX;
        max= -INT_MAX;

        for ( int i= 0; i < this->size(); ++i )
        {
            if ( (*this)[i]->min < this->min )
                this->min= (*this)[i]->min;
            if ( (*this)[i]->max > this->max )
                this->max= (*this)[i]->max;
        }
    }

    /**
     * prints the description of the parameter MaskSet2 object to the parameter output stream
     * @param o parameter output stream
     * @param m MaskSet2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    std::ostream& operator<<(std::ostream& o, const MaskSet2<INPUT, OUTPUT, WEIGHTS>& m)
    {
        o << "[MaskSet - number of filters: ";
        o << m.size();
        o << "]";

        return o;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaskSet2<INPUT, OUTPUT, WEIGHTS>::getMaxDistance(Image<INPUT>& input, int n)
    {
        float tmp, max= - FLT_MAX;

        for (int i= 0; i < this->size(); ++i )
        {
             tmp= (*this)[i]->apply(input, n);
             if ( tmp > max )
                max= tmp;
        }
        return max;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::applyMaxDistance(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        int i;
        int end= input.n - this->max;

        if ( mask == NULL )
            for ( i= -this->min; i < end; ++i )
                output(i)= this->getMaxDistance(input, i);
        else
            for ( i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->getMaxDistance(input, i);
                else
                    output(i)= notMatches;
    }

    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::applyMaxDistance(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask)
    {
        Image<OUTPUT>* tmp;
        c->get(&tmp);
        this->applyMaxDistance(input, *tmp, mask);
        input= *tmp;
        c->put(tmp);
    }*/

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaskSet2<INPUT, OUTPUT, WEIGHTS>::getMinDistance(Image<INPUT>& input, int n)
    {
        float tmp, min= FLT_MAX;

        for (int i= 0; i < this->size(); ++i )
        {
             tmp= (*this)[i]->apply(input, n);
             if ( tmp < min )
                min= tmp;
        }
        return min;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::applyMinDistance(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        int i, end= input.n - this->max;

        if ( mask == NULL )
            for ( i= -this->min; i < end; ++i )
                output(i)= this->getMinDistance(input, i);
        else
            for ( i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->getMinDistance(input, i);
                else
                    output(i)= notMatches;
    }

    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::applyMinDistance(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask)
    {
        Image<OUTPUT>* tmp;
        c->get(&tmp);
        this->applyMinDistance(input, *tmp, mask);
        input= *tmp;
        c->put(tmp);
    }*/

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchAny(Image<INPUT>& input, int n)
    {
        typename MaskSet2<INPUT, OUTPUT, WEIGHTS>::msIt mit;
        for (int i= 0; i < this->size(); ++i )
        {
            for ( mit= this->begin(); mit != this->end(); ++mit )
                if ( (*mit)->match(input, n) == (*mit)->matches )

                    return matches;
        }
        return notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchAll(Image<INPUT>& input, int n)
    {
        typename MaskSet2<INPUT, OUTPUT, WEIGHTS>::msIt mit;
        for (int i= 0; i < this->size(); ++i )
        {
            for ( mit= this->begin(); mit != this->end(); ++mit )
                if ( (*mit)->match(input, n) == (*mit)->notMatches )
                    return notMatches;
        }
        return matches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    OUTPUT MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchNone(Image<INPUT>& input, int n)
    {
        typename MaskSet2<INPUT, OUTPUT, WEIGHTS>::msIt mit;
        for (int i= 0; i < this->size(); ++i )
        {
            for ( mit= this->begin(); mit != this->end(); ++mit )
                if ( (*mit)->match(input, n) == (*mit)->matches )
                    return notMatches;
        }
        return matches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchAny(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        int i, end= input.n - this->max;

        if ( mask == NULL )
            for ( i= -this->min; i < end; ++i )
                output(i)= this->matchAny(input, i);
        else
            for ( i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchAny(input, i);
                else
                    output(i)= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchAll(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        int i, end= input.n - this->max;

        if ( mask == NULL )
            for ( i= -this->min; i < end; ++i )
                output(i)= this->matchAll(input, i);
        else
            for ( i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchAll(input, i);
                else
                    output(i)= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::matchNone(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        int i, end= input.n - this->max;

        if ( mask == NULL )
            for ( i= -this->min; i < end; ++i )
                output(i)= this->matchNone(input, i);
        else
            for ( i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchNone(input, i);
                else
                    output(i)= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::match(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* mask)
    {
        if ( mode == MATCH_ANY )
            this->matchAny(input, output, mask);
        else if ( mode == MATCH_ALL )
            this->matchAll(input, output, mask);
        else if ( mode == MATCH_NONE )
            this->matchNone(input, output, mask);
    }

    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSet2<INPUT, OUTPUT, WEIGHTS>::match(Image<INPUT>& input, Cache2* c, Image<unsigned char>* mask)
    {
        Image<OUTPUT>* tmp;
        c->get(&tmp);
        this->match(input, *tmp, mask);
        input= *tmp;
        c->put(&tmp);
    }*/
}


#endif	/* _MASKSET2_H */

