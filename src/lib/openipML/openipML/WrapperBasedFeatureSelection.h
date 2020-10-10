/**
 * @file WrapperBasedFeatureSelection.h
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
 * WrapperBasedFeatureSelection is the base class of all wrapper based feature
 * selection methods.
 */

#ifndef WRAPPERBASEDFEATURESELECTION_H
#define	WRAPPERBASEDFEATURESELECTION_H

#include <openipML/Classifier.h>
#include <openipML/Validator.h>

namespace openip
{
    /**
     * WrapperBasedFeatureSelection is the base class of all wrapper based feature
     * selection methods.
     */
    template<typename T>
    class WrapperBasedFeatureSelection
    {
    public:
        /**
         * default constructor
         */
        WrapperBasedFeatureSelection();

        /**
         * copy constructor
         * @param f instance to copy
         */
        WrapperBasedFeatureSelection(const WrapperBasedFeatureSelection& f);

        /**
         * destructor
         */
        virtual ~WrapperBasedFeatureSelection();

        /**
         * do feature selection
         * @param c classifier object wrapped in the feature selection
         * @param v validator object
         * @param d dataset used in the feature selection
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d);

        /**
         * filters out features from the dataset
         * @param input input dataset
         * @param mask binary filter
         * @param output output dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int filterDataSet(DataSet<T>& input, Vector<int>& mask, DataSet<T>& output);

        /** actual feature selection */
        Vector<int> selection;
    };

    template<typename T>
    WrapperBasedFeatureSelection<T>::WrapperBasedFeatureSelection()
    {
    }

    template<typename T>
    WrapperBasedFeatureSelection<T>::WrapperBasedFeatureSelection(const WrapperBasedFeatureSelection<T>& f)
    {
    }

    template<typename T>
    WrapperBasedFeatureSelection<T>::~WrapperBasedFeatureSelection()
    {
    }

    template<typename T>
    int WrapperBasedFeatureSelection<T>::select(Classifier<T>*, Validator<T>*, DataSet<T>&)
    {
        return 0;
    }

    template<typename T>
    int WrapperBasedFeatureSelection<T>::filterDataSet(DataSet<T>& input, Vector<int>& mask, DataSet<T>& output)
    {
        output.clear();
        output.classNames.clear();
        output.featureNames.clear();
        output.classNames= input.classNames;

        int size= 0;
        for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) != 0 )
                size++;

        vector<int> hash;
        hash.resize(size);

        int j= 0;
        for ( unsigned int i= 0; i < mask.size(); ++i )
        {
            if ( mask(i) != 0 )
            {
                output.featureNames.push_back(input.featureNames(i));
                hash[j++]= i;
            }
        }

        FeatureVector<T> f(size);
        for ( unsigned int i= 0; i < input.size(); ++i )
        {
            f.classIndex= input(i).classIndex;
            f.weightForeground= input(i).weightForeground;
            f.weightBackground= input(i).weightBackground;
            for ( int j= 0; j < size; ++j )
                f(j)= input(i)(hash[j]);

            output.push_back(f);
        }

        return 0;
    }
}


#endif	/* WRAPPERBASEDFEATURESELECTION_H */

