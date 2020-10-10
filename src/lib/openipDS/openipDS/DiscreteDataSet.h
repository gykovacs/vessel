/**
 * @file DiscreteDataSet.h
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
 * The DiscreteDataSet class represents a DataSet object containing
 * feature vectors of integers numbers
 */

#ifndef DISCRETEDATASET_H
#define	DISCRETEDATASET_H

#include <openipDS/DataSet.h>
#include <openipDS/DiscreteFeatureVector.h>

namespace openip
{
    /**
     * DiscreteDataSet class represents a DataSet containing feature vectors
     * of integer numbers.
     */
    class DiscreteDataSet: public DataSet<int>
    {
    public:
        using DataSet<int>::classNames;
        using DataSet<int>::push_back;
        using DataSet<int>::operator();
        using DataSet<int>::add;
        using DataSet<int>::init;
        using DataSet<int>::equalize;

        /**
         * default constructor
         */
        DiscreteDataSet();

        /**
         * copy constructor
         * @param d instance to copy
         */
        DiscreteDataSet(const DiscreteDataSet& d);

        /**
         * destructor
         */
        ~DiscreteDataSet();
    };
}

#endif	/* DISCRETEDATASET_H */

