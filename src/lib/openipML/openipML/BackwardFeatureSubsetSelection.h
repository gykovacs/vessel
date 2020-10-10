/**
 * @file BackwardFeatureSubsetSelection.h
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
 * BackwardFeatureSubsetSelection realizes th wrapper based backward feature
 * subset selection (BFSS) method.
 */

#ifndef BACKWARDFEATURESUBSETSELECTION_H
#define	BACKWARDFEATURESUBSETSELECTION_H

#include <openipML/WrapperBasedFeatureSelection.h>
#include <openipML/BestFirstSearch.h>

namespace openip
{
    template<typename T>
    class BackwardFeatureSubsetSelection;

    /**
     * Graph search class for BFSS
     */
    template<typename T>
    class BFSSState
    {
    public:
        /**
         * default constructor
         */
        BFSSState();

        /**
         * constructor with BFSS object
         * @param d BFSS object
         */
        BFSSState(BackwardFeatureSubsetSelection<T>* d);

        /**
         * copy constructor
         * @param c instance to copy
         */
        BFSSState(const BFSSState& c);

        /**
         * destructor
         */
        ~BFSSState();

        /**
         * equals operator
         * @param c parameter to check equality with
         * @return true if equals, false otherwise
         */
        bool operator==(BFSSState& c);

        /**
         * assignment operator
         * @param c parameter to assign to this
         * @return reference to this object
         */
        BFSSState& operator=(const BFSSState& c);

        void dump();

        /**
         * evaluates the goodness of the state
         * @return
         */
        float evaluate();

        /** actual selection mask */
        Vector<int> selection;
        /** pointer to the BFSS object */
        BackwardFeatureSubsetSelection<T>* f;
    };

    /**
     * Operator for BFSS feature selection
     */
    template<typename T>
    class BFSSOperator
    {
    public:
        /**
         * constructor
         * @param k feature index to change
         */
        BFSSOperator(int k);

        /**
         * copy constructor
         * @param c instance to copy
         */
        BFSSOperator(const BFSSOperator& c);

        /**
         * destructor
         */
        ~BFSSOperator();

        /**
         * equals operator
         * @param c object to check equality with
         * @return true if equals, false otherwise
         */
        bool operator==(BFSSOperator& c);

        /**
         * applies the operator to the parameter
         * @param c state, graph node
         * @return new state, that is new node
         */
        BFSSState<T> apply(BFSSState<T>& c);

        /** feature to exclude from selection */
        int k;
    };

    /**
     * BackwardFeatureSubsetSelection class realizes an object for backward feature subset selection
     */
    template<typename T>
    class BackwardFeatureSubsetSelection: public WrapperBasedFeatureSelection<T>
    {
    public:
        using WrapperBasedFeatureSelection<T>::selection;
        using WrapperBasedFeatureSelection<T>::filterDataSet;

        /**
         * default constructor
         */
        BackwardFeatureSubsetSelection();

        /**
         * copy constructor
         * @param f instance to copy
         */
        BackwardFeatureSubsetSelection(const BackwardFeatureSubsetSelection& f);

        /**
         * destructor
         */
        ~BackwardFeatureSubsetSelection();

        /**
         * do feature selection
         * @param c classifier
         * @param v validator
         * @param d dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d);

        /**
         * evaluates the current feature subset
         * @param selection feature subset
         * @return goodness of selection
         */
        float evaluate(Vector<int>& selection);

        /** classifier object */
        Classifier<T>* c;
        /** validator object */
        Validator<T>* v;
        /** dataset */
        DataSet<T>* d;
        /** temporary dataset */
        DataSet<T>* d2;
    };

    template<typename T>
    BFSSState<T>::BFSSState()
    {
    }

    template<typename T>
    BFSSState<T>::BFSSState(BackwardFeatureSubsetSelection<T>* f)
    {
        this->f= f;
        selection.resize(((f->d)->operator()(0)).size());
        this->selection= 1;
    }

    template<typename T>
    BFSSState<T>::BFSSState(const BFSSState& c)
    {
        this->f= c.f;
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
    }

    template<typename T>
    BFSSState<T>::~BFSSState()
    {
    }

    template<typename T>
    void BFSSState<T>::dump()
    {
    }

    template<typename T>
    float BFSSState<T>::evaluate()
    {
        return f->evaluate(this->selection);
    }

    template<typename T>
    BFSSState<T>& BFSSState<T>::operator=(const BFSSState& c)
    {
        this->selection.resize(c.selection.size());
        this->selection= c.selection;
        this->f= c.f;
        return *this;
    }

    template<typename T>
    bool BFSSState<T>::operator==(BFSSState& c)
    {
        for ( int i= 0; i < selection.size(); ++i )
            if ( selection[i] != c.selection[i] )
                return false;
        return true;
    }

    template<typename T>
    BFSSOperator<T>::BFSSOperator(int k)
    {
        this->k= k;
    }

    template<typename T>
    BFSSOperator<T>::BFSSOperator(const BFSSOperator& c)
    {
        this->k= c.k;
    }

    template<typename T>
    BFSSOperator<T>::~BFSSOperator()
    {
    }

    template<typename T>
    bool BFSSOperator<T>::operator==(BFSSOperator& c)
    {
        return this->k == c.k;
    }

    template<typename T>
    BFSSState<T> BFSSOperator<T>::apply(BFSSState<T>& c)
    {
        BFSSState<T> tmp(c);
        if ( tmp.selection[k] != 0 )
            tmp.selection[k]= 0;

        return tmp;
    }

    template<typename T>
    BackwardFeatureSubsetSelection<T>::BackwardFeatureSubsetSelection()
    {
        d2= new DataSet<T>();
    }

    template<typename T>
    BackwardFeatureSubsetSelection<T>::BackwardFeatureSubsetSelection(const BackwardFeatureSubsetSelection& f)
    {
    }

    template<typename T>
    BackwardFeatureSubsetSelection<T>::~BackwardFeatureSubsetSelection()
    {
        delete d2;
    }

    template<typename T>
    int BackwardFeatureSubsetSelection<T>::select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d)
    {
        this->d= &d;
        this->c= c;
        this->v= v;

        BestFirstSearch<BFSSState<T>, BFSSOperator<T> > bfs;

        BFSSState<T> act(this);
        for ( unsigned int i= 0; i < d(0).size(); ++i )
        {
            bfs.operators.push_back(BFSSOperator<T>(i));
        }
        bfs.search(act);

        cout << bfs.actualNode.selection << endl;

        selection= bfs.actualNode.selection;

        return 0;
    }

    template<typename T>
    float BackwardFeatureSubsetSelection<T>::evaluate(Vector<int>& selection)
    {
        int s= 0;
        for ( unsigned int i= 0; i < selection.size(); ++i )
            s+= selection(i);
        if ( s != 0 )
        {
            this->filterDataSet(*d, selection, *d2);
            return v->validate(this->c, *d2);
        }
        return 0;
    }
}


#endif	/* BACKWARDFEATURESUBSETSELECTION_H */

