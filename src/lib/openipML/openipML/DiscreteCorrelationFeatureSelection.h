/**
 * @file DiscreteCorrelationFeatureSelection.h
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
 * The DiscreteCorrelationFeatureSelection method is the realization of the
 * filter based feature selection method using correlation.
 */

#ifndef DISCRETECORRELATIONFEATURESELECTION_H
#define	DISCRETECORRELATIONFEATURESELECTION_H

#include <stdlib.h>
#include <math.h>

#include <openipML/FilterBasedFeatureSelection.h>
#include <openipML/BestFirstSearch.h>

/**
 * Based on the paper
 * Mark A. Hall: Feature Selection for Discrete and Numeric Class Machine Learning
 */

namespace openip
{
    class DiscreteCorrelationFeatureSelection;

    /**
     * CorrelationFeatureSelection graph node
     */
    class CFSState
    {
    public:
        /**
         * default constructor
         */
        CFSState();

        /**
         * constructor
         * @param d DiscreteCorrelationFeatureSelection object
         */
        CFSState(DiscreteCorrelationFeatureSelection* d);

        /**
         * copy constructor
         * @param c instance to copy
         */
        CFSState(const CFSState& c);

        /**
         * destructor
         */
        ~CFSState();

        /**
         * equals operator
         * @param c object to check equality with
         * @return true if equals, false otherwise
         */
        bool operator==(CFSState& c);

        /**
         * assignment operator
         * @param c object to assign to *this
         * @return reference to *this
         */
        CFSState& operator=(const CFSState& c);

        /**
         * evaluates the actual state
         * @return goodness of the selection
         */
        float evaluate();

        void dump();

        /** actual selection */
        Vector<int> selection;
        /** pointer to discrete correlation feature selection object */
        DiscreteCorrelationFeatureSelection* d;
    };

    /**
     * Correlation feature selection operator
     */
    class CFSOperator
    {
    public:
        /**
         * constructor
         * @param k the feature index the operator changes
         */
        CFSOperator(int k);

        /**
         * copy constructor
         * @param c instance to copy
         */
        CFSOperator(const CFSOperator& c);

        /**
         * destructor
         */
        ~CFSOperator();

        /**
         * equality operator
         * @param c object to check the equality with
         * @return true if equals, false otherwise
         */
        bool operator==(CFSOperator& c);

        /**
         * applies the operator to the parameter state (graph node)
         * @param c state (graph node) to apply the operator on
         * @return the new state
         */
        CFSState apply(CFSState& c);

        /** the feature changed by the operator */
        int k;
    };

    /**
     * DiscreteCorrelationFeatureSelection represents the filter based feature
     * selection method using correlation
     */
    class DiscreteCorrelationFeatureSelection: public FilterBasedFeatureSelection<int>
    {
    public:
        using FilterBasedFeatureSelection<int>::mask;

        /**
         * default constructor
         */
        DiscreteCorrelationFeatureSelection();

        /**
         * copy constructor
         * @param c instance to copy
         */
        DiscreteCorrelationFeatureSelection(const DiscreteCorrelationFeatureSelection& c);

        /**
         * destructor
         */
        ~DiscreteCorrelationFeatureSelection();

        /**
         * do feature selection
         * @param d the feature selection is done using this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(DataSet<int>& d);

        /**
         * computes correlation between the xth and yth features in the DataSet d
         * @param d the feature selection is done using this dataset
         * @param x feature index
         * @param y feature index
         * @return correlation value
         */
        float computeCorrelation(DataSet<int>& d, int x, int y);

        /**
         * computes correlation between two vectors
         * @param a parameter vector
         * @param b parameter vector
         * @return correlation of the vectors
         */
        float computeCorrelation(Vector<int>& a, Vector<int>& b);

        /**
         * evaluates the current selection
         * @param selection binary vector representing the current selection
         * @return goodness of the selection
         */
        float evaluate(Vector<int> selection);

        /** class vs feature correlations */
        Vector<float> classFeatureCorrelations;
        /** feature vs feature correlations */
        Matrix<float> featureFeatureCorrelations;
        /** current selection */
        Vector<unsigned char> selection;
    };
}

#endif	/* CORRELATIONFEATURESELECTION_H */

