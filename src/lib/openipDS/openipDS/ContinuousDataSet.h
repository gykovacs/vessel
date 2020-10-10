/**
 * @file ContinuousDataSet.h
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
 * The ContinuousDataSet class represents a DataSet object containing
 * feature vectors of real numbers
 */

#ifndef CONTINUOUSDATASET_H
#define	CONTINUOUSDATASET_H

#include <openipDS/DataSet.h>
#include <openipDS/ContinuousFeatureVector.h>

namespace openip
{
    /**
     * ContinuousDataSet class represents a DataSet containing feature vectors
     * of real numbers.
     */
    class ContinuousDataSet: public DataSet<float>
    {
    public:
        using DataSet<float>::classNames;
        using DataSet<float>::push_back;
        using DataSet<float>::operator();
        using DataSet<float>::add;
        using DataSet<float>::init;
        using DataSet<float>::equalize;

        /**
         * default constructor
         */
        ContinuousDataSet();

        /**
         * copy constructor
         * @param c instance to copy
         */
        ContinuousDataSet(const ContinuousDataSet& c);

        /**
         * destructor
         */
        ~ContinuousDataSet();
    };
}

#endif	/* CONTINUOUSDATASET_H */

