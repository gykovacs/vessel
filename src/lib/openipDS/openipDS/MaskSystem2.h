/**
 * @file MaskSystem2.h
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
 * The MaskSystem2 represents a set of MaskSet2 objects. Using this abstraction,
 * a system of masks can be defined with arbitrary logical connection
 * among them.
 */


#ifndef _MASKSYSTEM_H
#define	_MASKSYSTEM_H

#include <openipDS/Set.h>
#include <openipDS/MaskSet2.h>

namespace openip
{
    /**
     * MaskSystem2 represents a set of MaskSet objects. Using this abstraction,
     * a system of masks can be defined with arbitrary logical connection
     * among them.
     */
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class MaskSystem2: public vector<MaskSet2<INPUT, OUTPUT, WEIGHTS>* >
    {
    public:
        /**
         * vector<MaskSet*>::iterator typedef
         */
        typedef typename vector<MaskSet2<INPUT, OUTPUT, WEIGHTS>* >::iterator msIt;

        /**
         * vector<MaskSet*>::const_iterator typedef
         */
        typedef typename vector<MaskSet2<INPUT, OUTPUT, WEIGHTS>* >::const_iterator const_msIt;

        /**
         * MaskSystem constructor
         * @param matches value to set when the system matches
         * @param notMatches value to set when the system not matches
         */
        MaskSystem2(OUTPUT matches= 255, OUTPUT notMatches= 0);

        /**
         * copy destructor
         * @param m instance to copy
         */
        MaskSystem2(MaskSystem2& m);

        /**
         * destructor
         */
        ~MaskSystem2();

        /**
         * applies the mask system in all the positions of an image
         * @param in input image
         * @param out output image
         * @param mask mask in which's positions to apply
         */
        void apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* mask= NULL);

        /**
         * applies the mask system in all the positions of an image
         * @param in input image
         * @param c Cache2 object
         * @param mask mask in which's positions to apply
         */
        //void apply(Image<INPUT>& in, Cache2* c, Image<unsigned char>* mask= NULL);

        /**
         * computes the min and max pixel positions in the system
         */
        void computeMinMax();

        /**
         * output value when the mask system matches
         */
        OUTPUT matches;

        /**
         * output value when the mask system does not match
         */
        OUTPUT notMatches;

        /**
         * minimum pixel position
         */
        int min;

        /**
         * maximum pixel position
         */
        int max;
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaskSystem2<INPUT, OUTPUT, WEIGHTS>::MaskSystem2(OUTPUT matches, OUTPUT notMatches)
    {
        this->matches= matches;
        this->notMatches= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    MaskSystem2<INPUT, OUTPUT, WEIGHTS>::MaskSystem2(MaskSystem2& m)
    : vector<Filter2<INPUT, OUTPUT, WEIGHTS>* >(m)
    {
        this->matches= matches;
        this->notMatches= notMatches;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSystem2<INPUT, OUTPUT, WEIGHTS>::computeMinMax()
    {
        min= (*(this->begin()))->min;
        max= (*(this->begin()))->max;
        for ( typename MaskSystem2<INPUT, OUTPUT, WEIGHTS>::msIt msit= this->begin(); msit != this->end(); ++msit )
        {
            if ( (*msit)->min < this->min )
                min= (*msit)->min;
            else if ( (*msit)->max > this->max )
                max= (*msit)->max;
        }
    }

    /*template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSystem2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Cache2* c, Image<unsigned char>* mask)
    {
        Image<OUTPUT>* tmp;
        c->get(&tmp);
        this->apply(in, *tmp, mask);
        in= *tmp;
        c->put(tmp);
    }*/

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    void MaskSystem2<INPUT, OUTPUT, WEIGHTS>::apply(Image<INPUT>& in, Image<OUTPUT>& out, Image<unsigned char>* mask)
    {
        typename MaskSystem2<INPUT, OUTPUT, WEIGHTS>::msIt msit;

        int end= in.n - max;

        if ( mask == NULL )
        {
            for ( int i= -min; i < end; ++i )
            {
                for ( msit= this->begin(); msit != this->end(); ++msit )
                    if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_ALL )
                    {
                        if ( (*msit)->matchAll(in, i) == (*msit)->notMatches )
                        {
                            out(i)= notMatches;
                            break;
                        }
                    }
                    else if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_ANY )
                    {
                        if ( (*msit)->matchAny(in, i) == (*msit)->notMatches )
                        {
                            out(i)= notMatches;
                            break;
                        }
                    }
                    else if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_NONE )
                    {
                        if ( (*msit)->matchNone(in, i) == (*msit)->notMatches )
                        {
                            out(i)= notMatches;
                            break;
                        }
                    }
                if ( msit == this->end() )
                    out(i)= matches;
            }
        }
        else
        {
            for ( int i= -min; i < end; ++i )
            {
                if ( (*mask)(i) > 0 )
                {
                    for ( msit= this->begin(); msit != this->end(); ++msit )
                        if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_ALL )
                        {
                            if ( (*msit)->matchAll(in, i) == (*msit)->notMatches )
                            {
                                out(i)= notMatches;
                                break;
                            }
                        }
                        else if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_ANY )
                        {
                            if ( (*msit)->matchAny(in, i) == (*msit)->notMatches )
                            {
                                out(i)= notMatches;
                                break;
                            }
                        }
                        else if ( (*msit)->mode == MaskSet2<unsigned char, unsigned char, unsigned char>::MATCH_NONE )
                        {
                            if ( (*msit)->matchNone(in, i) == (*msit)->notMatches )
                            {
                                out(i)= notMatches;
                                break;
                            }
                        }
                    if ( msit == this->end() )
                        out(i)= matches;
                }
                else
                    out(i)= notMatches;

            }
        }

    }
}

#endif	/* _MASKSYSTEM_H */

