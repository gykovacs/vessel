/**
 * @file SimulatedAnnealingFeatureSubsetSelection.h
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
 * SimulatedAnnealingFeatureSubsetSelection represents simulated annealing based
 * feature subset selection.
 */

#ifndef SIMULATEDANNEALINGFEATURESUBSETSELECTION_H
#define	SIMULATEDANNEALINGFEATURESUBSETSELECTION_H

#include <openipML/SimulatedAnnealing.h>
#include <openipML/WrapperBasedFeatureSelection.h>
#include <openipML/Classifier.h>
#include <openipML/Validator.h>

namespace openip
{
    /**
     * SimulatedAnnealingFeatureSubsetSelection represents simulated annealing based
     * feature subset selection.
     */
    template<typename T>
    class SimulatedAnnealingFeatureSubsetSelection: public WrapperBasedFeatureSelection<T>, public SimulatedAnnealing
    {
    public:
        using WrapperBasedFeatureSelection<T>::selection;

        /**
         * default constructor
         */
        SimulatedAnnealingFeatureSubsetSelection();

        /**
         * copy constructor
         * @param s instance to copy
         */
        SimulatedAnnealingFeatureSubsetSelection(const SimulatedAnnealingFeatureSubsetSelection& s);

        /**
         * destructor
         */
        ~SimulatedAnnealingFeatureSubsetSelection();

        /**
         * initializes the method
         */
        virtual void init();

        /**
         * do the sampling
         */
        virtual void sample();

        /**
         * do the feature selection
         * @param c classifier to wrap
         * @param v validation object
         * @param d dataset used in feature selection
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d);

        /**
         * evaluates the actual selection
         * @return
         */
        virtual double evaluate();

        /**
         * reverts the last sampling
         */
        virtual void revert();

        /** pointer to classifier object */
        Classifier<T>* c;
        /** pointer to validator object */
        Validator<T>* v;
        /** pointer to dataset */
        DataSet<T>* d;
        /** temporary dataset */
        DataSet<T> d2;
        /** random number generator */
        UniformRandomIntegerGenerator urig;
        /** last change by sampling */
        int change;
    };

    template<typename T>
    SimulatedAnnealingFeatureSubsetSelection<T>::SimulatedAnnealingFeatureSubsetSelection()
    : SimulatedAnnealing(100, 0.00001, 0.7, 1.0)
    {
    }

    template<typename T>
    SimulatedAnnealingFeatureSubsetSelection<T>::SimulatedAnnealingFeatureSubsetSelection(const SimulatedAnnealingFeatureSubsetSelection& s)
    : SimulatedAnnealing(s), c(s.c), v(s.v), d(s.d), d2(s.d2), urig(s.urig), change(s.change)
    {
        selection= s.selection;
    }

    template<typename T>
    SimulatedAnnealingFeatureSubsetSelection<T>::~SimulatedAnnealingFeatureSubsetSelection()
    {
    }

    template<typename T>
    void SimulatedAnnealingFeatureSubsetSelection<T>::init()
    {
        objective= 0;
        newObjective= 0 + 2*epsilon;
        temperature= 1.0;
        iteration= 0;
        
        tprintf("init selection\n");
        d->init();
        tprintf("init finished\n");
        selection.resize(d->dimension);
        selection= 0;
        selection(0)= 1;
        urig.n= d->dimension;
    }
    
    template<typename T>
    void SimulatedAnnealingFeatureSubsetSelection<T>::sample()
    {
        while ( 1 )
        {
            change= urig.generate();
            if ( selection(change) == 0 )
                selection(change)= 1;
            else
                selection(change)= 0;
            
            int s= 0;
            for ( unsigned int i= 0; i < selection.size(); ++i )
                s+= selection(i);
            if ( s != 0 )
                return;
        }
    }

    template<typename T>
    int SimulatedAnnealingFeatureSubsetSelection<T>::select(Classifier<T>* c, Validator<T>* v, DataSet<T>& d)
    {
        this->c= c;
        this->v= v;
        this->d= &d;

        tprintf("starting selection\n");
        this->SimulatedAnnealing::solve();

        return 0;
    }

    template<typename T>
    double SimulatedAnnealingFeatureSubsetSelection<T>::evaluate()
    {
        this->filterDataSet(*d, selection, d2);
        tprintf("validate\n");
        double tmp= v->validate(c, d2);
        cout << "score: " << tmp << " temperature: " << temperature << endl;
        return tmp;
    }

    template<typename T>
    void SimulatedAnnealingFeatureSubsetSelection<T>::revert()
    {
        if ( selection(change) == 0 )
            selection(change)= 1;
        else
            selection(change)= 0;
    }
}

#endif	/* SIMULATEDANNEALINGFEATURESUBSETSELECTION_H */

