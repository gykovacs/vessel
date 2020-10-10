/**
 * @file FilterBasedFeatureSelection.h
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
 * FilterBasedFeatureSelection is the base class for all filter based feature
 * selection methods.
 */

#ifndef FILTERBASEDFEATURESELECTION_H
#define	FILTERBASEDFEATURESELECTION_H

#include <openipML/Classifier.h>

namespace openip
{
    /**
     * FilterBasedFeatureSelection is the base class for all filter based feature
     * selection methods.
     */
    template<typename T>
    class FilterBasedFeatureSelection
    {
    public:
        /**
         * default constructor
         */
        FilterBasedFeatureSelection();

        /**
         * copy constructor
         * @param f instance to copy
         */
        FilterBasedFeatureSelection(const FilterBasedFeatureSelection& f);

        /**
         * destructor
         */
        ~FilterBasedFeatureSelection();

        /**
         * do feature selection
         * @param d the feature selection is carried out using this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(DataSet<T>& d);

        /** selection */
        Vector<unsigned char> mask;
    };

    template<typename T>
    FilterBasedFeatureSelection<T>::FilterBasedFeatureSelection()
    {
    }

    template<typename T>
    FilterBasedFeatureSelection<T>::FilterBasedFeatureSelection(const FilterBasedFeatureSelection<T>& f)
    : mask(f.mask)
    {
    }

    template<typename T>
    FilterBasedFeatureSelection<T>::~FilterBasedFeatureSelection()
    {
    }

    template<typename T>
    int FilterBasedFeatureSelection<T>::select(DataSet<T>&)
    {
        return 0;
    }
}

#endif	/* FILTERBASEDFEATURESELECTION_H */

