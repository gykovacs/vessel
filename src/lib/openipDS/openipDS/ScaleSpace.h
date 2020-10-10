/**
 * @file ScaleSpace.h
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
 * The ScaleSpace class represents a general scale space object.
 */
#ifndef _SCALESPACE_H
#define	_SCALESPACE_H

#include <openipDS/Image.h>

namespace openip
{
    /**
    * ScaleSpace represents a scale space object
    */
    template<typename T>
    class ScaleSpace: public vector<Image<T>* >
    {
    public:
	/**
	* ScaleSpace default constructor
	*/
        ScaleSpace();

	/**
	* ScaleSpace constructor
	* @param input input image (lowest level)
	* @param c Cache2 object containing images to build up the scale space
	* @param scales number of levels
	*/
        //ScaleSpace(Image<unsigned char>& input, Cache2* c, int scales);

	/**
	* destructor
	*/
        ~ScaleSpace();

	/**
	* clear the scale space
	*/
        void clear();

	/**
	* init the scale space
	* @param input input image (lowest level)
	* @param c Cache2 object containing images to build up the scale space
	* @param scales number of levels
	*/
        //virtual void init(Image<unsigned char>& input, Cache2* c, int scales);

	/**
	* pointer of the Cache2 object from whom the elements of the scale space are get out
	*/
        //Cache2* c;
    };

    template<typename T>
    ScaleSpace<T>::ScaleSpace()
    {
    }

    /*template<typename T>
    ScaleSpace<T>::ScaleSpace(Image<unsigned char>& input, Cache2* c, int scales)
    {
        init(input, c, scales);
    }*/

    template<typename T>
    ScaleSpace<T>::~ScaleSpace()
    {
    }

    /*template<typename T>
    void ScaleSpace<T>::init(Image<unsigned char>& input, Cache2* cNew, int scales)
    {
        for ( typename vector<Image<T>* >::iterator vit= this->begin(); vit != this->end(); ++vit )
            c->put(*vit);
        c= cNew;
        Image<T>* tmp;
        for ( int i= 0; i < scales; ++i )
        {
            c->get(&tmp);
            this->push_back(tmp);
        }
    }*/


}

#endif	/* _SCALESPACE_H */

