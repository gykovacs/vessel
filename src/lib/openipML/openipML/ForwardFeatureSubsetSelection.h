/**
 * @file ForwardFeatureSubsetSelection.h
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
 * ForwardFeatureSubsetSelection represents the wrapper based forward feature subset
 * selection method.
 */

#ifndef FORWARDSUBSETFEATURESELECTION_H
#define	FORWARDSUBSETFEATURESELECTION_H

#include <openipML/WrapperBasedFeatureSelection.h>
#include <openipML/BestFirstSearch.h>

namespace openip
{
    template<typename T>
    class ForwardFeatureSubsetSelection;

    /**
     * FFSSState is a state (graph node) of the forward feature subset selection method.
     */
    template<typename T>
    class FFSSState
    {
    public:
        /**
         * default constructor
         */
        FFSSState();

        /**
         * constructor
         * @param d ForwardFeatureSubsetSelection object
         */
        FFSSState(ForwardFeatureSubsetSelection<T>* d);

        /**
         * copy constructor
         * @param c instance to copy
         */
        FFSSState(const FFSSState& c);

        /**
         * destructor
         */
        ~FFSSState();

        /**
         * equals operator
         * @param c object to check equality with
         * @return true if equals, false otherwise
         */
        bool operator==(FFSSState& c);

        /**
         * assignment operator
         * @param c object to assign to *this
         * @return reference to *this
         */
        FFSSState& operator=(const FFSSState& c);

        /**
         * evaluates the state
         * @return goodness of the state
         */
        float evaluate();

        void dump();

        /** feature selection */
        Vector<int> selection;
        /** pointer to ForwardFeatureSubsetSelection object*/
        ForwardFeatureSubsetSelection<T>* f;
    };

    /**
     * FFSSOperator is an operator of the forward feature subset selection method
     */
    template<typename T>
    class FFSSOperator
    {
    public:
        /**
         * constructor
         * @param k the operator modifies the kth feature
         */
        FFSSOperator(int k, Vector<string>& featureNames);

        /**
         * copy constructor
         * @param c instance to copy
         */
        FFSSOperator(const FFSSOperator& c);

        /**
         * destructor
         */
        ~FFSSOperator();

        /**
         * equals operator
         * @param c object to check equality with
         * @return true if equals, false otherwise
         */
        bool operator==(FFSSOperator& c);

        /**
         * applies the operator to the parameter state
         * @param c state
         * @return new state
         */
        FFSSState<T> apply(FFSSState<T>& c);

        /** the operator modifies the kth feature */
        Vector<int> k;
    };

    /**
     * ForwardFeatureSubsetSelection represents the forward feature subset selection method.
     */
    template<typename T>
    class ForwardFeatureSubsetSelection: public WrapperBasedFeatureSelection<T>
    {
    public:
        using WrapperBasedFeatureSelection<T>::selection;
        using WrapperBasedFeatureSelection<T>::filterDataSet;

        /**
         * constructor
         */
        ForwardFeatureSubsetSelection();

        /**
         * copy constructor
         * @param f instance to copy
         */
        ForwardFeatureSubsetSelection(const ForwardFeatureSubsetSelection& f);

        /**
         * destructor
         */
        ~ForwardFeatureSubsetSelection();

        /**
         * do the feature selection
         * @param c the method wraps this classifier
         * @param v validator object
         * @param d the feature selection is based on this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d);

        /**
         * evaluates the parameter selection
         * @param selection feature selection
         * @return goodness of the selection
         */
        float evaluate(Vector<int>& selection);

        /** pointer to the classifier object */
        Classifier<T>* c;
        /** pointer to the validation object */
        Validator<T>* v;
        /** pointer to the dataset */
        DataSet<T>* d;
        /** pointer to temporary dataset */
        DataSet<T>* d2;
    };

    template<typename T>
    FFSSState<T>::FFSSState()
    {
    }

    template<typename T>
    FFSSState<T>::FFSSState(ForwardFeatureSubsetSelection<T>* f)
    {
        this->f= f;
        selection.resize(((f->d)->operator()(0)).size());
        this->selection= 0;
    }

    template<typename T>
    FFSSState<T>::FFSSState(const FFSSState& c)
    {
        this->f= c.f;
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
    }

    template<typename T>
    FFSSState<T>::~FFSSState()
    {
    }

    template<typename T>
    float FFSSState<T>::evaluate()
    {
        return f->evaluate(this->selection);
    }

    template<typename T>
    FFSSState<T>& FFSSState<T>::operator=(const FFSSState& c)
    {
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
        this->f= c.f;
        return *this;
    }

    template<typename T>
    bool FFSSState<T>::operator==(FFSSState& c)
    {
        for ( int i= 0; i < selection.size(); ++i )
            if ( selection[i] != c.selection[i] )
                return false;
        return true;
    }

    template<typename T>
    void FFSSState<T>::dump()
    {
        int sum= 0;
        for ( unsigned int i= 0; i < selection.size(); ++i )
            sum+= selection[i];
        tprintf("number of features: %d/%d\n", sum, selection.size());
        for ( unsigned int i= 0; i < f->d->featureNames.size(); ++i )
            if ( selection[i] && f->d->featureNames(i).find("padding") != 0 )
                tprintf("%s\n", f->d->featureNames(i).c_str());
    }

    template<typename T>
    FFSSOperator<T>::FFSSOperator(int k, Vector<string>& featureNames)
    {
        unsigned int i;
        int m= -1;
        for ( i= 0; i < featureNames.size(); ++i )
        {
            if ( featureNames(i).find("padding") == 0 )
            {
                //printf("."); fflush(stdout);
                continue;
            }
            else
                ++m;
            if ( m == k )
                break;
        }

        //printf("%d\n", m); fflush(stdout);
        this->k.resize(featureNames.size());
        this->k= 0;
        for ( unsigned int j= i; j < featureNames.size(); ++j )
            if ( j == i  )
                this->k.operator()(j)= 1;
            else if ( j != i && featureNames(j).find("padding") == 0 )
                this->k.operator()(j)= 1;
            else if ( j != i && featureNames(j).find("padding") != 0 )
                break;

    }

    template<typename T>
    FFSSOperator<T>::FFSSOperator(const FFSSOperator& c)
    {
        this->k= c.k;
    }

    template<typename T>
    FFSSOperator<T>::~FFSSOperator()
    {
    }

    template<typename T>
    bool FFSSOperator<T>::operator==(FFSSOperator& c)
    {
        return this->k == c.k;
    }

    template<typename T>
    FFSSState<T> FFSSOperator<T>::apply(FFSSState<T>& c)
    {
        //int t= 0;
        /*for ( int i= 0; i < c.selection.size(); ++i )
            t+= c.selection[i];
        printf("suma: %d\n", t);*/

        FFSSState<T> tmp(c);
        for ( unsigned int i= 0; i < k.size(); ++i )
            if ( k(i) == 1 )
                tmp.selection[i]= 1;

        //t= 0;
        /*for ( int i= 0; i < tmp.selection.size(); ++i )
            t+= tmp.selection[i];
        printf("sumb: %d\n", t);*/

        return tmp;
    }

    template<typename T>
    ForwardFeatureSubsetSelection<T>::ForwardFeatureSubsetSelection()
    {
    }

    template<typename T>
    ForwardFeatureSubsetSelection<T>::ForwardFeatureSubsetSelection(const ForwardFeatureSubsetSelection& f)
    {
    }

    template<typename T>
    ForwardFeatureSubsetSelection<T>::~ForwardFeatureSubsetSelection()
    {
    }

    template<typename T>
    int ForwardFeatureSubsetSelection<T>::select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d)
    {
        this->d= &d;
        this->c= c;
        this->v= v;
        this->d2= new DataSet<T>();

        BestFirstSearch<FFSSState<T>, FFSSOperator<T> > bfs;
        bfs.actualNode= FFSSState<T>(this);

        FFSSState<T> act(this);

        int numberOfFeatures= 0;
        for ( unsigned int i= 0; i < d.featureNames.size(); ++i )
        {
            if ( d.featureNames(i).find("padding") == 0 )
                continue;
            else
                ++numberOfFeatures;
        }

        for ( int i= 0; i < numberOfFeatures; ++i )
            bfs.operators.push_back(FFSSOperator<T>(i, d.featureNames));

        bfs.search(act);

        cout << bfs.actualNode.selection << endl;

        selection= bfs.actualNode.selection;

        this->evaluate(selection);

        delete d2;

        return 0;
    }

    template<typename T>
    float ForwardFeatureSubsetSelection<T>::evaluate(Vector<int>& selection)
    {
        this->filterDataSet(*d, selection, *d2);
        float f= v->validate(this->c, *d2);
        return f;
    }
}

#endif	/* FORWARDSUBSETFEATURESELECTION_H */

